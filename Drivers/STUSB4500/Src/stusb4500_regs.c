/**
  ******************************************************************************
  * @file    stusb4500_regs.c
  * @brief   STUSB4500QTR USB PD sink controller - Register-level I2C access.
  *          Low-level read/write functions with error checking.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "../Inc/stusb4500.h"

/* ==========================================================================
 *  Single Register Read
 * ========================================================================== */
STUSB4500_Status_t STUSB4500_ReadReg(const STUSB4500_Handle_t *dev, uint8_t reg, uint8_t *data)
{
  if (dev == NULL || dev->hi2c == NULL || data == NULL)
  {
    return STUSB4500_ERR_PARAM;
  }

  HAL_StatusTypeDef hal_status;
  hal_status = HAL_I2C_Mem_Read(dev->hi2c,
                                (uint16_t)(dev->i2c_addr << 1),
                                reg,
                                I2C_MEMADD_SIZE_8BIT,
                                data,
                                1U,
                                dev->timeout);

  if (hal_status != HAL_OK)
  {
    return STUSB4500_ERR_I2C;
  }

  return STUSB4500_OK;
}

/* ==========================================================================
 *  Multiple Register Read
 * ========================================================================== */
STUSB4500_Status_t STUSB4500_ReadRegs(const STUSB4500_Handle_t *dev, uint8_t reg, uint8_t *data, uint16_t len)
{
  if (dev == NULL || dev->hi2c == NULL || data == NULL || len == 0U)
  {
    return STUSB4500_ERR_PARAM;
  }

  HAL_StatusTypeDef hal_status;
  hal_status = HAL_I2C_Mem_Read(dev->hi2c,
                                (uint16_t)(dev->i2c_addr << 1),
                                reg,
                                I2C_MEMADD_SIZE_8BIT,
                                data,
                                len,
                                dev->timeout);

  if (hal_status != HAL_OK)
  {
    return STUSB4500_ERR_I2C;
  }

  return STUSB4500_OK;
}

/* ==========================================================================
 *  Single Register Write
 * ========================================================================== */
STUSB4500_Status_t STUSB4500_WriteReg(const STUSB4500_Handle_t *dev, uint8_t reg, uint8_t data)
{
  if (dev == NULL || dev->hi2c == NULL)
  {
    return STUSB4500_ERR_PARAM;
  }

  HAL_StatusTypeDef hal_status;
  hal_status = HAL_I2C_Mem_Write(dev->hi2c,
                                 (uint16_t)(dev->i2c_addr << 1),
                                 reg,
                                 I2C_MEMADD_SIZE_8BIT,
                                 &data,
                                 1U,
                                 dev->timeout);

  if (hal_status != HAL_OK)
  {
    return STUSB4500_ERR_I2C;
  }

  return STUSB4500_OK;
}

/* ==========================================================================
 *  Multiple Register Write
 * ========================================================================== */
STUSB4500_Status_t STUSB4500_WriteRegs(const STUSB4500_Handle_t *dev, uint8_t reg, const uint8_t *data, uint16_t len)
{
  if (dev == NULL || dev->hi2c == NULL || data == NULL || len == 0U)
  {
    return STUSB4500_ERR_PARAM;
  }

  HAL_StatusTypeDef hal_status;
  hal_status = HAL_I2C_Mem_Write(dev->hi2c,
                                 (uint16_t)(dev->i2c_addr << 1),
                                 reg,
                                 I2C_MEMADD_SIZE_8BIT,
                                 (uint8_t *)data,
                                 len,
                                 dev->timeout);

  if (hal_status != HAL_OK)
  {
    return STUSB4500_ERR_I2C;
  }

  return STUSB4500_OK;
}

/* ==========================================================================
 *  Read-Modify-Write Register
 * ========================================================================== */
STUSB4500_Status_t STUSB4500_ModifyReg(const STUSB4500_Handle_t *dev, uint8_t reg, uint8_t mask, uint8_t value)
{
  if (dev == NULL || dev->hi2c == NULL)
  {
    return STUSB4500_ERR_PARAM;
  }

  uint8_t reg_val;
  STUSB4500_Status_t status;

  /* Read current value */
  status = STUSB4500_ReadReg(dev, reg, &reg_val);
  if (status != STUSB4500_OK)
  {
    return status;
  }

  /* Modify: clear masked bits, then set new value */
  reg_val = (reg_val & ~mask) | (value & mask);

  /* Write back */
  status = STUSB4500_WriteReg(dev, reg, reg_val);
  return status;
}

/* ==========================================================================
 *  Read 32-bit Register (4 bytes, little-endian)
 * ========================================================================== */
STUSB4500_Status_t STUSB4500_ReadReg32(const STUSB4500_Handle_t *dev, uint8_t reg, uint32_t *data)
{
  if (dev == NULL || dev->hi2c == NULL || data == NULL)
  {
    return STUSB4500_ERR_PARAM;
  }

  uint8_t buf[4];
  STUSB4500_Status_t status;

  status = STUSB4500_ReadRegs(dev, reg, buf, 4U);
  if (status != STUSB4500_OK)
  {
    return status;
  }

  /* Little-endian: byte 0 = LSB */
  *data = (uint32_t)buf[0]
        | ((uint32_t)buf[1] << 8)
        | ((uint32_t)buf[2] << 16)
        | ((uint32_t)buf[3] << 24);

  return STUSB4500_OK;
}

/* ==========================================================================
 *  Write 32-bit Register (4 bytes, little-endian)
 * ========================================================================== */
STUSB4500_Status_t STUSB4500_WriteReg32(const STUSB4500_Handle_t *dev, uint8_t reg, uint32_t data)
{
  if (dev == NULL || dev->hi2c == NULL)
  {
    return STUSB4500_ERR_PARAM;
  }

  uint8_t buf[4];

  buf[0] = (uint8_t)(data & 0xFFU);
  buf[1] = (uint8_t)((data >> 8) & 0xFFU);
  buf[2] = (uint8_t)((data >> 16) & 0xFFU);
  buf[3] = (uint8_t)((data >> 24) & 0xFFU);

  return STUSB4500_WriteRegs(dev, reg, buf, 4U);
}
