/**
  ******************************************************************************
  * @file    bq28z610_regs.h
  * @brief   BQ28Z610DRZT-R1 Battery Management IC - Register map, bit definitions,
  *          and conversion macros.
  *
  * @note    Based on TI BQ28Z610 datasheets:
  *          - SLUSE07B Revision B (January 2022)
  *          - SLUUC81 (January 2020) Technical Reference
  *          Default I2C address: 0x55 (7-bit).
  *          Standard and Block Data (SMBus) support.
  ******************************************************************************
  */

#ifndef __BQ28Z610_REGS_H
#define __BQ28Z610_REGS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* ==========================================================================
 *  I2C ADDRESS (SMBus Standard)
 * ========================================================================== */
#define BQ28Z610_I2C_ADDR_7BIT          0x55U
#define BQ28Z610_I2C_ADDR_WRITE         (BQ28Z610_I2C_ADDR_7BIT << 1)        /* 0xAA */
#define BQ28Z610_I2C_ADDR_READ          ((BQ28Z610_I2C_ADDR_7BIT << 1) | 1U) /* 0xAB */

/* ==========================================================================
 *  STANDARD DATA COMMANDS (0x00-0x2F)
 * ========================================================================== */
#define BQ28Z610_CMD_CONTROL            0x00U  /* Control register (2-byte command) */
#define BQ28Z610_CMD_TEMP               0x02U  /* Temperature in 0.1K (2 bytes, little-endian) */
#define BQ28Z610_CMD_VOLT               0x04U  /* Voltage in mV (2 bytes, little-endian) */
#define BQ28Z610_CMD_FLAGS              0x06U  /* Flags register (2 bytes) */
#define BQ28Z610_CMD_NOMINAL_AVL_CAP    0x08U  /* Nominal available capacity in mAh (2 bytes) */
#define BQ28Z610_CMD_FULL_CHARGE_CAP    0x0AU  /* Full charge capacity in mAh (2 bytes) */
#define BQ28Z610_CMD_REM_CAP            0x0CU  /* Remaining capacity in mAh (2 bytes) */
#define BQ28Z610_CMD_FULL_CHARGE_CAP2   0x0EU  /* Full charge capacity (repeat) */
#define BQ28Z610_CMD_AVG_CURRENT        0x10U  /* Average current in mA (2 bytes, signed) */
#define BQ28Z610_CMD_STDBY_CURRENT      0x12U  /* Standby current in mA (2 bytes, signed) */
#define BQ28Z610_CMD_MAX_LOAD_CURRENT   0x14U  /* Max load current in mA (2 bytes) */
#define BQ28Z610_CMD_AVG_POWER          0x16U  /* Average power in mW (2 bytes, signed) */
#define BQ28Z610_CMD_STATE_OF_CHARGE    0x18U  /* State of charge in % (1 byte) */
#define BQ28Z610_CMD_CYCLE_COUNT        0x1AU  /* Cycle count (2 bytes) */
#define BQ28Z610_CMD_STATE_OF_HEALTH    0x1CU  /* State of health in % (1 byte) */
#define BQ28Z610_CMD_CHARGING_VOLTAGE   0x1EU  /* Charging voltage in mV (2 bytes) */
#define BQ28Z610_CMD_CHARGING_CURRENT   0x20U  /* Charging current in mA (2 bytes) */
#define BQ28Z610_CMD_BATTERY_STATUS     0x22U  /* Battery status flags (2 bytes) */
#define BQ28Z610_CMD_CYCLE_THRESHOLD    0x24U  /* Cycle count threshold (2 bytes) */
#define BQ28Z610_CMD_TS1_TEMP           0x26U  /* TS1 thermistor temperature in 0.1K (2 bytes) */
#define BQ28Z610_CMD_TS2_TEMP           0x28U  /* TS2 thermistor temperature in 0.1K (2 bytes) */
#define BQ28Z610_CMD_INT_TEMP           0x2AU  /* Internal temperature in 0.1K (2 bytes) */
#define BQ28Z610_CMD_CELL1_VOLT         0x2CU  /* Cell 1 voltage in mV (2 bytes) */
#define BQ28Z610_CMD_CELL2_VOLT         0x2EU  /* Cell 2 voltage in mV (2 bytes) */

/* ==========================================================================
 *  TEMPERATURE - Register 0x02 (in 0.1K)
 * ========================================================================== */
#define BQ28Z610_TEMP_BASE_K_X10        2731U  /* 273.1K = 0°C (stored as 0x0AAB) */
#define BQ28Z610_TEMP_STEP_K_X10        1U     /* 0.1K per unit */

/* ==========================================================================
 *  VOLTAGE - Register 0x04 (in mV)
 * ========================================================================== */
#define BQ28Z610_VOLT_BASE_MV           0U
#define BQ28Z610_VOLT_STEP_MV           1U
#define BQ28Z610_VOLT_MAX_MV            65535U

/* ==========================================================================
 *  FLAGS - Register 0x06 (2 bytes, little-endian)
 * ========================================================================== */
#define BQ28Z610_FLAGS_DSG              0x0001U  /* Discharging */
#define BQ28Z610_FLAGS_CHG              0x0002U  /* Charging */
#define BQ28Z610_FLAGS_OCV              0x0004U  /* Open Circuit Voltage */
#define BQ28Z610_FLAGS_TDA              0x0008U  /* Terminate Discharge Alert */
#define BQ28Z610_FLAGS_RSVD1            0x0010U  /* Reserved */
#define BQ28Z610_FLAGS_RSVD2            0x0020U  /* Reserved */
#define BQ28Z610_FLAGS_AUTH             0x0040U  /* Authenticated (for secure protocols) */
#define BQ28Z610_FLAGS_CF               0x0080U  /* Charge Flag */
#define BQ28Z610_FLAGS_FC               0x0100U  /* Fully Charged */
#define BQ28Z610_FLAGS_FD               0x0200U  /* Fully Discharged */
#define BQ28Z610_FLAGS_EDV              0x0400U  /* End-of-Discharge Voltage */
#define BQ28Z610_FLAGS_TC               0x0800U  /* Terminate Charge Alert */
#define BQ28Z610_FLAGS_TBX              0x1000U  /* Battery Exceeded Temperature */
#define BQ28Z610_FLAGS_OTD              0x2000U  /* Over-Temperature Discharge */
#define BQ28Z610_FLAGS_OTC              0x4000U  /* Over-Temperature Charge */

/* ==========================================================================
 *  CAPACITY REGISTERS (in mAh)
 * ========================================================================== */
#define BQ28Z610_CAPACITY_BASE_MAH      0U
#define BQ28Z610_CAPACITY_STEP_MAH      1U
#define BQ28Z610_CAPACITY_MAX_MAH       65535U

/* ==========================================================================
 *  CURRENT REGISTERS (in mA, signed 16-bit)
 * ========================================================================== */
#define BQ28Z610_CURRENT_BASE_MA        0
#define BQ28Z610_CURRENT_STEP_MA        1
#define BQ28Z610_CURRENT_MIN_MA         -32768
#define BQ28Z610_CURRENT_MAX_MA         32767

/* ==========================================================================
 *  STATE OF CHARGE - Register 0x18 (percentage, 0-100)
 * ========================================================================== */
#define BQ28Z610_SOC_BASE_PCT           0U
#define BQ28Z610_SOC_STEP_PCT           1U
#define BQ28Z610_SOC_MAX_PCT            100U

/* ==========================================================================
 *  CYCLE COUNT - Register 0x1A (count)
 * ========================================================================== */
#define BQ28Z610_CYCLE_COUNT_BASE       0U
#define BQ28Z610_CYCLE_COUNT_STEP       1U

/* ==========================================================================
 *  STATE OF HEALTH - Register 0x1C (percentage, 0-100)
 * ========================================================================== */
#define BQ28Z610_SOH_BASE_PCT           0U
#define BQ28Z610_SOH_STEP_PCT           1U
#define BQ28Z610_SOH_MAX_PCT            100U

/* ==========================================================================
 *  POWER - Register 0x16 (in mW, signed)
 * ========================================================================== */
#define BQ28Z610_POWER_BASE_MW          0
#define BQ28Z610_POWER_STEP_MW          1

/* ==========================================================================
 *  CONTROL SUB-COMMANDS (0x00-0xFF, issued via CMD 0x00)
 * ========================================================================== */
#define BQ28Z610_CTRL_STATUS            0x0000U  /* Get device status */
#define BQ28Z610_CTRL_DEVICE_TYPE       0x0001U  /* Get device type */
#define BQ28Z610_CTRL_FW_VERSION        0x0002U  /* Get firmware version */
#define BQ28Z610_CTRL_HW_VERSION        0x0003U  /* Get hardware version */
#define BQ28Z610_CTRL_RESET             0x0041U  /* Reset device */
#define BQ28Z610_CTRL_SLEEP             0x0043U  /* Enter sleep mode */
#define BQ28Z610_CTRL_SHUTDOWN          0x0010U  /* Shutdown device */

/* ==========================================================================
 *  BLOCK DATA ACCESS (for extended data)
 * ========================================================================== */
#define BQ28Z610_BD_CLASS               0x3DU  /* BlockData class selector */
#define BQ28Z610_BD_OFFSET              0x3EU  /* BlockData offset */
#define BQ28Z610_BD_DATA_START          0x40U  /* BlockData starts at 0x40 */
#define BQ28Z610_BD_DATA_SIZE           32U    /* Block data is 32 bytes */
#define BQ28Z610_BD_CHECKSUM            0x60U  /* BlockData checksum */
#define BQ28Z610_BD_CHECKSUM_CHKSUM     0x61U  /* BlockData checksum validation */

/* ==========================================================================
 *  BATTERY STATUS FLAGS (2 bytes from CMD 0x22)
 * ========================================================================== */
#define BQ28Z610_BATT_STAT_OK           0x0000U  /* Battery OK */
#define BQ28Z610_BATT_STAT_OVER_CHG_V   0x0001U  /* Over-Charge Voltage */
#define BQ28Z610_BATT_STAT_OVER_CHG_I   0x0002U  /* Over-Charge Current */
#define BQ28Z610_BATT_STAT_OVER_TEMP_C  0x0004U  /* Over-Temperature Charge */
#define BQ28Z610_BATT_STAT_UNDER_TEMP_C 0x0008U  /* Under-Temperature Charge */
#define BQ28Z610_BATT_STAT_UNDER_TEMP_D 0x0010U  /* Under-Temperature Discharge */
#define BQ28Z610_BATT_STAT_OVER_CURRENT 0x0020U  /* Over-Current Discharge */
#define BQ28Z610_BATT_STAT_CFETshort    0x0040U  /* Charge FET Short */
#define BQ28Z610_BATT_STAT_DFETshort    0x0080U  /* Discharge FET Short */
#define BQ28Z610_BATT_STAT_FET_BAD      0x0100U  /* FET Bad */
#define BQ28Z610_BATT_STAT_AFE_REG_ERR  0x0200U  /* AFE Register Error */
#define BQ28Z610_BATT_STAT_CHG_PROTECT  0x0400U  /* Charge Protection */
#define BQ28Z610_BATT_STAT_DSG_PROTECT  0x0800U  /* Discharge Protection */
#define BQ28Z610_BATT_STAT_OTHER_FAULT  0x1000U  /* Other Fault */
#define BQ28Z610_BATT_STAT_CHG_TIMEOUT  0x2000U  /* Charge Timeout */
#define BQ28Z610_BATT_STAT_DSG_TIMEOUT  0x4000U  /* Discharge Timeout */

/* ==========================================================================
 *  DEVICE TYPE VALUES
 * ========================================================================== */
#define BQ28Z610_DEVICE_TYPE_VALUE      0x0610U  /* BQ28Z610 device type */

/* ==========================================================================
 *  HELPER MACROS FOR CONVERSIONS
 * ========================================================================== */

/* Temperature conversion: 0.1K units to/from Celsius */
#define BQ28Z610_TEMP_TO_DEGC(temp_x10) \
  ((int16_t)(((int32_t)(temp_x10) - (int32_t)BQ28Z610_TEMP_BASE_K_X10) / 10))

#define BQ28Z610_DEGC_TO_TEMP(temp_c) \
  ((uint16_t)(((int32_t)(temp_c) * 10) + (int32_t)BQ28Z610_TEMP_BASE_K_X10))

/* Percentage saturation (0-100%) */
#define BQ28Z610_PERCENT_TO_VALUE(percent) \
  ((uint8_t)((percent) > 100U ? 100U : (percent)))

#define BQ28Z610_VALUE_TO_PERCENT(value) \
  ((uint8_t)((value) > 100U ? 100U : (value)))

#ifdef __cplusplus
}
#endif

#endif /* __BQ28Z610_REGS_H */
