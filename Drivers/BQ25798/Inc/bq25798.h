/**
  ******************************************************************************
  * @file    bq25798.h
  * @brief   BQ25798RQMR Buck-Boost Battery Charger - Driver API header.
  *          Provides high-level functions for initialization, charge parameter
  *          configuration, ADC reading, fault monitoring, and OTG control.
  *
  * @note    Based on TI BQ25798 datasheet (SLUSE18).
  *          I2C address: 0x6B (7-bit).
  ******************************************************************************
  */

#ifndef __BQ25798_H
#define __BQ25798_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "../../STM32L4xx_HAL_Driver/Inc/stm32l4xx_hal.h"
#include "bq25798_regs.h"
#include <stdbool.h>

/* ==========================================================================
 *  Error / Status Codes
 * ========================================================================== */
typedef enum {
  BQ25798_OK              =  0,
  BQ25798_ERR_I2C         = -1,  /**< I2C communication error */
  BQ25798_ERR_DEVICE_ID   = -2,  /**< Part number mismatch */
  BQ25798_ERR_PARAM       = -3,  /**< Invalid parameter */
  BQ25798_ERR_TIMEOUT     = -4,  /**< Operation timeout */
  BQ25798_ERR_FAULT       = -5,  /**< Hardware fault detected */
} BQ25798_Status_t;

/* ==========================================================================
 *  Charge Configuration
 * ========================================================================== */
typedef struct {
  uint16_t vreg_mv;        /**< Charge voltage limit in mV (3000-18800, step 10) */
  uint16_t ichg_ma;        /**< Charge current limit in mA (50-5000, step 10) */
  uint16_t vindpm_mv;      /**< Input voltage limit in mV (3600-22000, step 100) */
  uint16_t iindpm_ma;      /**< Input current limit in mA (100-3300, step 10) */
  uint16_t vsysmin_mv;     /**< Minimum system voltage in mV (2500-16000, step 250) */
  uint16_t iprechg_ma;     /**< Precharge current in mA (40-2000, step 40) */
  uint16_t iterm_ma;       /**< Termination current in mA (40-1000, step 40) */
  uint8_t  cell_count;     /**< Battery cell count: 1-4 */
} BQ25798_Charge_Config_t;

/* ==========================================================================
 *  OTG (Boost) Configuration
 * ========================================================================== */
typedef struct {
  uint16_t votg_mv;        /**< OTG output voltage in mV (2800-22000, step 10) */
  uint16_t iotg_ma;        /**< OTG output current limit in mA */
} BQ25798_OTG_Config_t;

/* ==========================================================================
 *  Charge Status (read-only)
 * ========================================================================== */
typedef enum {
  BQ25798_CHRG_NOT_CHARGING = 0,
  BQ25798_CHRG_TRICKLE      = 1,
  BQ25798_CHRG_PRECHARGE     = 2,
  BQ25798_CHRG_FAST_CHARGE   = 3,
  BQ25798_CHRG_TAPER_CHARGE  = 4,
  BQ25798_CHRG_TOP_OFF       = 6,
  BQ25798_CHRG_DONE          = 7,
} BQ25798_Charge_State_t;

/* ==========================================================================
 *  Input Source Type (VBUS_STAT)
 * ========================================================================== */
typedef enum {
  BQ25798_INPUT_NONE       = 0,
  BQ25798_INPUT_USB_SDP    = 1,
  BQ25798_INPUT_USB_CDP    = 2,
  BQ25798_INPUT_USB_DCP    = 3,
  BQ25798_INPUT_ADJ_DCP    = 4,
  BQ25798_INPUT_UNKNOWN    = 5,
  BQ25798_INPUT_NON_STD    = 6,
  BQ25798_INPUT_OTG        = 7,
  BQ25798_INPUT_NOT_QUAL   = 8,
  BQ25798_INPUT_HVDCP      = 11,
  BQ25798_INPUT_POOR_SRC   = 15,
} BQ25798_Input_Source_t;

/* ==========================================================================
 *  Charger Status (combined readback)
 * ========================================================================== */
typedef struct {
  /* Status 0 */
  bool     iindpm_stat;       /**< In IINDPM regulation */
  bool     vindpm_stat;       /**< In VINDPM regulation */
  bool     wd_fault;          /**< Watchdog timer expired */
  bool     pg_stat;           /**< Power good */
  bool     ac2_present;       /**< AC2 input present */
  bool     ac1_present;       /**< AC1 input present */
  bool     vbus_present;      /**< VBUS present */

  /* Status 1 */
  BQ25798_Charge_State_t  charge_state; /**< Charging state */
  BQ25798_Input_Source_t  input_source; /**< Input source type */
  bool     bc12_done;         /**< BC1.2 detection complete */

  /* Status 2 */
  uint8_t  ico_status;        /**< ICO status (0=dis, 1=progress, 2=done) */
  bool     treg_stat;         /**< In thermal regulation */
  bool     dpdm_stat;         /**< D+/D- detection ongoing */
  bool     vbat_present;      /**< Battery present */

  /* Status 3 */
  bool     adc_done;          /**< ADC conversion complete */
  bool     vsys_stat;         /**< VSYS regulation active */
  bool     chg_tmr_stat;      /**< Fast charge timer expired */
  bool     trichg_tmr_stat;   /**< Trickle charge timer expired */
  bool     prechg_tmr_stat;   /**< Precharge timer expired */
} BQ25798_Charger_Status_t;

/* ==========================================================================
 *  Fault Status
 * ========================================================================== */
typedef struct {
  /* Fault Status 0 */
  bool     vbus_ovp;          /**< VBUS over-voltage protection fault */
  bool     vbat_ovp;          /**< VBAT over-voltage protection fault */
  bool     ibus_ocp;          /**< IBUS over-current protection fault */
  bool     ibat_ocp;          /**< IBAT over-current protection fault */
  bool     conv_ocp;          /**< Converter over-current fault */
  bool     vac2_ovp;          /**< VAC2 over-voltage fault */
  bool     vac1_ovp;          /**< VAC1 over-voltage fault */

  /* Fault Status 1 */
  bool     vsys_short;        /**< VSYS short circuit fault */
  bool     vsys_ovp;          /**< VSYS over-voltage fault */
  bool     otg_ovp;           /**< OTG over-voltage fault */
  bool     otg_uvp;           /**< OTG under-voltage fault */
  bool     tshut;             /**< Thermal shutdown fault */
} BQ25798_Fault_Status_t;

/* ==========================================================================
 *  ADC Readings
 * ========================================================================== */
typedef struct {
  int16_t  ibus_ma;           /**< Input bus current (mA, signed) */
  int16_t  ibat_ma;           /**< Battery current (mA, signed, + = charging) */
  uint16_t vbus_mv;           /**< Input bus voltage (mV) */
  uint16_t vbat_mv;           /**< Battery voltage (mV) */
  uint16_t vsys_mv;           /**< System voltage (mV) */
  uint16_t vac1_mv;           /**< AC1 input voltage (mV) */
  uint16_t vac2_mv;           /**< AC2 input voltage (mV) */
  int16_t  tdie_degc_x10;     /**< Die temperature (°C × 10) */
  uint16_t ts_pct_x100;       /**< TS pin (% of REGN × 100) */
} BQ25798_ADC_Readings_t;

/* ==========================================================================
 *  Device Handle
 * ========================================================================== */
typedef struct {
  I2C_HandleTypeDef *hi2c;     /**< Pointer to HAL I2C handle */
  uint16_t           i2c_addr; /**< 7-bit I2C address (default 0x6B) */
  uint32_t           timeout;  /**< I2C timeout in ms */
} BQ25798_Handle_t;

/* ==========================================================================
 *  Register-Level Access API  (implemented in bq25798_regs.c)
 * ========================================================================== */

/**
 * @brief  Read a single 8-bit register.
 */
BQ25798_Status_t BQ25798_ReadReg(const BQ25798_Handle_t *dev, uint8_t reg, uint8_t *data);

/**
 * @brief  Read multiple consecutive registers.
 */
BQ25798_Status_t BQ25798_ReadRegs(const BQ25798_Handle_t *dev, uint8_t reg, uint8_t *data, uint16_t len);

/**
 * @brief  Write a single 8-bit register.
 */
BQ25798_Status_t BQ25798_WriteReg(const BQ25798_Handle_t *dev, uint8_t reg, uint8_t data);

/**
 * @brief  Write multiple consecutive registers.
 */
BQ25798_Status_t BQ25798_WriteRegs(const BQ25798_Handle_t *dev, uint8_t reg, const uint8_t *data, uint16_t len);

/**
 * @brief  Read-modify-write an 8-bit register (set/clear specific bits).
 */
BQ25798_Status_t BQ25798_ModifyReg(const BQ25798_Handle_t *dev, uint8_t reg, uint8_t mask, uint8_t value);

/**
 * @brief  Read a 16-bit register (big-endian: MSB at lower address).
 */
BQ25798_Status_t BQ25798_ReadReg16(const BQ25798_Handle_t *dev, uint8_t reg, uint16_t *data);

/**
 * @brief  Write a 16-bit register (big-endian: MSB at lower address).
 */
BQ25798_Status_t BQ25798_WriteReg16(const BQ25798_Handle_t *dev, uint8_t reg, uint16_t data);

/**
 * @brief  Read a signed 16-bit ADC register (big-endian).
 */
BQ25798_Status_t BQ25798_ReadADC16(const BQ25798_Handle_t *dev, uint8_t reg, int16_t *data);

/* ==========================================================================
 *  High-Level Driver API  (implemented in bq25798.c)
 * ========================================================================== */

/**
 * @brief  Initialize the BQ25798 driver and verify device identity.
 * @param  dev    Device handle (hi2c, i2c_addr, timeout must be set)
 * @retval BQ25798_Status_t
 */
BQ25798_Status_t BQ25798_Init(BQ25798_Handle_t *dev);

/**
 * @brief  Perform a full register reset of the BQ25798.
 * @param  dev    Device handle
 * @retval BQ25798_Status_t
 */
BQ25798_Status_t BQ25798_Reset(const BQ25798_Handle_t *dev);

/**
 * @brief  Verify the part number matches BQ25798.
 * @param  dev          Device handle
 * @param  part_info    Pointer to store raw part info register (can be NULL)
 * @retval BQ25798_Status_t  BQ25798_OK if part matches, ERR_DEVICE_ID otherwise
 */
BQ25798_Status_t BQ25798_VerifyPartNumber(const BQ25798_Handle_t *dev, uint8_t *part_info);

/**
 * @brief  Configure charging parameters (voltage, current, limits).
 * @param  dev    Device handle
 * @param  cfg    Pointer to charge configuration struct
 * @retval BQ25798_Status_t
 */
BQ25798_Status_t BQ25798_ConfigureCharging(const BQ25798_Handle_t *dev,
                                            const BQ25798_Charge_Config_t *cfg);

/**
 * @brief  Enable or disable charging.
 * @param  dev     Device handle
 * @param  enable  true = enable charging, false = disable
 * @retval BQ25798_Status_t
 */
BQ25798_Status_t BQ25798_EnableCharging(const BQ25798_Handle_t *dev, bool enable);

/**
 * @brief  Check if charging is currently enabled.
 * @param  dev      Device handle
 * @param  enabled  Pointer to store result
 * @retval BQ25798_Status_t
 */
BQ25798_Status_t BQ25798_IsChargingEnabled(const BQ25798_Handle_t *dev, bool *enabled);

/**
 * @brief  Set charge voltage limit (VREG).
 * @param  dev       Device handle
 * @param  vreg_mv   Voltage in millivolts (3000-18800, step 10)
 * @retval BQ25798_Status_t
 */
BQ25798_Status_t BQ25798_SetChargeVoltage(const BQ25798_Handle_t *dev, uint16_t vreg_mv);

/**
 * @brief  Set charge current limit (ICHG).
 * @param  dev       Device handle
 * @param  ichg_ma   Current in milliamps (50-5000, step 10)
 * @retval BQ25798_Status_t
 */
BQ25798_Status_t BQ25798_SetChargeCurrent(const BQ25798_Handle_t *dev, uint16_t ichg_ma);

/**
 * @brief  Set input voltage limit (VINDPM).
 * @param  dev        Device handle
 * @param  vindpm_mv  Voltage in millivolts (3600-22000, step 100)
 * @retval BQ25798_Status_t
 */
BQ25798_Status_t BQ25798_SetInputVoltageLimit(const BQ25798_Handle_t *dev, uint16_t vindpm_mv);

/**
 * @brief  Set input current limit (IINDPM).
 * @param  dev        Device handle
 * @param  iindpm_ma  Current in milliamps (100-3300, step 10)
 * @retval BQ25798_Status_t
 */
BQ25798_Status_t BQ25798_SetInputCurrentLimit(const BQ25798_Handle_t *dev, uint16_t iindpm_ma);

/**
 * @brief  Set minimum system voltage (VSYSMIN).
 * @param  dev         Device handle
 * @param  vsysmin_mv  Voltage in millivolts (2500-16000, step 250)
 * @retval BQ25798_Status_t
 */
BQ25798_Status_t BQ25798_SetMinSysVoltage(const BQ25798_Handle_t *dev, uint16_t vsysmin_mv);

/**
 * @brief  Configure and enable OTG (boost) mode.
 * @param  dev    Device handle
 * @param  cfg    OTG configuration
 * @retval BQ25798_Status_t
 */
BQ25798_Status_t BQ25798_ConfigureOTG(const BQ25798_Handle_t *dev,
                                       const BQ25798_OTG_Config_t *cfg);

/**
 * @brief  Enable or disable OTG (boost) mode.
 * @param  dev     Device handle
 * @param  enable  true = enable OTG, false = disable
 * @retval BQ25798_Status_t
 */
BQ25798_Status_t BQ25798_EnableOTG(const BQ25798_Handle_t *dev, bool enable);

/**
 * @brief  Read full charger status (status registers 0-3).
 * @param  dev     Device handle
 * @param  status  Pointer to store charger status
 * @retval BQ25798_Status_t
 */
BQ25798_Status_t BQ25798_GetChargerStatus(const BQ25798_Handle_t *dev,
                                           BQ25798_Charger_Status_t *status);

/**
 * @brief  Read fault status registers.
 * @param  dev    Device handle
 * @param  fault  Pointer to store fault information
 * @retval BQ25798_Status_t
 */
BQ25798_Status_t BQ25798_GetFaultStatus(const BQ25798_Handle_t *dev,
                                         BQ25798_Fault_Status_t *fault);

/**
 * @brief  Enable the ADC for continuous or one-shot measurements.
 * @param  dev         Device handle
 * @param  continuous  true = continuous conversion, false = one-shot
 * @retval BQ25798_Status_t
 */
BQ25798_Status_t BQ25798_EnableADC(const BQ25798_Handle_t *dev, bool continuous);

/**
 * @brief  Disable the ADC.
 * @param  dev    Device handle
 * @retval BQ25798_Status_t
 */
BQ25798_Status_t BQ25798_DisableADC(const BQ25798_Handle_t *dev);

/**
 * @brief  Read all ADC channels (VBUS, VBAT, VSYS, IBUS, IBAT, TS, TDIE).
 * @param  dev       Device handle
 * @param  readings  Pointer to store ADC readings
 * @retval BQ25798_Status_t
 */
BQ25798_Status_t BQ25798_ReadADC(const BQ25798_Handle_t *dev,
                                  BQ25798_ADC_Readings_t *readings);

/**
 * @brief  Read battery voltage from ADC.
 * @param  dev      Device handle
 * @param  vbat_mv  Pointer to store battery voltage in mV
 * @retval BQ25798_Status_t
 */
BQ25798_Status_t BQ25798_ReadVBAT(const BQ25798_Handle_t *dev, uint16_t *vbat_mv);

/**
 * @brief  Read input bus voltage from ADC.
 * @param  dev      Device handle
 * @param  vbus_mv  Pointer to store VBUS voltage in mV
 * @retval BQ25798_Status_t
 */
BQ25798_Status_t BQ25798_ReadVBUS(const BQ25798_Handle_t *dev, uint16_t *vbus_mv);

/**
 * @brief  Read battery current from ADC.
 * @param  dev      Device handle
 * @param  ibat_ma  Pointer to store battery current in mA (signed)
 * @retval BQ25798_Status_t
 */
BQ25798_Status_t BQ25798_ReadIBAT(const BQ25798_Handle_t *dev, int16_t *ibat_ma);

/**
 * @brief  Read die temperature from ADC.
 * @param  dev            Device handle
 * @param  temp_degc_x10  Pointer to store die temperature (°C × 10)
 * @retval BQ25798_Status_t
 */
BQ25798_Status_t BQ25798_ReadDieTemp(const BQ25798_Handle_t *dev, int16_t *temp_degc_x10);

/**
 * @brief  Configure watchdog timer.
 * @param  dev      Device handle
 * @param  setting  Watchdog setting (use BQ25798_WATCHDOG_xxx defines)
 * @retval BQ25798_Status_t
 */
BQ25798_Status_t BQ25798_SetWatchdog(const BQ25798_Handle_t *dev, uint8_t setting);

/**
 * @brief  Reset the watchdog timer.
 * @param  dev    Device handle
 * @retval BQ25798_Status_t
 */
BQ25798_Status_t BQ25798_ResetWatchdog(const BQ25798_Handle_t *dev);

/**
 * @brief  Enter High Impedance (Hi-Z) mode — disconnect input.
 * @param  dev     Device handle
 * @param  enable  true = enter Hi-Z, false = exit Hi-Z
 * @retval BQ25798_Status_t
 */
BQ25798_Status_t BQ25798_SetHiZ(const BQ25798_Handle_t *dev, bool enable);

/**
 * @brief  Disable/enable NTC temperature sensing (TS_IGNORE).
 * @param  dev     Device handle
 * @param  ignore  true = ignore TS pin, false = use TS pin
 * @retval BQ25798_Status_t
 */
BQ25798_Status_t BQ25798_IgnoreTS(const BQ25798_Handle_t *dev, bool ignore);

/**
 * @brief  Enter ship mode (ultra-low-power mode).
 * @param  dev    Device handle
 * @retval BQ25798_Status_t
 */
BQ25798_Status_t BQ25798_EnterShipMode(const BQ25798_Handle_t *dev);

/**
 * @brief  Clear all fault flag registers by reading them.
 * @param  dev    Device handle
 * @retval BQ25798_Status_t
 */
BQ25798_Status_t BQ25798_ClearFaults(const BQ25798_Handle_t *dev);

#ifdef __cplusplus
}
#endif

#endif /* __BQ25798_H */
