/**
  ******************************************************************************
  * @file    tps55288.c
  * @brief   TPS55288RPMR Dual-Channel Integrated Voltage Regulator - Driver.
  *          Implements high-level API for device initialization, channel
  *          configuration, monitoring, and protection control.
  *
  * @note    Based on TI TPS55288 datasheet (SLVSF01B Revision B, December 2020).
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "../Inc/tps55288.h"

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

/**
 * @brief  Initialize the TPS55288 driver and verify device identity.
 */
TPS55288_RetVal_t TPS55288_Init(TPS55288_Handle_t *dev)
{
  if (dev == NULL || dev->hi2c == NULL)
  {
    return TPS55288_ERR_PARAM;
  }

  /* Use default I2C address if not set */
  if (dev->i2c_addr == 0U)
  {
    dev->i2c_addr = TPS55288_I2C_ADDR_7BIT;
  }

  /* Set default timeout if not set */
  if (dev->timeout == 0U)
  {
    dev->timeout = 100U;
  }

  /* Verify the chip ID */
  return TPS55288_VerifyChipID(dev, NULL);
}

/**
 * @brief  Verify the chip ID matches TPS55288.
 */
TPS55288_RetVal_t TPS55288_VerifyChipID(const TPS55288_Handle_t *dev, uint8_t *chip_id)
{
  if (dev == NULL || dev->hi2c == NULL)
  {
    return TPS55288_ERR_PARAM;
  }

  uint8_t reg_val;
  TPS55288_RetVal_t status;

  status = TPS55288_ReadReg(dev, TPS55288_REG_CHIPID, &reg_val);
  if (status != TPS55288_OK)
  {
    return status;
  }

  if (chip_id != NULL)
  {
    *chip_id = reg_val;
  }

  /* Verify chip ID matches expected value */
  if (reg_val != TPS55288_CHIPID_DEFAULT)
  {
    return TPS55288_ERR_DEVICE_ID;
  }

  return TPS55288_OK;
}

/* ==========================================================================
 *  Channel Configuration
 * ========================================================================== */

/**
 * @brief  Configure a single channel.
 */
TPS55288_RetVal_t TPS55288_ConfigureChannel(const TPS55288_Handle_t *dev,
                                             TPS55288_Channel_t ch,
                                             const TPS55288_Channel_Config_t *cfg)
{
  if (dev == NULL || cfg == NULL || (ch != TPS55288_CH1 && ch != TPS55288_CH2))
  {
    return TPS55288_ERR_PARAM;
  }

  TPS55288_RetVal_t status;

  /* Set mode */
  status = TPS55288_SetMode(dev, ch, cfg->mode);
  if (status != TPS55288_OK) return status;

  /* Set output voltage */
  status = TPS55288_SetVoltage(dev, ch, cfg->vout_mv);
  if (status != TPS55288_OK) return status;

  /* Set current limit */
  status = TPS55288_SetCurrentLimit(dev, ch, cfg->ilim_ma);
  if (status != TPS55288_OK) return status;

  /* Enable or disable channel */
  status = TPS55288_EnableChannel(dev, ch, cfg->enabled);
  if (status != TPS55288_OK) return status;

  return TPS55288_OK;
}

/**
 * @brief  Configure device-wide settings.
 */
TPS55288_RetVal_t TPS55288_ConfigureDevice(const TPS55288_Handle_t *dev,
                                            const TPS55288_Device_Config_t *cfg)
{
  if (dev == NULL || cfg == NULL)
  {
    return TPS55288_ERR_PARAM;
  }

  TPS55288_RetVal_t status;

  /* Set switching frequency */
  status = TPS55288_SetFrequency(dev, cfg->freq);
  if (status != TPS55288_OK) return status;

  /* Set tracking mode */
  status = TPS55288_SetTrackingMode(dev, cfg->track_mode);
  if (status != TPS55288_OK) return status;

  /* Set ramp rate */
  status = TPS55288_SetRampRate(dev, cfg->ramp_rate);
  if (status != TPS55288_OK) return status;

  /* Set thermal protection */
  status = TPS55288_SetThermalProtection(dev, cfg->thermal_protection);
  if (status != TPS55288_OK) return status;

  return TPS55288_OK;
}

/* ==========================================================================
 *  Channel Control Functions
 * ========================================================================== */

/**
 * @brief  Enable or disable a channel.
 */
TPS55288_RetVal_t TPS55288_EnableChannel(const TPS55288_Handle_t *dev,
                                          TPS55288_Channel_t ch,
                                          bool enable)
{
  if (dev == NULL || (ch != TPS55288_CH1 && ch != TPS55288_CH2))
  {
    return TPS55288_ERR_PARAM;
  }

  uint8_t mask = (ch == TPS55288_CH1) ? TPS55288_EN1_MSK : TPS55288_EN2_MSK;
  uint8_t value = enable ? mask : 0U;

  return TPS55288_ModifyReg(dev, TPS55288_REG_ENABLE, mask, value);
}

/**
 * @brief  Set operating mode for a channel.
 */
TPS55288_RetVal_t TPS55288_SetMode(const TPS55288_Handle_t *dev,
                                    TPS55288_Channel_t ch,
                                    TPS55288_Mode_t mode)
{
  if (dev == NULL || (ch != TPS55288_CH1 && ch != TPS55288_CH2) || mode > 2U)
  {
    return TPS55288_ERR_PARAM;
  }

  uint8_t pos = (ch == TPS55288_CH1) ? TPS55288_MODE1_POS : TPS55288_MODE2_POS;
  uint8_t mask = (ch == TPS55288_CH1) ? TPS55288_MODE1_MSK : TPS55288_MODE2_MSK;
  uint8_t value = (uint8_t)(mode << pos) & mask;

  return TPS55288_ModifyReg(dev, TPS55288_REG_MODECFG, mask, value);
}

/**
 * @brief  Set output voltage for a channel.
 */
TPS55288_RetVal_t TPS55288_SetVoltage(const TPS55288_Handle_t *dev,
                                       TPS55288_Channel_t ch,
                                       uint16_t vout_mv)
{
  if (dev == NULL || (ch != TPS55288_CH1 && ch != TPS55288_CH2))
  {
    return TPS55288_ERR_PARAM;
  }

  /* Clamp voltage to valid range */
  uint16_t clamped_mv = clamp_u16(vout_mv, TPS55288_VOUT1_BASE_MV, TPS55288_VOUT1_MAX_MV);

  /* Convert voltage to DAC value */
  uint8_t dac_val = (uint8_t)TPS55288_MV_TO_VOUT_DAC(clamped_mv);

  /* Write to appropriate register */
  uint8_t reg = (ch == TPS55288_CH1) ? TPS55288_REG_VOUT1 : TPS55288_REG_VOUT2;

  return TPS55288_WriteReg(dev, reg, dac_val);
}

/**
 * @brief  Get current output voltage setting for a channel.
 */
TPS55288_RetVal_t TPS55288_GetVoltage(const TPS55288_Handle_t *dev,
                                       TPS55288_Channel_t ch,
                                       uint16_t *vout_mv)
{
  if (dev == NULL || (ch != TPS55288_CH1 && ch != TPS55288_CH2) || vout_mv == NULL)
  {
    return TPS55288_ERR_PARAM;
  }

  uint8_t reg = (ch == TPS55288_CH1) ? TPS55288_REG_VOUT1 : TPS55288_REG_VOUT2;
  uint8_t dac_val;

  TPS55288_RetVal_t status = TPS55288_ReadReg(dev, reg, &dac_val);
  if (status != TPS55288_OK)
  {
    return status;
  }

  /* Convert DAC value back to voltage */
  *vout_mv = TPS55288_VOUT_DAC_TO_MV(dac_val);

  return TPS55288_OK;
}

/**
 * @brief  Set current limit for a channel.
 */
TPS55288_RetVal_t TPS55288_SetCurrentLimit(const TPS55288_Handle_t *dev,
                                            TPS55288_Channel_t ch,
                                            uint16_t ilim_ma)
{
  if (dev == NULL || (ch != TPS55288_CH1 && ch != TPS55288_CH2))
  {
    return TPS55288_ERR_PARAM;
  }

  /* Clamp current limit to valid range */
  uint16_t clamped_ma = clamp_u16(ilim_ma, TPS55288_ILIM1_BASE_MA, TPS55288_ILIM1_MAX_MA);

  /* Convert current to DAC value */
  uint8_t dac_val = (uint8_t)TPS55288_MA_TO_ILIM_DAC(clamped_ma);

  /* Ensure DAC value fits in the available bits [6:0] */
  dac_val &= 0x7FU;

  /* Write to appropriate register */
  uint8_t reg = (ch == TPS55288_CH1) ? TPS55288_REG_ILIM1 : TPS55288_REG_ILIM2;

  return TPS55288_WriteReg(dev, reg, dac_val);
}

/**
 * @brief  Get current limit for a channel.
 */
TPS55288_RetVal_t TPS55288_GetCurrentLimit(const TPS55288_Handle_t *dev,
                                            TPS55288_Channel_t ch,
                                            uint16_t *ilim_ma)
{
  if (dev == NULL || (ch != TPS55288_CH1 && ch != TPS55288_CH2) || ilim_ma == NULL)
  {
    return TPS55288_ERR_PARAM;
  }

  uint8_t reg = (ch == TPS55288_CH1) ? TPS55288_REG_ILIM1 : TPS55288_REG_ILIM2;
  uint8_t dac_val;

  TPS55288_RetVal_t status = TPS55288_ReadReg(dev, reg, &dac_val);
  if (status != TPS55288_OK)
  {
    return status;
  }

  /* Mask to valid bits and convert DAC value back to current */
  dac_val &= 0x7FU;
  *ilim_ma = TPS55288_ILIM_DAC_TO_MA(dac_val);

  return TPS55288_OK;
}

/* ==========================================================================
 *  Device Configuration Functions
 * ========================================================================== */

/**
 * @brief  Set switching frequency.
 */
TPS55288_RetVal_t TPS55288_SetFrequency(const TPS55288_Handle_t *dev,
                                         TPS55288_Frequency_t freq)
{
  if (dev == NULL || freq > 7U)
  {
    return TPS55288_ERR_PARAM;
  }

  return TPS55288_ModifyReg(dev, TPS55288_REG_FREQ,
                             TPS55288_FREQ_MSK,
                             (uint8_t)freq & TPS55288_FREQ_MSK);
}

/**
 * @brief  Set voltage ramp rate.
 */
TPS55288_RetVal_t TPS55288_SetRampRate(const TPS55288_Handle_t *dev,
                                        TPS55288_RampRate_t ramp_rate)
{
  if (dev == NULL || ramp_rate > 3U)
  {
    return TPS55288_ERR_PARAM;
  }

  return TPS55288_ModifyReg(dev, TPS55288_REG_TRACK,
                             TPS55288_RAMP_RATE_MSK,
                             ((uint8_t)ramp_rate << TPS55288_RAMP_RATE_POS) & TPS55288_RAMP_RATE_MSK);
}

/**
 * @brief  Set channel tracking mode.
 */
TPS55288_RetVal_t TPS55288_SetTrackingMode(const TPS55288_Handle_t *dev,
                                            TPS55288_TrackMode_t track_mode)
{
  if (dev == NULL || track_mode > 3U)
  {
    return TPS55288_ERR_PARAM;
  }

  return TPS55288_ModifyReg(dev, TPS55288_REG_TRACK,
                             TPS55288_TRACK_MODE_MSK,
                             (uint8_t)track_mode & TPS55288_TRACK_MODE_MSK);
}

/* ==========================================================================
 *  Monitoring / Status Functions
 * ========================================================================== */

/**
 * @brief  Read device status.
 */
TPS55288_RetVal_t TPS55288_GetStatus(const TPS55288_Handle_t *dev,
                                      TPS55288_Status_Info_t *status)
{
  if (dev == NULL || status == NULL)
  {
    return TPS55288_ERR_PARAM;
  }

  TPS55288_RetVal_t driver_status;
  uint8_t reg_val;

  /* Read voltage monitoring register */
  driver_status = TPS55288_ReadReg(dev, TPS55288_REG_VOLTMON, &reg_val);
  if (driver_status != TPS55288_OK)
  {
    return driver_status;
  }

  status->ch1_uvp = (reg_val & TPS55288_UVP1_MSK) ? true : false;
  status->ch1_ovp = (reg_val & TPS55288_OVP1_MSK) ? true : false;
  status->ch2_uvp = (reg_val & TPS55288_UVP2_MSK) ? true : false;
  status->ch2_ovp = (reg_val & TPS55288_OVP2_MSK) ? true : false;

  /* Read status register */
  driver_status = TPS55288_ReadReg(dev, TPS55288_REG_STATUS, &reg_val);
  if (driver_status != TPS55288_OK)
  {
    return driver_status;
  }

  status->ch1_active = (reg_val & TPS55288_ACTIVE1_MSK) ? true : false;
  status->ch2_active = (reg_val & TPS55288_ACTIVE2_MSK) ? true : false;
  status->ch1_interrupt = (reg_val & TPS55288_INT1_MSK) ? true : false;
  status->ch2_interrupt = (reg_val & TPS55288_INT2_MSK) ? true : false;

  /* Read die temperature */
  driver_status = TPS55288_ReadReg(dev, TPS55288_REG_THERMAL, &reg_val);
  if (driver_status != TPS55288_OK)
  {
    return driver_status;
  }

  status->die_temp_c = reg_val;

  return TPS55288_OK;
}

/**
 * @brief  Read output current measurements.
 */
TPS55288_RetVal_t TPS55288_GetMeasurements(const TPS55288_Handle_t *dev,
                                            TPS55288_Measurements_t *measurements)
{
  if (dev == NULL || measurements == NULL)
  {
    return TPS55288_ERR_PARAM;
  }

  uint8_t reg_val;
  TPS55288_RetVal_t status;

  status = TPS55288_ReadReg(dev, TPS55288_REG_IOUT, &reg_val);
  if (status != TPS55288_OK)
  {
    return status;
  }

  /* Extract channel 1 current [3:0] */
  uint8_t iout1_val = (reg_val >> TPS55288_IOUT1_POS) & 0x0FU;
  measurements->ch1_iout_ma = (uint16_t)iout1_val * TPS55288_IOUT1_STEP_MA;

  /* Extract channel 2 current [7:4] */
  uint8_t iout2_val = (reg_val >> TPS55288_IOUT2_POS) & 0x0FU;
  measurements->ch2_iout_ma = (uint16_t)iout2_val * TPS55288_IOUT2_STEP_MA;

  return TPS55288_OK;
}

/**
 * @brief  Get die temperature.
 */
TPS55288_RetVal_t TPS55288_GetDieTemperature(const TPS55288_Handle_t *dev,
                                              uint8_t *temp_c)
{
  if (dev == NULL || temp_c == NULL)
  {
    return TPS55288_ERR_PARAM;
  }

  return TPS55288_ReadReg(dev, TPS55288_REG_THERMAL, temp_c);
}

/**
 * @brief  Enable or disable thermal protection.
 */
TPS55288_RetVal_t TPS55288_SetThermalProtection(const TPS55288_Handle_t *dev,
                                                 bool enable)
{
  if (dev == NULL)
  {
    return TPS55288_ERR_PARAM;
  }

  uint8_t value = enable ? 0U : TPS55288_THERMAL_DIS_MSK;

  return TPS55288_ModifyReg(dev, TPS55288_REG_PROTECT,
                             TPS55288_THERMAL_DIS_MSK,
                             value);
}

/**
 * @brief  Clear interrupt flags.
 */
TPS55288_RetVal_t TPS55288_ClearInterrupts(const TPS55288_Handle_t *dev)
{
  if (dev == NULL)
  {
    return TPS55288_ERR_PARAM;
  }

  /* Clear interrupt flags by writing 0 to them */
  return TPS55288_ModifyReg(dev, TPS55288_REG_STATUS,
                             (TPS55288_INT1_MSK | TPS55288_INT2_MSK),
                             0U);
}

/* ==========================================================================
 *  Convenience Functions
 * ========================================================================== */

/**
 * @brief  Perform a complete channel setup.
 */
TPS55288_RetVal_t TPS55288_SetupChannel(const TPS55288_Handle_t *dev,
                                         TPS55288_Channel_t ch,
                                         const TPS55288_Channel_Config_t *cfg)
{
  if (dev == NULL || cfg == NULL || (ch != TPS55288_CH1 && ch != TPS55288_CH2))
  {
    return TPS55288_ERR_PARAM;
  }

  /* Use the main configuration function */
  return TPS55288_ConfigureChannel(dev, ch, cfg);
}
