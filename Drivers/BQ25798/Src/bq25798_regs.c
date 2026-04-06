/**
  ******************************************************************************
  * @file    bq25798_regs.c
  * @brief   BQ25798RQMR Buck-Boost Battery Charger - Register-level I2C access.
  *          Low-level read/write functions with error checking.
  *
  * @note    The BQ25798 uses big-endian byte order for 16-bit registers:
  *          the first byte address holds the MSB.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "../Inc/bq25798.h"

/* ==========================================================================
 *  Single Register Read (8-bit)
 * ========================================================================== */
BQ25798_Status_t BQ25798_ReadReg(const BQ25798_Handle_t *dev, uint8_t reg, uint8_t *data)
{
  if (dev == NULL || dev->hi2c == NULL || data == NULL)
  {
    return BQ25798_ERR_PARAM;
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
    return BQ25798_ERR_I2C;
  }

  return BQ25798_OK;
}

/* ==========================================================================
 *  Multiple Register Read
 * ========================================================================== */
BQ25798_Status_t BQ25798_ReadRegs(const BQ25798_Handle_t *dev, uint8_t reg, uint8_t *data, uint16_t len)
{
  if (dev == NULL || dev->hi2c == NULL || data == NULL || len == 0U)
  {
    return BQ25798_ERR_PARAM;
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
    return BQ25798_ERR_I2C;
  }

  return BQ25798_OK;
}

/* ==========================================================================
 *  Single Register Write (8-bit)
 * ========================================================================== */
BQ25798_Status_t BQ25798_WriteReg(const BQ25798_Handle_t *dev, uint8_t reg, uint8_t data)
{
  if (dev == NULL || dev->hi2c == NULL)
  {
    return BQ25798_ERR_PARAM;
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
    return BQ25798_ERR_I2C;
  }

  return BQ25798_OK;
}

/* ==========================================================================
 *  Multiple Register Write
 * ========================================================================== */
BQ25798_Status_t BQ25798_WriteRegs(const BQ25798_Handle_t *dev, uint8_t reg, const uint8_t *data, uint16_t len)
{
  if (dev == NULL || dev->hi2c == NULL || data == NULL || len == 0U)
  {
    return BQ25798_ERR_PARAM;
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
    return BQ25798_ERR_I2C;
  }

  return BQ25798_OK;
}

/* ==========================================================================
 *  Read-Modify-Write Register (8-bit)
 * ========================================================================== */
BQ25798_Status_t BQ25798_ModifyReg(const BQ25798_Handle_t *dev, uint8_t reg, uint8_t mask, uint8_t value)
{
  if (dev == NULL || dev->hi2c == NULL)
  {
    return BQ25798_ERR_PARAM;
  }

  uint8_t reg_val;
  BQ25798_Status_t status;

  /* Read current value */
  status = BQ25798_ReadReg(dev, reg, &reg_val);
  if (status != BQ25798_OK)
  {
    return status;
  }

  /* Modify: clear masked bits, then set new value */
  reg_val = (reg_val & ~mask) | (value & mask);

  /* Write back */
  status = BQ25798_WriteReg(dev, reg, reg_val);
  return status;
}

/* ==========================================================================
 *  Read 16-bit Register (big-endian: MSB at lower address)
 *
 *  BQ25798 stores 16-bit values with MSB first (addr) and LSB second (addr+1).
 * ========================================================================== */
BQ25798_Status_t BQ25798_ReadReg16(const BQ25798_Handle_t *dev, uint8_t reg, uint16_t *data)
{
  if (dev == NULL || dev->hi2c == NULL || data == NULL)
  {
    return BQ25798_ERR_PARAM;
  }

  uint8_t buf[2];
  BQ25798_Status_t status;

  status = BQ25798_ReadRegs(dev, reg, buf, 2U);
  if (status != BQ25798_OK)
  {
    return status;
  }

  /* Big-endian: byte 0 = MSB, byte 1 = LSB */
  *data = ((uint16_t)buf[0] << 8) | (uint16_t)buf[1];

  return BQ25798_OK;
}

/* ==========================================================================
 *  Write 16-bit Register (big-endian: MSB at lower address)
 * ========================================================================== */
BQ25798_Status_t BQ25798_WriteReg16(const BQ25798_Handle_t *dev, uint8_t reg, uint16_t data)
{
  if (dev == NULL || dev->hi2c == NULL)
  {
    return BQ25798_ERR_PARAM;
  }

  uint8_t buf[2];

  /* Big-endian: MSB first */
  buf[0] = (uint8_t)((data >> 8) & 0xFFU);
  buf[1] = (uint8_t)(data & 0xFFU);

  return BQ25798_WriteRegs(dev, reg, buf, 2U);
}

/* ==========================================================================
 *  Read 16-bit ADC Register (signed, big-endian)
 *  Returns the raw signed 16-bit value.
 * ========================================================================== */
BQ25798_Status_t BQ25798_ReadADC16(const BQ25798_Handle_t *dev, uint8_t reg, int16_t *data)
{
  if (dev == NULL || dev->hi2c == NULL || data == NULL)
  {
    return BQ25798_ERR_PARAM;
  }

  uint16_t raw;
  BQ25798_Status_t status;

  status = BQ25798_ReadReg16(dev, reg, &raw);
  if (status != BQ25798_OK)
  {
    return status;
  }

  /* Cast to signed */
  *data = (int16_t)raw;

  return BQ25798_OK;
}
