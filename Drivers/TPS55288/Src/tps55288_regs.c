/**
  ******************************************************************************
  * @file    tps55288_regs.c
  * @brief   TPS55288RPMR - Low-level register access functions.
  *          Implements I2C read/write operations for the device registers.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "../Inc/tps55288.h"

/* ==========================================================================
 *  Register-Level Access Functions
 * ========================================================================== */

/**
 * @brief  Read a single 8-bit register via I2C.
 * @param  dev    Device handle
 * @param  reg    Register address
 * @param  data   Pointer to store read data
 * @retval TPS55288_RetVal_t
 */
TPS55288_RetVal_t TPS55288_ReadReg(const TPS55288_Handle_t *dev, uint8_t reg, uint8_t *data)
{
  if (dev == NULL || dev->hi2c == NULL || data == NULL)
  {
    return TPS55288_ERR_PARAM;
  }

  HAL_StatusTypeDef hal_status = HAL_I2C_Mem_Read(
    dev->hi2c,
    dev->i2c_addr << 1,      /* Convert 7-bit to 8-bit address */
    (uint16_t)reg,
    I2C_MEMADD_SIZE_8BIT,
    data,
    1,
    dev->timeout
  );

  if (hal_status != HAL_OK)
  {
    return TPS55288_ERR_I2C;
  }

  return TPS55288_OK;
}

/**
 * @brief  Write a single 8-bit register via I2C.
 * @param  dev    Device handle
 * @param  reg    Register address
 * @param  data   Data to write
 * @retval TPS55288_RetVal_t
 */
TPS55288_RetVal_t TPS55288_WriteReg(const TPS55288_Handle_t *dev, uint8_t reg, uint8_t data)
{
  if (dev == NULL || dev->hi2c == NULL)
  {
    return TPS55288_ERR_PARAM;
  }

  HAL_StatusTypeDef hal_status = HAL_I2C_Mem_Write(
    dev->hi2c,
    dev->i2c_addr << 1,      /* Convert 7-bit to 8-bit address */
    (uint16_t)reg,
    I2C_MEMADD_SIZE_8BIT,
    &data,
    1,
    dev->timeout
  );

  if (hal_status != HAL_OK)
  {
    return TPS55288_ERR_I2C;
  }

  return TPS55288_OK;
}

/**
 * @brief  Read-modify-write an 8-bit register.
 *         Reads the current register value, applies a mask, sets specific bits,
 *         and writes it back.
 * @param  dev    Device handle
 * @param  reg    Register address
 * @param  mask   Bit mask to clear (bits to affect)
 * @param  value  Bits to set (must be within mask)
 * @retval TPS55288_RetVal_t
 *
 * @note   Example: to set bits [2:0] to 0b101 while preserving other bits:
 *         TPS55288_ModifyReg(dev, REG, 0x07, 0x05)
 *         where 0x07 is the mask (bits [2:0]) and 0x05 is the value.
 */
TPS55288_RetVal_t TPS55288_ModifyReg(const TPS55288_Handle_t *dev, uint8_t reg,
                                      uint8_t mask, uint8_t value)
{
  if (dev == NULL || dev->hi2c == NULL)
  {
    return TPS55288_ERR_PARAM;
  }

  uint8_t reg_val;
  TPS55288_RetVal_t status;

  /* Read current register value */
  status = TPS55288_ReadReg(dev, reg, &reg_val);
  if (status != TPS55288_OK)
  {
    return status;
  }

  /* Clear masked bits and set new value */
  reg_val = (reg_val & ~mask) | (value & mask);

  /* Write back modified value */
  status = TPS55288_WriteReg(dev, reg, reg_val);
  if (status != TPS55288_OK)
  {
    return status;
  }

  return TPS55288_OK;
}
