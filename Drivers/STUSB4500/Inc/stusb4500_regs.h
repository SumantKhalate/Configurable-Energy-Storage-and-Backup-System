/**
  ******************************************************************************
  * @file    stusb4500_regs.h
  * @brief   STUSB4500QTR USB PD sink controller - Register map, bit definitions,
  *          masks, and shift values.
  *
  * @note    Based on the STUSB4500 datasheet (DS12936) and application note
  *          AN5601 from STMicroelectronics.
  *          Default I2C address: 0x28 (7-bit), 0x50/0x51 (8-bit R/W).
  ******************************************************************************
  */

#ifndef __STUSB4500_REGS_H
#define __STUSB4500_REGS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* ==========================================================================
 *  I2C ADDRESS
 * ========================================================================== */
#define STUSB4500_I2C_ADDR_7BIT         0x28U
#define STUSB4500_I2C_ADDR_WRITE        (STUSB4500_I2C_ADDR_7BIT << 1)        /* 0x50 */
#define STUSB4500_I2C_ADDR_READ         ((STUSB4500_I2C_ADDR_7BIT << 1) | 1U) /* 0x51 */

/* ==========================================================================
 *  DEVICE IDENTIFICATION
 * ========================================================================== */
#define STUSB4500_REG_DEVICE_ID         0x2FU
#define STUSB4500_DEVICE_ID_VALUE       0x25U  /* Expected value for STUSB4500 */

/* ==========================================================================
 *  BCD_TYPEC_REV_LOW & HIGH  (0x06 - 0x07)
 *  Type-C Revision supported
 * ========================================================================== */
#define STUSB4500_REG_BCD_TYPEC_REV_LOW   0x06U
#define STUSB4500_REG_BCD_TYPEC_REV_HIGH  0x07U

/* ==========================================================================
 *  BCD_USBPD_REV_LOW & HIGH  (0x08 - 0x09)
 *  USB PD Revision supported
 * ========================================================================== */
#define STUSB4500_REG_BCD_USBPD_REV_LOW  0x08U
#define STUSB4500_REG_BCD_USBPD_REV_HIGH 0x09U

/* ==========================================================================
 *  ALERT_STATUS_1  (0x0B)
 *  Alert status register - read to determine interrupt source
 * ========================================================================== */
#define STUSB4500_REG_ALERT_STATUS_1    0x0BU

#define STUSB4500_ALERT_PHY_STATUS_CHANGED_POS     0U
#define STUSB4500_ALERT_PHY_STATUS_CHANGED_MSK     (1U << 0)
#define STUSB4500_ALERT_PRT_STATUS_CHANGED_POS     1U
#define STUSB4500_ALERT_PRT_STATUS_CHANGED_MSK     (1U << 1)
#define STUSB4500_ALERT_PD_TYPEC_STATUS_CHANGED_POS 4U
#define STUSB4500_ALERT_PD_TYPEC_STATUS_CHANGED_MSK (1U << 4)
#define STUSB4500_ALERT_HW_FAULT_STATUS_CHANGED_POS 5U
#define STUSB4500_ALERT_HW_FAULT_STATUS_CHANGED_MSK (1U << 5)
#define STUSB4500_ALERT_MONITORING_STATUS_CHANGED_POS 6U
#define STUSB4500_ALERT_MONITORING_STATUS_CHANGED_MSK (1U << 6)
#define STUSB4500_ALERT_CC_HW_FAULT_STATUS_CHANGED_POS 7U
#define STUSB4500_ALERT_CC_HW_FAULT_STATUS_CHANGED_MSK (1U << 7)

/* ==========================================================================
 *  ALERT_STATUS_MASK  (0x0C)
 *  Mask register for ALERT_STATUS_1 (1 = masked / disabled)
 * ========================================================================== */
#define STUSB4500_REG_ALERT_STATUS_MASK 0x0CU

/* Same bit positions as ALERT_STATUS_1 */

/* ==========================================================================
 *  CC_CONNECTION_STATUS_TRANS  (0x0D)
 *  Transition flag on Type-C connection status (read to clear)
 * ========================================================================== */
#define STUSB4500_REG_CC_CONNECTION_STATUS_TRANS  0x0DU

#define STUSB4500_CC_ATTACH_TRANS_POS              0U
#define STUSB4500_CC_ATTACH_TRANS_MSK              (1U << 0)

/* ==========================================================================
 *  CC_CONNECTION_STATUS  (0x0E)
 *  Current Type-C connection status
 * ========================================================================== */
#define STUSB4500_REG_CC_CONNECTION_STATUS  0x0EU

#define STUSB4500_CC_ATTACH_POS                    0U
#define STUSB4500_CC_ATTACH_MSK                    (1U << 0)
/* VCONN Supply State */
#define STUSB4500_CC_VCONN_SUPPLY_POS              1U
#define STUSB4500_CC_VCONN_SUPPLY_MSK              (1U << 1)
/* Data Role */
#define STUSB4500_CC_DATA_ROLE_POS                 2U
#define STUSB4500_CC_DATA_ROLE_MSK                 (1U << 2)
/* Power Role */
#define STUSB4500_CC_POWER_ROLE_POS                3U
#define STUSB4500_CC_POWER_ROLE_MSK                (1U << 3)
/* START_SINKCONDITION_STATUS */
#define STUSB4500_CC_START_SINKCOND_POS            4U
#define STUSB4500_CC_START_SINKCOND_MSK            (1U << 4)
/* CC State */
#define STUSB4500_CC_STATE_POS                     5U
#define STUSB4500_CC_STATE_MSK                     (0x03U << 5) /* 2 bits */
#define STUSB4500_CC_STATE_NOT_LOOKING              0x00U
#define STUSB4500_CC_STATE_LOOKING                  0x01U
#define STUSB4500_CC_STATE_CONN_WITH_Rd             0x02U
#define STUSB4500_CC_STATE_CONN_WITH_Ra             0x03U
/* Attached Device */
#define STUSB4500_CC_ATTACHED_DEVICE_POS           7U
#define STUSB4500_CC_ATTACHED_DEVICE_MSK           (1U << 7) /* Extends to 2 bits across registers */

/* ==========================================================================
 *  MONITORING_STATUS_TRANS  (0x0F)
 *  Monitoring status transition (read to clear)
 * ========================================================================== */
#define STUSB4500_REG_MONITORING_STATUS_TRANS  0x0FU

#define STUSB4500_VBUS_VALID_SNK_TRANS_POS         0U
#define STUSB4500_VBUS_VALID_SNK_TRANS_MSK         (1U << 0)
#define STUSB4500_VBUS_VSAFE0V_TRANS_POS           1U
#define STUSB4500_VBUS_VSAFE0V_TRANS_MSK           (1U << 1)
#define STUSB4500_VBUS_READY_TRANS_POS             2U
#define STUSB4500_VBUS_READY_TRANS_MSK             (1U << 2)

/* ==========================================================================
 *  MONITORING_STATUS  (0x10)
 *  Current monitoring status
 * ========================================================================== */
#define STUSB4500_REG_MONITORING_STATUS  0x10U

#define STUSB4500_VBUS_VALID_SNK_POS               0U
#define STUSB4500_VBUS_VALID_SNK_MSK               (1U << 0)
#define STUSB4500_VBUS_VSAFE0V_POS                 1U
#define STUSB4500_VBUS_VSAFE0V_MSK                 (1U << 1)
#define STUSB4500_VBUS_READY_POS                   2U
#define STUSB4500_VBUS_READY_MSK                   (1U << 2)

/* ==========================================================================
 *  CC_OPERATION_STATUS  (0x11)
 *  CC line operation status
 * ========================================================================== */
#define STUSB4500_REG_CC_OPERATION_STATUS  0x11U

#define STUSB4500_CC_MSG_MODE_POS                  0U
#define STUSB4500_CC_MSG_MODE_MSK                  (1U << 0)
#define STUSB4500_CC_ATTACHED_POS                  1U
#define STUSB4500_CC_ATTACHED_MSK                  (1U << 1)
#define STUSB4500_CC_VCONN_SWITCH_POS              2U
#define STUSB4500_CC_VCONN_SWITCH_MSK              (1U << 2)
#define STUSB4500_CC_VCONN_DISCHARGE_POS           3U
#define STUSB4500_CC_VCONN_DISCHARGE_MSK           (1U << 3)
#define STUSB4500_CC_DATA_ROLE_STATUS_POS          4U
#define STUSB4500_CC_DATA_ROLE_STATUS_MSK          (1U << 4)
#define STUSB4500_CC_POWER_ROLE_STATUS_POS         5U
#define STUSB4500_CC_POWER_ROLE_STATUS_MSK         (1U << 5)
#define STUSB4500_CC_TYPEC_FSM_STATE_POS           6U
#define STUSB4500_CC_TYPEC_FSM_STATE_MSK           (0x03U << 6)

/* ==========================================================================
 *  HW_FAULT_STATUS_TRANS  (0x12)
 *  Hardware fault status transition (read to clear)
 * ========================================================================== */
#define STUSB4500_REG_HW_FAULT_STATUS_TRANS  0x12U

#define STUSB4500_VBUS_DISCH_FAULT_TRANS_POS       0U
#define STUSB4500_VBUS_DISCH_FAULT_TRANS_MSK       (1U << 0)
#define STUSB4500_VPU_VALID_TRANS_POS              4U
#define STUSB4500_VPU_VALID_TRANS_MSK              (1U << 4)
#define STUSB4500_VPU_OVP_FAULT_TRANS_POS          5U
#define STUSB4500_VPU_OVP_FAULT_TRANS_MSK          (1U << 5)
#define STUSB4500_THERMAL_FAULT_TRANS_POS          7U
#define STUSB4500_THERMAL_FAULT_TRANS_MSK          (1U << 7)

/* ==========================================================================
 *  HW_FAULT_STATUS  (0x13)
 *  Current hardware fault status
 * ========================================================================== */
#define STUSB4500_REG_HW_FAULT_STATUS  0x13U

#define STUSB4500_VBUS_DISCH_FAULT_POS             0U
#define STUSB4500_VBUS_DISCH_FAULT_MSK             (1U << 0)
#define STUSB4500_VPU_VALID_POS                    4U
#define STUSB4500_VPU_VALID_MSK                    (1U << 4)
#define STUSB4500_VPU_OVP_FAULT_POS                5U
#define STUSB4500_VPU_OVP_FAULT_MSK                (1U << 5)
#define STUSB4500_THERMAL_FAULT_POS                7U
#define STUSB4500_THERMAL_FAULT_MSK                (1U << 7)

/* ==========================================================================
 *  PD_TYPEC_STATUS  (0x14)
 *  PD & Type-C status
 * ========================================================================== */
#define STUSB4500_REG_PD_TYPEC_STATUS  0x14U

#define STUSB4500_PD_TYPEC_HAND_CHECK_POS          0U
#define STUSB4500_PD_TYPEC_HAND_CHECK_MSK          (0x0FU << 0)

/* ==========================================================================
 *  PRT_STATUS  (0x16)
 *  Protocol layer status
 * ========================================================================== */
#define STUSB4500_REG_PRT_STATUS  0x16U

#define STUSB4500_PRT_MSG_RECEIVED_POS             2U
#define STUSB4500_PRT_MSG_RECEIVED_MSK             (1U << 2)

/* ==========================================================================
 *  PHY_STATUS  (0x17)
 *  Physical layer status
 * ========================================================================== */
#define STUSB4500_REG_PHY_STATUS  0x17U

#define STUSB4500_PHY_BUS_IDLE_POS                 0U
#define STUSB4500_PHY_BUS_IDLE_MSK                 (1U << 0)

/* ==========================================================================
 *  CC_CAPABILITY_CTRL  (0x18)
 *  CC capabilities control
 * ========================================================================== */
#define STUSB4500_REG_CC_CAPABILITY_CTRL  0x18U

#define STUSB4500_CC_CURRENT_ADVERTISED_POS        0U
#define STUSB4500_CC_CURRENT_ADVERTISED_MSK        (0x0FU << 0)
#define STUSB4500_CC_VCONN_DISCHARGE_EN_POS        4U
#define STUSB4500_CC_VCONN_DISCHARGE_EN_MSK        (1U << 4)
#define STUSB4500_CC_VCONN_SWITCH_EN_POS           5U
#define STUSB4500_CC_VCONN_SWITCH_EN_MSK           (1U << 5)
#define STUSB4500_CC_DR_SWAP_EN_POS                6U
#define STUSB4500_CC_DR_SWAP_EN_MSK                (1U << 6)
#define STUSB4500_CC_PR_SWAP_EN_POS                7U
#define STUSB4500_CC_PR_SWAP_EN_MSK                (1U << 7)

/* ==========================================================================
 *  PD_COMMAND_CTRL  (0x1A)
 *  Send PD commands
 * ========================================================================== */
#define STUSB4500_REG_PD_COMMAND_CTRL  0x1AU

#define STUSB4500_PD_CMD_POS                       0U
#define STUSB4500_PD_CMD_MSK                       (0xFFU)

/* PD_COMMAND values */
#define STUSB4500_PD_CMD_NONE                      0x00U
#define STUSB4500_PD_CMD_SRCCAP_REQ                0x26U  /* Send SRC_CAP request */

/* ==========================================================================
 *  MONITORING_CTRL_0  (0x20)
 *  VBUS monitoring control 0
 * ========================================================================== */
#define STUSB4500_REG_MONITORING_CTRL_0  0x20U

#define STUSB4500_VBUS_SNK_DISC_THRESHOLD_POS      0U
#define STUSB4500_VBUS_SNK_DISC_THRESHOLD_MSK      (0x0FU << 0)

/* ==========================================================================
 *  MONITORING_CTRL_2  (0x22)
 *  VBUS monitoring control 2
 * ========================================================================== */
#define STUSB4500_REG_MONITORING_CTRL_2  0x22U

#define STUSB4500_VSHIFT_LOW_POS                   0U
#define STUSB4500_VSHIFT_LOW_MSK                   (0x0FU << 0)
#define STUSB4500_VSHIFT_HIGH_POS                  4U
#define STUSB4500_VSHIFT_HIGH_MSK                  (0x0FU << 4)

/* ==========================================================================
 *  RESET_CTRL  (0x23)
 *  Software reset
 * ========================================================================== */
#define STUSB4500_REG_RESET_CTRL  0x23U

#define STUSB4500_SW_RESET_EN_POS                  0U
#define STUSB4500_SW_RESET_EN_MSK                  (1U << 0)

/* ==========================================================================
 *  VBUS_DISCHARGE_TIME_CTRL  (0x25)
 *  VBUS discharge time control
 * ========================================================================== */
#define STUSB4500_REG_VBUS_DISCHARGE_TIME_CTRL  0x25U

#define STUSB4500_VBUS_DISCH_TIME_TRANSITION_POS   0U
#define STUSB4500_VBUS_DISCH_TIME_TRANSITION_MSK   (0x0FU << 0)

/* ==========================================================================
 *  VBUS_DISCHARGE_CTRL  (0x26)
 *  VBUS discharge enable
 * ========================================================================== */
#define STUSB4500_REG_VBUS_DISCHARGE_CTRL  0x26U

#define STUSB4500_VBUS_DISCHARGE_EN_POS            7U
#define STUSB4500_VBUS_DISCHARGE_EN_MSK            (1U << 7)

/* ==========================================================================
 *  VBUS_CTRL  (0x27)
 *  VBUS control register
 * ========================================================================== */
#define STUSB4500_REG_VBUS_CTRL  0x27U

#define STUSB4500_VBUS_SNK_EN_POS                  1U
#define STUSB4500_VBUS_SNK_EN_MSK                  (1U << 1)

/* ==========================================================================
 *  PE_FSM  (0x29)
 *  Policy Engine finite state machine
 * ========================================================================== */
#define STUSB4500_REG_PE_FSM  0x29U

#define STUSB4500_PE_FSM_STATE_POS                 0U
#define STUSB4500_PE_FSM_STATE_MSK                 (0xFFU)

/* PE FSM States */
#define STUSB4500_PE_DISABLED                      0x00U
#define STUSB4500_PE_SNK_STARTUP                   0x12U
#define STUSB4500_PE_SNK_DISCOVERY                 0x13U
#define STUSB4500_PE_SNK_WAIT_FOR_CAP              0x14U
#define STUSB4500_PE_SNK_EVAL_CAP                  0x15U
#define STUSB4500_PE_SNK_SELECT_CAP                0x16U
#define STUSB4500_PE_SNK_TRANSITION_SINK           0x17U
#define STUSB4500_PE_SNK_READY                     0x18U
#define STUSB4500_PE_ERRORRECOVERY                 0x40U

/* ==========================================================================
 *  GPIO_SW_GPIO  (0x2D)
 *  GPIO control register
 * ========================================================================== */
#define STUSB4500_REG_GPIO_SW_GPIO  0x2DU

#define STUSB4500_GPIO_SW_GPIO_POS                 0U
#define STUSB4500_GPIO_SW_GPIO_MSK                 (1U << 0)

/* ==========================================================================
 *  RX_HEADER  (0x31 - 0x32)
 *  Received PD message header
 * ========================================================================== */
#define STUSB4500_REG_RX_HEADER_LOW   0x31U
#define STUSB4500_REG_RX_HEADER_HIGH  0x32U

/* PD Header bit fields */
#define STUSB4500_RX_HDR_MSG_TYPE_POS              0U
#define STUSB4500_RX_HDR_MSG_TYPE_MSK              (0x1FU << 0)
#define STUSB4500_RX_HDR_DATA_ROLE_POS             5U
#define STUSB4500_RX_HDR_DATA_ROLE_MSK             (1U << 5)
#define STUSB4500_RX_HDR_SPEC_REV_POS              6U
#define STUSB4500_RX_HDR_SPEC_REV_MSK              (0x03U << 6)
/* High byte */
#define STUSB4500_RX_HDR_PWR_ROLE_POS              0U  /* bit 8 in full header */
#define STUSB4500_RX_HDR_PWR_ROLE_MSK              (1U << 0)
#define STUSB4500_RX_HDR_MSG_ID_POS                1U  /* bits 9-11 */
#define STUSB4500_RX_HDR_MSG_ID_MSK                (0x07U << 1)
#define STUSB4500_RX_HDR_NUM_DATAOBJ_POS           4U  /* bits 12-14 */
#define STUSB4500_RX_HDR_NUM_DATAOBJ_MSK           (0x07U << 4)
#define STUSB4500_RX_HDR_EXTENDED_POS              7U  /* bit 15 */
#define STUSB4500_RX_HDR_EXTENDED_MSK              (1U << 7)

/* ==========================================================================
 *  RX_DATA_OBJ  (0x33 - 0x4E)
 *  Received PD data objects (up to 7 x 4 bytes)
 * ========================================================================== */
#define STUSB4500_REG_RX_DATA_OBJ_START  0x33U
#define STUSB4500_RX_DATA_OBJ_SIZE       4U    /* Each data object is 4 bytes */
#define STUSB4500_RX_DATA_OBJ_MAX        7U

/* ==========================================================================
 *  TX_HEADER  (0x51 - 0x52)
 *  Transmit PD message header
 * ========================================================================== */
#define STUSB4500_REG_TX_HEADER_LOW   0x51U
#define STUSB4500_REG_TX_HEADER_HIGH  0x52U

/* ==========================================================================
 *  DPM_PDO_NUMB  (0x70)
 *  Number of sink PDOs (1 to 3)
 * ========================================================================== */
#define STUSB4500_REG_DPM_PDO_NUMB  0x70U

#define STUSB4500_DPM_PDO_NUMB_POS                 0U
#define STUSB4500_DPM_PDO_NUMB_MSK                 (0x07U << 0)

/* ==========================================================================
 *  DPM_SNK_PDO1  (0x85 - 0x88)  - Fixed, always 5V, USB default
 *  DPM_SNK_PDO2  (0x89 - 0x8C)
 *  DPM_SNK_PDO3  (0x8D - 0x90)
 *  Sink Power Data Objects (32-bit each)
 * ========================================================================== */
#define STUSB4500_REG_DPM_SNK_PDO1_0  0x85U  /* PDO1 byte 0 (LSB) */
#define STUSB4500_REG_DPM_SNK_PDO1_1  0x86U
#define STUSB4500_REG_DPM_SNK_PDO1_2  0x87U
#define STUSB4500_REG_DPM_SNK_PDO1_3  0x88U  /* PDO1 byte 3 (MSB) */

#define STUSB4500_REG_DPM_SNK_PDO2_0  0x89U
#define STUSB4500_REG_DPM_SNK_PDO2_1  0x8AU
#define STUSB4500_REG_DPM_SNK_PDO2_2  0x8BU
#define STUSB4500_REG_DPM_SNK_PDO2_3  0x8CU

#define STUSB4500_REG_DPM_SNK_PDO3_0  0x8DU
#define STUSB4500_REG_DPM_SNK_PDO3_1  0x8EU
#define STUSB4500_REG_DPM_SNK_PDO3_2  0x8FU
#define STUSB4500_REG_DPM_SNK_PDO3_3  0x90U

/* PDO base addresses as arrays */
#define STUSB4500_REG_DPM_SNK_PDO_BASE  0x85U
#define STUSB4500_PDO_SIZE              4U   /* 4 bytes per PDO */

/* --------------------------------------------------------------------------
 *  Fixed Supply PDO Bit Fields (Type = 00b)
 *  Applies to PDO1, PDO2, PDO3
 * -------------------------------------------------------------------------- */

/* Bits [9:0] - Operational Current in 10mA units */
#define STUSB4500_PDO_CURRENT_POS                  0U
#define STUSB4500_PDO_CURRENT_MSK                  0x000003FFU
#define STUSB4500_PDO_CURRENT_STEP_MA              10U

/* Bits [19:10] - Voltage in 50mV units */
#define STUSB4500_PDO_VOLTAGE_POS                  10U
#define STUSB4500_PDO_VOLTAGE_MSK                  0x000FFC00U
#define STUSB4500_PDO_VOLTAGE_STEP_MV              50U

/* Bit [20] - Fast Role Swap required current */
#define STUSB4500_PDO_FAST_ROLE_SWAP_POS           20U
#define STUSB4500_PDO_FAST_ROLE_SWAP_MSK           (0x03U << 20)

/* Bit [22] - Dual-Role Data */
#define STUSB4500_PDO_DUAL_ROLE_DATA_POS           22U
#define STUSB4500_PDO_DUAL_ROLE_DATA_MSK           (1U << 22)

/* Bit [23] - USB Communications Capable */
#define STUSB4500_PDO_USB_COMM_CAPABLE_POS         23U
#define STUSB4500_PDO_USB_COMM_CAPABLE_MSK         (1U << 23)

/* Bit [24] - Unconstrained Power */
#define STUSB4500_PDO_UNCONSTRAINED_PWR_POS        24U
#define STUSB4500_PDO_UNCONSTRAINED_PWR_MSK        (1U << 24)

/* Bit [25] - Higher Capability */
#define STUSB4500_PDO_HIGHER_CAPABILITY_POS        25U
#define STUSB4500_PDO_HIGHER_CAPABILITY_MSK        (1U << 25)

/* Bit [26] - Dual-Role Power */
#define STUSB4500_PDO_DUAL_ROLE_PWR_POS            26U
#define STUSB4500_PDO_DUAL_ROLE_PWR_MSK            (1U << 26)

/* Bits [31:30] - Fixed Supply type = 00 */
#define STUSB4500_PDO_TYPE_POS                     30U
#define STUSB4500_PDO_TYPE_MSK                     (0x03U << 30)
#define STUSB4500_PDO_TYPE_FIXED                   (0x00U << 30)

/* ==========================================================================
 *  RDO_REG_STATUS  (0x91 - 0x94)
 *  Active Request Data Object (what was negotiated)
 * ========================================================================== */
#define STUSB4500_REG_RDO_STATUS_0  0x91U  /* byte 0 (LSB) */
#define STUSB4500_REG_RDO_STATUS_1  0x92U
#define STUSB4500_REG_RDO_STATUS_2  0x93U
#define STUSB4500_REG_RDO_STATUS_3  0x94U  /* byte 3 (MSB) */

/* RDO bit fields */
/* Bits [9:0] - Max operating current in 10mA units */
#define STUSB4500_RDO_MAX_CURRENT_POS              0U
#define STUSB4500_RDO_MAX_CURRENT_MSK              0x000003FFU

/* Bits [19:10] - Operating current in 10mA units */
#define STUSB4500_RDO_OP_CURRENT_POS               10U
#define STUSB4500_RDO_OP_CURRENT_MSK               0x000FFC00U

/* Bits [22:20] - reserved */

/* Bit [23] - Unchunked Extended Messages Supported */
#define STUSB4500_RDO_UNCHUNK_EXT_MSG_POS          23U
#define STUSB4500_RDO_UNCHUNK_EXT_MSG_MSK          (1U << 23)

/* Bit [24] - No USB Suspend */
#define STUSB4500_RDO_NO_USB_SUSPEND_POS           24U
#define STUSB4500_RDO_NO_USB_SUSPEND_MSK           (1U << 24)

/* Bit [25] - USB Communications Capable */
#define STUSB4500_RDO_USB_COMM_CAPABLE_POS         25U
#define STUSB4500_RDO_USB_COMM_CAPABLE_MSK         (1U << 25)

/* Bit [26] - Capability Mismatch */
#define STUSB4500_RDO_CAP_MISMATCH_POS             26U
#define STUSB4500_RDO_CAP_MISMATCH_MSK             (1U << 26)

/* Bit [27] - GiveBack Flag */
#define STUSB4500_RDO_GIVEBACK_FLAG_POS            27U
#define STUSB4500_RDO_GIVEBACK_FLAG_MSK            (1U << 27)

/* Bits [31:28] - Object Position (1-based index of source PDO selected) */
#define STUSB4500_RDO_OBJ_POS_POS                 28U
#define STUSB4500_RDO_OBJ_POS_MSK                 (0x07U << 28)

/* ==========================================================================
 *  NVM (Non-Volatile Memory) Registers
 *  Used for loading/storing NVM sectors with PDO configuration
 * ========================================================================== */
#define STUSB4500_REG_FTP_CUST_PASSWORD  0x95U
#define STUSB4500_FTP_CUST_PASSWORD_KEY  0x47U  /* Unlock key */

#define STUSB4500_REG_FTP_CTRL_0        0x96U
#define STUSB4500_FTP_SECTOR_POS                   0U
#define STUSB4500_FTP_SECTOR_MSK                   (0x07U << 0)
#define STUSB4500_FTP_REQ_POS                      4U
#define STUSB4500_FTP_REQ_MSK                      (1U << 4)
#define STUSB4500_FTP_SECT_ERASE_POS               5U
#define STUSB4500_FTP_SECT_ERASE_MSK               (0x1FU << 5) /* 5 bits, one per sector */

#define STUSB4500_REG_FTP_CTRL_1        0x97U
#define STUSB4500_FTP_OPCODE_POS                   0U
#define STUSB4500_FTP_OPCODE_MSK                   (0x07U << 0)

/* FTP Opcodes */
#define STUSB4500_FTP_OPCODE_READ                  0x00U
#define STUSB4500_FTP_OPCODE_WRITE_PL              0x01U  /* Write to PL */
#define STUSB4500_FTP_OPCODE_WRITE_SER             0x02U  /* Write SER (Sector Erase) */
#define STUSB4500_FTP_OPCODE_READ_PL               0x03U  /* Read from PL */
#define STUSB4500_FTP_OPCODE_READ_SER              0x04U  /* Read SER */
#define STUSB4500_FTP_OPCODE_ERASE_SECTOR          0x05U  /* Erase Sector */
#define STUSB4500_FTP_OPCODE_PROG_SECTOR           0x06U  /* Program Sector */
#define STUSB4500_FTP_OPCODE_SOFT_PROG_SECTOR      0x07U  /* Soft Program Sector */

#define STUSB4500_REG_RW_BUFFER         0x53U  /* 8-byte RW buffer for NVM */
#define STUSB4500_RW_BUFFER_SIZE        8U

/* ==========================================================================
 *  NVM Sector Layout (Sector 4 contains PDO configuration)
 *  Each sector = 8 bytes
 * ========================================================================== */
#define STUSB4500_NVM_SECTOR_PDO        4U  /* Sector 4: PDO config */

/* ==========================================================================
 *  Helpers / Utility Macros
 * ========================================================================== */

/** @brief Build a 32-bit Fixed Supply Sink PDO from voltage (mV) and current (mA) */
#define STUSB4500_BUILD_FIXED_PDO(voltage_mv, current_ma) \
  ( (STUSB4500_PDO_TYPE_FIXED) | \
    (((uint32_t)((voltage_mv) / STUSB4500_PDO_VOLTAGE_STEP_MV) << STUSB4500_PDO_VOLTAGE_POS) & STUSB4500_PDO_VOLTAGE_MSK) | \
    (((uint32_t)((current_ma) / STUSB4500_PDO_CURRENT_STEP_MA) << STUSB4500_PDO_CURRENT_POS) & STUSB4500_PDO_CURRENT_MSK) )

/** @brief Extract voltage in mV from a PDO word */
#define STUSB4500_PDO_GET_VOLTAGE_MV(pdo) \
  ((uint32_t)(((pdo) & STUSB4500_PDO_VOLTAGE_MSK) >> STUSB4500_PDO_VOLTAGE_POS) * STUSB4500_PDO_VOLTAGE_STEP_MV)

/** @brief Extract current in mA from a PDO word */
#define STUSB4500_PDO_GET_CURRENT_MA(pdo) \
  ((uint32_t)(((pdo) & STUSB4500_PDO_CURRENT_MSK) >> STUSB4500_PDO_CURRENT_POS) * STUSB4500_PDO_CURRENT_STEP_MA)

#ifdef __cplusplus
}
#endif

#endif /* __STUSB4500_REGS_H */
