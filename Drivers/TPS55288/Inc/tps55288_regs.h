/**
  ******************************************************************************
  * @file    tps55288_regs.h
  * @brief   TPS55288RPMR Dual-Channel Integrated Voltage Regulator - Register map,
  *          bit definitions, masks, and shift values.
  *
  * @note    Based on the TPS55288 datasheet (SLVSF01B Revision B, December 2020)
  *          from Texas Instruments.
  *          Default I2C address: 0x68 (7-bit).
  *          Register width: 8-bit.
  ******************************************************************************
  */

#ifndef __TPS55288_REGS_H
#define __TPS55288_REGS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* ==========================================================================
 *  I2C ADDRESS
 * ========================================================================== */
#define TPS55288_I2C_ADDR_7BIT          0x68U
#define TPS55288_I2C_ADDR_WRITE         (TPS55288_I2C_ADDR_7BIT << 1)         /* 0xD0 */
#define TPS55288_I2C_ADDR_READ          ((TPS55288_I2C_ADDR_7BIT << 1) | 1U)  /* 0xD1 */

/* ==========================================================================
 *  DEVICE IDENTIFICATION
 * ========================================================================== */
#define TPS55288_REG_CHIPID             0x00U  /* Chip ID Register */
#define TPS55288_CHIPID_DEFAULT         0x24U  /* Expected chip ID value */

/* ==========================================================================
 *  REG01 - Enable Configuration  (0x01)
 *  8-bit register
 * ========================================================================== */
#define TPS55288_REG_ENABLE             0x01U

/* EN1: Enable Channel 1
 * Bit [0]
 * 0 = Disabled
 * 1 = Enabled
 */
#define TPS55288_EN1_POS                0U
#define TPS55288_EN1_MSK                (1U << 0)

/* EN2: Enable Channel 2
 * Bit [1]
 * 0 = Disabled
 * 1 = Enabled
 */
#define TPS55288_EN2_POS                1U
#define TPS55288_EN2_MSK                (1U << 1)

/* ==========================================================================
 *  REG02 - Mode Configuration  (0x02)
 *  8-bit register
 * ========================================================================== */
#define TPS55288_REG_MODECFG            0x02U

/* MODE1: Mode Selection Channel 1
 * Bits [1:0]
 * 00 = Disabled/off
 * 01 = Buck mode
 * 10 = Boost mode
 * 11 = Reserved
 */
#define TPS55288_MODE1_POS              0U
#define TPS55288_MODE1_MSK              (3U << 0)
#define TPS55288_MODE1_OFF              (0U << 0)
#define TPS55288_MODE1_BUCK             (1U << 0)
#define TPS55288_MODE1_BOOST            (2U << 0)

/* MODE2: Mode Selection Channel 2
 * Bits [3:2]
 * 00 = Disabled/off
 * 01 = Buck mode
 * 10 = Boost mode
 * 11 = Reserved
 */
#define TPS55288_MODE2_POS              2U
#define TPS55288_MODE2_MSK              (3U << 2)
#define TPS55288_MODE2_OFF              (0U << 2)
#define TPS55288_MODE2_BUCK             (1U << 2)
#define TPS55288_MODE2_BOOST            (2U << 2)

/* ==========================================================================
 *  REG03 - Output Voltage Configuration CH1  (0x03)
 *  8-bit register
 * ========================================================================== */
#define TPS55288_REG_VOUT1              0x03U
#define TPS55288_VOUT1_BASE_MV          500U    /* 0.5V minimum */
#define TPS55288_VOUT1_STEP_MV          20U     /* ~20mV per step */
#define TPS55288_VOUT1_MAX_MV           5500U
#define TPS55288_VOUT1_MSK              0xFFU   /* [7:0] */

/* ==========================================================================
 *  REG04 - Output Voltage Configuration CH2  (0x04)
 *  8-bit register
 * ========================================================================== */
#define TPS55288_REG_VOUT2              0x04U
#define TPS55288_VOUT2_BASE_MV          500U
#define TPS55288_VOUT2_STEP_MV          20U
#define TPS55288_VOUT2_MAX_MV           5500U
#define TPS55288_VOUT2_MSK              0xFFU

/* ==========================================================================
 *  REG05 - Current Limit Channel 1  (0x05)
 *  8-bit register
 * ========================================================================== */
#define TPS55288_REG_ILIM1              0x05U
#define TPS55288_ILIM1_POS              0U
#define TPS55288_ILIM1_MSK              0x7FU
#define TPS55288_ILIM1_BASE_MA          1000U   /* 1A minimum */
#define TPS55288_ILIM1_STEP_MA          20U     /* 20mA per step */
#define TPS55288_ILIM1_MAX_MA           3500U

/* ==========================================================================
 *  REG06 - Current Limit Channel 2  (0x06)
 *  8-bit register
 * ========================================================================== */
#define TPS55288_REG_ILIM2              0x06U
#define TPS55288_ILIM2_POS              0U
#define TPS55288_ILIM2_MSK              0x7FU
#define TPS55288_ILIM2_BASE_MA          1000U
#define TPS55288_ILIM2_STEP_MA          20U
#define TPS55288_ILIM2_MAX_MA           3500U

/* ==========================================================================
 *  REG07 - Frequency Control  (0x07)
 *  8-bit register
 * ========================================================================== */
#define TPS55288_REG_FREQ               0x07U
#define TPS55288_FREQ_POS               0U
#define TPS55288_FREQ_MSK               0x07U
#define TPS55288_FREQ_500kHz            0U
#define TPS55288_FREQ_667kHz            1U
#define TPS55288_FREQ_800kHz            2U
#define TPS55288_FREQ_1MHz              3U
#define TPS55288_FREQ_1_33MHz           4U
#define TPS55288_FREQ_1_67MHz           5U
#define TPS55288_FREQ_2MHz              6U
#define TPS55288_FREQ_2_67MHz           7U

#define TPS55288_SYNCIN_POS             4U
#define TPS55288_SYNCIN_MSK             (1U << 4)

/* ==========================================================================
 *  REG08 - Input/Output Voltage Monitoring  (0x08)
 *  8-bit register (Read-only status)
 * ========================================================================== */
#define TPS55288_REG_VOLTMON            0x08U
#define TPS55288_UVP1_POS               0U
#define TPS55288_UVP1_MSK               (1U << 0)
#define TPS55288_OVP1_POS               1U
#define TPS55288_OVP1_MSK               (1U << 1)
#define TPS55288_UVP2_POS               2U
#define TPS55288_UVP2_MSK               (1U << 2)
#define TPS55288_OVP2_POS               3U
#define TPS55288_OVP2_MSK               (1U << 3)

/* ==========================================================================
 *  REG09 - Temperature/Thermal Status  (0x09)
 *  8-bit register (Read-only status)
 * ========================================================================== */
#define TPS55288_REG_THERMAL            0x09U
#define TPS55288_TDIE_POS               0U
#define TPS55288_TDIE_MSK               0xFFU
#define TPS55288_TDIE_STEP_DEGC         1U

/* ==========================================================================
 *  REG0A - Output Current Monitoring  (0x0A)
 *  8-bit register (Read-only status)
 * ========================================================================== */
#define TPS55288_REG_IOUT               0x0AU
#define TPS55288_IOUT1_POS              0U
#define TPS55288_IOUT1_MSK              0x0FU
#define TPS55288_IOUT1_STEP_MA          100U
#define TPS55288_IOUT2_POS              4U
#define TPS55288_IOUT2_MSK              0xF0U
#define TPS55288_IOUT2_STEP_MA          100U

/* ==========================================================================
 *  REG0B - Status/Interrupt Register  (0x0B)
 *  8-bit register
 * ========================================================================== */
#define TPS55288_REG_STATUS             0x0BU
#define TPS55288_INT1_POS               0U
#define TPS55288_INT1_MSK               (1U << 0)
#define TPS55288_INT2_POS               1U
#define TPS55288_INT2_MSK               (1U << 1)
#define TPS55288_ACTIVE1_POS            2U
#define TPS55288_ACTIVE1_MSK            (1U << 2)
#define TPS55288_ACTIVE2_POS            3U
#define TPS55288_ACTIVE2_MSK            (1U << 3)

/* ==========================================================================
 *  REG0C - Protection Thresholds  (0x0C)
 *  8-bit register
 * ========================================================================== */
#define TPS55288_REG_PROTECT            0x0CU
#define TPS55288_ILIM_OVP_POS           0U
#define TPS55288_ILIM_OVP_MSK           0x07U
#define TPS55288_THERMAL_DIS_POS        4U
#define TPS55288_THERMAL_DIS_MSK        (1U << 4)

/* ==========================================================================
 *  REG0D - Tracking Control  (0x0D)
 *  8-bit register
 * ========================================================================== */
#define TPS55288_REG_TRACK              0x0DU
#define TPS55288_TRACK_MODE_POS         0U
#define TPS55288_TRACK_MODE_MSK         0x03U
#define TPS55288_TRACK_INDEPENDENT      0U
#define TPS55288_TRACK_CH2_FOLLOWS_CH1  1U
#define TPS55288_TRACK_CH1_FOLLOWS_CH2  2U
#define TPS55288_TRACK_PHASE_SHIFT      3U

#define TPS55288_RAMP_RATE_POS          2U
#define TPS55288_RAMP_RATE_MSK          0x0CU
#define TPS55288_RAMP_1mV_us            0U
#define TPS55288_RAMP_2mV_us            1U
#define TPS55288_RAMP_5mV_us            2U
#define TPS55288_RAMP_10mV_us           3U

/* ==========================================================================
 *  Helper Macros for Conversion
 * ========================================================================== */
#define TPS55288_MV_TO_VOUT_DAC(mv) \
  (((uint16_t)(mv) - TPS55288_VOUT1_BASE_MV) / TPS55288_VOUT1_STEP_MV)

#define TPS55288_VOUT_DAC_TO_MV(dac) \
  (((uint16_t)(dac) * TPS55288_VOUT1_STEP_MV) + TPS55288_VOUT1_BASE_MV)

#define TPS55288_MA_TO_ILIM_DAC(ma) \
  (((uint16_t)(ma) - TPS55288_ILIM1_BASE_MA) / TPS55288_ILIM1_STEP_MA)

#define TPS55288_ILIM_DAC_TO_MA(dac) \
  (((uint16_t)(dac) * TPS55288_ILIM1_STEP_MA) + TPS55288_ILIM1_BASE_MA)

#ifdef __cplusplus
}
#endif

#endif /* __TPS55288_REGS_H */
