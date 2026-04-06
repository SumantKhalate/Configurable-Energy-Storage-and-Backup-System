/**
  ******************************************************************************
  * @file    bq25798_regs.h
  * @brief   BQ25798RQMR Buck-Boost Battery Charger - Register map, bit
  *          definitions, masks, and shift values.
  *
  * @note    Based on the BQ25798 datasheet (SLUSE18) from Texas Instruments.
  *          Default I2C address: 0x6B (7-bit).
  *          Register width: 8-bit, some fields span 16-bit (big-endian).
  ******************************************************************************
  */

#ifndef __BQ25798_REGS_H
#define __BQ25798_REGS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* ==========================================================================
 *  I2C ADDRESS
 * ========================================================================== */
#define BQ25798_I2C_ADDR_7BIT           0x6BU
#define BQ25798_I2C_ADDR_WRITE          (BQ25798_I2C_ADDR_7BIT << 1)          /* 0xD6 */
#define BQ25798_I2C_ADDR_READ           ((BQ25798_I2C_ADDR_7BIT << 1) | 1U)   /* 0xD7 */

/* ==========================================================================
 *  DEVICE IDENTIFICATION
 * ========================================================================== */
#define BQ25798_REG_PART_INFO           0x48U  /* Part Information Register */
#define BQ25798_PART_NUMBER_POS         3U
#define BQ25798_PART_NUMBER_MSK         (0x07U << 3)
#define BQ25798_PART_NUMBER_BQ25798     0x03U  /* Expected part number for BQ25798 */
#define BQ25798_DEV_REV_POS             0U
#define BQ25798_DEV_REV_MSK             0x07U

/* ==========================================================================
 *  REG00 - Minimal System Voltage  (0x00)
 *  8-bit register
 * ========================================================================== */
#define BQ25798_REG_VSYSMIN             0x00U
/* VSYS_MIN: Minimum System Voltage
 * Bits [5:0]
 * Range: 2500mV - 16000mV
 * Step: 250mV
 * Default: 3500mV (for 1S), varies by configuration
 */
#define BQ25798_VSYSMIN_POS             0U
#define BQ25798_VSYSMIN_MSK             0x3FU   /* bits [5:0] of REG00 */
#define BQ25798_VSYSMIN_BASE_MV         2500U
#define BQ25798_VSYSMIN_STEP_MV         250U

/* ==========================================================================
 *  REG01 - Charge Voltage Limit  (0x01-0x02)
 *  16-bit register
 * ========================================================================== */
#define BQ25798_REG_VREG                0x01U
/* VREG: Charge Voltage Limit
 * Range: 3000mV - 18800mV
 * Step: 10mV
 * Default: 4200mV
 */
#define BQ25798_VREG_POS                0U
#define BQ25798_VREG_MSK                0x07FFU  /* bits [10:0] */
#define BQ25798_VREG_BASE_MV            0U
#define BQ25798_VREG_STEP_MV            10U
#define BQ25798_VREG_MIN_MV             3000U
#define BQ25798_VREG_MAX_MV             18800U
#define BQ25798_VREG_DEFAULT_MV         4200U

/* ==========================================================================
 *  REG03 - Charge Current Limit  (0x03-0x04)
 *  16-bit register
 * ========================================================================== */
#define BQ25798_REG_ICHG                0x03U
/* ICHG: Charge Current Limit
 * Range: 50mA - 5000mA
 * Step: 10mA
 * Default: 1000mA
 */
#define BQ25798_ICHG_POS                0U
#define BQ25798_ICHG_MSK                0x01FFU  /* bits [8:0] */
#define BQ25798_ICHG_BASE_MA            0U
#define BQ25798_ICHG_STEP_MA            10U
#define BQ25798_ICHG_MIN_MA             50U
#define BQ25798_ICHG_MAX_MA             5000U
#define BQ25798_ICHG_DEFAULT_MA         1000U

/* ==========================================================================
 *  REG05 - Input Voltage Limit  (0x05)
 *  8-bit register
 * ========================================================================== */
#define BQ25798_REG_VINDPM              0x05U
/* VINDPM: Input Voltage Limit (Dynamic Power Management)
 * Bits [7:0]
 * Range: 3600mV - 22000mV
 * Step: 100mV
 * Default: 4300mV (auto)
 */
#define BQ25798_VINDPM_POS              0U
#define BQ25798_VINDPM_MSK              0xFFU   /* bits [7:0] */
#define BQ25798_VINDPM_BASE_MV          0U
#define BQ25798_VINDPM_STEP_MV          100U
#define BQ25798_VINDPM_MIN_MV           3600U
#define BQ25798_VINDPM_MAX_MV           22000U

/* ==========================================================================
 *  REG06 - Input Current Limit  (0x06-0x07)
 *  16-bit register
 * ========================================================================== */
#define BQ25798_REG_IINDPM              0x06U
/* IINDPM: Input Current Limit
 * Range: 100mA - 3300mA
 * Step: 10mA
 * Default: 500mA
 */
#define BQ25798_IINDPM_POS              0U
#define BQ25798_IINDPM_MSK              0x01FFU  /* bits [8:0] */
#define BQ25798_IINDPM_BASE_MA          0U
#define BQ25798_IINDPM_STEP_MA          10U
#define BQ25798_IINDPM_MIN_MA           100U
#define BQ25798_IINDPM_MAX_MA           3300U
#define BQ25798_IINDPM_DEFAULT_MA       500U

/* ==========================================================================
 *  REG08 - Precharge Control  (0x08)
 *  8-bit register
 * ========================================================================== */
#define BQ25798_REG_PRECHG              0x08U

/* VBAT_LOWV: Battery voltage threshold for precharge-to-fast-charge transition */
#define BQ25798_VBAT_LOWV_POS           6U
#define BQ25798_VBAT_LOWV_MSK           (0x03U << 6)
#define BQ25798_VBAT_LOWV_15PCT         0x00U  /* 15% of VREG (default) */
#define BQ25798_VBAT_LOWV_62_2PCT       0x01U  /* 62.2% of VREG */
#define BQ25798_VBAT_LOWV_66_7PCT       0x02U  /* 66.7% of VREG */
#define BQ25798_VBAT_LOWV_71_4PCT       0x03U  /* 71.4% of VREG */

/* IPRECHG: Precharge Current Limit
 * Range: 40mA - 2000mA
 * Step: 40mA
 * Default: 120mA
 */
#define BQ25798_IPRECHG_POS             0U
#define BQ25798_IPRECHG_MSK             0x3FU
#define BQ25798_IPRECHG_BASE_MA         0U
#define BQ25798_IPRECHG_STEP_MA         40U
#define BQ25798_IPRECHG_MIN_MA          40U
#define BQ25798_IPRECHG_MAX_MA          2000U

/* ==========================================================================
 *  REG09 - Termination Control  (0x09)
 *  8-bit register
 * ========================================================================== */
#define BQ25798_REG_TERM                0x09U

/* REG_RST: Register reset (write 1 to reset all registers) */
#define BQ25798_REG_RST_POS             7U
#define BQ25798_REG_RST_MSK             (1U << 7)

/* ITERM: Termination Current Limit
 * Range: 40mA - 1000mA
 * Step: 40mA
 * Default: 200mA
 */
#define BQ25798_ITERM_POS               0U
#define BQ25798_ITERM_MSK               0x1FU
#define BQ25798_ITERM_BASE_MA           0U
#define BQ25798_ITERM_STEP_MA           40U
#define BQ25798_ITERM_MIN_MA            40U
#define BQ25798_ITERM_MAX_MA            1000U

/* ==========================================================================
 *  REG0A - Recharge Control  (0x0A)
 *  8-bit register
 * ========================================================================== */
#define BQ25798_REG_RECHRG              0x0AU

/* CELL_CFG: Battery cell configuration */
#define BQ25798_CELL_CFG_POS            6U
#define BQ25798_CELL_CFG_MSK            (0x03U << 6)
#define BQ25798_CELL_1S                 0x00U
#define BQ25798_CELL_2S                 0x01U
#define BQ25798_CELL_3S                 0x02U
#define BQ25798_CELL_4S                 0x03U

/* TRECHG: Recharge threshold below VREG */
#define BQ25798_VRECHG_POS              4U
#define BQ25798_VRECHG_MSK              (0x03U << 4)
#define BQ25798_VRECHG_50MV             0x00U  /* 50mV below VREG (default) */
#define BQ25798_VRECHG_100MV            0x01U  /* 100mV below VREG */
#define BQ25798_VRECHG_200MV            0x02U  /* 200mV below VREG */
#define BQ25798_VRECHG_400MV            0x03U  /* 400mV below VREG */

/* ==========================================================================
 *  REG0B - VOTG Regulation  (0x0B-0x0C)
 *  16-bit register
 * ========================================================================== */
#define BQ25798_REG_VOTG                0x0BU
/* VOTG: OTG Mode Output Voltage
 * Range: 2800mV - 22000mV
 * Step: 10mV
 * Default: 5000mV
 */
#define BQ25798_VOTG_POS                0U
#define BQ25798_VOTG_MSK                0x07FFU
#define BQ25798_VOTG_BASE_MV            0U
#define BQ25798_VOTG_STEP_MV            10U
#define BQ25798_VOTG_MIN_MV             2800U
#define BQ25798_VOTG_MAX_MV             22000U

/* ==========================================================================
 *  REG0D - IOTG Regulation  (0x0D)
 *  8-bit register
 * ========================================================================== */
#define BQ25798_REG_IOTG                0x0DU
/* IOTG: OTG Mode Output Current Limit
 * Bits [6:0]
 * Range: 0mA - 3200mA
 * Step: 50mA
 * Default: 1280mA
 */
#define BQ25798_IOTG_POS                0U
#define BQ25798_IOTG_MSK                0x7FU    /* bits [6:0] */
#define BQ25798_IOTG_STEP_MA            50U
#define BQ25798_IOTG_MIN_MA             0U
#define BQ25798_IOTG_MAX_MA             3200U

/* ==========================================================================
 *  REG0E - Timer Control  (0x0E)
 *  8-bit register
 * ========================================================================== */
#define BQ25798_REG_TIMER_CTRL          0x0EU

/* TRECHG_TMR / TOP_OFF_TMR combined */
#define BQ25798_TOP_OFF_TMR_POS         6U
#define BQ25798_TOP_OFF_TMR_MSK         (0x03U << 6)
#define BQ25798_TOP_OFF_TMR_DIS         0x00U
#define BQ25798_TOP_OFF_TMR_15MIN       0x01U
#define BQ25798_TOP_OFF_TMR_30MIN       0x02U
#define BQ25798_TOP_OFF_TMR_45MIN       0x03U

/* EN_TRICHG_TMR: Enable trickle charge safety timer */
#define BQ25798_EN_TRICHG_TMR_POS       5U
#define BQ25798_EN_TRICHG_TMR_MSK       (1U << 5)

/* EN_PRECHG_TMR: Enable precharge safety timer */
#define BQ25798_EN_PRECHG_TMR_POS       4U
#define BQ25798_EN_PRECHG_TMR_MSK       (1U << 4)

/* EN_CHG_TMR: Enable fast charge safety timer */
#define BQ25798_EN_CHG_TMR_POS          3U
#define BQ25798_EN_CHG_TMR_MSK          (1U << 3)

/* CHG_TMR: Fast charge timer setting */
#define BQ25798_CHG_TMR_POS             0U
#define BQ25798_CHG_TMR_MSK             (0x03U << 0)
#define BQ25798_CHG_TMR_5H              0x00U
#define BQ25798_CHG_TMR_8H              0x01U
#define BQ25798_CHG_TMR_12H             0x02U  /* Default */
#define BQ25798_CHG_TMR_24H             0x03U

/* TMR2X_EN: Enable 2x timer */
#define BQ25798_TMR2X_EN_POS             2U
#define BQ25798_TMR2X_EN_MSK             (1U << 2)

/* ==========================================================================
 *  REG0F - Charger Control 0  (0x0F)
 *  8-bit register
 * ========================================================================== */
#define BQ25798_REG_CHG_CTRL0           0x0FU

/* EN_AUTO_IBATDIS: Enable auto battery discharging during input overvoltage */
#define BQ25798_EN_AUTO_IBATDIS_POS     7U
#define BQ25798_EN_AUTO_IBATDIS_MSK     (1U << 7)

/* FORCE_IBATDIS: Force battery discharging */
#define BQ25798_FORCE_IBATDIS_POS       6U
#define BQ25798_FORCE_IBATDIS_MSK       (1U << 6)

/* EN_CHG: Charger Enable (1=enable, 0=disable) */
#define BQ25798_EN_CHG_POS              5U
#define BQ25798_EN_CHG_MSK              (1U << 5)

/* EN_ICO: Enable Input Current Optimizer */
#define BQ25798_EN_ICO_POS              4U
#define BQ25798_EN_ICO_MSK              (1U << 4)

/* FORCE_ICO: Force Input Current Optimizer */
#define BQ25798_FORCE_ICO_POS           3U
#define BQ25798_FORCE_ICO_MSK           (1U << 3)

/* EN_HIZ: Enable High Impedance mode */
#define BQ25798_EN_HIZ_POS              2U
#define BQ25798_EN_HIZ_MSK              (1U << 2)

/* EN_TERM: Enable termination (1=enable, 0=disable) */
#define BQ25798_EN_TERM_POS             1U
#define BQ25798_EN_TERM_MSK             (1U << 1)

/* ==========================================================================
 *  REG10 - Charger Control 1  (0x10)
 *  8-bit register
 * ========================================================================== */
#define BQ25798_REG_CHG_CTRL1           0x10U

/* VAC_OVP: VAC Over-Voltage Protection threshold */
#define BQ25798_VAC_OVP_POS             5U
#define BQ25798_VAC_OVP_MSK             (0x03U << 5)
#define BQ25798_VAC_OVP_26V             0x00U
#define BQ25798_VAC_OVP_22V             0x01U
#define BQ25798_VAC_OVP_12V             0x02U
#define BQ25798_VAC_OVP_7V              0x03U

/* WD_RST: Watchdog timer reset (write 1 to reset) */
#define BQ25798_WD_RST_POS              3U
#define BQ25798_WD_RST_MSK              (1U << 3)

/* WATCHDOG: Watchdog timer setting */
#define BQ25798_WATCHDOG_POS            0U
#define BQ25798_WATCHDOG_MSK            0x07U
#define BQ25798_WATCHDOG_DIS            0x00U
#define BQ25798_WATCHDOG_0_5S           0x01U
#define BQ25798_WATCHDOG_1S             0x02U
#define BQ25798_WATCHDOG_2S             0x03U
#define BQ25798_WATCHDOG_20S            0x04U
#define BQ25798_WATCHDOG_40S            0x05U
#define BQ25798_WATCHDOG_80S            0x06U
#define BQ25798_WATCHDOG_160S           0x07U

/* ==========================================================================
 *  REG11 - Charger Control 2  (0x11)
 *  8-bit register
 * ========================================================================== */
#define BQ25798_REG_CHG_CTRL2           0x11U

/* FORCE_INDET: Force input source detection */
#define BQ25798_FORCE_INDET_POS         7U
#define BQ25798_FORCE_INDET_MSK         (1U << 7)

/* AUTO_INDET_EN: Enable automatic input source detection */
#define BQ25798_AUTO_INDET_EN_POS       6U
#define BQ25798_AUTO_INDET_EN_MSK       (1U << 6)

/* EN_12V: Enable 12V input support */
#define BQ25798_EN_12V_POS              5U
#define BQ25798_EN_12V_MSK              (1U << 5)

/* EN_9V: Enable 9V HVDCP handshake */
#define BQ25798_EN_9V_POS               4U
#define BQ25798_EN_9V_MSK               (1U << 4)

/* HVDCP_EN: Enable HVDCP handshake */
#define BQ25798_HVDCP_EN_POS            3U
#define BQ25798_HVDCP_EN_MSK            (1U << 3)

/* SDRV_CTRL: External SFET driver control */
#define BQ25798_SDRV_CTRL_POS           1U
#define BQ25798_SDRV_CTRL_MSK           (0x03U << 1)
#define BQ25798_SDRV_IDLE               0x00U
#define BQ25798_SDRV_SHUTDOWN           0x01U
#define BQ25798_SDRV_SHIP               0x02U
#define BQ25798_SDRV_SYS_PWR_RST       0x03U

/* SDRV_DLY: Ship mode delay */
#define BQ25798_SDRV_DLY_POS            0U
#define BQ25798_SDRV_DLY_MSK            (1U << 0)

/* ==========================================================================
 *  REG12 - Charger Control 3  (0x12)
 *  8-bit register
 * ========================================================================== */
#define BQ25798_REG_CHG_CTRL3           0x12U

/* DIS_ACDRV: Disable AC driver */
#define BQ25798_DIS_ACDRV_POS           7U
#define BQ25798_DIS_ACDRV_MSK           (1U << 7)

/* EN_OTG: Enable OTG (boost) mode */
#define BQ25798_EN_OTG_POS              6U
#define BQ25798_EN_OTG_MSK              (1U << 6)

/* PFM_OTG_DIS: Disable PFM in OTG mode */
#define BQ25798_PFM_OTG_DIS_POS         5U
#define BQ25798_PFM_OTG_DIS_MSK         (1U << 5)

/* PFM_FWD_DIS: Disable PFM in forward mode */
#define BQ25798_PFM_FWD_DIS_POS         4U
#define BQ25798_PFM_FWD_DIS_MSK         (1U << 4)

/* DIS_OTG_OOA: Disable OTG out-of-audio mode */
#define BQ25798_DIS_OTG_OOA_POS         3U
#define BQ25798_DIS_OTG_OOA_MSK         (1U << 3)

/* DIS_FWD_OOA: Disable forward out-of-audio mode */
#define BQ25798_DIS_FWD_OOA_POS         2U
#define BQ25798_DIS_FWD_OOA_MSK         (1U << 2)

/* EN_ACDRV2: Enable ACDRV2 gate driver */
#define BQ25798_EN_ACDRV2_POS           1U
#define BQ25798_EN_ACDRV2_MSK           (1U << 1)

/* EN_ACDRV1: Enable ACDRV1 gate driver */
#define BQ25798_EN_ACDRV1_POS           0U
#define BQ25798_EN_ACDRV1_MSK           (1U << 0)

/* ==========================================================================
 *  REG13 - Charger Control 4  (0x13)
 *  8-bit register
 * ========================================================================== */
#define BQ25798_REG_CHG_CTRL4           0x13U

/* EN_ACDRV2_HiZ: ACDRV2 high-Z control */
#define BQ25798_EN_ACDRV2_HIZ_POS       7U
#define BQ25798_EN_ACDRV2_HIZ_MSK       (1U << 7)

/* EN_ACDRV1_HiZ: ACDRV1 high-Z control */
#define BQ25798_EN_ACDRV1_HIZ_POS       6U
#define BQ25798_EN_ACDRV1_HIZ_MSK       (1U << 6)

/* PWM_FREQ: Switching frequency */
#define BQ25798_PWM_FREQ_POS            5U
#define BQ25798_PWM_FREQ_MSK            (1U << 5)
#define BQ25798_PWM_FREQ_750K           0x00U
#define BQ25798_PWM_FREQ_1500K          0x01U

/* DIS_STAT: Disable STAT pin output */
#define BQ25798_DIS_STAT_POS            4U
#define BQ25798_DIS_STAT_MSK            (1U << 4)

/* DIS_VSYS_SHORT: Disable VSYS short hiccup protection */
#define BQ25798_DIS_VSYS_SHORT_POS      3U
#define BQ25798_DIS_VSYS_SHORT_MSK      (1U << 3)

/* DIS_VOTG_UVP: Disable VOTG UVP hiccup */
#define BQ25798_DIS_VOTG_UVP_POS        2U
#define BQ25798_DIS_VOTG_UVP_MSK        (1U << 2)

/* FORCE_VINDPM_DET: Force VINDPM detection */
#define BQ25798_FORCE_VINDPM_DET_POS    1U
#define BQ25798_FORCE_VINDPM_DET_MSK    (1U << 1)

/* EN_IBUS_OCP: Enable IBUS OCP */
#define BQ25798_EN_IBUS_OCP_POS         0U
#define BQ25798_EN_IBUS_OCP_MSK         (1U << 0)

/* ==========================================================================
 *  REG14 - Charger Control 5  (0x14)
 *  8-bit register
 * ========================================================================== */
#define BQ25798_REG_CHG_CTRL5           0x14U

/* SFET_PRESENT: External ship FET present */
#define BQ25798_SFET_PRESENT_POS        7U
#define BQ25798_SFET_PRESENT_MSK        (1U << 7)

/* EN_IBAT: Enable IBAT pin for battery discharge current sensing */
#define BQ25798_EN_IBAT_POS             6U
#define BQ25798_EN_IBAT_MSK             (1U << 6)

/* IBAT_REG: IBAT regulation threshold */
#define BQ25798_IBAT_REG_POS            4U
#define BQ25798_IBAT_REG_MSK            (0x03U << 4)

/* EN_IINDPM: Enable IINDPM (auto IINDPM detect) */
#define BQ25798_EN_IINDPM_POS           3U
#define BQ25798_EN_IINDPM_MSK           (1U << 3)

/* EN_EXTILIM: Enable external ILIM pin */
#define BQ25798_EN_EXTILIM_POS          2U
#define BQ25798_EN_EXTILIM_MSK          (1U << 2)

/* EN_BATOC: Enable battery OCP */
#define BQ25798_EN_BATOC_POS            1U
#define BQ25798_EN_BATOC_MSK            (1U << 1)

/* PKFB_DIS: Disable peak current feedback */
#define BQ25798_PKFB_DIS_POS            0U
#define BQ25798_PKFB_DIS_MSK            (1U << 0)

/* ==========================================================================
 *  REG15 - MPPT Control  (0x15)
 *  8-bit register
 * ========================================================================== */
#define BQ25798_REG_MPPT_CTRL           0x15U

/* MPPT_EN: Enable MPPT (Maximum Power Point Tracking) */
#define BQ25798_MPPT_EN_POS             7U
#define BQ25798_MPPT_EN_MSK             (1U << 7)

/* MPPT_RATIO: VINDPM = MPPT_RATIO * VBAT */
#define BQ25798_MPPT_RATIO_POS          4U
#define BQ25798_MPPT_RATIO_MSK          (0x07U << 4)

/* ==========================================================================
 *  REG16 - Temperature Control  (0x16)
 *  8-bit register
 * ========================================================================== */
#define BQ25798_REG_TEMP_CTRL           0x16U

/* TREG: Thermal regulation threshold */
#define BQ25798_TREG_POS                6U
#define BQ25798_TREG_MSK                (0x03U << 6)
#define BQ25798_TREG_60C                0x00U
#define BQ25798_TREG_80C                0x01U
#define BQ25798_TREG_100C               0x02U
#define BQ25798_TREG_120C               0x03U  /* Default */

/* TSHUT: Thermal shutdown threshold */
#define BQ25798_TSHUT_POS               4U
#define BQ25798_TSHUT_MSK               (0x03U << 4)
#define BQ25798_TSHUT_150C              0x00U  /* Default */
#define BQ25798_TSHUT_130C              0x01U
#define BQ25798_TSHUT_120C              0x02U
#define BQ25798_TSHUT_85C               0x03U

/* ==========================================================================
 *  REG17 - NTC Control 0  (0x17)
 *  8-bit register
 * ========================================================================== */
#define BQ25798_REG_NTC_CTRL0           0x17U

/* JEITA_VSET: JEITA high temperature voltage setting */
#define BQ25798_JEITA_VSET_POS          7U
#define BQ25798_JEITA_VSET_MSK          (1U << 7)
#define BQ25798_JEITA_VSET_VREG         0x00U  /* Charge to VREG */
#define BQ25798_JEITA_VSET_VREG_200MV   0x01U  /* Charge to VREG - 200mV */

/* JEITA_ISETH: JEITA high temperature current setting */
#define BQ25798_JEITA_ISETH_POS         5U
#define BQ25798_JEITA_ISETH_MSK         (0x03U << 5)

/* JEITA_ISETC: JEITA cool temperature current setting */
#define BQ25798_JEITA_ISETC_POS         3U
#define BQ25798_JEITA_ISETC_MSK         (0x03U << 3)

/* TS_IGNORE: Ignore TS (disable NTC temperature sensing) */
#define BQ25798_TS_IGNORE_POS           2U
#define BQ25798_TS_IGNORE_MSK           (1U << 2)

/* TS_ISET_COOL: Reserved/TS cool current fold-back */
#define BQ25798_FORCE_TSB_POS           1U
#define BQ25798_FORCE_TSB_MSK           (1U << 1)

/* FORCE_TS_COLD: Force TS cold */
#define BQ25798_FORCE_TS_COLD_POS       0U
#define BQ25798_FORCE_TS_COLD_MSK       (1U << 0)

/* ==========================================================================
 *  REG18 - NTC Control 1  (0x18)
 *  8-bit register
 * ========================================================================== */
#define BQ25798_REG_NTC_CTRL1           0x18U

/* TS_COOL: TS cool temperature threshold (% of REGN) */
#define BQ25798_TS_COOL_POS             6U
#define BQ25798_TS_COOL_MSK             (0x03U << 6)

/* TS_WARM: TS warm temperature threshold (% of REGN) */
#define BQ25798_TS_WARM_POS             4U
#define BQ25798_TS_WARM_MSK             (0x03U << 4)

/* BHOT: TS hot boost mode threshold */
#define BQ25798_BHOT_POS                2U
#define BQ25798_BHOT_MSK                (0x03U << 2)

/* BCOLD: TS cold boost mode threshold */
#define BQ25798_BCOLD_POS               0U
#define BQ25798_BCOLD_MSK               (0x03U << 0)

/* ==========================================================================
 *  REG19 - ICO Current Limit  (0x19-0x1A)
 *  16-bit register (read-only)
 * ========================================================================== */
#define BQ25798_REG_ICO_ILIM            0x19U
#define BQ25798_ICO_ILIM_POS            0U
#define BQ25798_ICO_ILIM_MSK            0x01FFU
#define BQ25798_ICO_ILIM_STEP_MA        10U

/* ==========================================================================
 *  REG1B - Charger Status 0  (0x1B)
 *  8-bit register (read-only)
 * ========================================================================== */
#define BQ25798_REG_CHG_STATUS0         0x1BU

/* IINDPM_STAT: In IINDPM regulation */
#define BQ25798_IINDPM_STAT_POS         7U
#define BQ25798_IINDPM_STAT_MSK         (1U << 7)

/* VINDPM_STAT: In VINDPM regulation */
#define BQ25798_VINDPM_STAT_POS         6U
#define BQ25798_VINDPM_STAT_MSK         (1U << 6)

/* WD_STAT: Watchdog timer expiration status */
#define BQ25798_WD_STAT_POS             5U
#define BQ25798_WD_STAT_MSK             (1U << 5)

/* PG_STAT: Power good status */
#define BQ25798_PG_STAT_POS             3U
#define BQ25798_PG_STAT_MSK             (1U << 3)

/* AC2_PRESENT_STAT: AC2 present */
#define BQ25798_AC2_PRESENT_STAT_POS    2U
#define BQ25798_AC2_PRESENT_STAT_MSK    (1U << 2)

/* AC1_PRESENT_STAT: AC1 present */
#define BQ25798_AC1_PRESENT_STAT_POS    1U
#define BQ25798_AC1_PRESENT_STAT_MSK    (1U << 1)

/* VBUS_PRESENT_STAT: VBUS present */
#define BQ25798_VBUS_PRESENT_STAT_POS   0U
#define BQ25798_VBUS_PRESENT_STAT_MSK   (1U << 0)

/* ==========================================================================
 *  REG1C - Charger Status 1  (0x1C)
 *  8-bit register (read-only)
 * ========================================================================== */
#define BQ25798_REG_CHG_STATUS1         0x1CU

/* CHG_STAT: Charge status */
#define BQ25798_CHG_STAT_POS            5U
#define BQ25798_CHG_STAT_MSK            (0x07U << 5)
#define BQ25798_CHG_STAT_NOT_CHARGING   0x00U
#define BQ25798_CHG_STAT_TRICKLE        0x01U
#define BQ25798_CHG_STAT_PRECHARGE      0x02U
#define BQ25798_CHG_STAT_FAST_CHARGE    0x03U
#define BQ25798_CHG_STAT_TAPER_CHARGE   0x04U
#define BQ25798_CHG_STAT_TOP_OFF_TMR    0x06U
#define BQ25798_CHG_STAT_DONE           0x07U

/* VBUS_STAT: VBUS status (input source type) */
#define BQ25798_VBUS_STAT_POS           1U
#define BQ25798_VBUS_STAT_MSK           (0x0FU << 1)
#define BQ25798_VBUS_STAT_NOINPUT       0x00U
#define BQ25798_VBUS_STAT_USB_SDP       0x01U
#define BQ25798_VBUS_STAT_USB_CDP       0x02U
#define BQ25798_VBUS_STAT_USB_DCP       0x03U
#define BQ25798_VBUS_STAT_ADJ_DCP       0x04U
#define BQ25798_VBUS_STAT_UNKNOWN       0x05U
#define BQ25798_VBUS_STAT_NON_STD       0x06U
#define BQ25798_VBUS_STAT_OTG           0x07U
#define BQ25798_VBUS_STAT_NOT_QUAL      0x08U
#define BQ25798_VBUS_STAT_HVDCP         0x0BU
#define BQ25798_VBUS_STAT_POORSRC       0x0FU

/* BC1_2_DONE_STAT: BC1.2 detection done */
#define BQ25798_BC12_DONE_STAT_POS      0U
#define BQ25798_BC12_DONE_STAT_MSK      (1U << 0)

/* ==========================================================================
 *  REG1D - Charger Status 2  (0x1D)
 *  8-bit register (read-only)
 * ========================================================================== */
#define BQ25798_REG_CHG_STATUS2         0x1DU

/* ICO_STAT: ICO status */
#define BQ25798_ICO_STAT_POS            6U
#define BQ25798_ICO_STAT_MSK            (0x03U << 6)
#define BQ25798_ICO_STAT_DISABLED       0x00U
#define BQ25798_ICO_STAT_IN_PROGRESS    0x01U
#define BQ25798_ICO_STAT_MAX_DETECTED   0x02U

/* TREG_STAT: In thermal regulation */
#define BQ25798_TREG_STAT_POS           2U
#define BQ25798_TREG_STAT_MSK           (1U << 2)

/* DPDM_STAT: D+/D- detection ongoing */
#define BQ25798_DPDM_STAT_POS           1U
#define BQ25798_DPDM_STAT_MSK           (1U << 1)

/* VBAT_PRESENT_STAT: Battery present */
#define BQ25798_VBAT_PRESENT_STAT_POS   0U
#define BQ25798_VBAT_PRESENT_STAT_MSK   (1U << 0)

/* ==========================================================================
 *  REG1E - Charger Status 3  (0x1E)
 *  8-bit register (read-only)
 * ========================================================================== */
#define BQ25798_REG_CHG_STATUS3         0x1EU

/* ACRB2_STAT: ACDRV2/RBFET2 status */
#define BQ25798_ACRB2_STAT_POS          7U
#define BQ25798_ACRB2_STAT_MSK          (1U << 7)

/* ACRB1_STAT: ACDRV1/RBFET1 status */
#define BQ25798_ACRB1_STAT_POS          6U
#define BQ25798_ACRB1_STAT_MSK          (1U << 6)

/* ADC_DONE_STAT: ADC conversion complete */
#define BQ25798_ADC_DONE_STAT_POS       5U
#define BQ25798_ADC_DONE_STAT_MSK       (1U << 5)

/* VSYS_STAT: VSYS regulation status */
#define BQ25798_VSYS_STAT_POS           4U
#define BQ25798_VSYS_STAT_MSK           (1U << 4)

/* CHG_TMR_STAT: Fast charge timer expiration */
#define BQ25798_CHG_TMR_STAT_POS        3U
#define BQ25798_CHG_TMR_STAT_MSK        (1U << 3)

/* TRICHG_TMR_STAT: Trickle charge timer expiration */
#define BQ25798_TRICHG_TMR_STAT_POS     2U
#define BQ25798_TRICHG_TMR_STAT_MSK     (1U << 2)

/* PRECHG_TMR_STAT: Precharge timer expiration */
#define BQ25798_PRECHG_TMR_STAT_POS     1U
#define BQ25798_PRECHG_TMR_STAT_MSK     (1U << 1)

/* ==========================================================================
 *  REG1F - Charger Status 4  (0x1F)
 *  8-bit register (read-only)
 * ========================================================================== */
#define BQ25798_REG_CHG_STATUS4         0x1FU

/* VBUS_OVP_STAT: VBUS OVP */
#define BQ25798_VBUS_OVP_STAT_POS       7U
#define BQ25798_VBUS_OVP_STAT_MSK       (1U << 7)

/* IBUS_OCP_STAT: IBUS OCP */
#define BQ25798_IBUS_OCP_STAT_POS       6U
#define BQ25798_IBUS_OCP_STAT_MSK       (1U << 6)

/* CONV_OCP_STAT: Converter OCP */
#define BQ25798_CONV_OCP_STAT_POS       5U
#define BQ25798_CONV_OCP_STAT_MSK       (1U << 5)

/* IBAT_OCP_STAT: IBAT OCP */
#define BQ25798_IBAT_OCP_STAT_POS       4U
#define BQ25798_IBAT_OCP_STAT_MSK       (1U << 4)

/* VBAT_OVP_STAT: VBAT OVP */
#define BQ25798_VBAT_OVP_STAT_POS       3U
#define BQ25798_VBAT_OVP_STAT_MSK       (1U << 3)

/* IBAT_REG_STAT: IBAT regulation */
#define BQ25798_IBAT_REG_STAT_POS       2U
#define BQ25798_IBAT_REG_STAT_MSK       (1U << 2)

/* TSHUT_STAT: Thermal shutdown */
#define BQ25798_TSHUT_STAT_POS          0U
#define BQ25798_TSHUT_STAT_MSK          (1U << 0)

/* ==========================================================================
 *  REG20 - FAULT Status 0  (0x20)
 *  8-bit register (read-only, latched)
 * ========================================================================== */
#define BQ25798_REG_FAULT_STATUS0       0x20U

/* IBAT_REG_STAT_F: IBAT regulation fault (latched) */
#define BQ25798_IBAT_REG_F_POS          7U
#define BQ25798_IBAT_REG_F_MSK          (1U << 7)

/* VBUS_OVP_FAULT: VBUS OVP fault */
#define BQ25798_VBUS_OVP_F_POS          6U
#define BQ25798_VBUS_OVP_F_MSK          (1U << 6)

/* VBAT_OVP_FAULT: VBAT OVP fault */
#define BQ25798_VBAT_OVP_F_POS          5U
#define BQ25798_VBAT_OVP_F_MSK          (1U << 5)

/* IBUS_OCP_FAULT: IBUS OCP fault */
#define BQ25798_IBUS_OCP_F_POS          4U
#define BQ25798_IBUS_OCP_F_MSK          (1U << 4)

/* IBAT_OCP_FAULT: IBAT OCP fault */
#define BQ25798_IBAT_OCP_F_POS          3U
#define BQ25798_IBAT_OCP_F_MSK          (1U << 3)

/* CONV_OCP_FAULT: Converter OCP fault */
#define BQ25798_CONV_OCP_F_POS          2U
#define BQ25798_CONV_OCP_F_MSK          (1U << 2)

/* VAC2_OVP_FAULT */
#define BQ25798_VAC2_OVP_F_POS          1U
#define BQ25798_VAC2_OVP_F_MSK          (1U << 1)

/* VAC1_OVP_FAULT */
#define BQ25798_VAC1_OVP_F_POS          0U
#define BQ25798_VAC1_OVP_F_MSK          (1U << 0)

/* ==========================================================================
 *  REG21 - FAULT Status 1  (0x21)
 *  8-bit register (read-only, latched)
 * ========================================================================== */
#define BQ25798_REG_FAULT_STATUS1       0x21U

/* VSYS_SHORT_FAULT */
#define BQ25798_VSYS_SHORT_F_POS        7U
#define BQ25798_VSYS_SHORT_F_MSK        (1U << 7)

/* VSYS_OVP_FAULT */
#define BQ25798_VSYS_OVP_F_POS          6U
#define BQ25798_VSYS_OVP_F_MSK          (1U << 6)

/* OTG_OVP_FAULT */
#define BQ25798_OTG_OVP_F_POS           5U
#define BQ25798_OTG_OVP_F_MSK           (1U << 5)

/* OTG_UVP_FAULT */
#define BQ25798_OTG_UVP_F_POS           4U
#define BQ25798_OTG_UVP_F_MSK           (1U << 4)

/* TSHUT_FAULT: Thermal shutdown fault */
#define BQ25798_TSHUT_F_POS             2U
#define BQ25798_TSHUT_F_MSK             (1U << 2)

/* ==========================================================================
 *  REG22-23 - Charger Flag 0/1 (interrupt flags, read to clear)
 * ========================================================================== */
#define BQ25798_REG_CHG_FLAG0           0x22U
#define BQ25798_REG_CHG_FLAG1           0x23U
#define BQ25798_REG_CHG_FLAG2           0x24U
#define BQ25798_REG_CHG_FLAG3           0x25U

/* ==========================================================================
 *  REG26-29 - Charger Mask 0-3 (interrupt masks)
 * ========================================================================== */
#define BQ25798_REG_CHG_MASK0           0x26U
#define BQ25798_REG_CHG_MASK1           0x27U
#define BQ25798_REG_CHG_MASK2           0x28U
#define BQ25798_REG_CHG_MASK3           0x29U

/* ==========================================================================
 *  REG2A - Fault Mask 0
 * ========================================================================== */
#define BQ25798_REG_FAULT_MASK0         0x2AU
#define BQ25798_REG_FAULT_MASK1         0x2BU

/* ==========================================================================
 *  REG2E - ADC Control  (0x2E)
 *  8-bit register
 * ========================================================================== */
#define BQ25798_REG_ADC_CTRL            0x2EU

/* ADC_EN: Enable ADC */
#define BQ25798_ADC_EN_POS              7U
#define BQ25798_ADC_EN_MSK              (1U << 7)

/* ADC_RATE: ADC conversion rate */
#define BQ25798_ADC_RATE_POS            6U
#define BQ25798_ADC_RATE_MSK            (1U << 6)
#define BQ25798_ADC_RATE_CONT           0x00U  /* Continuous conversion */
#define BQ25798_ADC_RATE_ONESHOT        0x01U  /* One-shot conversion */

/* ADC_SAMPLE: ADC sample speed */
#define BQ25798_ADC_SAMPLE_POS          4U
#define BQ25798_ADC_SAMPLE_MSK          (0x03U << 4)
#define BQ25798_ADC_SAMPLE_15BIT        0x00U  /* 15-bit effective resolution */
#define BQ25798_ADC_SAMPLE_14BIT        0x01U  /* 14-bit */
#define BQ25798_ADC_SAMPLE_13BIT        0x02U  /* 13-bit */
#define BQ25798_ADC_SAMPLE_12BIT        0x03U  /* 12-bit */

/* ADC_AVG: Enable ADC averaging */
#define BQ25798_ADC_AVG_POS             3U
#define BQ25798_ADC_AVG_MSK             (1U << 3)
#define BQ25798_ADC_AVG_SINGLE          0x00U
#define BQ25798_ADC_AVG_RUNNING         0x01U

/* ADC_AVG_INIT: ADC averaging initial value */
#define BQ25798_ADC_AVG_INIT_POS        2U
#define BQ25798_ADC_AVG_INIT_MSK        (1U << 2)

/* ==========================================================================
 *  REG2F - ADC Function Disable  (0x2F)
 *  8-bit register
 * ========================================================================== */
#define BQ25798_REG_ADC_FUNC_DIS        0x2FU

#define BQ25798_IBUS_ADC_DIS_POS        7U
#define BQ25798_IBUS_ADC_DIS_MSK        (1U << 7)
#define BQ25798_IBAT_ADC_DIS_POS        6U
#define BQ25798_IBAT_ADC_DIS_MSK        (1U << 6)
#define BQ25798_VBUS_ADC_DIS_POS        5U
#define BQ25798_VBUS_ADC_DIS_MSK        (1U << 5)
#define BQ25798_VBAT_ADC_DIS_POS        4U
#define BQ25798_VBAT_ADC_DIS_MSK        (1U << 4)
#define BQ25798_VSYS_ADC_DIS_POS        3U
#define BQ25798_VSYS_ADC_DIS_MSK        (1U << 3)
#define BQ25798_TS_ADC_DIS_POS          2U
#define BQ25798_TS_ADC_DIS_MSK          (1U << 2)
#define BQ25798_TDIE_ADC_DIS_POS        1U
#define BQ25798_TDIE_ADC_DIS_MSK        (1U << 1)
#define BQ25798_DP_ADC_DIS_POS          0U
#define BQ25798_DP_ADC_DIS_MSK          (1U << 0)

/* ==========================================================================
 *  REG30 - ADC Function Disable 1  (0x30)
 *  8-bit register
 * ========================================================================== */
#define BQ25798_REG_ADC_FUNC_DIS1       0x30U

#define BQ25798_DM_ADC_DIS_POS          7U
#define BQ25798_DM_ADC_DIS_MSK          (1U << 7)
#define BQ25798_VAC2_ADC_DIS_POS        6U
#define BQ25798_VAC2_ADC_DIS_MSK        (1U << 6)
#define BQ25798_VAC1_ADC_DIS_POS        5U
#define BQ25798_VAC1_ADC_DIS_MSK        (1U << 5)

/* ==========================================================================
 *  REG31-32 - IBUS ADC  (0x31-0x32)
 *  16-bit signed, read-only
 * ========================================================================== */
#define BQ25798_REG_IBUS_ADC            0x31U
#define BQ25798_IBUS_ADC_STEP_MA        1U     /* 1mA/LSB */

/* ==========================================================================
 *  REG33-34 - IBAT ADC  (0x33-0x34)
 *  16-bit signed, read-only
 * ========================================================================== */
#define BQ25798_REG_IBAT_ADC            0x33U
#define BQ25798_IBAT_ADC_STEP_MA        1U     /* 1mA/LSB */

/* ==========================================================================
 *  REG35-36 - VBUS ADC  (0x35-0x36)
 *  16-bit unsigned, read-only
 * ========================================================================== */
#define BQ25798_REG_VBUS_ADC            0x35U
#define BQ25798_VBUS_ADC_STEP_MV        1U     /* 1mV/LSB */

/* ==========================================================================
 *  REG37-38 - VAC1 ADC  (0x37-0x38)
 *  16-bit unsigned, read-only
 * ========================================================================== */
#define BQ25798_REG_VAC1_ADC            0x37U
#define BQ25798_VAC1_ADC_STEP_MV        1U

/* ==========================================================================
 *  REG39-3A - VAC2 ADC  (0x39-0x3A)
 *  16-bit unsigned, read-only
 * ========================================================================== */
#define BQ25798_REG_VAC2_ADC            0x39U
#define BQ25798_VAC2_ADC_STEP_MV        1U

/* ==========================================================================
 *  REG3B-3C - VBAT ADC  (0x3B-0x3C)
 *  16-bit unsigned, read-only
 * ========================================================================== */
#define BQ25798_REG_VBAT_ADC            0x3BU
#define BQ25798_VBAT_ADC_STEP_MV        1U     /* 1mV/LSB */

/* ==========================================================================
 *  REG3D-3E - VSYS ADC  (0x3D-0x3E)
 *  16-bit unsigned, read-only
 * ========================================================================== */
#define BQ25798_REG_VSYS_ADC            0x3DU
#define BQ25798_VSYS_ADC_STEP_MV        1U

/* ==========================================================================
 *  REG3F-40 - TS ADC  (0x3F-0x40)
 *  16-bit unsigned, read-only (percentage of REGN × 0.0976%)
 * ========================================================================== */
#define BQ25798_REG_TS_ADC              0x3FU
#define BQ25798_TS_ADC_STEP_PCT_X100    9766U  /* 0.09766% per LSB (×10000) */

/* ==========================================================================
 *  REG41-42 - TDIE ADC  (0x41-0x42)
 *  16-bit signed, read-only
 * ========================================================================== */
#define BQ25798_REG_TDIE_ADC            0x41U
#define BQ25798_TDIE_ADC_STEP_DEGC_X10  5U     /* 0.5°C per LSB (×10) */

/* ==========================================================================
 *  REG43-44 - D+ ADC  (0x43-0x44)
 *  16-bit unsigned, read-only
 * ========================================================================== */
#define BQ25798_REG_DP_ADC              0x43U
#define BQ25798_DP_ADC_STEP_MV          1U

/* ==========================================================================
 *  REG45-46 - D- ADC  (0x45-0x46)
 *  16-bit unsigned, read-only
 * ========================================================================== */
#define BQ25798_REG_DM_ADC              0x45U
#define BQ25798_DM_ADC_STEP_MV          1U

/* ==========================================================================
 *  REG47 - DPDM Driver  (0x47)
 *  8-bit register
 * ========================================================================== */
#define BQ25798_REG_DPDM_DRV            0x47U

/* DP_DRV: D+ driver */
#define BQ25798_DP_DRV_POS              5U
#define BQ25798_DP_DRV_MSK              (0x07U << 5)

/* DM_DRV: D- driver */
#define BQ25798_DM_DRV_POS              2U
#define BQ25798_DM_DRV_MSK              (0x07U << 2)

/* ==========================================================================
 *  REG48 - Part Information  (already defined above)
 * ========================================================================== */
/* (see BQ25798_REG_PART_INFO at top) */

/* ==========================================================================
 *  Helper Macros
 * ========================================================================== */

/** Convert millivolts to VREG register value */
#define BQ25798_MV_TO_VREG(mv)          ((uint16_t)((mv) / BQ25798_VREG_STEP_MV))
/** Convert VREG register value to millivolts */
#define BQ25798_VREG_TO_MV(reg)         ((uint16_t)((reg) * BQ25798_VREG_STEP_MV))

/** Convert milliamps to ICHG register value */
#define BQ25798_MA_TO_ICHG(ma)          ((uint16_t)((ma) / BQ25798_ICHG_STEP_MA))
/** Convert ICHG register value to milliamps */
#define BQ25798_ICHG_TO_MA(reg)         ((uint16_t)((reg) * BQ25798_ICHG_STEP_MA))

/** Convert milliamps to IINDPM register value */
#define BQ25798_MA_TO_IINDPM(ma)        ((uint16_t)((ma) / BQ25798_IINDPM_STEP_MA))
/** Convert IINDPM register value to milliamps */
#define BQ25798_IINDPM_TO_MA(reg)       ((uint16_t)((reg) * BQ25798_IINDPM_STEP_MA))

/** Convert millivolts to VINDPM register value */
#define BQ25798_MV_TO_VINDPM(mv)        ((uint8_t)((mv) / BQ25798_VINDPM_STEP_MV))
/** Convert VINDPM register value to millivolts */
#define BQ25798_VINDPM_TO_MV(reg)       ((uint16_t)((reg) * BQ25798_VINDPM_STEP_MV))

/** Convert millivolts to VSYSMIN register value */
#define BQ25798_MV_TO_VSYSMIN(mv)       ((uint8_t)(((mv) - BQ25798_VSYSMIN_BASE_MV) / BQ25798_VSYSMIN_STEP_MV))
/** Convert VSYSMIN register value to millivolts */
#define BQ25798_VSYSMIN_TO_MV(reg)      ((uint16_t)(((reg) * BQ25798_VSYSMIN_STEP_MV) + BQ25798_VSYSMIN_BASE_MV))

/** Convert millivolts to VOTG register value */
#define BQ25798_MV_TO_VOTG(mv)          ((uint16_t)((mv) / BQ25798_VOTG_STEP_MV))

/** Convert milliamps to IOTG register value */
#define BQ25798_MA_TO_IOTG(ma)          ((uint8_t)((ma) / BQ25798_IOTG_STEP_MA))

/** Convert milliamps to precharge current register value */
#define BQ25798_MA_TO_IPRECHG(ma)       ((uint8_t)((ma) / BQ25798_IPRECHG_STEP_MA))

/** Convert milliamps to termination current register value */
#define BQ25798_MA_TO_ITERM(ma)         ((uint8_t)((ma) / BQ25798_ITERM_STEP_MA))

#ifdef __cplusplus
}
#endif

#endif /* __BQ25798_REGS_H */
