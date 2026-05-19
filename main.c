#include "hardware.h"
#include "systick.h"
#include "timer.h"
#include "pid.h"
#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

#define GYRO_SENSITIVITY    0.070f
#define COMP_FILTER_ALPHA   0.98f
#define THROTTLE_HOVER      4000
#define MAX_DUTY            TIM1_Get_Max_Duty()
#define CLAMP(x, lo, hi)    ((x) < (lo) ? (lo) : (x) > (hi) ? (hi) : (x))

PID pid_roll_rate   = { .kp = 7.0f, .ki = 0.0f, .kd = 0.0f, .integral_limit = 200.0f };
PID pid_pitch_rate  = { .kp = 7.0f, .ki = 0.0f, .kd = 0.0f, .integral_limit = 200.0f };
PID pid_yaw_rate    = { .kp = 2.0f, .ki = 0.0f, .kd = 0.0f, .integral_limit = 200.0f };
PID pid_roll_angle  = { .kp = 2.0f, .ki = 0.0f, .kd = 0.0f, .integral_limit = 20.0f  };
PID pid_pitch_angle = { .kp = 2.0f, .ki = 0.0f, .kd = 0.0f, .integral_limit = 20.0f  };

 void main() {
    SysTick_Init();
#ifdef UART_REDIRECT
    USART_Init(&usart2);
#endif
    TIM1_Init(20000);
    TIM1_Config_PWM(TIM_CHANNEL_1, &ccw_pin_1);
    TIM1_Config_PWM(TIM_CHANNEL_2, &cw_pin_1);
    TIM1_Config_PWM(TIM_CHANNEL_3, &ccw_pin_2);
    TIM1_Config_PWM(TIM_CHANNEL_4, &cw_pin_2);
    TIM1_Set_Duty_Cycle(TIM_CHANNEL_1, 0);
    TIM1_Set_Duty_Cycle(TIM_CHANNEL_2, 0);
    TIM1_Set_Duty_Cycle(TIM_CHANNEL_3, 0);
    TIM1_Set_Duty_Cycle(TIM_CHANNEL_4, 0);
//    while(1);
    LSM6DS3_Init(&lsm6ds3);
    uint8_t who = LSM6DS3_Read_Reg(&lsm6ds3, 0x0F);
    printf("WHOAMI: %x\n", who);
    if (who != 0x69) {
        printf("LSM6DS3 init failed, halting\n");
        while (1);
    }

    float gyro_bias_x = 0, gyro_bias_y = 0, gyro_bias_z = 0;
    for (int i = 0; i < 2000; i++) {
        int16_t gx, gy, gz;
        LSM6DS3_ReadGyro(&lsm6ds3, &gx, &gy, &gz);
        gyro_bias_x += gx;
        gyro_bias_y += gy;
        gyro_bias_z += gz;
    }
    gyro_bias_x /= 2000.0f;
    gyro_bias_y /= 2000.0f;
    gyro_bias_z /= 2000.0f;
    printf("Gyro bias: %.2f %.2f %.2f\n", gyro_bias_x, gyro_bias_y, gyro_bias_z);

    float roll_angle  = 0.0f;
    float pitch_angle = 0.0f;

    {
        int16_t ax, ay, az, gx, gy, gz;

        LSM6DS3_ReadAccel(&lsm6ds3, &ax, &ay, &az);
        roll_angle  = atan2f(ay, az) * 180.0f / M_PI;
        pitch_angle = atan2f(-ax, az) * 180.0f / M_PI;

        for (int i = 0; i < 1000; i++) {
            LSM6DS3_ReadAccel(&lsm6ds3, &ax, &ay, &az);
            LSM6DS3_ReadGyro(&lsm6ds3, &gx, &gy, &gz);

            float gx_dps = (gx - gyro_bias_x) * GYRO_SENSITIVITY;
            float gy_dps = (gy - gyro_bias_y) * GYRO_SENSITIVITY;

            float accel_roll  = atan2f(ay, az) * 180.0f / M_PI;
            float accel_pitch = atan2f(-ax, az) * 180.0f / M_PI;

            roll_angle  = 0.8f * (roll_angle  + gx_dps * 0.0025f) + 0.2f * accel_roll;
            pitch_angle = 0.8f * (pitch_angle + gy_dps * 0.0025f) + 0.2f * accel_pitch;

            SysTick_Delay_Microseconds(2500);
        }
        printf("Initial angle: R:%.2f P:%.2f\n", roll_angle, pitch_angle);
    }

    float roll_offset  = roll_angle;
    float pitch_offset = pitch_angle;
    printf("Offsets: R:%.2f P:%.2f\n", roll_offset, pitch_offset);

    uint32_t throttle = THROTTLE_HOVER;

    while (1) {
        int16_t ax, ay, az, gx, gy, gz;
        LSM6DS3_ReadAccel(&lsm6ds3, &ax, &ay, &az);
        LSM6DS3_ReadGyro(&lsm6ds3, &gx, &gy, &gz);

        float gx_dps = (gx - gyro_bias_x) * GYRO_SENSITIVITY;
        float gy_dps = (gy - gyro_bias_y) * GYRO_SENSITIVITY;
        float gz_dps = (gz - gyro_bias_z) * GYRO_SENSITIVITY;

        float accel_roll  = atan2f(ay, az) * 180.0f / M_PI;
        float accel_pitch = atan2f(-ax, az) * 180.0f / M_PI;

        roll_angle  = COMP_FILTER_ALPHA * (roll_angle  + gx_dps * 0.0025f) + (1.0f - COMP_FILTER_ALPHA) * accel_roll;
        pitch_angle = COMP_FILTER_ALPHA * (pitch_angle + gy_dps * 0.0025f) + (1.0f - COMP_FILTER_ALPHA) * accel_pitch;

        float roll_corrected  = roll_angle  - roll_offset;
        float pitch_corrected = pitch_angle - pitch_offset;

        float roll_rate_sp  = PID_Update(&pid_roll_angle,  0.0f, roll_corrected,  0.0025f);
        float pitch_rate_sp = PID_Update(&pid_pitch_angle, 0.0f, pitch_corrected, 0.0025f);

        float roll_out  = PID_Update(&pid_roll_rate,  roll_rate_sp,  gx_dps, 0.0025f);
        float pitch_out = PID_Update(&pid_pitch_rate, pitch_rate_sp, gy_dps, 0.0025f);
        float yaw_out   = PID_Update(&pid_yaw_rate,   0.0f,          gz_dps, 0.0025f);

        int32_t m1 = throttle + (int32_t)( roll_out - pitch_out - yaw_out);
        int32_t m2 = throttle + (int32_t)(-roll_out - pitch_out + yaw_out);
        int32_t m3 = throttle + (int32_t)(-roll_out + pitch_out - yaw_out);
        int32_t m4 = throttle + (int32_t)( roll_out + pitch_out + yaw_out);

        TIM1_Set_Duty_Cycle(TIM_CHANNEL_1, CLAMP(m1, 0, MAX_DUTY));
        TIM1_Set_Duty_Cycle(TIM_CHANNEL_2, CLAMP(m2, 0, MAX_DUTY));
        TIM1_Set_Duty_Cycle(TIM_CHANNEL_3, CLAMP(m3, 0, MAX_DUTY));
        TIM1_Set_Duty_Cycle(TIM_CHANNEL_4, CLAMP(m4, 0, MAX_DUTY));

        printf("R:%.1f P:%.1f | rsp:%.2f psp:%.2f | M:%d %d %d %d\n",
            roll_corrected, pitch_corrected, roll_rate_sp, pitch_rate_sp,
            m1, m2, m3, m4);

        SysTick_Delay_Microseconds(2500);
    }
}