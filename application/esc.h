#include <stdio.h>
#include <math.h>
#include "hardware.h"
#include "systick.h"
#include "timer.h"
#include "drone_constants.h"

#define ESC_MIN             1000
#define ESC_MAX             2000

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

void test_calibration() {
  
    printf("Place level, hold still\n");
    SysTick_Delay_Milliseconds(3000);

    float roll_level = 0, pitch_level = 0;
    for (int i = 0; i < 500; i++) {
        int16_t ax, ay, az;
        LSM6DS3_ReadAccel(&lsm6ds3, &ax, &ay, &az);
        roll_level  += atan2f(ay, az) * 180.0f / M_PI;
        pitch_level += atan2f(-ax, az) * 180.0f / M_PI;
    }
    roll_level  /= 500.0f;
    pitch_level /= 500.0f;
    printf("Level: R:%.2f P:%.2f\n", roll_level, pitch_level);

    printf("Tilt forward, hold still\n");
    SysTick_Delay_Milliseconds(3000);

    float roll_fwd = 0, pitch_fwd = 0;
    for (int i = 0; i < 500; i++) {
        int16_t ax, ay, az;
        LSM6DS3_ReadAccel(&lsm6ds3, &ax, &ay, &az);
        roll_fwd  += atan2f(ay, az) * 180.0f / M_PI;
        pitch_fwd += atan2f(-ax, az) * 180.0f / M_PI;
    }
    roll_fwd  /= 500.0f;
    pitch_fwd /= 500.0f;
    printf("Forward: R:%.2f P:%.2f\n", roll_fwd, pitch_fwd);

    float delta_roll  = roll_fwd  - roll_level;
    float delta_pitch = pitch_fwd - pitch_level;

    float align_angle = atan2f(delta_roll, -delta_pitch) * 180.0f / M_PI;
    printf("Align angle: %.4f degrees\n", align_angle);;
    printf("Delta R:%.2f P:%.2f\n", delta_roll, delta_pitch);
    printf("Align angle: %.4f degrees\n", align_angle);

    while (1);
}

void esc_calibration(){
    printf("Starting calibration process\n");
    TIM1_Set_Duty_Cycle(TIM_CHANNEL_1, ESC_MAX);
    TIM1_Set_Duty_Cycle(TIM_CHANNEL_2, ESC_MAX);
    TIM1_Set_Duty_Cycle(TIM_CHANNEL_3, ESC_MAX);
    TIM1_Set_Duty_Cycle(TIM_CHANNEL_4, ESC_MAX);
    
    SysTick_Delay_Milliseconds(6000);
    printf("Should be on");
    TIM1_Set_Duty_Cycle(TIM_CHANNEL_1, ESC_MIN);
    TIM1_Set_Duty_Cycle(TIM_CHANNEL_2, ESC_MIN);
    TIM1_Set_Duty_Cycle(TIM_CHANNEL_3, ESC_MIN);
    TIM1_Set_Duty_Cycle(TIM_CHANNEL_4, ESC_MIN);
}

void arm_esc() {
    TIM1_Set_Duty_Cycle(TIM_CHANNEL_1, ESC_MIN);
    TIM1_Set_Duty_Cycle(TIM_CHANNEL_2, ESC_MIN);
    TIM1_Set_Duty_Cycle(TIM_CHANNEL_3, ESC_MIN);
    TIM1_Set_Duty_Cycle(TIM_CHANNEL_4, ESC_MIN);
    SysTick_Delay_Milliseconds(3000);
    printf("ESCs armed\n");
}

// ik this isnt an esc thing, leave me alone
void calibrate(float *roll_offset, float *pitch_offset, float *gyro_bias_x, float *gyro_bias_y, float *gyro_bias_z) 
{
  
    float bias_x = 0, bias_y = 0, bias_z = 0;
    for (int i = 0; i < 5000; i++) {
        int16_t gx, gy, gz;
        LSM6DS3_ReadGyro(&lsm6ds3, &gx, &gy, &gz);
        bias_x += gx;
        bias_y += gy;
        bias_z += gz;
    }
    bias_x /= 5000.0f;
    bias_y /= 5000.0f;
    bias_z /= 5000.0f;
    
    *gyro_bias_x = bias_x;
    *gyro_bias_y = bias_y;
    *gyro_bias_z = bias_z;
    printf("Gyro bias: %.2f %.2f %.2f\n", *gyro_bias_x, *gyro_bias_y, *gyro_bias_z);

    
    int16_t ax, ay, az, gx, gy, gz;

    float roll_angle  = 0.f;
    float pitch_angle = 0.f;
    
    LSM6DS3_ReadAccel(&lsm6ds3, &ax, &ay, &az);
    roll_angle  = atan2f(ay, az) * 180.0f / M_PI;
    pitch_angle = atan2f(-ax, az) * 180.0f / M_PI;

    for (int i = 0; i < 500; i++) {
        LSM6DS3_ReadAccel(&lsm6ds3, &ax, &ay, &az);
        LSM6DS3_ReadGyro(&lsm6ds3, &gx, &gy, &gz);

        float gx_dps = (gx - bias_x) * GYRO_SENSITIVITY;
        float gy_dps = (gy - bias_y) * GYRO_SENSITIVITY;

        float accel_roll  = atan2f(ay, az) * 180.0f / M_PI;
        float accel_pitch = atan2f(-ax, az) * 180.0f / M_PI;

        roll_angle  = 0.8f * (roll_angle  + gx_dps * DT) + 0.2f * accel_roll;
        pitch_angle = 0.8f * (pitch_angle + gy_dps * DT) + 0.2f * accel_pitch;

        SysTick_Delay_Microseconds(LOOP_US);
    }
    printf("Calibration angle: R:%.2f P:%.2f\n", roll_angle, pitch_angle);
    *roll_offset  = roll_angle;
    *pitch_offset = pitch_angle;
}
