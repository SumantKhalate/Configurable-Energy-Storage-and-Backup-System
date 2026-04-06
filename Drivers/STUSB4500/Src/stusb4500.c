/**
  ******************************************************************************
  * @file    stusb4500.c
  * @brief   STUSB4500QTR USB PD sink controller - Logic-level driver.
  *          High-level functions: init, PDO configuration, status reading,
  *          cable detection, renegotiation, software reset, error handling.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "../Inc/stusb4500.h"
#include <string.h>

/* Private defines -----------------------------------------------------------*/
#define STUSB4500_MAX_PDO_COUNT   3U
#define STUSB4500_MIN_VOLTAGE_MV  5000U
#define STUSB4500_MAX_VOLTAGE_MV  20000U
#define STUSB4500_MAX_CURRENT_MA  5000U
#define STUSB4500_RESET_DELAY_MS  30U    /* Delay after soft reset */
#define STUSB4500_POLL_INTERVAL   10U    /* Polling interval in ms */

/* ==========================================================================
 *  Initialization
 * ========================================================================== */
STUSB4500_Status_t STUSB4500_Init(STUSB4500_Handle_t *dev)
{
  if (dev == NULL || dev->hi2c == NULL)
  {
    return STUSB4500_ERR_PARAM;
  }

  /* Set default I2C address if not configured */
  if (dev->i2c_addr == 0U)
  {
    dev->i2c_addr = STUSB4500_I2C_ADDR_7BIT;
  }

  /* Set default timeout if not configured */
  if (dev->timeout == 0U)
  {
    dev->timeout = 100U;
  }

  /* Verify device identity */
  STUSB4500_Status_t status;
  status = STUSB4500_VerifyDeviceID(dev, NULL);
  if (status != STUSB4500_OK)
  {
    return status;
  }

  /* Clear all pending alerts by reading the transition registers */
  status = STUSB4500_ClearAlerts(dev);
  if (status != STUSB4500_OK)
  {
    return status;
  }

  return STUSB4500_OK;
}

/* ==========================================================================
 *  Software Reset
 * ========================================================================== */
STUSB4500_Status_t STUSB4500_SoftReset(const STUSB4500_Handle_t *dev)
{
  if (dev == NULL || dev->hi2c == NULL)
  {
    return STUSB4500_ERR_PARAM;
  }

  STUSB4500_Status_t status;

  /* Assert software reset */
  status = STUSB4500_WriteReg(dev, STUSB4500_REG_RESET_CTRL, STUSB4500_SW_RESET_EN_MSK);
  if (status != STUSB4500_OK)
  {
    return status;
  }

  /* Wait for reset to complete */
  HAL_Delay(STUSB4500_RESET_DELAY_MS);

  /* Deassert software reset */
  status = STUSB4500_WriteReg(dev, STUSB4500_REG_RESET_CTRL, 0x00U);
  if (status != STUSB4500_OK)
  {
    return status;
  }

  /* Additional settling time */
  HAL_Delay(STUSB4500_RESET_DELAY_MS);

  return STUSB4500_OK;
}

/* ==========================================================================
 *  Device ID Verification
 * ========================================================================== */
STUSB4500_Status_t STUSB4500_VerifyDeviceID(const STUSB4500_Handle_t *dev, uint8_t *device_id)
{
  if (dev == NULL || dev->hi2c == NULL)
  {
    return STUSB4500_ERR_PARAM;
  }

  uint8_t id;
  STUSB4500_Status_t status;

  status = STUSB4500_ReadReg(dev, STUSB4500_REG_DEVICE_ID, &id);
  if (status != STUSB4500_OK)
  {
    return status;
  }

  if (device_id != NULL)
  {
    *device_id = id;
  }

  /* Verify against expected value (check only bits [4:0] per datasheet) */
  if ((id & 0x1FU) != (STUSB4500_DEVICE_ID_VALUE & 0x1FU))
  {
    return STUSB4500_ERR_DEVICE_ID;
  }

  return STUSB4500_OK;
}

/* ==========================================================================
 *  Set Sink PDO Configuration
 * ========================================================================== */
STUSB4500_Status_t STUSB4500_SetPDO(const STUSB4500_Handle_t *dev,
                                     const STUSB4500_PDO_Config_t *pdo_cfg,
                                     uint8_t num_pdos)
{
  if (dev == NULL || dev->hi2c == NULL || pdo_cfg == NULL)
  {
    return STUSB4500_ERR_PARAM;
  }

  if (num_pdos == 0U || num_pdos > STUSB4500_MAX_PDO_COUNT)
  {
    return STUSB4500_ERR_PARAM;
  }

  STUSB4500_Status_t status;

  /* Validate all PDO parameters before writing anything */
  for (uint8_t i = 0U; i < num_pdos; i++)
  {
    /* PDO1 must be 5V per USB PD specification */
    if (i == 0U && pdo_cfg[i].voltage_mv != STUSB4500_MIN_VOLTAGE_MV)
    {
      return STUSB4500_ERR_PARAM;
    }

    /* Voltage range check */
    if (pdo_cfg[i].voltage_mv < STUSB4500_MIN_VOLTAGE_MV ||
        pdo_cfg[i].voltage_mv > STUSB4500_MAX_VOLTAGE_MV)
    {
      return STUSB4500_ERR_PARAM;
    }

    /* Voltage must be a multiple of 50mV */
    if ((pdo_cfg[i].voltage_mv % STUSB4500_PDO_VOLTAGE_STEP_MV) != 0U)
    {
      return STUSB4500_ERR_PARAM;
    }

    /* Current range check */
    if (pdo_cfg[i].current_ma == 0U || pdo_cfg[i].current_ma > STUSB4500_MAX_CURRENT_MA)
    {
      return STUSB4500_ERR_PARAM;
    }

    /* Current must be a multiple of 10mA */
    if ((pdo_cfg[i].current_ma % STUSB4500_PDO_CURRENT_STEP_MA) != 0U)
    {
      return STUSB4500_ERR_PARAM;
    }
  }

  /* Write each PDO register */
  uint8_t pdo_reg_base[3] = {
    STUSB4500_REG_DPM_SNK_PDO1_0,
    STUSB4500_REG_DPM_SNK_PDO2_0,
    STUSB4500_REG_DPM_SNK_PDO3_0
  };

  for (uint8_t i = 0U; i < num_pdos; i++)
  {
    uint32_t pdo_val = STUSB4500_BUILD_FIXED_PDO(pdo_cfg[i].voltage_mv, pdo_cfg[i].current_ma);

    status = STUSB4500_WriteReg32(dev, pdo_reg_base[i], pdo_val);
    if (status != STUSB4500_OK)
    {
      return status;
    }
  }

  /* Set the number of active PDOs */
  status = STUSB4500_WriteReg(dev, STUSB4500_REG_DPM_PDO_NUMB, num_pdos);
  if (status != STUSB4500_OK)
  {
    return status;
  }

  return STUSB4500_OK;
}

/* ==========================================================================
 *  Read Sink PDO Configuration
 * ========================================================================== */
STUSB4500_Status_t STUSB4500_GetPDO(const STUSB4500_Handle_t *dev,
                                     STUSB4500_PDO_Config_t *pdo_cfg,
                                     uint8_t *num_pdos)
{
  if (dev == NULL || dev->hi2c == NULL || pdo_cfg == NULL || num_pdos == NULL)
  {
    return STUSB4500_ERR_PARAM;
  }

  STUSB4500_Status_t status;
  uint8_t pdo_count;

  /* Read number of active PDOs */
  status = STUSB4500_ReadReg(dev, STUSB4500_REG_DPM_PDO_NUMB, &pdo_count);
  if (status != STUSB4500_OK)
  {
    return status;
  }

  pdo_count &= STUSB4500_DPM_PDO_NUMB_MSK;
  if (pdo_count > STUSB4500_MAX_PDO_COUNT)
  {
    pdo_count = STUSB4500_MAX_PDO_COUNT;
  }
  *num_pdos = pdo_count;

  /* Read each PDO */
  uint8_t pdo_reg_base[3] = {
    STUSB4500_REG_DPM_SNK_PDO1_0,
    STUSB4500_REG_DPM_SNK_PDO2_0,
    STUSB4500_REG_DPM_SNK_PDO3_0
  };

  for (uint8_t i = 0U; i < pdo_count; i++)
  {
    uint32_t pdo_raw;
    status = STUSB4500_ReadReg32(dev, pdo_reg_base[i], &pdo_raw);
    if (status != STUSB4500_OK)
    {
      return status;
    }

    pdo_cfg[i].voltage_mv = STUSB4500_PDO_GET_VOLTAGE_MV(pdo_raw);
    pdo_cfg[i].current_ma = STUSB4500_PDO_GET_CURRENT_MA(pdo_raw);
  }

  return STUSB4500_OK;
}

/* ==========================================================================
 *  Read Active RDO (Negotiated Power Contract)
 * ========================================================================== */
STUSB4500_Status_t STUSB4500_GetRDO(const STUSB4500_Handle_t *dev, STUSB4500_RDO_Status_t *rdo)
{
  if (dev == NULL || dev->hi2c == NULL || rdo == NULL)
  {
    return STUSB4500_ERR_PARAM;
  }

  STUSB4500_Status_t status;
  uint32_t rdo_raw;

  status = STUSB4500_ReadReg32(dev, STUSB4500_REG_RDO_STATUS_0, &rdo_raw);
  if (status != STUSB4500_OK)
  {
    return status;
  }

  rdo->max_current_ma       = ((rdo_raw & STUSB4500_RDO_MAX_CURRENT_MSK) >> STUSB4500_RDO_MAX_CURRENT_POS) * STUSB4500_PDO_CURRENT_STEP_MA;
  rdo->operating_current_ma = ((rdo_raw & STUSB4500_RDO_OP_CURRENT_MSK)  >> STUSB4500_RDO_OP_CURRENT_POS)  * STUSB4500_PDO_CURRENT_STEP_MA;
  rdo->object_position      = (uint8_t)((rdo_raw & STUSB4500_RDO_OBJ_POS_MSK) >> STUSB4500_RDO_OBJ_POS_POS);
  rdo->cap_mismatch         = ((rdo_raw & STUSB4500_RDO_CAP_MISMATCH_MSK) != 0U);

  /* To get the negotiated voltage, read the corresponding source PDO
   * For simplicity, read from the sink PDO matching the object position */
  if (rdo->object_position >= 1U && rdo->object_position <= STUSB4500_MAX_PDO_COUNT)
  {
    uint32_t pdo_raw;
    uint8_t pdo_reg = STUSB4500_REG_DPM_SNK_PDO_BASE +
                      ((rdo->object_position - 1U) * STUSB4500_PDO_SIZE);

    status = STUSB4500_ReadReg32(dev, pdo_reg, &pdo_raw);
    if (status != STUSB4500_OK)
    {
      return status;
    }
    rdo->voltage_mv = STUSB4500_PDO_GET_VOLTAGE_MV(pdo_raw);
  }
  else
  {
    rdo->voltage_mv = 0U;
  }

  return STUSB4500_OK;
}

/* ==========================================================================
 *  Read Connection / Cable Status
 * ========================================================================== */
STUSB4500_Status_t STUSB4500_GetConnectionStatus(const STUSB4500_Handle_t *dev,
                                                   STUSB4500_Connection_Status_t *status)
{
  if (dev == NULL || dev->hi2c == NULL || status == NULL)
  {
    return STUSB4500_ERR_PARAM;
  }

  STUSB4500_Status_t ret;
  uint8_t reg_val;

  /* Read CC connection status */
  ret = STUSB4500_ReadReg(dev, STUSB4500_REG_CC_CONNECTION_STATUS, &reg_val);
  if (ret != STUSB4500_OK)
  {
    return ret;
  }
  status->attached = ((reg_val & STUSB4500_CC_ATTACH_MSK) != 0U);
  status->cc_state = (reg_val & STUSB4500_CC_STATE_MSK) >> STUSB4500_CC_STATE_POS;

  /* Read monitoring status */
  ret = STUSB4500_ReadReg(dev, STUSB4500_REG_MONITORING_STATUS, &reg_val);
  if (ret != STUSB4500_OK)
  {
    return ret;
  }
  status->vbus_valid_snk = ((reg_val & STUSB4500_VBUS_VALID_SNK_MSK) != 0U);
  status->vbus_vsafe0v   = ((reg_val & STUSB4500_VBUS_VSAFE0V_MSK) != 0U);
  status->vbus_ready     = ((reg_val & STUSB4500_VBUS_READY_MSK) != 0U);

  /* Read PE FSM state */
  ret = STUSB4500_ReadReg(dev, STUSB4500_REG_PE_FSM, &reg_val);
  if (ret != STUSB4500_OK)
  {
    return ret;
  }
  status->pe_fsm_state = reg_val;

  return STUSB4500_OK;
}

/* ==========================================================================
 *  Read Hardware Fault Status
 * ========================================================================== */
STUSB4500_Status_t STUSB4500_GetHWFault(const STUSB4500_Handle_t *dev, STUSB4500_HW_Fault_t *fault)
{
  if (dev == NULL || dev->hi2c == NULL || fault == NULL)
  {
    return STUSB4500_ERR_PARAM;
  }

  STUSB4500_Status_t status;
  uint8_t reg_val;

  status = STUSB4500_ReadReg(dev, STUSB4500_REG_HW_FAULT_STATUS, &reg_val);
  if (status != STUSB4500_OK)
  {
    return status;
  }

  fault->vbus_disch_fault = ((reg_val & STUSB4500_VBUS_DISCH_FAULT_MSK) != 0U);
  fault->vpu_valid        = ((reg_val & STUSB4500_VPU_VALID_MSK) != 0U);
  fault->vpu_ovp_fault    = ((reg_val & STUSB4500_VPU_OVP_FAULT_MSK) != 0U);
  fault->thermal_fault    = ((reg_val & STUSB4500_THERMAL_FAULT_MSK) != 0U);

  return STUSB4500_OK;
}

/* ==========================================================================
 *  Read and Decode Alert Status
 * ========================================================================== */
STUSB4500_Status_t STUSB4500_GetAlert(const STUSB4500_Handle_t *dev, STUSB4500_Alert_t *alert)
{
  if (dev == NULL || dev->hi2c == NULL || alert == NULL)
  {
    return STUSB4500_ERR_PARAM;
  }

  STUSB4500_Status_t status;
  uint8_t reg_val;

  status = STUSB4500_ReadReg(dev, STUSB4500_REG_ALERT_STATUS_1, &reg_val);
  if (status != STUSB4500_OK)
  {
    return status;
  }

  alert->phy_status_changed         = ((reg_val & STUSB4500_ALERT_PHY_STATUS_CHANGED_MSK) != 0U);
  alert->prt_status_changed         = ((reg_val & STUSB4500_ALERT_PRT_STATUS_CHANGED_MSK) != 0U);
  alert->pd_typec_status_changed    = ((reg_val & STUSB4500_ALERT_PD_TYPEC_STATUS_CHANGED_MSK) != 0U);
  alert->hw_fault_status_changed    = ((reg_val & STUSB4500_ALERT_HW_FAULT_STATUS_CHANGED_MSK) != 0U);
  alert->monitoring_status_changed  = ((reg_val & STUSB4500_ALERT_MONITORING_STATUS_CHANGED_MSK) != 0U);
  alert->cc_hw_fault_status_changed = ((reg_val & STUSB4500_ALERT_CC_HW_FAULT_STATUS_CHANGED_MSK) != 0U);

  return STUSB4500_OK;
}

/* ==========================================================================
 *  Clear All Alert / Transition Registers
 * ========================================================================== */
STUSB4500_Status_t STUSB4500_ClearAlerts(const STUSB4500_Handle_t *dev)
{
  if (dev == NULL || dev->hi2c == NULL)
  {
    return STUSB4500_ERR_PARAM;
  }

  STUSB4500_Status_t status;
  uint8_t dummy;

  /* Reading transition registers clears them */
  status = STUSB4500_ReadReg(dev, STUSB4500_REG_ALERT_STATUS_1, &dummy);
  if (status != STUSB4500_OK) return status;

  status = STUSB4500_ReadReg(dev, STUSB4500_REG_CC_CONNECTION_STATUS_TRANS, &dummy);
  if (status != STUSB4500_OK) return status;

  status = STUSB4500_ReadReg(dev, STUSB4500_REG_MONITORING_STATUS_TRANS, &dummy);
  if (status != STUSB4500_OK) return status;

  status = STUSB4500_ReadReg(dev, STUSB4500_REG_HW_FAULT_STATUS_TRANS, &dummy);
  if (status != STUSB4500_OK) return status;

  return STUSB4500_OK;
}

/* ==========================================================================
 *  Trigger PD Renegotiation
 * ========================================================================== */
STUSB4500_Status_t STUSB4500_Renegotiate(const STUSB4500_Handle_t *dev)
{
  if (dev == NULL || dev->hi2c == NULL)
  {
    return STUSB4500_ERR_PARAM;
  }

  /* Send a SRC_CAP request command to trigger renegotiation */
  return STUSB4500_WriteReg(dev, STUSB4500_REG_PD_COMMAND_CTRL, STUSB4500_PD_CMD_SRCCAP_REQ);
}

/* ==========================================================================
 *  Wait for Policy Engine SNK_READY State
 * ========================================================================== */
STUSB4500_Status_t STUSB4500_WaitForReady(const STUSB4500_Handle_t *dev, uint32_t timeout_ms)
{
  if (dev == NULL || dev->hi2c == NULL)
  {
    return STUSB4500_ERR_PARAM;
  }

  uint32_t start_tick = HAL_GetTick();
  uint8_t pe_state;
  STUSB4500_Status_t status;

  while ((HAL_GetTick() - start_tick) < timeout_ms)
  {
    status = STUSB4500_ReadReg(dev, STUSB4500_REG_PE_FSM, &pe_state);
    if (status != STUSB4500_OK)
    {
      return status;
    }

    if (pe_state == STUSB4500_PE_SNK_READY)
    {
      return STUSB4500_OK;
    }

    HAL_Delay(STUSB4500_POLL_INTERVAL);
  }

  return STUSB4500_ERR_TIMEOUT;
}

/* ==========================================================================
 *  Set Alert Mask
 * ========================================================================== */
STUSB4500_Status_t STUSB4500_SetAlertMask(const STUSB4500_Handle_t *dev, uint8_t mask)
{
  if (dev == NULL || dev->hi2c == NULL)
  {
    return STUSB4500_ERR_PARAM;
  }

  return STUSB4500_WriteReg(dev, STUSB4500_REG_ALERT_STATUS_MASK, mask);
}

/* ==========================================================================
 *  GPIO Control
 * ========================================================================== */
STUSB4500_Status_t STUSB4500_SetGPIO(const STUSB4500_Handle_t *dev, bool state)
{
  if (dev == NULL || dev->hi2c == NULL)
  {
    return STUSB4500_ERR_PARAM;
  }

  uint8_t val = state ? STUSB4500_GPIO_SW_GPIO_MSK : 0x00U;
  return STUSB4500_WriteReg(dev, STUSB4500_REG_GPIO_SW_GPIO, val);
}

/* ==========================================================================
 *  Read Source PDOs (received from attached charger)
 * ========================================================================== */
STUSB4500_Status_t STUSB4500_GetSourcePDOs(const STUSB4500_Handle_t *dev,
                                            uint32_t *src_pdos,
                                            uint8_t *num_pdos)
{
  if (dev == NULL || dev->hi2c == NULL || src_pdos == NULL || num_pdos == NULL)
  {
    return STUSB4500_ERR_PARAM;
  }

  STUSB4500_Status_t status;
  uint8_t hdr_high;

  /* Read RX header high byte to get number of data objects */
  status = STUSB4500_ReadReg(dev, STUSB4500_REG_RX_HEADER_HIGH, &hdr_high);
  if (status != STUSB4500_OK)
  {
    return status;
  }

  uint8_t count = (hdr_high & STUSB4500_RX_HDR_NUM_DATAOBJ_MSK) >> STUSB4500_RX_HDR_NUM_DATAOBJ_POS;
  if (count > STUSB4500_RX_DATA_OBJ_MAX)
  {
    count = STUSB4500_RX_DATA_OBJ_MAX;
  }
  *num_pdos = count;

  /* Read each source PDO from RX data object buffer */
  for (uint8_t i = 0U; i < count; i++)
  {
    uint8_t reg_addr = STUSB4500_REG_RX_DATA_OBJ_START + (i * STUSB4500_RX_DATA_OBJ_SIZE);
    status = STUSB4500_ReadReg32(dev, reg_addr, &src_pdos[i]);
    if (status != STUSB4500_OK)
    {
      return status;
    }
  }

  return STUSB4500_OK;
}
