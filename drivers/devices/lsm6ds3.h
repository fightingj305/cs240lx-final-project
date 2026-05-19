#pragma once
#include "spi.h"

typedef struct {
    SPI *spi;
} LSM6DS3;

void LSM6DS3_Init(LSM6DS3 *lsm);
void LSM6DS3_Reset(LSM6DS3 *lsm);
void LSM6DS3_Write_Reg(LSM6DS3 *lsm, uint8_t reg, uint8_t value);
uint8_t LSM6DS3_Read_Reg(LSM6DS3 *lsm, uint8_t reg);
void LSM6DS3_ReadAccel(LSM6DS3 *lsm, int16_t *x, int16_t *y, int16_t *z);
void LSM6DS3_ReadGyro(LSM6DS3 *lsm, int16_t *x, int16_t *y, int16_t *z);


enum LSM6DS3_Embedded_Func_Regs {
    LSM6DS3_FUNC_CFG_ACCESS         = 0x01,
    LSM6DS3_SENSOR_SYNC_TIME_FRAME  = 0x04,
    LSM6DS3_SENSOR_SYNC_RES_RATIO   = 0x05,
    LSM6DS3_FIFO_CTRL1              = 0x06,
    LSM6DS3_FIFO_CTRL2              = 0x07,
    LSM6DS3_FIFO_CTRL3              = 0x08,
    LSM6DS3_FIFO_CTRL4              = 0x09,
    LSM6DS3_FIFO_CTRL5              = 0x0A,
    LSM6DS3_ORIENT_CFG_G            = 0x0B,
    LSM6DS3_INT1_CTRL               = 0x0D,
    LSM6DS3_INT2_CTRL               = 0x0E,
    LSM6DS3_WHO_AM_I                = 0x0F,
    LSM6DS3_CTRL1_XL                = 0x10,
    LSM6DS3_CTRL2_G                 = 0x11,
    LSM6DS3_CTRL3_C                 = 0x12,
    LSM6DS3_CTRL4_C                 = 0x13,
    LSM6DS3_CTRL5_C                 = 0x14,
    LSM6DS3_CTRL6_C                 = 0x15,
    LSM6DS3_CTRL7_G                 = 0x16,
    LSM6DS3_CTRL8_XL                = 0x17,
    LSM6DS3_CTRL9_XL                = 0x18,
    LSM6DS3_CTRL10_C                = 0x19,
    LSM6DS3_MASTER_CONFIG           = 0x1A,
    LSM6DS3_WAKE_UP_SRC             = 0x1B,
    LSM6DS3_TAP_SRC                 = 0x1C,
    LSM6DS3_D6D_SRC                 = 0x1D,
    LSM6DS3_STATUS_REG              = 0x1E,
    LSM6DS3_OUT_TEMP_L              = 0x20,
    LSM6DS3_OUT_TEMP_H              = 0x21,
    LSM6DS3_OUTX_L_G                = 0x22,
    LSM6DS3_OUTX_H_G                = 0x23,
    LSM6DS3_OUTY_L_G                = 0x24,
    LSM6DS3_OUTY_H_G                = 0x25,
    LSM6DS3_OUTZ_L_G                = 0x26,
    LSM6DS3_OUTZ_H_G                = 0x27,
    LSM6DS3_OUTX_L_XL               = 0x28,
    LSM6DS3_OUTX_H_XL               = 0x29,
    LSM6DS3_OUTY_L_XL               = 0x2A,
    LSM6DS3_OUTY_H_XL               = 0x2B,
    LSM6DS3_OUTZ_L_XL               = 0x2C,
    LSM6DS3_OUTZ_H_XL               = 0x2D,
    LSM6DS3_SENSORHUB1_REG          = 0x2E,
    LSM6DS3_SENSORHUB2_REG          = 0x2F,
    LSM6DS3_SENSORHUB3_REG          = 0x30,
    LSM6DS3_SENSORHUB4_REG          = 0x31,
    LSM6DS3_SENSORHUB5_REG          = 0x32,
    LSM6DS3_SENSORHUB6_REG          = 0x33,
    LSM6DS3_SENSORHUB7_REG          = 0x34,
    LSM6DS3_SENSORHUB8_REG          = 0x35,
    LSM6DS3_SENSORHUB9_REG          = 0x36,
    LSM6DS3_SENSORHUB10_REG         = 0x37,
    LSM6DS3_SENSORHUB11_REG         = 0x38,
    LSM6DS3_SENSORHUB12_REG         = 0x39,
    LSM6DS3_FIFO_STATUS1            = 0x3A,
    LSM6DS3_FIFO_STATUS2            = 0x3B,
    LSM6DS3_FIFO_STATUS3            = 0x3C,
    LSM6DS3_FIFO_STATUS4            = 0x3D,
    LSM6DS3_FIFO_DATA_OUT_L         = 0x3E,
    LSM6DS3_FIFO_DATA_OUT_H         = 0x3F,
    LSM6DS3_TIMESTAMP0_REG          = 0x40,
    LSM6DS3_TIMESTAMP1_REG          = 0x41,
    LSM6DS3_TIMESTAMP2_REG          = 0x42,
    LSM6DS3_STEP_TIMESTAMP_L        = 0x49,
    LSM6DS3_STEP_TIMESTAMP_H        = 0x4A,
    LSM6DS3_STEP_COUNTER_L          = 0x4B,
    LSM6DS3_STEP_COUNTER_H          = 0x4C,
    LSM6DS3_SENSORHUB13_REG         = 0x4D,
    LSM6DS3_SENSORHUB14_REG         = 0x4E,
    LSM6DS3_SENSORHUB15_REG         = 0x4F,
    LSM6DS3_SENSORHUB16_REG         = 0x50,
    LSM6DS3_SENSORHUB17_REG         = 0x51,
    LSM6DS3_SENSORHUB18_REG         = 0x52,
    LSM6DS3_FUNC_SRC                = 0x53,
    LSM6DS3_TAP_CFG                 = 0x58,
    LSM6DS3_TAP_THS_6D              = 0x59,
    LSM6DS3_INT_DUR2                = 0x5A,
    LSM6DS3_WAKE_UP_THS             = 0x5B,
    LSM6DS3_WAKE_UP_DUR             = 0x5C,
    LSM6DS3_FREE_FALL               = 0x5D,
    LSM6DS3_MD1_CFG                 = 0x5E,
    LSM6DS3_MD2_CFG                 = 0x5F,
    LSM6DS3_OUT_MAG_RAW_X_L         = 0x66,
    LSM6DS3_OUT_MAG_RAW_X_H         = 0x67,
    LSM6DS3_OUT_MAG_RAW_Y_L         = 0x68,
    LSM6DS3_OUT_MAG_RAW_Y_H         = 0x69,
    LSM6DS3_OUT_MAG_RAW_Z_L         = 0x6A,
    LSM6DS3_OUT_MAG_RAW_Z_H         = 0x6B,
};


enum LSM6DS3_CTRL1_XL_Bits {
    // Output data rate and power mode selection
    LSM6DS3_XL_ODR_POWER_DOWN   = (0x0 << 4),
    LSM6DS3_XL_ODR_12_5_HZ      = (0x1 << 4),
    LSM6DS3_XL_ODR_26_HZ        = (0x2 << 4),
    LSM6DS3_XL_ODR_52_HZ        = (0x3 << 4),
    LSM6DS3_XL_ODR_104_HZ       = (0x4 << 4),
    LSM6DS3_XL_ODR_208_HZ       = (0x5 << 4),
    LSM6DS3_XL_ODR_416_HZ       = (0x6 << 4),
    LSM6DS3_XL_ODR_833_HZ       = (0x7 << 4),
    LSM6DS3_XL_ODR_1660_HZ      = (0x8 << 4),
    LSM6DS3_XL_ODR_3330_HZ      = (0x9 << 4),
    LSM6DS3_XL_ODR_6660_HZ      = (0xA << 4),
    LSM6DS3_XL_ODR_MASK         = (0xF << 4),
    
    // Full-scale selection
    LSM6DS3_XL_FS_2G            = (0x0 << 2),
    LSM6DS3_XL_FS_16G           = (0x1 << 2),
    LSM6DS3_XL_FS_4G            = (0x2 << 2),
    LSM6DS3_XL_FS_8G            = (0x3 << 2),
    LSM6DS3_XL_FS_MASK          = (0x3 << 2),
    
    // Anti-aliasing filter bandwidth
    LSM6DS3_XL_BW_400HZ         = (0x0 << 0),
    LSM6DS3_XL_BW_200HZ         = (0x1 << 0),
    LSM6DS3_XL_BW_100HZ         = (0x2 << 0),
    LSM6DS3_XL_BW_50HZ          = (0x3 << 0),
    LSM6DS3_XL_BW_MASK          = (0x3 << 0)
};

// CTRL2_G (11h) - Gyroscope control
enum LSM6DS3_CTRL2_G_Bits {
    // Output data rate selection
    LSM6DS3_G_ODR_POWER_DOWN    = (0x0 << 4),
    LSM6DS3_G_ODR_12_5_HZ       = (0x1 << 4),
    LSM6DS3_G_ODR_26_HZ         = (0x2 << 4),
    LSM6DS3_G_ODR_52_HZ         = (0x3 << 4),
    LSM6DS3_G_ODR_104_HZ        = (0x4 << 4),
    LSM6DS3_G_ODR_208_HZ        = (0x5 << 4),
    LSM6DS3_G_ODR_416_HZ        = (0x6 << 4),
    LSM6DS3_G_ODR_833_HZ        = (0x7 << 4),
    LSM6DS3_G_ODR_1660_HZ       = (0x8 << 4),
    LSM6DS3_G_ODR_MASK          = (0xF << 4),
    
    // Full-scale selection
    LSM6DS3_G_FS_250_DPS        = (0x0 << 2),
    LSM6DS3_G_FS_500_DPS        = (0x1 << 2),
    LSM6DS3_G_FS_1000_DPS       = (0x2 << 2),
    LSM6DS3_G_FS_2000_DPS       = (0x3 << 2),
    LSM6DS3_G_FS_MASK           = (0x3 << 2),
    
    // Full-scale at 125 dps
    LSM6DS3_G_FS_125_DPS        = (0x1 << 1)
};

// CTRL3_C (12h) - Control register 3
enum LSM6DS3_CTRL3_C_Bits {
    LSM6DS3_BOOT                = (1 << 7),  // Reboot memory content
    LSM6DS3_BDU                 = (1 << 6),  // Block data update
    LSM6DS3_H_LACTIVE           = (1 << 5),  // Interrupt activation level
    LSM6DS3_PP_OD               = (1 << 4),  // Push-pull/open-drain
    LSM6DS3_SIM                 = (1 << 3),  // SPI mode selection
    LSM6DS3_IF_INC              = (1 << 2),  // Register address auto-increment
    LSM6DS3_BLE                 = (1 << 1),  // Big/little endian
    LSM6DS3_SW_RESET            = (1 << 0)   // Software reset
};

// CTRL4_C (13h) - Control register 4
enum LSM6DS3_CTRL4_C_Bits {
    XL_BW_SCAL_ODR              = (1 << 7),  // Accel bandwidth selection
    LSM6DS3_SLEEP               = (1 << 6),  // Gyro sleep mode
    LSM6DS3_INT2_ON_INT1        = (1 << 5),  // All INT2 signals on INT1
    LSM6DS3_FIFO_TEMP_EN        = (1 << 4),  // Enable temperature data as 4th FIFO data
    LSM6DS3_DRDY_MASK           = (1 << 3),  // Data available mask
    LSM6DS3_I2C_DISABLE         = (1 << 2),  // Disable I2C interface
    LSM6DS3_STOP_ON_FTH         = (1 << 0)   // Enable FIFO threshold level use
};

// CTRL5_C (14h) - Control register 5
enum LSM6DS3_CTRL5_C_Bits {
    LSM6DS3_ROUNDING_2          = (1 << 7),  // Rounding mode bit 2
    LSM6DS3_ROUNDING_1          = (1 << 6),  // Rounding mode bit 1
    LSM6DS3_ROUNDING_0          = (1 << 5),  // Rounding mode bit 0
    LSM6DS3_ROUNDING_MASK       = (0x7 << 5),
    
    // Self-test enables
    LSM6DS3_ST_XL_1             = (1 << 1),  // Accel self-test bit 1
    LSM6DS3_ST_XL_0             = (1 << 0),  // Accel self-test bit 0
    LSM6DS3_ST_XL_MASK          = (0x3 << 0),
    LSM6DS3_ST_G_1              = (1 << 3),  // Gyro self-test bit 1
    LSM6DS3_ST_G_0              = (1 << 2),  // Gyro self-test bit 0
    LSM6DS3_ST_G_MASK           = (0x3 << 2)
};

// CTRL6_C (15h) - Control register 6
enum LSM6DS3_CTRL6_C_Bits {
    LSM6DS3_TRIG_EN             = (1 << 7),  // Data ready pulsed mode
    LSM6DS3_LVL_EN              = (1 << 6),  // Data ready level mode
    LSM6DS3_LVL2_EN             = (1 << 5),  // Level-sensitive latched enable
    LSM6DS3_XL_HM_MODE          = (1 << 4),  // High-performance mode disable (XL)
};

// CTRL7_G (16h) - Gyroscope control register 7
enum LSM6DS3_CTRL7_G_Bits {
    LSM6DS3_G_HM_MODE           = (1 << 7),  // High-performance mode disable
    LSM6DS3_HP_EN_G             = (1 << 6),  // High-pass filter enable
    LSM6DS3_HP_G_RST            = (1 << 3),  // High-pass filter reset

    // High-pass filter cutoff frequency
    LSM6DS3_HPCF_G_0_0081HZ     = (0x0 << 4),
    LSM6DS3_HPCF_G_0_0324HZ     = (0x1 << 4),
    LSM6DS3_HPCF_G_2_07HZ       = (0x2 << 4),
    LSM6DS3_HPCF_G_16_32HZ      = (0x3 << 4),
    LSM6DS3_HPCF_G_MASK         = (0x3 << 4),
    
    LSM6DS3_ROUNDING_STATUS     = (1 << 2)   // Source register rounding
};

// CTRL8_XL (17h) - Accelerometer control register 8
enum LSM6DS3_CTRL8_XL_Bits {
    LSM6DS3_LPF2_XL_EN          = (1 << 7),  // LPF2 enable
    LSM6DS3_HPCF_XL_1           = (1 << 6),  // HPF cutoff bit 1, see pg.60
    LSM6DS3_HPCF_XL_0           = (1 << 5),  // HPF cutoff bit 0
    LSM6DS3_HPCF_XL_MASK        = (0x3 << 5),
    
    // Filter selection
    LSM6DS3_HP_SLOPE_XL_EN      = (1 << 2),  // Slope filter/high pass filter selection
    LSM6DS3_LOW_PASS_ON_6D      = (1 << 0)   // LPF on 6D function
};

// CTRL9_XL (18h) - Accelerometer control register 9
enum LSM6DS3_CTRL9_XL_Bits {
    LSM6DS3_ZEN_XL              = (1 << 5),  // Accelerometer sensor enable Z
    LSM6DS3_YEN_XL              = (1 << 4),  // Accelerometer sensor enable Y
    LSM6DS3_XEN_XL              = (1 << 3),  // Accelerometer sensor enable X
    LSM6DS3_SOFT_EN             = (1 << 2)   // Soft-iron correction enable
};

// CTRL10_C (19h) - Control register 10
enum LSM6DS3_CTRL10_C_Bits {
    LSM6DS3_ZEN_G               = (1 << 5),  // Gyroscope sensor enable Z
    LSM6DS3_YEN_G               = (1 << 4),  // Gyroscope sensor enable Y
    LSM6DS3_XEN_G               = (1 << 3),  // Gyroscope sensor enable X
    LSM6DS3_FUNC_EN             = (1 << 2),  // Enable embedded functions
    LSM6DS3_PEDO_RST_STEP       = (1 << 1),  // Pedometer step counter reset
    LSM6DS3_SIGN_MOTION_EN      = (1 << 0)   // Significant motion enable
};

// STATUS_REG (1Eh) - Status register
enum LSM6DS3_STATUS_REG_Bits {
    LSM6DS3_TDA                 = (1 << 2),  // Temperature data available
    LSM6DS3_GDA                 = (1 << 1),  // Gyroscope data available
    LSM6DS3_XLDA                = (1 << 0)   // Accelerometer data available
};

// INT1_CTRL (0Dh) - INT1 pin control
enum LSM6DS3_INT1_CTRL_Bits {
    LSM6DS3_INT1_STEP_DETECTOR  = (1 << 7),
    LSM6DS3_INT1_SIGN_MOT       = (1 << 6),
    LSM6DS3_INT1_FULL_FLAG      = (1 << 5),
    LSM6DS3_INT1_FIFO_OVR       = (1 << 4),
    LSM6DS3_INT1_FTH            = (1 << 3),  // FIFO threshold
    LSM6DS3_INT1_BOOT           = (1 << 2),
    LSM6DS3_INT1_DRDY_G         = (1 << 1),  // Gyro data ready
    LSM6DS3_INT1_DRDY_XL        = (1 << 0)   // Accel data ready
};

// INT2_CTRL (0Eh) - INT2 pin control
enum LSM6DS3_INT2_CTRL_Bits {
    LSM6DS3_INT2_STEP_DELTA     = (1 << 7),
    LSM6DS3_INT2_STEP_COUNT_OV  = (1 << 6),
    LSM6DS3_INT2_FULL_FLAG      = (1 << 5),
    LSM6DS3_INT2_FIFO_OVR       = (1 << 4),
    LSM6DS3_INT2_FTH            = (1 << 3),  // FIFO threshold
    LSM6DS3_INT2_DRDY_TEMP      = (1 << 2),  // Temperature data ready
    LSM6DS3_INT2_DRDY_G         = (1 << 1),  // Gyro data ready
    LSM6DS3_INT2_DRDY_XL        = (1 << 0)   // Accel data ready
};

// FIFO_CTRL5 (0Ah) - FIFO control register 5
enum LSM6DS3_FIFO_CTRL5_Bits {
    // FIFO mode selection
    LSM6DS3_FIFO_MODE_BYPASS            = (0x0 << 0),
    LSM6DS3_FIFO_MODE_FIFO              = (0x1 << 0),
    LSM6DS3_FIFO_MODE_CONTINUOUS_TO_FIFO = (0x3 << 0),
    LSM6DS3_FIFO_MODE_BYPASS_TO_CONTINUOUS = (0x4 << 0),
    LSM6DS3_FIFO_MODE_CONTINUOUS        = (0x6 << 0),
    LSM6DS3_FIFO_MODE_MASK              = (0x7 << 0),
    
    // FIFO ODR selection
    LSM6DS3_FIFO_ODR_DISABLED   = (0x0 << 3),
    LSM6DS3_FIFO_ODR_12_5_HZ    = (0x1 << 3),
    LSM6DS3_FIFO_ODR_26_HZ      = (0x2 << 3),
    LSM6DS3_FIFO_ODR_52_HZ      = (0x3 << 3),
    LSM6DS3_FIFO_ODR_104_HZ     = (0x4 << 3),
    LSM6DS3_FIFO_ODR_208_HZ     = (0x5 << 3),
    LSM6DS3_FIFO_ODR_416_HZ     = (0x6 << 3),
    LSM6DS3_FIFO_ODR_833_HZ     = (0x7 << 3),
    LSM6DS3_FIFO_ODR_1660_HZ    = (0x8 << 3),
    LSM6DS3_FIFO_ODR_3330_HZ    = (0x9 << 3),
    LSM6DS3_FIFO_ODR_6660_HZ    = (0xA << 3),
    LSM6DS3_FIFO_ODR_MASK       = (0xF << 3)
};

// Sensitivity values for conversion
#define LSM6DS3_ACCEL_SENSITIVITY_2G   0.061f  // mg/LSB
#define LSM6DS3_ACCEL_SENSITIVITY_4G   0.122f  // mg/LSB
#define LSM6DS3_ACCEL_SENSITIVITY_8G   0.244f  // mg/LSB
#define LSM6DS3_ACCEL_SENSITIVITY_16G  0.488f  // mg/LSB

#define LSM6DS3_GYRO_SENSITIVITY_125DPS   4.375f  // mdps/LSB
#define LSM6DS3_GYRO_SENSITIVITY_250DPS   8.75f   // mdps/LSB
#define LSM6DS3_GYRO_SENSITIVITY_500DPS   17.50f  // mdps/LSB
#define LSM6DS3_GYRO_SENSITIVITY_1000DPS  35.0f   // mdps/LSB
#define LSM6DS3_GYRO_SENSITIVITY_2000DPS  70.0f   // mdps/LSB

#define LSM6DS3_TEMP_SENSITIVITY  16.0f   // LSB/°C
#define LSM6DS3_TEMP_OFFSET       25.0f   // °C