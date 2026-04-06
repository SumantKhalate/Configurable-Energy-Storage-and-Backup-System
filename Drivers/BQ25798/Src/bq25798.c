/**
  ******************************************************************************
  * @file    bq25798.c
  * @brief   BQ25798RQMR Buck-Boost Battery Charger - High-level driver API.
  *          Provides initialization, charge configuration, ADC reading,
  *          fault monitoring, OTG control, and power-mode functions.
  *
  * @note    Based on TI BQ25798 datasheet (SLUSE18).
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "../Inc/bq25798.h"

/* Private helper: clamp a value to [min, max] */
static inline uint16_t clamp_u16(uint16_t val, uint16_t min, uint16_t max)
{
  if (val < min) return min;
  if (val > max) return max;
  return val;
}

/* ==========================================================================
 *  Initialization / Identity
 * ========================================================================== */

BQ25798_Status_t BQ25798_Init(BQ25798_Handle_t *dev)
{
  if (dev == NULL || dev->hi2c == NULL)
  {
    return BQ25798_ERR_PARAM;
  }

  /* Use default I2C address if not set */
  if (dev->i2c_addr == 0U)
  {
    dev->i2c_addr = BQ25798_I2C_ADDR_7BIT;
  }

  /* Set default timeout if not set */
  if (dev->timeout == 0U)
  {
    dev->timeout = 100U;
  }

  /* Verify the part number */
  return BQ25798_VerifyPartNumber(dev, NULL);
}

BQ25798_Status_t BQ25798_Reset(const BQ25798_Handle_t *dev)
{
  /* Write REG_RST bit in the Termination Control register */
  return BQ25798_ModifyReg(dev, BQ25798_REG_TERM,
                           BQ25798_REG_RST_MSK,
                           (1U << BQ25798_REG_RST_POS));
}

BQ25798_Status_t BQ25798_VerifyPartNumber(const BQ25798_Handle_t *dev, uint8_t *part_info)
{
  if (dev == NULL || dev->hi2c == NULL)
  {
    return BQ25798_ERR_PARAM;
  }

  uint8_t reg_val;
  BQ25798_Status_t status;

  status = BQ25798_ReadReg(dev, BQ25798_REG_PART_INFO, &reg_val);
  if (status != BQ25798_OK)
  {
    return status;
  }

  if (part_info != NULL)
  {
    *part_info = reg_val;
  }

  /* Extract part number field and compare */
  uint8_t pn = (reg_val & BQ25798_PART_NUMBER_MSK) >> BQ25798_PART_NUMBER_POS;
  if (pn != BQ25798_PART_NUMBER_BQ25798)
  {
    return BQ25798_ERR_DEVICE_ID;
  }

  return BQ25798_OK;
}

/* ==========================================================================
 *  Charge Configuration
 * ========================================================================== */

BQ25798_Status_t BQ25798_ConfigureCharging(const BQ25798_Handle_t *dev,
                                            const BQ25798_Charge_Config_t *cfg)
{
  if (dev == NULL || cfg == NULL)
  {
    return BQ25798_ERR_PARAM;
  }

  BQ25798_Status_t status;

  /* Set charge voltage */
  status = BQ25798_SetChargeVoltage(dev, cfg->vreg_mv);
  if (status != BQ25798_OK) return status;

  /* Set charge current */
  status = BQ25798_SetChargeCurrent(dev, cfg->ichg_ma);
  if (status != BQ25798_OK) return status;

  /* Set input voltage limit */
  status = BQ25798_SetInputVoltageLimit(dev, cfg->vindpm_mv);
  if (status != BQ25798_OK) return status;

  /* Set input current limit */
  status = BQ25798_SetInputCurrentLimit(dev, cfg->iindpm_ma);
  if (status != BQ25798_OK) return status;

  /* Set minimum system voltage */
  status = BQ25798_SetMinSysVoltage(dev, cfg->vsysmin_mv);
  if (status != BQ25798_OK) return status;

  /* Set precharge current */
  if (cfg->iprechg_ma > 0U)
  {
    uint16_t iprechg = clamp_u16(cfg->iprechg_ma, BQ25798_IPRECHG_MIN_MA, BQ25798_IPRECHG_MAX_MA);
    uint8_t reg_val = BQ25798_MA_TO_IPRECHG(iprechg);
    status = BQ25798_ModifyReg(dev, BQ25798_REG_PRECHG,
                               BQ25798_IPRECHG_MSK,
                               (reg_val << BQ25798_IPRECHG_POS) & BQ25798_IPRECHG_MSK);
    if (status != BQ25798_OK) return status;
  }

  /* Set termination current */
  if (cfg->iterm_ma > 0U)
  {
    uint16_t iterm = clamp_u16(cfg->iterm_ma, BQ25798_ITERM_MIN_MA, BQ25798_ITERM_MAX_MA);
    uint8_t reg_val = BQ25798_MA_TO_ITERM(iterm);
    status = BQ25798_ModifyReg(dev, BQ25798_REG_TERM,
                               BQ25798_ITERM_MSK,
                               (reg_val << BQ25798_ITERM_POS) & BQ25798_ITERM_MSK);
    if (status != BQ25798_OK) return status;
  }

  /* Set cell count */
  if (cfg->cell_count >= 1U && cfg->cell_count <= 4U)
  {
    uint8_t cell_val = (uint8_t)(cfg->cell_count - 1U);
    status = BQ25798_ModifyReg(dev, BQ25798_REG_RECHRG,
                               BQ25798_CELL_CFG_MSK,
                               (cell_val << BQ25798_CELL_CFG_POS) & BQ25798_CELL_CFG_MSK);
    if (status != BQ25798_OK) return status;
  }

  return BQ25798_OK;
}

BQ25798_Status_t BQ25798_EnableCharging(const BQ25798_Handle_t *dev, bool enable)
{
  uint8_t val = enable ? (1U << BQ25798_EN_CHG_POS) : 0U;
  return BQ25798_ModifyReg(dev, BQ25798_REG_CHG_CTRL0,
                           BQ25798_EN_CHG_MSK, val);
}

BQ25798_Status_t BQ25798_IsChargingEnabled(const BQ25798_Handle_t *dev, bool *enabled)
{
  if (enabled == NULL)
  {
    return BQ25798_ERR_PARAM;
  }

  uint8_t reg_val;
  BQ25798_Status_t status = BQ25798_ReadReg(dev, BQ25798_REG_CHG_CTRL0, &reg_val);
  if (status != BQ25798_OK)
  {
    return status;
  }

  *enabled = (reg_val & BQ25798_EN_CHG_MSK) != 0U;
  return BQ25798_OK;
}

BQ25798_Status_t BQ25798_SetChargeVoltage(const BQ25798_Handle_t *dev, uint16_t vreg_mv)
{
  vreg_mv = clamp_u16(vreg_mv, BQ25798_VREG_MIN_MV, BQ25798_VREG_MAX_MV);
  uint16_t reg_val = BQ25798_MV_TO_VREG(vreg_mv) & BQ25798_VREG_MSK;
  return BQ25798_WriteReg16(dev, BQ25798_REG_VREG, reg_val);
}

BQ25798_Status_t BQ25798_SetChargeCurrent(const BQ25798_Handle_t *dev, uint16_t ichg_ma)
{
  ichg_ma = clamp_u16(ichg_ma, BQ25798_ICHG_MIN_MA, BQ25798_ICHG_MAX_MA);
  uint16_t reg_val = BQ25798_MA_TO_ICHG(ichg_ma) & BQ25798_ICHG_MSK;
  return BQ25798_WriteReg16(dev, BQ25798_REG_ICHG, reg_val);
}

BQ25798_Status_t BQ25798_SetInputVoltageLimit(const BQ25798_Handle_t *dev, uint16_t vindpm_mv)
{
  vindpm_mv = clamp_u16(vindpm_mv, BQ25798_VINDPM_MIN_MV, BQ25798_VINDPM_MAX_MV);
  uint8_t reg_val = BQ25798_MV_TO_VINDPM(vindpm_mv) & BQ25798_VINDPM_MSK;
  return BQ25798_WriteReg(dev, BQ25798_REG_VINDPM, reg_val);
}

BQ25798_Status_t BQ25798_SetInputCurrentLimit(const BQ25798_Handle_t *dev, uint16_t iindpm_ma)
{
  iindpm_ma = clamp_u16(iindpm_ma, BQ25798_IINDPM_MIN_MA, BQ25798_IINDPM_MAX_MA);
  uint16_t reg_val = BQ25798_MA_TO_IINDPM(iindpm_ma) & BQ25798_IINDPM_MSK;
  return BQ25798_WriteReg16(dev, BQ25798_REG_IINDPM, reg_val);
}

BQ25798_Status_t BQ25798_SetMinSysVoltage(const BQ25798_Handle_t *dev, uint16_t vsysmin_mv)
{
  vsysmin_mv = clamp_u16(vsysmin_mv, BQ25798_VSYSMIN_BASE_MV,
                         BQ25798_VSYSMIN_BASE_MV + (BQ25798_VSYSMIN_MSK * BQ25798_VSYSMIN_STEP_MV));
  uint8_t reg_val = BQ25798_MV_TO_VSYSMIN(vsysmin_mv) & BQ25798_VSYSMIN_MSK;
  return BQ25798_ModifyReg(dev, BQ25798_REG_VSYSMIN,
                           BQ25798_VSYSMIN_MSK,
                           (reg_val << BQ25798_VSYSMIN_POS));
}

/* ==========================================================================
 *  OTG (Boost) Mode
 * ========================================================================== */

BQ25798_Status_t BQ25798_ConfigureOTG(const BQ25798_Handle_t *dev,
                                       const BQ25798_OTG_Config_t *cfg)
{
  if (dev == NULL || cfg == NULL)
  {
    return BQ25798_ERR_PARAM;
  }

  BQ25798_Status_t status;

  /* Set OTG voltage */
  uint16_t votg = clamp_u16(cfg->votg_mv, BQ25798_VOTG_MIN_MV, BQ25798_VOTG_MAX_MV);
  uint16_t votg_reg = BQ25798_MV_TO_VOTG(votg) & BQ25798_VOTG_MSK;
  status = BQ25798_WriteReg16(dev, BQ25798_REG_VOTG, votg_reg);
  if (status != BQ25798_OK) return status;

  /* Set OTG current limit (8-bit register, 50mA/bit) */
  uint16_t iotg = clamp_u16(cfg->iotg_ma, BQ25798_IOTG_MIN_MA, BQ25798_IOTG_MAX_MA);
  uint8_t iotg_reg = BQ25798_MA_TO_IOTG(iotg) & BQ25798_IOTG_MSK;
  status = BQ25798_WriteReg(dev, BQ25798_REG_IOTG, iotg_reg);
  if (status != BQ25798_OK) return status;

  return BQ25798_OK;
}

BQ25798_Status_t BQ25798_EnableOTG(const BQ25798_Handle_t *dev, bool enable)
{
  uint8_t val = enable ? (1U << BQ25798_EN_OTG_POS) : 0U;
  return BQ25798_ModifyReg(dev, BQ25798_REG_CHG_CTRL3,
                           BQ25798_EN_OTG_MSK, val);
}

/* ==========================================================================
 *  Status Readback
 * ========================================================================== */

BQ25798_Status_t BQ25798_GetChargerStatus(const BQ25798_Handle_t *dev,
                                           BQ25798_Charger_Status_t *status)
{
  if (dev == NULL || status == NULL)
  {
    return BQ25798_ERR_PARAM;
  }

  uint8_t regs[4];
  BQ25798_Status_t ret;

  /* Read Status 0-3 (registers 0x1B-0x1E) */
  ret = BQ25798_ReadRegs(dev, BQ25798_REG_CHG_STATUS0, regs, 4U);
  if (ret != BQ25798_OK)
  {
    return ret;
  }

  /* Parse Status 0 (0x1B) */
  status->iindpm_stat  = (regs[0] & BQ25798_IINDPM_STAT_MSK)       != 0U;
  status->vindpm_stat  = (regs[0] & BQ25798_VINDPM_STAT_MSK)       != 0U;
  status->wd_fault     = (regs[0] & BQ25798_WD_STAT_MSK)           != 0U;
  status->pg_stat      = (regs[0] & BQ25798_PG_STAT_MSK)           != 0U;
  status->ac2_present  = (regs[0] & BQ25798_AC2_PRESENT_STAT_MSK)  != 0U;
  status->ac1_present  = (regs[0] & BQ25798_AC1_PRESENT_STAT_MSK)  != 0U;
  status->vbus_present = (regs[0] & BQ25798_VBUS_PRESENT_STAT_MSK) != 0U;

  /* Parse Status 1 (0x1C) */
  status->charge_state = (BQ25798_Charge_State_t)((regs[1] & BQ25798_CHG_STAT_MSK) >> BQ25798_CHG_STAT_POS);
  status->input_source = (BQ25798_Input_Source_t)((regs[1] & BQ25798_VBUS_STAT_MSK) >> BQ25798_VBUS_STAT_POS);
  status->bc12_done    = (regs[1] & BQ25798_BC12_DONE_STAT_MSK)    != 0U;

  /* Parse Status 2 (0x1D) */
  status->ico_status   = (regs[2] & BQ25798_ICO_STAT_MSK)          >> BQ25798_ICO_STAT_POS;
  status->treg_stat    = (regs[2] & BQ25798_TREG_STAT_MSK)         != 0U;
  status->dpdm_stat    = (regs[2] & BQ25798_DPDM_STAT_MSK)         != 0U;
  status->vbat_present = (regs[2] & BQ25798_VBAT_PRESENT_STAT_MSK) != 0U;

  /* Parse Status 3 (0x1E) */
  status->adc_done        = (regs[3] & BQ25798_ADC_DONE_STAT_MSK)    != 0U;
  status->vsys_stat       = (regs[3] & BQ25798_VSYS_STAT_MSK)        != 0U;
  status->chg_tmr_stat    = (regs[3] & BQ25798_CHG_TMR_STAT_MSK)     != 0U;
  status->trichg_tmr_stat = (regs[3] & BQ25798_TRICHG_TMR_STAT_MSK)  != 0U;
  status->prechg_tmr_stat = (regs[3] & BQ25798_PRECHG_TMR_STAT_MSK)  != 0U;

  return BQ25798_OK;
}

BQ25798_Status_t BQ25798_GetFaultStatus(const BQ25798_Handle_t *dev,
                                         BQ25798_Fault_Status_t *fault)
{
  if (dev == NULL || fault == NULL)
  {
    return BQ25798_ERR_PARAM;
  }

  uint8_t regs[2];
  BQ25798_Status_t ret;

  /* Read Fault Status 0-1 (registers 0x20-0x21) */
  ret = BQ25798_ReadRegs(dev, BQ25798_REG_FAULT_STATUS0, regs, 2U);
  if (ret != BQ25798_OK)
  {
    return ret;
  }

  /* Parse Fault Status 0 */
  fault->vbus_ovp  = (regs[0] & BQ25798_VBUS_OVP_F_MSK)  != 0U;
  fault->vbat_ovp  = (regs[0] & BQ25798_VBAT_OVP_F_MSK)  != 0U;
  fault->ibus_ocp  = (regs[0] & BQ25798_IBUS_OCP_F_MSK)  != 0U;
  fault->ibat_ocp  = (regs[0] & BQ25798_IBAT_OCP_F_MSK)  != 0U;
  fault->conv_ocp  = (regs[0] & BQ25798_CONV_OCP_F_MSK)  != 0U;
  fault->vac2_ovp  = (regs[0] & BQ25798_VAC2_OVP_F_MSK)  != 0U;
  fault->vac1_ovp  = (regs[0] & BQ25798_VAC1_OVP_F_MSK)  != 0U;

  /* Parse Fault Status 1 */
  fault->vsys_short = (regs[1] & BQ25798_VSYS_SHORT_F_MSK) != 0U;
  fault->vsys_ovp   = (regs[1] & BQ25798_VSYS_OVP_F_MSK)  != 0U;
  fault->otg_ovp    = (regs[1] & BQ25798_OTG_OVP_F_MSK)   != 0U;
  fault->otg_uvp    = (regs[1] & BQ25798_OTG_UVP_F_MSK)   != 0U;
  fault->tshut      = (regs[1] & BQ25798_TSHUT_F_MSK)      != 0U;

  return BQ25798_OK;
}

/* ==========================================================================
 *  ADC Control & Readback
 * ========================================================================== */

BQ25798_Status_t BQ25798_EnableADC(const BQ25798_Handle_t *dev, bool continuous)
{
  /* Set ADC_EN = 1, ADC_RATE = continuous(0) or one-shot(1) */
  uint8_t val = (1U << BQ25798_ADC_EN_POS);
  if (!continuous)
  {
    val |= (BQ25798_ADC_RATE_ONESHOT << BQ25798_ADC_RATE_POS);
  }

  return BQ25798_ModifyReg(dev, BQ25798_REG_ADC_CTRL,
                           BQ25798_ADC_EN_MSK | BQ25798_ADC_RATE_MSK, val);
}

BQ25798_Status_t BQ25798_DisableADC(const BQ25798_Handle_t *dev)
{
  return BQ25798_ModifyReg(dev, BQ25798_REG_ADC_CTRL,
                           BQ25798_ADC_EN_MSK, 0U);
}

BQ25798_Status_t BQ25798_ReadADC(const BQ25798_Handle_t *dev,
                                  BQ25798_ADC_Readings_t *readings)
{
  if (dev == NULL || readings == NULL)
  {
    return BQ25798_ERR_PARAM;
  }

  BQ25798_Status_t status;

  /* IBUS (signed) */
  status = BQ25798_ReadADC16(dev, BQ25798_REG_IBUS_ADC, &readings->ibus_ma);
  if (status != BQ25798_OK) return status;

  /* IBAT (signed) */
  status = BQ25798_ReadADC16(dev, BQ25798_REG_IBAT_ADC, &readings->ibat_ma);
  if (status != BQ25798_OK) return status;

  /* VBUS (unsigned) */
  status = BQ25798_ReadReg16(dev, BQ25798_REG_VBUS_ADC, &readings->vbus_mv);
  if (status != BQ25798_OK) return status;

  /* VBAT (unsigned) */
  status = BQ25798_ReadReg16(dev, BQ25798_REG_VBAT_ADC, &readings->vbat_mv);
  if (status != BQ25798_OK) return status;

  /* VSYS (unsigned) */
  status = BQ25798_ReadReg16(dev, BQ25798_REG_VSYS_ADC, &readings->vsys_mv);
  if (status != BQ25798_OK) return status;

  /* VAC1 (unsigned) */
  status = BQ25798_ReadReg16(dev, BQ25798_REG_VAC1_ADC, &readings->vac1_mv);
  if (status != BQ25798_OK) return status;

  /* VAC2 (unsigned) */
  status = BQ25798_ReadReg16(dev, BQ25798_REG_VAC2_ADC, &readings->vac2_mv);
  if (status != BQ25798_OK) return status;

  /* TDIE (signed, 0.5 deg C/LSB -> multiply by 5 to get deg C x10) */
  int16_t tdie_raw;
  status = BQ25798_ReadADC16(dev, BQ25798_REG_TDIE_ADC, &tdie_raw);
  if (status != BQ25798_OK) return status;
  readings->tdie_degc_x10 = (int16_t)(tdie_raw * (int16_t)BQ25798_TDIE_ADC_STEP_DEGC_X10);

  /* TS (unsigned, raw value) */
  status = BQ25798_ReadReg16(dev, BQ25798_REG_TS_ADC, &readings->ts_pct_x100);
  if (status != BQ25798_OK) return status;

  return BQ25798_OK;
}

BQ25798_Status_t BQ25798_ReadVBAT(const BQ25798_Handle_t *dev, uint16_t *vbat_mv)
{
  if (vbat_mv == NULL)
  {
    return BQ25798_ERR_PARAM;
  }
  return BQ25798_ReadReg16(dev, BQ25798_REG_VBAT_ADC, vbat_mv);
}

BQ25798_Status_t BQ25798_ReadVBUS(const BQ25798_Handle_t *dev, uint16_t *vbus_mv)
{
  if (vbus_mv == NULL)
  {
    return BQ25798_ERR_PARAM;
  }
  return BQ25798_ReadReg16(dev, BQ25798_REG_VBUS_ADC, vbus_mv);
}

BQ25798_Status_t BQ25798_ReadIBAT(const BQ25798_Handle_t *dev, int16_t *ibat_ma)
{
  if (ibat_ma == NULL)
  {
    return BQ25798_ERR_PARAM;
  }
  return BQ25798_ReadADC16(dev, BQ25798_REG_IBAT_ADC, ibat_ma);
}

BQ25798_Status_t BQ25798_ReadDieTemp(const BQ25798_Handle_t *dev, int16_t *temp_degc_x10)
{
  if (temp_degc_x10 == NULL)
  {
    return BQ25798_ERR_PARAM;
  }

  int16_t raw;
  BQ25798_Status_t status = BQ25798_ReadADC16(dev, BQ25798_REG_TDIE_ADC, &raw);
  if (status != BQ25798_OK)
  {
    return status;
  }

  /* 0.5 deg C per LSB -> x5 gives deg C x10 */
  *temp_degc_x10 = (int16_t)(raw * (int16_t)BQ25798_TDIE_ADC_STEP_DEGC_X10);
  return BQ25798_OK;
}

/* ==========================================================================
 *  Watchdog
 * ========================================================================== */

BQ25798_Status_t BQ25798_SetWatchdog(const BQ25798_Handle_t *dev, uint8_t setting)
{
  return BQ25798_ModifyReg(dev, BQ25798_REG_CHG_CTRL1,
                           BQ25798_WATCHDOG_MSK,
                           (setting << BQ25798_WATCHDOG_POS) & BQ25798_WATCHDOG_MSK);
}

BQ25798_Status_t BQ25798_ResetWatchdog(const BQ25798_Handle_t *dev)
{
  return BQ25798_ModifyReg(dev, BQ25798_REG_CHG_CTRL1,
                           BQ25798_WD_RST_MSK,
                           (1U << BQ25798_WD_RST_POS));
}

/* ==========================================================================
 *  Power Mode / Misc Control
 * ========================================================================== */

BQ25798_Status_t BQ25798_SetHiZ(const BQ25798_Handle_t *dev, bool enable)
{
  uint8_t val = enable ? (1U << BQ25798_EN_HIZ_POS) : 0U;
  return BQ25798_ModifyReg(dev, BQ25798_REG_CHG_CTRL0,
                           BQ25798_EN_HIZ_MSK, val);
}

BQ25798_Status_t BQ25798_IgnoreTS(const BQ25798_Handle_t *dev, bool ignore)
{
  uint8_t val = ignore ? (1U << BQ25798_TS_IGNORE_POS) : 0U;
  return BQ25798_ModifyReg(dev, BQ25798_REG_NTC_CTRL0,
                           BQ25798_TS_IGNORE_MSK, val);
}

BQ25798_Status_t BQ25798_EnterShipMode(const BQ25798_Handle_t *dev)
{
  /* Set SDRV_CTRL = SHIP (0x02) in Charger Control 2 register */
  return BQ25798_ModifyReg(dev, BQ25798_REG_CHG_CTRL2,
                           BQ25798_SDRV_CTRL_MSK,
                           (BQ25798_SDRV_SHIP << BQ25798_SDRV_CTRL_POS));
}

BQ25798_Status_t BQ25798_ClearFaults(const BQ25798_Handle_t *dev)
{
  if (dev == NULL)
  {
    return BQ25798_ERR_PARAM;
  }

  uint8_t dummy[4];
  BQ25798_Status_t status;

  /* Read Fault Status 0 & 1 to clear latched faults */
  status = BQ25798_ReadRegs(dev, BQ25798_REG_FAULT_STATUS0, dummy, 2U);
  if (status != BQ25798_OK) return status;

  /* Read Flag registers 0-3 to clear interrupt flags */
  status = BQ25798_ReadRegs(dev, BQ25798_REG_CHG_FLAG0, dummy, 4U);
  return status;
}
