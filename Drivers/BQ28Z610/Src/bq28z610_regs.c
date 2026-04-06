/**
  ******************************************************************************
  * @file    bq28z610_regs.c
  * @brief   BQ28Z610 - Low-level register access functions.
  *          Implements I2C read/write operations for SMBus standard data
  *          and extended block data.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "../Inc/bq28z610.h"

/* ==========================================================================
 *  Register-Level Access Functions
 * ========================================================================== */

/**
 * @brief  Read a single 8-bit register via SMBus.
 * @param  dev    Device handle
 * @param  reg    Register address
 * @param  data   Pointer to store read data
 * @retval BQ28Z610_RetVal_t
 */
BQ28Z610_RetVal_t BQ28Z610_ReadReg(const BQ28Z610_Handle_t *dev, uint8_t reg, uint8_t *data)
{
  if (dev == NULL || dev->hi2c == NULL || data == NULL)
  {
    return BQ28Z610_ERR_PARAM;
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
    return BQ28Z610_ERR_I2C;
  }

  return BQ28Z610_OK;
}

/**
 * @brief  Write a single 8-bit register via SMBus.
 * @param  dev    Device handle
 * @param  reg    Register address
 * @param  data   Data to write
 * @retval BQ28Z610_RetVal_t
 */
BQ28Z610_RetVal_t BQ28Z610_WriteReg(const BQ28Z610_Handle_t *dev, uint8_t reg, uint8_t data)
{
  if (dev == NULL || dev->hi2c == NULL)
  {
    return BQ28Z610_ERR_PARAM;
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
    return BQ28Z610_ERR_I2C;
  }

  return BQ28Z610_OK;
}

/**
 * @brief  Read multiple consecutive registers (block read).
 * @param  dev    Device handle
 * @param  reg    Starting register address
 * @param  data   Pointer to buffer for read data
 * @param  len    Number of bytes to read (max 32 for block data)
 * @retval BQ28Z610_RetVal_t
 */
BQ28Z610_RetVal_t BQ28Z610_ReadRegs(const BQ28Z610_Handle_t *dev, uint8_t reg, uint8_t *data, uint16_t len)
{
  if (dev == NULL || dev->hi2c == NULL || data == NULL || len == 0)
  {
    return BQ28Z610_ERR_PARAM;
  }

  HAL_StatusTypeDef hal_status = HAL_I2C_Mem_Read(
    dev->hi2c,
    dev->i2c_addr << 1,
    (uint16_t)reg,
    I2C_MEMADD_SIZE_8BIT,
    data,
    len,
    dev->timeout
  );

  if (hal_status != HAL_OK)
  {
    return BQ28Z610_ERR_I2C;
  }

  return BQ28Z610_OK;
}

/**
 * @brief  Write multiple consecutive registers (block write).
 * @param  dev    Device handle
 * @param  reg    Starting register address
 * @param  data   Pointer to data buffer
 * @param  len    Number of bytes to write (max 32 for block data)
 * @retval BQ28Z610_RetVal_t
 */
BQ28Z610_RetVal_t BQ28Z610_WriteRegs(const BQ28Z610_Handle_t *dev, uint8_t reg, const uint8_t *data, uint16_t len)
{
  if (dev == NULL || dev->hi2c == NULL || data == NULL || len == 0)
  {
    return BQ28Z610_ERR_PARAM;
  }

  HAL_StatusTypeDef hal_status = HAL_I2C_Mem_Write(
    dev->hi2c,
    dev->i2c_addr << 1,
    (uint16_t)reg,
    I2C_MEMADD_SIZE_8BIT,
    (uint8_t *)data,
    len,
    dev->timeout
  );

  if (hal_status != HAL_OK)
  {
    return BQ28Z610_ERR_I2C;
  }

  return BQ28Z610_OK;
}

/**
 * @brief  Read a 16-bit register value (little-endian format).
 *         LSB is at the lower register address, MSB at higher.
 * @param  dev    Device handle
 * @param  reg    Register address for LSB
 * @param  data   Pointer to store 16-bit value
 * @retval BQ28Z610_RetVal_t
 */
BQ28Z610_RetVal_t BQ28Z610_ReadReg16(const BQ28Z610_Handle_t *dev, uint8_t reg, uint16_t *data)
{
  if (dev == NULL || dev->hi2c == NULL || data == NULL)
  {
    return BQ28Z610_ERR_PARAM;
  }

  uint8_t buf[2];
  BQ28Z610_RetVal_t status;

  /* Read 2 bytes (little-endian) */
  status = BQ28Z610_ReadRegs(dev, reg, buf, 2);
  if (status != BQ28Z610_OK)
  {
    return status;
  }

  /* Combine bytes: LSB first, then MSB */
  *data = ((uint16_t)buf[1] << 8) | buf[0];

  return BQ28Z610_OK;
}

/**
 * @brief  Write a 16-bit register value (little-endian format).
 * @param  dev    Device handle
 * @param  reg    Register address for LSB
 * @param  data   16-bit value to write
 * @retval BQ28Z610_RetVal_t
 */
BQ28Z610_RetVal_t BQ28Z610_WriteReg16(const BQ28Z610_Handle_t *dev, uint8_t reg, uint16_t data)
{
  if (dev == NULL || dev->hi2c == NULL)
  {
    return BQ28Z610_ERR_PARAM;
  }

  uint8_t buf[2];

  /* Convert to little-endian: LSB first */
  buf[0] = (uint8_t)(data & 0xFF);
  buf[1] = (uint8_t)((data >> 8) & 0xFF);

  return BQ28Z610_WriteRegs(dev, reg, buf, 2);
}

/**
 * @brief  Issue a control sub-command and read the response.
 *         Control commands are 2-byte values sent via register 0x00,
 *         and the response is read back from the same register.
 * @param  dev       Device handle
 * @param  cmd       16-bit control command value
 * @param  response  Pointer to store response (can be NULL)
 * @retval BQ28Z610_RetVal_t
 */
BQ28Z610_RetVal_t BQ28Z610_ControlCommand(const BQ28Z610_Handle_t *dev, uint16_t cmd, uint16_t *response)
{
  if (dev == NULL || dev->hi2c == NULL)
  {
    return BQ28Z610_ERR_PARAM;
  }

  BQ28Z610_RetVal_t status;

  /* Send control command (2 bytes, little-endian) */
  status = BQ28Z610_WriteReg16(dev, BQ28Z610_CMD_CONTROL, cmd);
  if (status != BQ28Z610_OK)
  {
    return status;
  }

  /* Optional: read response after a small delay */
  if (response != NULL)
  {
    HAL_Delay(10);  /* Wait for command to complete */
    status = BQ28Z610_ReadReg16(dev, BQ28Z610_CMD_CONTROL, response);
    if (status != BQ28Z610_OK)
    {
      return status;
    }
  }

  return BQ28Z610_OK;
}
