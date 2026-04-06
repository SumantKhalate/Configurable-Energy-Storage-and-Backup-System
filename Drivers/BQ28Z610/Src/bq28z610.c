/**
  ******************************************************************************
  * @file    bq28z610.c
  * @brief   BQ28Z610 Battery Management IC - High-level driver API.
  *          Implements initialization, battery measurements, status monitoring,
  *          and protection management functions.
  *
  * @note    Based on TI BQ28Z610 datasheets (SLUSE07B Rev B, SLUUC81).
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "../Inc/bq28z610.h"

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
 * @brief  Initialize the BQ28Z610 driver and verify device identity.
 */
BQ28Z610_RetVal_t BQ28Z610_Init(BQ28Z610_Handle_t *dev)
{
  if (dev == NULL || dev->hi2c == NULL)
  {
    return BQ28Z610_ERR_PARAM;
  }

  /* Use default I2C address if not set */
  if (dev->i2c_addr == 0U)
  {
    dev->i2c_addr = BQ28Z610_I2C_ADDR_7BIT;
  }

  /* Set default timeout if not set */
  if (dev->timeout == 0U)
  {
    dev->timeout = 100U;
  }

  /* Verify the device type */
  return BQ28Z610_VerifyDeviceType(dev, NULL);
}

/**
 * @brief  Verify the device type matches BQ28Z610.
 */
BQ28Z610_RetVal_t BQ28Z610_VerifyDeviceType(const BQ28Z610_Handle_t *dev, uint16_t *device_type)
{
  if (dev == NULL || dev->hi2c == NULL)
  {
    return BQ28Z610_ERR_PARAM;
  }

  uint16_t dev_type;
  BQ28Z610_RetVal_t status;

  /* Issue device type control command */
  status = BQ28Z610_ControlCommand(dev, BQ28Z610_CTRL_DEVICE_TYPE, &dev_type);
  if (status != BQ28Z610_OK)
  {
    return status;
  }

  if (device_type != NULL)
  {
    *device_type = dev_type;
  }

  /* Verify device type matches expected value */
  if (dev_type != BQ28Z610_DEVICE_TYPE_VALUE)
  {
    return BQ28Z610_ERR_DEVICE_ID;
  }

  return BQ28Z610_OK;
}

/**
 * @brief  Get device firmware version.
 */
BQ28Z610_RetVal_t BQ28Z610_GetFirmwareVersion(const BQ28Z610_Handle_t *dev, uint16_t *fw_version)
{
  if (dev == NULL || fw_version == NULL)
  {
    return BQ28Z610_ERR_PARAM;
  }

  return BQ28Z610_ControlCommand(dev, BQ28Z610_CTRL_FW_VERSION, fw_version);
}

/**
 * @brief  Get device hardware version.
 */
BQ28Z610_RetVal_t BQ28Z610_GetHardwareVersion(const BQ28Z610_Handle_t *dev, uint16_t *hw_version)
{
  if (dev == NULL || hw_version == NULL)
  {
    return BQ28Z610_ERR_PARAM;
  }

  return BQ28Z610_ControlCommand(dev, BQ28Z610_CTRL_HW_VERSION, hw_version);
}

/* ==========================================================================
 *  Battery Measurement Functions
 * ========================================================================== */

/**
 * @brief  Read all battery measurements in one operation.
 */
BQ28Z610_RetVal_t BQ28Z610_GetMeasurements(const BQ28Z610_Handle_t *dev,
                                            BQ28Z610_Measurements_t *measurements)
{
  if (dev == NULL || measurements == NULL)
  {
    return BQ28Z610_ERR_PARAM;
  }

  BQ28Z610_RetVal_t status;

  /* Read voltage */
  status = BQ28Z610_ReadVoltage(dev, &measurements->voltage_mv);
  if (status != BQ28Z610_OK) return status;

  /* Read current */
  status = BQ28Z610_ReadCurrent(dev, &measurements->current_ma);
  if (status != BQ28Z610_OK) return status;

  /* Read average current */
  status = BQ28Z610_ReadReg16(dev, BQ28Z610_CMD_AVG_CURRENT, (uint16_t *)&measurements->avg_current_ma);
  if (status != BQ28Z610_OK) return status;

  /* Read temperature */
  int16_t temp_c_signed;
  status = BQ28Z610_ReadTemperature(dev, &temp_c_signed);
  if (status != BQ28Z610_OK) return status;
  measurements->temp_c = (uint8_t)temp_c_signed;

  /* Read SOC */
  uint8_t soc_temp;
  status = BQ28Z610_ReadStateOfCharge(dev, &soc_temp);
  if (status != BQ28Z610_OK) return status;
  measurements->state_of_charge = (uint16_t)soc_temp;

  /* Read SOH */
  uint8_t soh_temp;
  status = BQ28Z610_ReadStateOfHealth(dev, &soh_temp);
  if (status != BQ28Z610_OK) return status;
  measurements->state_of_health = (uint16_t)soh_temp;

  /* Read remaining capacity */
  status = BQ28Z610_ReadRemainingCapacity(dev, &measurements->remaining_capacity_mah);
  if (status != BQ28Z610_OK) return status;

  /* Read full charge capacity */
  status = BQ28Z610_ReadFullChargeCapacity(dev, &measurements->full_charge_capacity_mah);
  if (status != BQ28Z610_OK) return status;

  /* Read cycle count */
  status = BQ28Z610_ReadCycleCount(dev, &measurements->cycle_count);
  if (status != BQ28Z610_OK) return status;

  return BQ28Z610_OK;
}

/**
 * @brief  Read battery voltage.
 */
BQ28Z610_RetVal_t BQ28Z610_ReadVoltage(const BQ28Z610_Handle_t *dev, uint16_t *volt_mv)
{
  if (dev == NULL || volt_mv == NULL)
  {
    return BQ28Z610_ERR_PARAM;
  }

  return BQ28Z610_ReadReg16(dev, BQ28Z610_CMD_VOLT, volt_mv);
}

/**
 * @brief  Read battery current.
 */
BQ28Z610_RetVal_t BQ28Z610_ReadCurrent(const BQ28Z610_Handle_t *dev, int16_t *curr_ma)
{
  if (dev == NULL || curr_ma == NULL)
  {
    return BQ28Z610_ERR_PARAM;
  }

  return BQ28Z610_ReadReg16(dev, BQ28Z610_CMD_STDBY_CURRENT, (uint16_t *)curr_ma);
}

/**
 * @brief  Read battery temperature.
 */
BQ28Z610_RetVal_t BQ28Z610_ReadTemperature(const BQ28Z610_Handle_t *dev, int16_t *temp_c)
{
  if (dev == NULL || temp_c == NULL)
  {
    return BQ28Z610_ERR_PARAM;
  }

  uint16_t temp_x10;
  BQ28Z610_RetVal_t status;

  status = BQ28Z610_ReadReg16(dev, BQ28Z610_CMD_TEMP, &temp_x10);
  if (status != BQ28Z610_OK)
  {
    return status;
  }

  /* Convert from 0.1K units to °C */
  *temp_c = BQ28Z610_TEMP_TO_DEGC(temp_x10);

  return BQ28Z610_OK;
}

/**
 * @brief  Read state of charge.
 */
BQ28Z610_RetVal_t BQ28Z610_ReadStateOfCharge(const BQ28Z610_Handle_t *dev, uint8_t *soc)
{
  if (dev == NULL || soc == NULL)
  {
    return BQ28Z610_ERR_PARAM;
  }

  return BQ28Z610_ReadReg(dev, BQ28Z610_CMD_STATE_OF_CHARGE, soc);
}

/**
 * @brief  Read state of health.
 */
BQ28Z610_RetVal_t BQ28Z610_ReadStateOfHealth(const BQ28Z610_Handle_t *dev, uint8_t *soh)
{
  if (dev == NULL || soh == NULL)
  {
    return BQ28Z610_ERR_PARAM;
  }

  return BQ28Z610_ReadReg(dev, BQ28Z610_CMD_STATE_OF_HEALTH, soh);
}

/**
 * @brief  Read remaining capacity.
 */
BQ28Z610_RetVal_t BQ28Z610_ReadRemainingCapacity(const BQ28Z610_Handle_t *dev, uint16_t *capacity_mah)
{
  if (dev == NULL || capacity_mah == NULL)
  {
    return BQ28Z610_ERR_PARAM;
  }

  return BQ28Z610_ReadReg16(dev, BQ28Z610_CMD_REM_CAP, capacity_mah);
}

/**
 * @brief  Read full charge capacity.
 */
BQ28Z610_RetVal_t BQ28Z610_ReadFullChargeCapacity(const BQ28Z610_Handle_t *dev, uint16_t *capacity_mah)
{
  if (dev == NULL || capacity_mah == NULL)
  {
    return BQ28Z610_ERR_PARAM;
  }

  return BQ28Z610_ReadReg16(dev, BQ28Z610_CMD_FULL_CHARGE_CAP, capacity_mah);
}

/**
 * @brief  Read cycle count.
 */
BQ28Z610_RetVal_t BQ28Z610_ReadCycleCount(const BQ28Z610_Handle_t *dev, uint16_t *count)
{
  if (dev == NULL || count == NULL)
  {
    return BQ28Z610_ERR_PARAM;
  }

  return BQ28Z610_ReadReg16(dev, BQ28Z610_CMD_CYCLE_COUNT, count);
}

/* ==========================================================================
 *  Status Functions
 * ========================================================================== */

/**
 * @brief  Get and parse all status flags.
 */
BQ28Z610_RetVal_t BQ28Z610_GetStatusFlags(const BQ28Z610_Handle_t *dev, BQ28Z610_StatusFlags_t *flags)
{
  if (dev == NULL || flags == NULL)
  {
    return BQ28Z610_ERR_PARAM;
  }

  uint16_t status_reg;
  BQ28Z610_RetVal_t status;

  status = BQ28Z610_ReadReg16(dev, BQ28Z610_CMD_FLAGS, &status_reg);
  if (status != BQ28Z610_OK)
  {
    return status;
  }

  /* Parse status flags into structure */
  flags->discharging            = (status_reg & BQ28Z610_FLAGS_DSG) ? true : false;
  flags->charging               = (status_reg & BQ28Z610_FLAGS_CHG) ? true : false;
  flags->fully_charged          = (status_reg & BQ28Z610_FLAGS_FC) ? true : false;
  flags->fully_discharged       = (status_reg & BQ28Z610_FLAGS_FD) ? true : false;
  flags->over_temp_charge       = (status_reg & BQ28Z610_FLAGS_OTC) ? true : false;
  flags->over_temp_discharge    = (status_reg & BQ28Z610_FLAGS_OTD) ? true : false;
  flags->under_temp_charge      = (status_reg & BQ28Z610_FLAGS_TBX) ? true : false;

  /* Battery status register flags */
  status = BQ28Z610_GetBatteryStatus(dev, &status_reg);
  if (status != BQ28Z610_OK)
  {
    return status;
  }

  flags->over_charge_voltage    = (status_reg & BQ28Z610_BATT_STAT_OVER_CHG_V) ? true : false;
  flags->over_charge_current    = (status_reg & BQ28Z610_BATT_STAT_OVER_CHG_I) ? true : false;
  flags->under_temp_discharge   = (status_reg & BQ28Z610_BATT_STAT_UNDER_TEMP_D) ? true : false;
  flags->over_current_discharge = (status_reg & BQ28Z610_BATT_STAT_OVER_CURRENT) ? true : false;
  flags->cfet_short             = (status_reg & BQ28Z610_BATT_STAT_CFETshort) ? true : false;
  flags->dfet_short             = (status_reg & BQ28Z610_BATT_STAT_DFETshort) ? true : false;
  flags->charge_timeout         = (status_reg & BQ28Z610_BATT_STAT_CHG_TIMEOUT) ? true : false;
  flags->discharge_timeout      = (status_reg & BQ28Z610_BATT_STAT_DSG_TIMEOUT) ? true : false;

  return BQ28Z610_OK;
}

/**
 * @brief  Get raw battery status register.
 */
BQ28Z610_RetVal_t BQ28Z610_GetBatteryStatus(const BQ28Z610_Handle_t *dev, uint16_t *status)
{
  if (dev == NULL || status == NULL)
  {
    return BQ28Z610_ERR_PARAM;
  }

  return BQ28Z610_ReadReg16(dev, BQ28Z610_CMD_BATTERY_STATUS, status);
}

/* ==========================================================================
 *  Control Functions
 * ========================================================================== */

/**
 * @brief  Reset the BQ28Z610 device.
 */
BQ28Z610_RetVal_t BQ28Z610_Reset(const BQ28Z610_Handle_t *dev)
{
  if (dev == NULL)
  {
    return BQ28Z610_ERR_PARAM;
  }

  return BQ28Z610_ControlCommand(dev, BQ28Z610_CTRL_RESET, NULL);
}

/**
 * @brief  Enter sleep mode.
 */
BQ28Z610_RetVal_t BQ28Z610_Sleep(const BQ28Z610_Handle_t *dev)
{
  if (dev == NULL)
  {
    return BQ28Z610_ERR_PARAM;
  }

  return BQ28Z610_ControlCommand(dev, BQ28Z610_CTRL_SLEEP, NULL);
}

/**
 * @brief  Shutdown device.
 */
BQ28Z610_RetVal_t BQ28Z610_Shutdown(const BQ28Z610_Handle_t *dev)
{
  if (dev == NULL)
  {
    return BQ28Z610_ERR_PARAM;
  }

  return BQ28Z610_ControlCommand(dev, BQ28Z610_CTRL_SHUTDOWN, NULL);
}

/* ==========================================================================
 *  Temperature Sensor Functions
 * ========================================================================== */

/**
 * @brief  Read TS1 thermistor temperature.
 */
BQ28Z610_RetVal_t BQ28Z610_ReadTS1Temperature(const BQ28Z610_Handle_t *dev, int16_t *temp_c)
{
  if (dev == NULL || temp_c == NULL)
  {
    return BQ28Z610_ERR_PARAM;
  }

  uint16_t temp_x10;
  BQ28Z610_RetVal_t status;

  status = BQ28Z610_ReadReg16(dev, BQ28Z610_CMD_TS1_TEMP, &temp_x10);
  if (status != BQ28Z610_OK)
  {
    return status;
  }

  *temp_c = BQ28Z610_TEMP_TO_DEGC(temp_x10);
  return BQ28Z610_OK;
}

/**
 * @brief  Read TS2 thermistor temperature.
 */
BQ28Z610_RetVal_t BQ28Z610_ReadTS2Temperature(const BQ28Z610_Handle_t *dev, int16_t *temp_c)
{
  if (dev == NULL || temp_c == NULL)
  {
    return BQ28Z610_ERR_PARAM;
  }

  uint16_t temp_x10;
  BQ28Z610_RetVal_t status;

  status = BQ28Z610_ReadReg16(dev, BQ28Z610_CMD_TS2_TEMP, &temp_x10);
  if (status != BQ28Z610_OK)
  {
    return status;
  }

  *temp_c = BQ28Z610_TEMP_TO_DEGC(temp_x10);
  return BQ28Z610_OK;
}

/**
 * @brief  Read internal die temperature.
 */
BQ28Z610_RetVal_t BQ28Z610_ReadInternalTemperature(const BQ28Z610_Handle_t *dev, int16_t *temp_c)
{
  if (dev == NULL || temp_c == NULL)
  {
    return BQ28Z610_ERR_PARAM;
  }

  uint16_t temp_x10;
  BQ28Z610_RetVal_t status;

  status = BQ28Z610_ReadReg16(dev, BQ28Z610_CMD_INT_TEMP, &temp_x10);
  if (status != BQ28Z610_OK)
  {
    return status;
  }

  *temp_c = BQ28Z610_TEMP_TO_DEGC(temp_x10);
  return BQ28Z610_OK;
}

/* ==========================================================================
 *  Cell Voltage Functions
 * ========================================================================== */

/**
 * @brief  Read cell 1 voltage.
 */
BQ28Z610_RetVal_t BQ28Z610_ReadCell1Voltage(const BQ28Z610_Handle_t *dev, uint16_t *cell_volt_mv)
{
  if (dev == NULL || cell_volt_mv == NULL)
  {
    return BQ28Z610_ERR_PARAM;
  }

  return BQ28Z610_ReadReg16(dev, BQ28Z610_CMD_CELL1_VOLT, cell_volt_mv);
}

/**
 * @brief  Read cell 2 voltage.
 */
BQ28Z610_RetVal_t BQ28Z610_ReadCell2Voltage(const BQ28Z610_Handle_t *dev, uint16_t *cell_volt_mv)
{
  if (dev == NULL || cell_volt_mv == NULL)
  {
    return BQ28Z610_ERR_PARAM;
  }

  return BQ28Z610_ReadReg16(dev, BQ28Z610_CMD_CELL2_VOLT, cell_volt_mv);
}

/* ==========================================================================
 *  Status Query Functions
 * ========================================================================== */

/**
 * @brief  Check if battery is charging.
 */
BQ28Z610_RetVal_t BQ28Z610_IsCharging(const BQ28Z610_Handle_t *dev, bool *charging)
{
  if (dev == NULL || charging == NULL)
  {
    return BQ28Z610_ERR_PARAM;
  }

  uint16_t flags;
  BQ28Z610_RetVal_t status;

  status = BQ28Z610_ReadReg16(dev, BQ28Z610_CMD_FLAGS, &flags);
  if (status != BQ28Z610_OK)
  {
    return status;
  }

  *charging = (flags & BQ28Z610_FLAGS_CHG) ? true : false;
  return BQ28Z610_OK;
}

/**
 * @brief  Check if battery is discharging.
 */
BQ28Z610_RetVal_t BQ28Z610_IsDischarging(const BQ28Z610_Handle_t *dev, bool *discharging)
{
  if (dev == NULL || discharging == NULL)
  {
    return BQ28Z610_ERR_PARAM;
  }

  uint16_t flags;
  BQ28Z610_RetVal_t status;

  status = BQ28Z610_ReadReg16(dev, BQ28Z610_CMD_FLAGS, &flags);
  if (status != BQ28Z610_OK)
  {
    return status;
  }

  *discharging = (flags & BQ28Z610_FLAGS_DSG) ? true : false;
  return BQ28Z610_OK;
}

/**
 * @brief  Check if battery is fully charged.
 */
BQ28Z610_RetVal_t BQ28Z610_IsFullyCharged(const BQ28Z610_Handle_t *dev, bool *charged)
{
  if (dev == NULL || charged == NULL)
  {
    return BQ28Z610_ERR_PARAM;
  }

  uint16_t flags;
  BQ28Z610_RetVal_t status;

  status = BQ28Z610_ReadReg16(dev, BQ28Z610_CMD_FLAGS, &flags);
  if (status != BQ28Z610_OK)
  {
    return status;
  }

  *charged = (flags & BQ28Z610_FLAGS_FC) ? true : false;
  return BQ28Z610_OK;
}
