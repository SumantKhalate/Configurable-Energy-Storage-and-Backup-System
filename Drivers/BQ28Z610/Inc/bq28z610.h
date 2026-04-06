/**
  ******************************************************************************
  * @file    bq28z610.h
  * @brief   BQ28Z610DRZT-R1 Battery Management IC - Driver API header.
  *          Provides high-level functions for battery monitoring, status
  *          reporting, and protection management.
  *
  * @note    Based on TI BQ28Z610 datasheets (SLUSE07B Rev B, SLUUC81).
  *          I2C address: 0x55 (7-bit).
  ******************************************************************************
  */

#ifndef __BQ28Z610_H
#define __BQ28Z610_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "../../STM32L4xx_HAL_Driver/Inc/stm32l4xx_hal.h"
#include "bq28z610_regs.h"
#include <stdbool.h>
#include <stdint.h>

/* ==========================================================================
 *  Error / Return Codes
 * ========================================================================== */
typedef enum {
  BQ28Z610_OK              =  0,
  BQ28Z610_ERR_I2C         = -1,  /**< I2C communication error */
  BQ28Z610_ERR_DEVICE_ID   = -2,  /**< Device type mismatch */
  BQ28Z610_ERR_PARAM       = -3,  /**< Invalid parameter */
  BQ28Z610_ERR_TIMEOUT     = -4,  /**< Operation timeout */
  BQ28Z610_ERR_FAULT       = -5,  /**< Battery fault detected */
  BQ28Z610_ERR_CHECKSUM    = -6,  /**< BlockData checksum error */
} BQ28Z610_RetVal_t;

/* ==========================================================================
 *  Battery Status Codes
 * ========================================================================== */
typedef enum {
  BQ28Z610_STATUS_OK                = 0x0000,
  BQ28Z610_STATUS_OVER_CHARGE_V     = 0x0001,
  BQ28Z610_STATUS_OVER_CHARGE_I     = 0x0002,
  BQ28Z610_STATUS_OVER_TEMP_CHG     = 0x0004,
  BQ28Z610_STATUS_UNDER_TEMP_CHG    = 0x0008,
  BQ28Z610_STATUS_UNDER_TEMP_DSG    = 0x0010,
  BQ28Z610_STATUS_OVER_CURRENT_DSG  = 0x0020,
  BQ28Z610_STATUS_CFET_SHORT        = 0x0040,
  BQ28Z610_STATUS_DFET_SHORT        = 0x0080,
  BQ28Z610_STATUS_FET_BAD           = 0x0100,
  BQ28Z610_STATUS_AFE_REG_ERR       = 0x0200,
  BQ28Z610_STATUS_CHG_PROTECT       = 0x0400,
  BQ28Z610_STATUS_DSG_PROTECT       = 0x0800,
  BQ28Z610_STATUS_OTHER_FAULT       = 0x1000,
  BQ28Z610_STATUS_CHG_TIMEOUT       = 0x2000,
  BQ28Z610_STATUS_DSG_TIMEOUT       = 0x4000,
} BQ28Z610_BatteryStatus_t;

/* ==========================================================================
 *  Battery Measurements
 * ========================================================================== */
typedef struct {
  uint16_t voltage_mv;              /**< Battery voltage in mV */
  int16_t  current_ma;              /**< Battery current in mA (+ = charging) */
  int16_t  avg_current_ma;          /**< Average current in mA */
  uint8_t  temp_c;                  /**< Battery temperature in °C */
  uint16_t state_of_charge;         /**< State of charge in % (0-100) */
  uint16_t state_of_health;         /**< State of health in % (0-100) */
  uint16_t remaining_capacity_mah;  /**< Remaining capacity in mAh */
  uint16_t full_charge_capacity_mah;/**< Full charge capacity in mAh */
  uint16_t cycle_count;             /**< Charge/discharge cycle count */
} BQ28Z610_Measurements_t;

/* ==========================================================================
 *  Battery Status Flags (Bitfield)
 * ========================================================================== */
typedef struct {
  bool discharging;                 /**< Battery is discharging */
  bool charging;                    /**< Battery is charging */
  bool ocv_reading;                 /**< Open-circuit voltage mode */
  bool fully_charged;               /**< Battery fully charged */
  bool fully_discharged;            /**< Battery fully discharged */
  bool over_charge_voltage;         /**< Over-charge voltage condition */
  bool over_charge_current;         /**< Over-charge current condition */
  bool over_temp_charge;            /**< Over-temperature during charge */
  bool over_temp_discharge;         /**< Over-temperature during discharge */
  bool under_temp_charge;           /**< Under-temperature during charge */
  bool under_temp_discharge;        /**< Under-temperature during discharge */
  bool over_current_discharge;      /**< Over-current discharge condition */
  bool cfet_short;                  /**< Charge FET short-circuit */
  bool dfet_short;                  /**< Discharge FET short-circuit */
  bool charge_timeout;              /**< Charge operation timeout */
  bool discharge_timeout;           /**< Discharge operation timeout */
} BQ28Z610_StatusFlags_t;

/* ==========================================================================
 *  Device Handle
 * ========================================================================== */
typedef struct {
  I2C_HandleTypeDef *hi2c;     /**< Pointer to HAL I2C handle */
  uint16_t          i2c_addr;  /**< 7-bit I2C address (default 0x55) */
  uint32_t          timeout;   /**< I2C timeout in ms */
} BQ28Z610_Handle_t;

/* ==========================================================================
 *  Register-Level Access API (implemented in bq28z610_regs.c)
 * ========================================================================== */

/**
 * @brief  Read a single 8-bit register.
 */
BQ28Z610_RetVal_t BQ28Z610_ReadReg(const BQ28Z610_Handle_t *dev, uint8_t reg, uint8_t *data);

/**
 * @brief  Write a single 8-bit register.
 */
BQ28Z610_RetVal_t BQ28Z610_WriteReg(const BQ28Z610_Handle_t *dev, uint8_t reg, uint8_t data);

/**
 * @brief  Read multiple consecutive registers (SMBus block read).
 */
BQ28Z610_RetVal_t BQ28Z610_ReadRegs(const BQ28Z610_Handle_t *dev, uint8_t reg, uint8_t *data, uint16_t len);

/**
 * @brief  Write multiple consecutive registers (SMBus block write).
 */
BQ28Z610_RetVal_t BQ28Z610_WriteRegs(const BQ28Z610_Handle_t *dev, uint8_t reg, const uint8_t *data, uint16_t len);

/**
 * @brief  Read a 16-bit register value (little-endian: LSB at lower address).
 */
BQ28Z610_RetVal_t BQ28Z610_ReadReg16(const BQ28Z610_Handle_t *dev, uint8_t reg, uint16_t *data);

/**
 * @brief  Write a 16-bit register value (little-endian).
 */
BQ28Z610_RetVal_t BQ28Z610_WriteReg16(const BQ28Z610_Handle_t *dev, uint8_t reg, uint16_t data);

/**
 * @brief  Issue a control sub-command and read the response.
 */
BQ28Z610_RetVal_t BQ28Z610_ControlCommand(const BQ28Z610_Handle_t *dev, uint16_t cmd, uint16_t *response);

/* ==========================================================================
 *  High-Level Driver API (implemented in bq28z610.c)
 * ========================================================================== */

/**
 * @brief  Initialize the BQ28Z610 driver and verify device identity.
 * @param  dev    Device handle (hi2c, i2c_addr, timeout must be set)
 * @retval BQ28Z610_RetVal_t
 */
BQ28Z610_RetVal_t BQ28Z610_Init(BQ28Z610_Handle_t *dev);

/**
 * @brief  Verify the device type matches BQ28Z610.
 * @param  dev           Device handle
 * @param  device_type   Pointer to store device type value (can be NULL)
 * @retval BQ28Z610_RetVal_t
 */
BQ28Z610_RetVal_t BQ28Z610_VerifyDeviceType(const BQ28Z610_Handle_t *dev, uint16_t *device_type);

/**
 * @brief  Get device firmware version.
 * @param  dev           Device handle
 * @param  fw_version    Pointer to store firmware version
 * @retval BQ28Z610_RetVal_t
 */
BQ28Z610_RetVal_t BQ28Z610_GetFirmwareVersion(const BQ28Z610_Handle_t *dev, uint16_t *fw_version);

/**
 * @brief  Get device hardware version.
 * @param  dev           Device handle
 * @param  hw_version    Pointer to store hardware version
 * @retval BQ28Z610_RetVal_t
 */
BQ28Z610_RetVal_t BQ28Z610_GetHardwareVersion(const BQ28Z610_Handle_t *dev, uint16_t *hw_version);

/**
 * @brief  Read all battery measurements (voltage, current, temp, SOC, etc.).
 * @param  dev          Device handle
 * @param  measurements Pointer to store measurement data
 * @retval BQ28Z610_RetVal_t
 */
BQ28Z610_RetVal_t BQ28Z610_GetMeasurements(const BQ28Z610_Handle_t *dev,
                                            BQ28Z610_Measurements_t *measurements);

/**
 * @brief  Read battery voltage from ADC.
 * @param  dev       Device handle
 * @param  volt_mv   Pointer to store battery voltage in mV
 * @retval BQ28Z610_RetVal_t
 */
BQ28Z610_RetVal_t BQ28Z610_ReadVoltage(const BQ28Z610_Handle_t *dev, uint16_t *volt_mv);

/**
 * @brief  Read battery current from ADC.
 * @param  dev       Device handle
 * @param  curr_ma   Pointer to store battery current in mA (signed)
 * @retval BQ28Z610_RetVal_t
 */
BQ28Z610_RetVal_t BQ28Z610_ReadCurrent(const BQ28Z610_Handle_t *dev, int16_t *curr_ma);

/**
 * @brief  Read battery temperature.
 * @param  dev      Device handle
 * @param  temp_c   Pointer to store temperature in °C
 * @retval BQ28Z610_RetVal_t
 */
BQ28Z610_RetVal_t BQ28Z610_ReadTemperature(const BQ28Z610_Handle_t *dev, int16_t *temp_c);

/**
 * @brief  Read state of charge (battery level percentage).
 * @param  dev   Device handle
 * @param  soc   Pointer to store SOC in % (0-100)
 * @retval BQ28Z610_RetVal_t
 */
BQ28Z610_RetVal_t BQ28Z610_ReadStateOfCharge(const BQ28Z610_Handle_t *dev, uint8_t *soc);

/**
 * @brief  Read state of health (battery health percentage).
 * @param  dev   Device handle
 * @param  soh   Pointer to store SOH in % (0-100)
 * @retval BQ28Z610_RetVal_t
 */
BQ28Z610_RetVal_t BQ28Z610_ReadStateOfHealth(const BQ28Z610_Handle_t *dev, uint8_t *soh);

/**
 * @brief  Read remaining capacity.
 * @param  dev           Device handle
 * @param  capacity_mah  Pointer to store remaining capacity in mAh
 * @retval BQ28Z610_RetVal_t
 */
BQ28Z610_RetVal_t BQ28Z610_ReadRemainingCapacity(const BQ28Z610_Handle_t *dev, uint16_t *capacity_mah);

/**
 * @brief  Read full charge capacity.
 * @param  dev           Device handle
 * @param  capacity_mah  Pointer to store full charge capacity in mAh
 * @retval BQ28Z610_RetVal_t
 */
BQ28Z610_RetVal_t BQ28Z610_ReadFullChargeCapacity(const BQ28Z610_Handle_t *dev, uint16_t *capacity_mah);

/**
 * @brief  Read cycle count.
 * @param  dev   Device handle
 * @param  count Pointer to store cycle count
 * @retval BQ28Z610_RetVal_t
 */
BQ28Z610_RetVal_t BQ28Z610_ReadCycleCount(const BQ28Z610_Handle_t *dev, uint16_t *count);

/**
 * @brief  Read and parse all status flags.
 * @param  dev    Device handle
 * @param  flags  Pointer to store parsed status flags
 * @retval BQ28Z610_RetVal_t
 */
BQ28Z610_RetVal_t BQ28Z610_GetStatusFlags(const BQ28Z610_Handle_t *dev, BQ28Z610_StatusFlags_t *flags);

/**
 * @brief  Read raw battery status register.
 * @param  dev    Device handle
 * @param  status Pointer to store raw status value
 * @retval BQ28Z610_RetVal_t
 */
BQ28Z610_RetVal_t BQ28Z610_GetBatteryStatus(const BQ28Z610_Handle_t *dev, uint16_t *status);

/**
 * @brief  Reset the BQ28Z610 device (soft reset).
 * @param  dev    Device handle
 * @retval BQ28Z610_RetVal_t
 */
BQ28Z610_RetVal_t BQ28Z610_Reset(const BQ28Z610_Handle_t *dev);

/**
 * @brief  Enter sleep mode (low-power mode).
 * @param  dev    Device handle
 * @retval BQ28Z610_RetVal_t
 */
BQ28Z610_RetVal_t BQ28Z610_Sleep(const BQ28Z610_Handle_t *dev);

/**
 * @brief  Shutdown device.
 * @param  dev    Device handle
 * @retval BQ28Z610_RetVal_t
 */
BQ28Z610_RetVal_t BQ28Z610_Shutdown(const BQ28Z610_Handle_t *dev);

/**
 * @brief  Read external thermistor temperature (TS1).
 * @param  dev      Device handle
 * @param  temp_c   Pointer to store TS1 temperature in °C
 * @retval BQ28Z610_RetVal_t
 */
BQ28Z610_RetVal_t BQ28Z610_ReadTS1Temperature(const BQ28Z610_Handle_t *dev, int16_t *temp_c);

/**
 * @brief  Read external thermistor temperature (TS2).
 * @param  dev      Device handle
 * @param  temp_c   Pointer to store TS2 temperature in °C
 * @retval BQ28Z610_RetVal_t
 */
BQ28Z610_RetVal_t BQ28Z610_ReadTS2Temperature(const BQ28Z610_Handle_t *dev, int16_t *temp_c);

/**
 * @brief  Read internal die temperature.
 * @param  dev      Device handle
 * @param  temp_c   Pointer to store die temperature in °C
 * @retval BQ28Z610_RetVal_t
 */
BQ28Z610_RetVal_t BQ28Z610_ReadInternalTemperature(const BQ28Z610_Handle_t *dev, int16_t *temp_c);

/**
 * @brief  Read cell 1 voltage.
 * @param  dev           Device handle
 * @param  cell_volt_mv  Pointer to store cell voltage in mV
 * @retval BQ28Z610_RetVal_t
 */
BQ28Z610_RetVal_t BQ28Z610_ReadCell1Voltage(const BQ28Z610_Handle_t *dev, uint16_t *cell_volt_mv);

/**
 * @brief  Read cell 2 voltage.
 * @param  dev           Device handle
 * @param  cell_volt_mv  Pointer to store cell voltage in mV
 * @retval BQ28Z610_RetVal_t
 */
BQ28Z610_RetVal_t BQ28Z610_ReadCell2Voltage(const BQ28Z610_Handle_t *dev, uint16_t *cell_volt_mv);

/**
 * @brief  Check if battery is charging.
 * @param  dev       Device handle
 * @param  charging  Pointer to store charging status
 * @retval BQ28Z610_RetVal_t
 */
BQ28Z610_RetVal_t BQ28Z610_IsCharging(const BQ28Z610_Handle_t *dev, bool *charging);

/**
 * @brief  Check if battery is discharging.
 * @param  dev           Device handle
 * @param  discharging   Pointer to store discharging status
 * @retval BQ28Z610_RetVal_t
 */
BQ28Z610_RetVal_t BQ28Z610_IsDischarging(const BQ28Z610_Handle_t *dev, bool *discharging);

/**
 * @brief  Check if battery is fully charged.
 * @param  dev     Device handle
 * @param  charged Pointer to store fully charged status
 * @retval BQ28Z610_RetVal_t
 */
BQ28Z610_RetVal_t BQ28Z610_IsFullyCharged(const BQ28Z610_Handle_t *dev, bool *charged);

#ifdef __cplusplus
}
#endif

#endif /* __BQ28Z610_H */
