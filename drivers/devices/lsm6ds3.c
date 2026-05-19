#include "lsm6ds3.h"
#include "utils.h"

void LSM6DS3_Init(LSM6DS3 *lsm) {
    ASSERT(lsm->spi != NULL);
    ASSERT(lsm->spi->cpol == SPI_CPOL_HIGH);
    ASSERT(lsm->spi->cpha == SPI_CPHA_1);
    SPI_Init(lsm->spi);
    LSM6DS3_Write_Reg(lsm, LSM6DS3_CTRL1_XL, LSM6DS3_XL_ODR_416_HZ | LSM6DS3_XL_FS_16G);
    LSM6DS3_Write_Reg(lsm, LSM6DS3_CTRL2_G, LSM6DS3_G_ODR_416_HZ | LSM6DS3_G_FS_2000_DPS);
    LSM6DS3_Write_Reg(lsm, LSM6DS3_CTRL3_C, LSM6DS3_BDU | LSM6DS3_IF_INC);
    LSM6DS3_Write_Reg(lsm, LSM6DS3_CTRL4_C, LSM6DS3_I2C_DISABLE);
    LSM6DS3_Write_Reg(lsm, LSM6DS3_CTRL8_XL, 0); // cutoff odr/9

    //    LSM6DS3_Write_Reg(lsm, LSM6DS3_CTRL8_XL, LSM6DS3_LPF2_XL_EN | LSM6DS3_HPCF_XL_1 | LSM6DS3_HP_SLOPE_XL_EN); // cutoff odr/9
    LSM6DS3_Write_Reg(lsm, LSM6DS3_CTRL9_XL, LSM6DS3_ZEN_XL | LSM6DS3_YEN_XL | LSM6DS3_XEN_XL);
    LSM6DS3_Write_Reg(lsm, LSM6DS3_CTRL10_C, LSM6DS3_ZEN_G | LSM6DS3_YEN_G | LSM6DS3_XEN_G);
}

void LSM6DS3_Write_Reg(LSM6DS3 *lsm, uint8_t reg, uint8_t value) {
    uint8_t buffer[2] = {reg & 0x7F, value}; // MSB=0 for write
    SPI_Write(lsm->spi, buffer, 2);
}

uint8_t LSM6DS3_Read_Reg(LSM6DS3 *lsm, uint8_t reg) {
    uint8_t buffer[2] = {reg | 0x80, 0}; // MSB=1 for read
    SPI_Transfer(lsm->spi, buffer, buffer, 2);
    return buffer[1];
}


void LSM6DS3_ReadAccel(LSM6DS3 *lsm, int16_t *x, int16_t *y, int16_t *z) {
    uint8_t buffer[7] = {LSM6DS3_OUTX_L_XL | 0x80}; // MSB=1 for read, auto-increment
    SPI_Transfer(lsm->spi, buffer, buffer, 7);
    *x = (int16_t)(buffer[2] << 8 | buffer[1]);
    *y = (int16_t)(buffer[4] << 8 | buffer[3]);
    *z = (int16_t)(buffer[6] << 8 | buffer[5]);
}

void LSM6DS3_ReadGyro(LSM6DS3 *lsm, int16_t *x, int16_t *y, int16_t *z) {
    uint8_t buffer[7] = {LSM6DS3_OUTX_L_G | 0x80}; // MSB=1 for read, auto-increment
    SPI_Transfer(lsm->spi, buffer, buffer, 7);
    *x = (int16_t)(buffer[2] << 8 | buffer[1]);
    *y = (int16_t)(buffer[4] << 8 | buffer[3]);
    *z = (int16_t)(buffer[6] << 8 | buffer[5]);
}