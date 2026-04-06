/**
  ******************************************************************************
  * @file    tps55288.h
  * @brief   TPS55288RPMR Dual-Channel Integrated Voltage Regulator - Driver API header.
  *          Provides high-level functions for initialization, voltage/current
  *          configuration, monitoring, and protection control.
  *
  * @note    Based on TI TPS55288 datasheet (SLVSF01B Revision B, December 2020).
  *          I2C address: 0x68 (7-bit).
  ******************************************************************************
  */

#ifndef __TPS55288_H
#define __TPS55288_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "../../STM32L4xx_HAL_Driver/Inc/stm32l4xx_hal.h"
#include "tps55288_regs.h"
#include <stdbool.h>

/* ==========================================================================
 *  Error / Return Codes
 * ========================================================================== */
typedef enum {
  TPS55288_OK              =  0,
  TPS55288_ERR_I2C         = -1,  /**< I2C communication error */
  TPS55288_ERR_DEVICE_ID   = -2,  /**< Chip ID mismatch */
  TPS55288_ERR_PARAM       = -3,  /**< Invalid parameter */
  TPS55288_ERR_TIMEOUT     = -4,  /**< Operation timeout */
  TPS55288_ERR_FAULT       = -5,  /**< Hardware fault detected (UV/OV) */
} TPS55288_RetVal_t;

/* ==========================================================================
 *  Channel Selection
 * ========================================================================== */
typedef enum {
  TPS55288_CH1             = 1,   /**< Channel 1 */
  TPS55288_CH2             = 2,   /**< Channel 2 */
} TPS55288_Channel_t;

/* ==========================================================================
 *  Operating Modes
 * ========================================================================== */
typedef enum {
  TPS55288_MODE_OFF        = 0,   /**< Disabled / Off */
  TPS55288_MODE_BUCK       = 1,   /**< Buck (step-down) mode */
  TPS55288_MODE_BOOST      = 2,   /**< Boost (step-up) mode */
} TPS55288_Mode_t;

/* ==========================================================================
 *  Switching Frequency
 * ========================================================================== */
typedef enum {
  TPS55288_FREQ_VAL_500kHz     = 0,
  TPS55288_FREQ_VAL_667kHz     = 1,
  TPS55288_FREQ_VAL_800kHz     = 2,
  TPS55288_FREQ_VAL_1MHz       = 3,
  TPS55288_FREQ_VAL_1_33MHz    = 4,
  TPS55288_FREQ_VAL_1_67MHz    = 5,
  TPS55288_FREQ_VAL_2MHz       = 6,
  TPS55288_FREQ_VAL_2_67MHz    = 7,
} TPS55288_Frequency_t;

/* ==========================================================================
 *  Channel Tracking Mode
 * ========================================================================== */
typedef enum {
  TPS55288_TRACK_VAL_INDEPENDENT   = 0,  /**< Independent operation */
  TPS55288_TRACK_VAL_CH2_FOLLOWS   = 1,  /**< CH2 follows CH1 */
  TPS55288_TRACK_VAL_CH1_FOLLOWS   = 2,  /**< CH1 follows CH2 */
  TPS55288_TRACK_VAL_PHASE_SHIFT   = 3,  /**< Phase-shifted operation */
} TPS55288_TrackMode_t;

/* ==========================================================================
 *  Voltage Ramp Rate
 * ========================================================================== */
typedef enum {
  TPS55288_RAMP_VAL_1mV_us     = 0,
  TPS55288_RAMP_VAL_2mV_us     = 1,
  TPS55288_RAMP_VAL_5mV_us     = 2,
  TPS55288_RAMP_VAL_10mV_us    = 3,
} TPS55288_RampRate_t;

/* ==========================================================================
 *  Channel Configuration
 * ========================================================================== */
typedef struct {
  TPS55288_Mode_t      mode;        /**< Operating mode (buck/boost) */
  uint16_t             vout_mv;     /**< Output voltage in mV (500-5500) */
  uint16_t             ilim_ma;     /**< Current limit in mA (1000-3500, step 20) */
  bool                 enabled;     /**< Channel enabled/disabled */
} TPS55288_Channel_Config_t;

/* ==========================================================================
 *  Device Configuration
 * ========================================================================== */
typedef struct {
  TPS55288_Frequency_t freq;        /**< Switching frequency */
  TPS55288_TrackMode_t track_mode;  /**< Channel tracking mode */
  TPS55288_RampRate_t  ramp_rate;   /**< Voltage ramp rate */
  bool                 thermal_protection;  /**< Enable thermal protection */
} TPS55288_Device_Config_t;

/* ==========================================================================
 *  Status Register (read-only)
 * ========================================================================== */
typedef struct {
  /* Channel 1 Status */
  bool     ch1_active;        /**< Channel 1 actively regulating */
  bool     ch1_uvp;           /**< Channel 1 under-voltage protection active */
  bool     ch1_ovp;           /**< Channel 1 over-voltage protection active */
  bool     ch1_interrupt;     /**< Channel 1 interrupt flag */

  /* Channel 2 Status */
  bool     ch2_active;        /**< Channel 2 actively regulating */
  bool     ch2_uvp;           /**< Channel 2 under-voltage protection active */
  bool     ch2_ovp;           /**< Channel 2 over-voltage protection active */
  bool     ch2_interrupt;     /**< Channel 2 interrupt flag */

  /* Die Temperature */
  uint8_t  die_temp_c;        /**< Die temperature in °C */
} TPS55288_Status_Info_t;

/* ==========================================================================
 *  Measurement Data (read-only)
 * ========================================================================== */
typedef struct {
  uint16_t ch1_iout_ma;       /**< Channel 1 output current (mA) */
  uint16_t ch2_iout_ma;       /**< Channel 2 output current (mA) */
} TPS55288_Measurements_t;

/* ==========================================================================
 *  Device Handle
 * ========================================================================== */
typedef struct {
  I2C_HandleTypeDef *hi2c;     /**< Pointer to HAL I2C handle */
  uint16_t           i2c_addr; /**< 7-bit I2C address (default 0x68) */
  uint32_t           timeout;  /**< I2C timeout in ms */
} TPS55288_Handle_t;

/* ==========================================================================
 *  Register-Level Access API  (implemented in tps55288_regs.c)
 * ========================================================================== */

/**
 * @brief  Read a single 8-bit register.
 */
TPS55288_RetVal_t TPS55288_ReadReg(const TPS55288_Handle_t *dev, uint8_t reg, uint8_t *data);

/**
 * @brief  Write a single 8-bit register.
 */
TPS55288_RetVal_t TPS55288_WriteReg(const TPS55288_Handle_t *dev, uint8_t reg, uint8_t data);

/**
 * @brief  Read-modify-write an 8-bit register (set/clear specific bits).
 */
TPS55288_RetVal_t TPS55288_ModifyReg(const TPS55288_Handle_t *dev, uint8_t reg, uint8_t mask, uint8_t value);

/* ==========================================================================
 *  High-Level Driver API  (implemented in tps55288.c)
 * ========================================================================== */

/**
 * @brief  Initialize the TPS55288 driver and verify device identity.
 * @param  dev    Device handle (hi2c, i2c_addr, timeout must be set)
 * @retval TPS55288_RetVal_t
 */
TPS55288_RetVal_t TPS55288_Init(TPS55288_Handle_t *dev);

/**
 * @brief  Verify the chip ID matches TPS55288.
 * @param  dev       Device handle
 * @param  chip_id   Pointer to store chip ID value (can be NULL)
 * @retval TPS55288_RetVal_t  TPS55288_OK if chip ID matches, ERR_DEVICE_ID otherwise
 */
TPS55288_RetVal_t TPS55288_VerifyChipID(const TPS55288_Handle_t *dev, uint8_t *chip_id);

/**
 * @brief  Configure a single channel (voltage, current limit, mode).
 * @param  dev    Device handle
 * @param  ch     Channel (TPS55288_CH1 or TPS55288_CH2)
 * @param  cfg    Pointer to channel configuration struct
 * @retval TPS55288_RetVal_t
 */
TPS55288_RetVal_t TPS55288_ConfigureChannel(const TPS55288_Handle_t *dev,
                                             TPS55288_Channel_t ch,
                                             const TPS55288_Channel_Config_t *cfg);

/**
 * @brief  Configure device-wide settings (frequency, tracking, ramp rate).
 * @param  dev    Device handle
 * @param  cfg    Pointer to device configuration struct
 * @retval TPS55288_RetVal_t
 */
TPS55288_RetVal_t TPS55288_ConfigureDevice(const TPS55288_Handle_t *dev,
                                            const TPS55288_Device_Config_t *cfg);

/**
 * @brief  Enable or disable a channel.
 * @param  dev     Device handle
 * @param  ch      Channel (TPS55288_CH1 or TPS55288_CH2)
 * @param  enable  true = enable, false = disable
 * @retval TPS55288_RetVal_t
 */
TPS55288_RetVal_t TPS55288_EnableChannel(const TPS55288_Handle_t *dev,
                                          TPS55288_Channel_t ch,
                                          bool enable);

/**
 * @brief  Set operating mode for a channel.
 * @param  dev    Device handle
 * @param  ch     Channel (TPS55288_CH1 or TPS55288_CH2)
 * @param  mode   Operating mode (buck, boost, or off)
 * @retval TPS55288_RetVal_t
 */
TPS55288_RetVal_t TPS55288_SetMode(const TPS55288_Handle_t *dev,
                                    TPS55288_Channel_t ch,
                                    TPS55288_Mode_t mode);

/**
 * @brief  Set output voltage for a channel.
 * @param  dev       Device handle
 * @param  ch        Channel (TPS55288_CH1 or TPS55288_CH2)
 * @param  vout_mv   Output voltage in mV (500-5500)
 * @retval TPS55288_RetVal_t
 */
TPS55288_RetVal_t TPS55288_SetVoltage(const TPS55288_Handle_t *dev,
                                       TPS55288_Channel_t ch,
                                       uint16_t vout_mv);

/**
 * @brief  Get current output voltage setting for a channel.
 * @param  dev       Device handle
 * @param  ch        Channel (TPS55288_CH1 or TPS55288_CH2)
 * @param  vout_mv   Pointer to store voltage in mV
 * @retval TPS55288_RetVal_t
 */
TPS55288_RetVal_t TPS55288_GetVoltage(const TPS55288_Handle_t *dev,
                                       TPS55288_Channel_t ch,
                                       uint16_t *vout_mv);

/**
 * @brief  Set current limit for a channel.
 * @param  dev      Device handle
 * @param  ch       Channel (TPS55288_CH1 or TPS55288_CH2)
 * @param  ilim_ma  Current limit in mA (1000-3500, step 20)
 * @retval TPS55288_RetVal_t
 */
TPS55288_RetVal_t TPS55288_SetCurrentLimit(const TPS55288_Handle_t *dev,
                                            TPS55288_Channel_t ch,
                                            uint16_t ilim_ma);

/**
 * @brief  Get current limit for a channel.
 * @param  dev      Device handle
 * @param  ch       Channel (TPS55288_CH1 or TPS55288_CH2)
 * @param  ilim_ma  Pointer to store current limit in mA
 * @retval TPS55288_RetVal_t
 */
TPS55288_RetVal_t TPS55288_GetCurrentLimit(const TPS55288_Handle_t *dev,
                                            TPS55288_Channel_t ch,
                                            uint16_t *ilim_ma);

/**
 * @brief  Set switching frequency.
 * @param  dev   Device handle
 * @param  freq  Frequency selection
 * @retval TPS55288_RetVal_t
 */
TPS55288_RetVal_t TPS55288_SetFrequency(const TPS55288_Handle_t *dev,
                                         TPS55288_Frequency_t freq);

/**
 * @brief  Set voltage ramp rate.
 * @param  dev        Device handle
 * @param  ramp_rate  Ramp rate selection
 * @retval TPS55288_RetVal_t
 */
TPS55288_RetVal_t TPS55288_SetRampRate(const TPS55288_Handle_t *dev,
                                        TPS55288_RampRate_t ramp_rate);

/**
 * @brief  Set channel tracking mode.
 * @param  dev        Device handle
 * @param  track_mode Tracking mode selection
 * @retval TPS55288_RetVal_t
 */
TPS55288_RetVal_t TPS55288_SetTrackingMode(const TPS55288_Handle_t *dev,
                                            TPS55288_TrackMode_t track_mode);

/**
 * @brief  Read device status (voltage protection, die temperature, active channels).
 * @param  dev     Device handle
 * @param  status  Pointer to store status
 * @retval TPS55288_RetVal_t (error code, not status info)
 */
TPS55288_RetVal_t TPS55288_GetStatus(const TPS55288_Handle_t *dev,
                                      TPS55288_Status_Info_t *status);

/**
 * @brief  Read output current measurements for both channels.
 * @param  dev          Device handle
 * @param  measurements Pointer to store measurement data
 * @retval TPS55288_RetVal_t
 */
TPS55288_RetVal_t TPS55288_GetMeasurements(const TPS55288_Handle_t *dev,
                                            TPS55288_Measurements_t *measurements);

/**
 * @brief  Get die temperature.
 * @param  dev      Device handle
 * @param  temp_c   Pointer to store die temperature in °C
 * @retval TPS55288_RetVal_t
 */
TPS55288_RetVal_t TPS55288_GetDieTemperature(const TPS55288_Handle_t *dev,
                                              uint8_t *temp_c);

/**
 * @brief  Enable or disable thermal protection.
 * @param  dev     Device handle
 * @param  enable  true = enable thermal protection, false = disable
 * @retval TPS55288_RetVal_t
 */
TPS55288_RetVal_t TPS55288_SetThermalProtection(const TPS55288_Handle_t *dev,
                                                 bool enable);

/**
 * @brief  Clear interrupt flags for both channels.
 * @param  dev    Device handle
 * @retval TPS55288_RetVal_t
 */
TPS55288_RetVal_t TPS55288_ClearInterrupts(const TPS55288_Handle_t *dev);

/**
 * @brief  Perform a complete channel setup (enable, set mode, voltage, and current).
 * @param  dev    Device handle
 * @param  ch     Channel (TPS55288_CH1 or TPS55288_CH2)
 * @param  cfg    Pointer to channel configuration struct
 * @retval TPS55288_RetVal_t
 *
 * @note   This is a convenience function that performs all necessary steps
 *         to set up a channel for operation.
 */
TPS55288_RetVal_t TPS55288_SetupChannel(const TPS55288_Handle_t *dev,
                                         TPS55288_Channel_t ch,
                                         const TPS55288_Channel_Config_t *cfg);

#ifdef __cplusplus
}
#endif

#endif /* __TPS55288_H */
