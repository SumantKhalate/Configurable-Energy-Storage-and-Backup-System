/**
  ******************************************************************************
  * @file    stusb4500.h
  * @brief   STUSB4500QTR USB PD sink controller - Driver API header.
  *          Provides high-level functions for initialization, PDO configuration,
  *          status reading, cable detection, and software reset.
  ******************************************************************************
  */

#ifndef __STUSB4500_H
#define __STUSB4500_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "../../STM32L4xx_HAL_Driver/Inc/stm32l4xx_hal.h"
#include "stusb4500_regs.h"
#include <stdbool.h>

/* ==========================================================================
 *  Error / Status Codes
 * ========================================================================== */
typedef enum {
  STUSB4500_OK              =  0,
  STUSB4500_ERR_I2C         = -1,  /**< I2C communication error */
  STUSB4500_ERR_DEVICE_ID   = -2,  /**< Device ID mismatch */
  STUSB4500_ERR_PARAM       = -3,  /**< Invalid parameter */
  STUSB4500_ERR_TIMEOUT     = -4,  /**< Operation timeout */
  STUSB4500_ERR_NVM         = -5,  /**< NVM read/write error */
  STUSB4500_ERR_HW_FAULT    = -6,  /**< Hardware fault detected */
} STUSB4500_Status_t;

/* ==========================================================================
 *  PDO Configuration
 * ========================================================================== */
typedef struct {
  uint32_t voltage_mv;   /**< Requested voltage in millivolts (e.g. 5000, 9000, 20000) */
  uint32_t current_ma;   /**< Requested current in milliamps  (e.g. 1000, 3000) */
} STUSB4500_PDO_Config_t;

/* ==========================================================================
 *  Negotiated Power (RDO readback)
 * ========================================================================== */
typedef struct {
  uint32_t voltage_mv;        /**< Negotiated voltage in mV */
  uint32_t operating_current_ma; /**< Negotiated operating current in mA */
  uint32_t max_current_ma;    /**< Maximum current in mA */
  uint8_t  object_position;   /**< Source PDO index selected (1-based) */
  bool     cap_mismatch;      /**< True if capability mismatch flagged */
} STUSB4500_RDO_Status_t;

/* ==========================================================================
 *  Connection / Cable Status
 * ========================================================================== */
typedef struct {
  bool     attached;          /**< True if a USB-C cable is attached */
  bool     vbus_ready;        /**< True if VBUS is at negotiated level */
  bool     vbus_valid_snk;    /**< True if VBUS above sink disconnect threshold */
  bool     vbus_vsafe0v;      /**< True if VBUS is near 0V */
  uint8_t  cc_state;          /**< CC line state (see CC_STATE defines) */
  uint8_t  pe_fsm_state;      /**< Policy Engine FSM state */
} STUSB4500_Connection_Status_t;

/* ==========================================================================
 *  Hardware Fault Info
 * ========================================================================== */
typedef struct {
  bool     thermal_fault;     /**< Over-temperature detected */
  bool     vpu_ovp_fault;     /**< VPU over-voltage protection fault */
  bool     vpu_valid;         /**< VPU voltage is valid */
  bool     vbus_disch_fault;  /**< VBUS discharge fault */
} STUSB4500_HW_Fault_t;

/* ==========================================================================
 *  Alert Status
 * ========================================================================== */
typedef struct {
  bool     phy_status_changed;
  bool     prt_status_changed;
  bool     pd_typec_status_changed;
  bool     hw_fault_status_changed;
  bool     monitoring_status_changed;
  bool     cc_hw_fault_status_changed;
} STUSB4500_Alert_t;

/* ==========================================================================
 *  Device Handle
 * ========================================================================== */
typedef struct {
  I2C_HandleTypeDef *hi2c;     /**< Pointer to HAL I2C handle */
  uint16_t           i2c_addr; /**< 7-bit I2C address (default 0x28) */
  uint32_t           timeout;  /**< I2C timeout in ms */
} STUSB4500_Handle_t;

/* ==========================================================================
 *  Register-Level Access API  (implemented in stusb4500_regs.c)
 * ========================================================================== */

/**
 * @brief  Read a single register.
 * @param  dev    Device handle
 * @param  reg    Register address
 * @param  data   Pointer to store the read byte
 * @retval STUSB4500_Status_t
 */
STUSB4500_Status_t STUSB4500_ReadReg(const STUSB4500_Handle_t *dev, uint8_t reg, uint8_t *data);

/**
 * @brief  Read multiple consecutive registers.
 * @param  dev    Device handle
 * @param  reg    Starting register address
 * @param  data   Buffer to store read bytes
 * @param  len    Number of bytes to read
 * @retval STUSB4500_Status_t
 */
STUSB4500_Status_t STUSB4500_ReadRegs(const STUSB4500_Handle_t *dev, uint8_t reg, uint8_t *data, uint16_t len);

/**
 * @brief  Write a single register.
 * @param  dev    Device handle
 * @param  reg    Register address
 * @param  data   Byte to write
 * @retval STUSB4500_Status_t
 */
STUSB4500_Status_t STUSB4500_WriteReg(const STUSB4500_Handle_t *dev, uint8_t reg, uint8_t data);

/**
 * @brief  Write multiple consecutive registers.
 * @param  dev    Device handle
 * @param  reg    Starting register address
 * @param  data   Buffer of bytes to write
 * @param  len    Number of bytes to write
 * @retval STUSB4500_Status_t
 */
STUSB4500_Status_t STUSB4500_WriteRegs(const STUSB4500_Handle_t *dev, uint8_t reg, const uint8_t *data, uint16_t len);

/**
 * @brief  Read-modify-write a register (set/clear specific bits).
 * @param  dev    Device handle
 * @param  reg    Register address
 * @param  mask   Bit mask of bits to modify
 * @param  value  New value for the masked bits
 * @retval STUSB4500_Status_t
 */
STUSB4500_Status_t STUSB4500_ModifyReg(const STUSB4500_Handle_t *dev, uint8_t reg, uint8_t mask, uint8_t value);

/**
 * @brief  Read a 32-bit register (4 consecutive bytes, little-endian).
 * @param  dev    Device handle
 * @param  reg    Starting register address (LSB first)
 * @param  data   Pointer to store the 32-bit value
 * @retval STUSB4500_Status_t
 */
STUSB4500_Status_t STUSB4500_ReadReg32(const STUSB4500_Handle_t *dev, uint8_t reg, uint32_t *data);

/**
 * @brief  Write a 32-bit register (4 consecutive bytes, little-endian).
 * @param  dev    Device handle
 * @param  reg    Starting register address (LSB first)
 * @param  data   32-bit value to write
 * @retval STUSB4500_Status_t
 */
STUSB4500_Status_t STUSB4500_WriteReg32(const STUSB4500_Handle_t *dev, uint8_t reg, uint32_t data);

/* ==========================================================================
 *  Logic-Level Driver API  (implemented in stusb4500.c)
 * ========================================================================== */

/**
 * @brief  Initialize the STUSB4500 driver and verify device identity.
 * @param  dev    Device handle (hi2c, i2c_addr, timeout must be set)
 * @retval STUSB4500_Status_t
 */
STUSB4500_Status_t STUSB4500_Init(STUSB4500_Handle_t *dev);

/**
 * @brief  Perform a software reset of the STUSB4500.
 * @param  dev    Device handle
 * @retval STUSB4500_Status_t
 */
STUSB4500_Status_t STUSB4500_SoftReset(const STUSB4500_Handle_t *dev);

/**
 * @brief  Read the device ID register and verify it matches expected value.
 * @param  dev       Device handle
 * @param  device_id Pointer to store the read device ID (can be NULL)
 * @retval STUSB4500_Status_t  STUSB4500_OK if device ID matches, ERR_DEVICE_ID otherwise
 */
STUSB4500_Status_t STUSB4500_VerifyDeviceID(const STUSB4500_Handle_t *dev, uint8_t *device_id);

/**
 * @brief  Configure sink PDOs (voltage/current) in the STUSB4500 registers.
 *         PDO1 is always 5V (USB default). PDO2 and PDO3 are user-configurable.
 * @param  dev       Device handle
 * @param  pdo_cfg   Array of PDO configs (index 0 = PDO1, 1 = PDO2, 2 = PDO3)
 * @param  num_pdos  Number of PDOs to configure (1 to 3)
 * @retval STUSB4500_Status_t
 */
STUSB4500_Status_t STUSB4500_SetPDO(const STUSB4500_Handle_t *dev,
                                     const STUSB4500_PDO_Config_t *pdo_cfg,
                                     uint8_t num_pdos);

/**
 * @brief  Read back the currently configured sink PDOs from registers.
 * @param  dev       Device handle
 * @param  pdo_cfg   Array to store PDO configs (must have space for 3)
 * @param  num_pdos  Pointer to store the number of active PDOs
 * @retval STUSB4500_Status_t
 */
STUSB4500_Status_t STUSB4500_GetPDO(const STUSB4500_Handle_t *dev,
                                     STUSB4500_PDO_Config_t *pdo_cfg,
                                     uint8_t *num_pdos);

/**
 * @brief  Read the active RDO (negotiated power contract).
 * @param  dev    Device handle
 * @param  rdo    Pointer to store RDO status
 * @retval STUSB4500_Status_t
 */
STUSB4500_Status_t STUSB4500_GetRDO(const STUSB4500_Handle_t *dev, STUSB4500_RDO_Status_t *rdo);

/**
 * @brief  Read cable/connection status.
 * @param  dev    Device handle
 * @param  status Pointer to store connection status
 * @retval STUSB4500_Status_t
 */
STUSB4500_Status_t STUSB4500_GetConnectionStatus(const STUSB4500_Handle_t *dev,
                                                   STUSB4500_Connection_Status_t *status);

/**
 * @brief  Read hardware fault status.
 * @param  dev    Device handle
 * @param  fault  Pointer to store fault information
 * @retval STUSB4500_Status_t
 */
STUSB4500_Status_t STUSB4500_GetHWFault(const STUSB4500_Handle_t *dev, STUSB4500_HW_Fault_t *fault);

/**
 * @brief  Read and clear the alert status register.
 * @param  dev    Device handle
 * @param  alert  Pointer to store alert flags
 * @retval STUSB4500_Status_t
 */
STUSB4500_Status_t STUSB4500_GetAlert(const STUSB4500_Handle_t *dev, STUSB4500_Alert_t *alert);

/**
 * @brief  Clear all alert/transition registers by reading them.
 * @param  dev    Device handle
 * @retval STUSB4500_Status_t
 */
STUSB4500_Status_t STUSB4500_ClearAlerts(const STUSB4500_Handle_t *dev);

/**
 * @brief  Trigger a USB PD renegotiation by sending a new source capability request.
 * @param  dev    Device handle
 * @retval STUSB4500_Status_t
 */
STUSB4500_Status_t STUSB4500_Renegotiate(const STUSB4500_Handle_t *dev);

/**
 * @brief  Wait until the Policy Engine reaches SNK_READY state or timeout.
 * @param  dev         Device handle
 * @param  timeout_ms  Maximum wait time in milliseconds
 * @retval STUSB4500_Status_t  STUSB4500_OK if ready, ERR_TIMEOUT otherwise
 */
STUSB4500_Status_t STUSB4500_WaitForReady(const STUSB4500_Handle_t *dev, uint32_t timeout_ms);

/**
 * @brief  Set the alert mask register (enable/disable specific alerts).
 * @param  dev    Device handle
 * @param  mask   Alert mask byte (1 = masked/disabled)
 * @retval STUSB4500_Status_t
 */
STUSB4500_Status_t STUSB4500_SetAlertMask(const STUSB4500_Handle_t *dev, uint8_t mask);

/**
 * @brief  Control the GPIO pin of the STUSB4500.
 * @param  dev    Device handle
 * @param  state  true = high, false = low
 * @retval STUSB4500_Status_t
 */
STUSB4500_Status_t STUSB4500_SetGPIO(const STUSB4500_Handle_t *dev, bool state);

/**
 * @brief  Read the source PDOs received from the attached source.
 * @param  dev        Device handle
 * @param  src_pdos   Array of uint32_t to store raw source PDOs (max 7)
 * @param  num_pdos   Pointer to store the number of source PDOs read
 * @retval STUSB4500_Status_t
 */
STUSB4500_Status_t STUSB4500_GetSourcePDOs(const STUSB4500_Handle_t *dev,
                                            uint32_t *src_pdos,
                                            uint8_t *num_pdos);

#ifdef __cplusplus
}
#endif

#endif /* __STUSB4500_H */
