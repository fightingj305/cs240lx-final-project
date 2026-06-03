/**
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#include "vl53l1_platform.h"
#include <string.h>
#include <time.h>
#include <math.h>
#include "i2c.h"
#include "systick.h"
#include "utils.h"

// helpers to process device and index
#define VL53_MAX_BUF 256
static void vl53_write(uint16_t dev, uint16_t index, uint8_t *data, uint32_t count) {
    ASSERT(count + 2 <= VL53_MAX_BUF);
    uint8_t buf[VL53_MAX_BUF];
    buf[0] = (index >> 8) & 0xFF;
    buf[1] = index & 0xFF;
    for (uint32_t i = 0; i < count; i++) {
        buf[i + 2] = data[i];
    }
    I2C_Write(vl53_i2c, dev >> 1, buf, count + 2);
}

static void vl53_read(uint16_t dev, uint16_t index, uint8_t *data, uint32_t count) {
    uint8_t idx[2] = { (index >> 8) & 0xFF, index & 0xFF };
    I2C_WriteRead(vl53_i2c, dev >> 1, idx, 2, data, count);
}

int8_t VL53L1_WriteMulti(uint16_t dev, uint16_t index, uint8_t *pdata, uint32_t count) {
    vl53_write(dev, index, pdata, count);
    return 0;
}

int8_t VL53L1_ReadMulti(uint16_t dev, uint16_t index, uint8_t *pdata, uint32_t count) {
    vl53_read(dev, index, pdata, count);
    return 0;
}

int8_t VL53L1_WrByte(uint16_t dev, uint16_t index, uint8_t data) {
    vl53_write(dev, index, &data, 1);
    return 0;
}

int8_t VL53L1_WrWord(uint16_t dev, uint16_t index, uint16_t data) {
    uint8_t buf[2] = { (data >> 8) & 0xFF, data & 0xFF };
    vl53_write(dev, index, buf, 2);
    return 0;
}

int8_t VL53L1_WrDWord(uint16_t dev, uint16_t index, uint32_t data) {
    uint8_t buf[4] = {
        (data >> 24) & 0xFF,
        (data >> 16) & 0xFF,
        (data >>  8) & 0xFF,
         data        & 0xFF
    };
    vl53_write(dev, index, buf, 4);
    return 0;
}

int8_t VL53L1_RdByte(uint16_t dev, uint16_t index, uint8_t *data) {
    vl53_read(dev, index, data, 1);
    return 0;
}

int8_t VL53L1_RdWord(uint16_t dev, uint16_t index, uint16_t *data) {
    uint8_t buf[2];
    vl53_read(dev, index, buf, 2);
    *data = ((uint16_t)buf[0] << 8) | buf[1];
    return 0;
}

int8_t VL53L1_RdDWord(uint16_t dev, uint16_t index, uint32_t *data) {
    uint8_t buf[4];
    vl53_read(dev, index, buf, 4);
    *data = ((uint32_t)buf[0] << 24) |
            ((uint32_t)buf[1] << 16) |
            ((uint32_t)buf[2] <<  8) |
             (uint32_t)buf[3];
    return 0;
}

int8_t VL53L1_WaitMs(uint16_t dev, int32_t wait_ms) {
    SysTick_Delay_Milliseconds((uint32_t)wait_ms);
    return 0;
}