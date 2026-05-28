#include "config.h"
#ifdef DRONE_CODE
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
#define ESC_MIN             1000
#define ESC_MAX             2000
#define THROTTLE_HOVER      1420
#define THROTTLE_TAKEOFF    1140
#define MAX_THROTTLE_DELTA  200
#define LOOP_HZ             400
#define MAX_TILT_DEGREES    5
#define LOOP_US             (1000000 / LOOP_HZ)
#define DT                  (1.0f / LOOP_HZ)
#define IMU_ALIGN_ANGLE_DEG  (-4.2068f)
#define CLAMP(x, lo, hi)    ((x) < (lo) ? (lo) : (x) > (hi) ? (hi) : (x))

PID pid_roll_rate   = { .kp = 2.2f, .ki = 0.0f, .kd = 0.0001f, .integral_limit = 200.0f };
PID pid_pitch_rate  = { .kp = 2.2f, .ki = 0.0f, .kd = 0.0001f, .integral_limit = 200.0f };
PID pid_yaw_rate    = { .kp = 0.5f, .ki = 0.0f, .kd = 0.0f,   .integral_limit = 200.0f };
PID pid_roll_angle  = { .kp = 4.0f, .ki = 0.0f, .kd = 0.0f,   .integral_limit = 20.0f  };
PID pid_pitch_angle = { .kp = 4.0f, .ki = 0.0f, .kd = 0.0f,   .integral_limit = 20.0f  };

static volatile float throttle = ESC_MIN;
static volatile float target_roll = 0.f;
static volatile float target_pitch = 0.f;
static volatile float target_yaw = 0.f;
static volatile bool drone_on = false;
static volatile uint8_t on_packets = 0;
#define ON_PACKET_COUNT 10
#define THROTTLE_DELTA_RATE 0.5f
#define THROTTLE_DOWN_DELTA_RATE 1.5f
typedef enum {
    OFF,
    TAKEOFF,
    FLIGHT,
    LANDING  
} State;

static State current_state = OFF;

void EXTI2_IRQHandler(void){
    uint16_t rx_data[NRF_MAX_PACKET_SIZE / 2];
    NRF_Receive_Nonblocking(&nrf24l01);
    NRF_Read_Pipe(&nrf24l01, NRF_PIPE1, (uint8_t *)rx_data);
    
    uint16_t adc_x_value = rx_data[0];
    uint16_t adc_y_value = rx_data[1];
    uint16_t adc_z_value = rx_data[2];
    uint16_t adc_yaw_value = rx_data[3];
    uint16_t start_value = rx_data[4];

//    printf("ADC: %d %d %d %d, Start: %d\n", adc_x_value, adc_y_value, adc_z_value, adc_yaw_value, start_value);
    // deadzones
    if (2050 < adc_x_value && adc_x_value < 2150) {
        adc_x_value = 2048;
    }
    if (2000 < adc_y_value && adc_y_value < 2100) {
        adc_y_value = 2048;
    }
    if (2000 < adc_yaw_value && adc_yaw_value < 2100) {
        adc_yaw_value = 2048;
    }
    target_roll = (2048.f - adc_x_value) * MAX_TILT_DEGREES / 2048.f;
    target_pitch = (2048.f - adc_y_value) * MAX_TILT_DEGREES / 2048.f;
//    target_yaw = (adc_yaw_value - 2048.f) * MAX_TILT_DEGREES / 2048.f;
    target_yaw = 0.f;
    
    if (adc_z_value < 2000) {
        throttle -= THROTTLE_DELTA_RATE;
    }
    else if (adc_z_value > 2200) {
        throttle += THROTTLE_DELTA_RATE;
    }
    
    if (start_value != drone_on) {
        on_packets++;
    }
    else {
        on_packets = 0;
    }
    if (on_packets == ON_PACKET_COUNT) {
        drone_on = !drone_on;
    }
    NRF_Clear_IRQ(&nrf24l01);
}

void main() {
    SysTick_Init();
    SysTick_Delay_Milliseconds(100);

#ifdef UART_REDIRECT 
    USART_Init(&usart2);
#endif
    
    TIM1_Init(1000000);
    TIM1_Config_PWM(TIM_CHANNEL_1, &ccw_pin_1);
    TIM1_Config_PWM(TIM_CHANNEL_2, &cw_pin_1);
    TIM1_Config_PWM(TIM_CHANNEL_3, &ccw_pin_2);
    TIM1_Config_PWM(TIM_CHANNEL_4, &cw_pin_2);
    
//    esc_calibration();
    
    // ESC arming
    TIM1_Set_Duty_Cycle(TIM_CHANNEL_1, ESC_MIN);
    TIM1_Set_Duty_Cycle(TIM_CHANNEL_2, ESC_MIN);
    TIM1_Set_Duty_Cycle(TIM_CHANNEL_3, ESC_MIN);
    TIM1_Set_Duty_Cycle(TIM_CHANNEL_4, ESC_MIN);
    SysTick_Delay_Milliseconds(5000);
    printf("ESCs armed\n");
    Pin_Config(&led_pin, PIN_MODE_OUTPUT, PIN_OT_PUSH_PULL, PIN_SPEED_LOW, PIN_PULL_NONE);
    Pin_Set_Low(&led_pin);
    
    LSM6DS3_Init(&lsm6ds3);
    uint8_t who = LSM6DS3_Read_Reg(&lsm6ds3, 0x0F);
    printf("WHOAMI: %02X\n", who);
    if (who != 0x69) {
        printf("LSM6DS3 init failed, halting\n");
        while (1);
    }
    
//    test_calibration();
    // Gyro bias calibration
    float gyro_bias_x = 0, gyro_bias_y = 0, gyro_bias_z = 0;
    for (int i = 0; i < 5000; i++) {
        int16_t gx, gy, gz;
        LSM6DS3_ReadGyro(&lsm6ds3, &gx, &gy, &gz);
//        printf("Raw gyro: %d %d %d\n", gx, gy, gz);
        gyro_bias_x += gx;
        gyro_bias_y += gy;
        gyro_bias_z += gz;
    }
    gyro_bias_x /= 5000.0f;
    gyro_bias_y /= 5000.0f;
    gyro_bias_z /= 5000.0f;
    printf("Gyro bias: %.2f %.2f %.2f\n", gyro_bias_x, gyro_bias_y, gyro_bias_z);

    // Initial angle calibration
    float roll_angle  = 0.0f;
    float pitch_angle = 0.0f;

    {
        int16_t ax, ay, az, gx, gy, gz;

        LSM6DS3_ReadAccel(&lsm6ds3, &ax, &ay, &az);
        roll_angle  = atan2f(ay, az) * 180.0f / M_PI;
        pitch_angle = atan2f(-ax, az) * 180.0f / M_PI;

        for (int i = 0; i < 125; i++) {
            LSM6DS3_ReadAccel(&lsm6ds3, &ax, &ay, &az);
            LSM6DS3_ReadGyro(&lsm6ds3, &gx, &gy, &gz);

            float gx_dps = (gx - gyro_bias_x) * GYRO_SENSITIVITY;
            float gy_dps = (gy - gyro_bias_y) * GYRO_SENSITIVITY;

            float accel_roll  = atan2f(ay, az) * 180.0f / M_PI;
            float accel_pitch = atan2f(-ax, az) * 180.0f / M_PI;

            roll_angle  = 0.8f * (roll_angle  + gx_dps * DT) + 0.2f * accel_roll;
            pitch_angle = 0.8f * (pitch_angle + gy_dps * DT) + 0.2f * accel_pitch;

            SysTick_Delay_Microseconds(LOOP_US);
        }
        printf("Initial angle: R:%.2f P:%.2f\n", roll_angle, pitch_angle);
    }

    float roll_offset  = roll_angle;
    float pitch_offset = pitch_angle;
    printf("Offsets: R:%.2f P:%.2f\n", roll_offset, pitch_offset);

    NRF_Config(&nrf24l01);
    NRF_Config_RX_IRQ(&nrf24l01);
    uint8_t pipe1_addr[5] = {0xE7, 0xE7, 0xE7, 0x00, 0x00};
    NRF_Setup_Pipe(&nrf24l01, NRF_PIPE1, pipe1_addr);
    
    NRF_Flush_RX(&nrf24l01);
    NRF_Set_RX_Mode(&nrf24l01);

    const float cos_a = cosf(IMU_ALIGN_ANGLE_DEG * M_PI / 180.0f);
    const float sin_a = sinf(IMU_ALIGN_ANGLE_DEG * M_PI / 180.0f);

    while (1) {
        uint32_t loop_start = SysTick_Get_Microseconds();
        printf("%f %f %f %f\n", target_roll, target_pitch, target_yaw, throttle);
        int16_t ax, ay, az, gx, gy, gz;
        LSM6DS3_ReadAccel(&lsm6ds3, &ax, &ay, &az);
        LSM6DS3_ReadGyro(&lsm6ds3, &gx, &gy, &gz);

        float gx_dps = (gx - gyro_bias_x) * GYRO_SENSITIVITY;
        float gy_dps = (gy - gyro_bias_y) * GYRO_SENSITIVITY;
        float gz_dps = (gz - gyro_bias_z) * GYRO_SENSITIVITY;

        float accel_roll  = atan2f(ay, az) * 180.0f / M_PI;
        float accel_pitch = atan2f(-ax, az) * 180.0f / M_PI;

        roll_angle  = COMP_FILTER_ALPHA * (roll_angle  + gx_dps * DT) + (1.0f - COMP_FILTER_ALPHA) * accel_roll;
        pitch_angle = COMP_FILTER_ALPHA * (pitch_angle + gy_dps * DT) + (1.0f - COMP_FILTER_ALPHA) * accel_pitch;

        float roll_corrected  = roll_angle  - roll_offset;
        float pitch_corrected = pitch_angle - pitch_offset;

        float roll_aligned  = roll_corrected * cos_a - pitch_corrected * sin_a;
        float pitch_aligned = roll_corrected * sin_a + pitch_corrected * cos_a;

        float gx_aligned = gx_dps * cos_a - gy_dps * sin_a;
        float gy_aligned = gx_dps * sin_a + gy_dps * cos_a;
        
        
        float roll_rate_sp  = PID_Update(&pid_roll_angle,  target_roll, roll_aligned,  DT);
        float pitch_rate_sp = PID_Update(&pid_pitch_angle, target_pitch, pitch_aligned, DT);

        float roll_out  = PID_Update(&pid_roll_rate,  roll_rate_sp,  gx_aligned, DT);
        float pitch_out = PID_Update(&pid_pitch_rate, pitch_rate_sp, gy_aligned, DT);
        float yaw_out   = PID_Update(&pid_yaw_rate,   target_yaw,          gz_dps,     DT);
        
        
        
        switch (current_state) {
            case OFF: {
                if (drone_on) {
                    current_state = TAKEOFF;
                    throttle = ESC_MIN;
                }
                break;
            }
            case TAKEOFF: {
                throttle += THROTTLE_DELTA_RATE;
                if (throttle >= THROTTLE_TAKEOFF) {
                    current_state = FLIGHT;
                    throttle = THROTTLE_TAKEOFF;
                }
                if (!drone_on) {
                    current_state = LANDING;
                }
                break;
            }
            case FLIGHT: {
                if (!drone_on) {
                    current_state = LANDING;
                }
                break;
            }
            case LANDING: {
                throttle -= THROTTLE_DOWN_DELTA_RATE;
                if (throttle <= ESC_MIN) {
                    current_state = OFF;
                }
                break;
            }
        }
        // M1 = front-right CCW, M2 = front-left CW
        // M3 = rear-left CCW,   M4 = rear-right CW
        int32_t m1 = (int32_t)throttle + (int32_t)( roll_out + pitch_out - yaw_out);
        int32_t m2 = (int32_t)throttle + (int32_t)(-roll_out + pitch_out + yaw_out);
        int32_t m3 = (int32_t)throttle + (int32_t)(-roll_out - pitch_out - yaw_out);
        int32_t m4 = (int32_t)throttle + (int32_t)( roll_out - pitch_out + yaw_out);
        
        if (current_state == OFF) {
            TIM1_Set_Duty_Cycle(TIM_CHANNEL_1, ESC_MIN);
            TIM1_Set_Duty_Cycle(TIM_CHANNEL_2, ESC_MIN);
            TIM1_Set_Duty_Cycle(TIM_CHANNEL_3, ESC_MIN);
            TIM1_Set_Duty_Cycle(TIM_CHANNEL_4, ESC_MIN);
        }
        else {
            TIM1_Set_Duty_Cycle(TIM_CHANNEL_1, CLAMP(m1, ESC_MIN, ESC_MAX));
            TIM1_Set_Duty_Cycle(TIM_CHANNEL_2, CLAMP(m2, ESC_MIN, ESC_MAX));
            TIM1_Set_Duty_Cycle(TIM_CHANNEL_3, CLAMP(m3, ESC_MIN, ESC_MAX));
            TIM1_Set_Duty_Cycle(TIM_CHANNEL_4, CLAMP(m4, ESC_MIN, ESC_MAX));
        }
        static uint32_t print_counter = 0;
        if (++print_counter >= 100) {
            printf("R:%.1f P:%.1f | M:%d %d %d %d\n",
                roll_aligned, pitch_aligned, m1, m2, m3, m4);
            print_counter = 0;
        }

        while (SysTick_Get_Microseconds() - loop_start < LOOP_US);
    }
}

#else 

#include "controller-hardware.h"
#include "systick.h"
#include "timer.h"
#include <stdio.h>

void main() {
    SysTick_Init();
    SysTick_Delay_Milliseconds(1000); // startup delay
#ifdef UART_REDIRECT
    USART_Init(&usart2);
#endif
    NRF_Config(&nrf24l01);
    uint8_t pipe1_addr[5] = {0xE7, 0xE7, 0xE7, 0x00, 0x00};
    NRF_Setup_Pipe(&nrf24l01, NRF_PIPE1, pipe1_addr);
    printf("NRF Config'd");
    ADC_Init();
    ADC_Config_Pin(&adc_x);
    ADC_Config_Pin(&adc_y);
    ADC_Config_Pin(&adc_z);
    ADC_Config_Pin(&adc_yaw);
    Pin_Config(&start_pin, PIN_MODE_INPUT, PIN_OT_PUSH_PULL, PIN_SPEED_LOW, PIN_PULL_UP);
    Pin_Config(&red_pin, PIN_MODE_OUTPUT, PIN_OT_PUSH_PULL, PIN_SPEED_LOW, PIN_PULL_NONE);
    Pin_Config(&green_pin, PIN_MODE_OUTPUT, PIN_OT_PUSH_PULL, PIN_SPEED_LOW, PIN_PULL_NONE);
    Pin_Config(&blue_pin, PIN_MODE_OUTPUT, PIN_OT_PUSH_PULL, PIN_SPEED_LOW, PIN_PULL_NONE);
    
    static bool drone_on = false;
    static bool last_start_value = false;
    while (1) {
        uint32_t loop_start = SysTick_Get_Microseconds();
        uint16_t adc_x_value = ADC_Read_Pin(&adc_x);
        uint16_t adc_y_value = ADC_Read_Pin(&adc_y);
        uint16_t adc_z_value = ADC_Read_Pin(&adc_z);
        uint16_t adc_yaw_value = ADC_Read_Pin(&adc_yaw);
        bool start_value = !Pin_Read(&start_pin);
        if (start_value && !last_start_value) {
            SysTick_Delay_Milliseconds(50);
            if (start_value && !last_start_value) {
                drone_on = !drone_on;
            }
        }
        last_start_value = start_value;
        printf("ADC: %d %d %d %d, Start: %d\n", adc_x_value, adc_y_value, adc_z_value, adc_yaw_value, drone_on);
        Pin_Set(&red_pin, !drone_on);
        Pin_Set(&green_pin, drone_on);
        uint16_t tx_data[NRF_MAX_PACKET_SIZE / 2] = { adc_x_value, adc_y_value, adc_z_value, adc_yaw_value, drone_on};
        bool success = NRF_Send(&nrf24l01, (uint8_t *)tx_data);
        Pin_Set(&blue_pin, success);
        while (SysTick_Get_Microseconds() - loop_start < 10000);
    }
}

#endif