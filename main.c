#include "config.h"
#ifdef DRONE_CODE
#include "hardware.h"
#include "systick.h"
#include "timer.h"
#include "pid.h"
#include <stdio.h>
#include <math.h>
#include "esc.h"
#include "drone_constants.h"
#include "utils.h"

PID pid_roll_rate   = { .kp = 1.1f, .ki = 0.0f, .kd = 0.0f, .integral_limit = 200.0f };
PID pid_pitch_rate  = { .kp = 1.1f, .ki = 0.0f, .kd = 0.0f, .integral_limit = 200.0f };
PID pid_yaw_rate    = { .kp = 1.5f, .ki = 0.1f, .kd = 0.0f,   .integral_limit = 200.0f };
PID pid_roll_angle  = { .kp = 4.2f, .ki = 0.0f, .kd = 0.0f,   .integral_limit = 20.0f  };
PID pid_pitch_angle = { .kp = 4.2f, .ki = 0.0f, .kd = 0.0f,   .integral_limit = 20.0f  };
PID pid_altitude = {    .kp = 0.08f, .ki = 0.0f, .kd = 0.09f,   .integral_limit = 100.0f  };

float target_altitude_mm = 50.0f;
float throttle_base = THROTTLE_TAKEOFF;
uint16_t distance_mm = 0;
uint8_t range_status = 255;
float alt_correction = 0.0f;
bool alt_hold_active = false;
float hover_throttle = THROTTLE_TAKEOFF;
static float alt_correction_filtered = 0.0f;

static volatile float throttle = ESC_MIN;
static volatile float target_roll = 0.f;
static volatile float target_pitch = 0.f;
static volatile float target_yaw = 0.f;


static volatile bool drone_on = false;
static volatile uint8_t on_packets = 0;
static volatile uint32_t last_packet_received = 0;

// State machine
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
    
    last_packet_received = SysTick_Get_Microseconds();
    
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
    if (1000 < adc_yaw_value && adc_yaw_value < 3000) {
        adc_yaw_value = 2048;
    }

    target_roll = (2048.f - adc_x_value) * MAX_TILT_DEGREES / 2048.f;
    target_pitch = (adc_y_value - 2048.f) * MAX_TILT_DEGREES / 2048.f;
    target_yaw = (adc_yaw_value - 2048.f) * MAX_YAW_DEGREES / 2048.f;
    
    if (adc_z_value < 2000) {
        target_altitude_mm -= THROTTLE_MM_SPEED;
        target_altitude_mm = CLAMP(target_altitude_mm, 0.0f, 1200.0f);
    }
    else if (adc_z_value > 2200) {
        target_altitude_mm += THROTTLE_MM_SPEED;
        target_altitude_mm = CLAMP(target_altitude_mm, 0.0f, 1200.0f);
    }
    if ((bool)start_value != drone_on) {
        on_packets++;
    }
    else {
        on_packets = 0;
    }
    if (on_packets >= ON_PACKET_COUNT) {
        drone_on = !drone_on;
    }
    NRF_Clear_IRQ(&nrf24l01);
}

 void main() {
    Drone_Hardware_Init();

    SysTick_Delay_Milliseconds(2000);
//    test_calibration();

    
    esc_calibration();
    arm_esc();

    Pin_Set_Low(&led_pin); // turn on blue led to indicate sensor calibration start
    
    float gyro_bias_x = 0, gyro_bias_y = 0, gyro_bias_z = 0;
    float roll_offset, pitch_offset;
    calibrate(&roll_offset, &pitch_offset, &gyro_bias_x, &gyro_bias_y, &gyro_bias_z);    
    Pin_Set_High(&led_pin);
    roll_offset = 1.15f;
    pitch_offset = 2.68f;
    float roll_angle  = roll_offset;
    float pitch_angle = pitch_offset;

    printf("Offsets: R:%.2f P:%.2f\n", roll_offset, pitch_offset);
    
    NRF_Flush_RX(&nrf24l01);
    NRF_Set_RX_Mode(&nrf24l01);

    const float cos_a = cosf(IMU_ALIGN_ANGLE_DEG * M_PI / 180.0f);
    const float sin_a = sinf(IMU_ALIGN_ANGLE_DEG * M_PI / 180.0f);
    
    uint32_t last_distance_time = SysTick_Get_Microseconds();

    while (1) {
        uint32_t loop_start = SysTick_Get_Microseconds();
        int16_t ax, ay, az, gx, gy, gz;
        LSM6DS3_ReadAccel(&lsm6ds3, &ax, &ay, &az);
        LSM6DS3_ReadGyro(&lsm6ds3, &gx, &gy, &gz);
        uint8_t data_ready = 0;
        VL53L1X_CheckForDataReady(VL53L1X_ADDR, &data_ready);
        if (data_ready) {
            VL53L1X_GetRangeStatus(VL53L1X_ADDR, &range_status);
            VL53L1X_GetDistance(VL53L1X_ADDR, &distance_mm);
//            printf("Distance: %d\n", distance_mm);
            VL53L1X_ClearInterrupt(VL53L1X_ADDR);
            
            if (range_status == 0) {
                alt_correction = PID_Update(&pid_altitude,
                    target_altitude_mm,
                    (float)distance_mm,
                    (SysTick_Get_Microseconds() - last_distance_time) / 1000000.f);
                alt_correction_filtered = ALT_CORRECTION_LPF * alt_correction + 
                                          (1.0f - ALT_CORRECTION_LPF) * alt_correction_filtered;

                last_distance_time = SysTick_Get_Microseconds();
                alt_hold_active = true;
            } else {
                alt_hold_active = false;
            }
        }

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
        
        float roll_rate_sp  = PID_Update(&pid_roll_angle, target_roll + TRIM_ROLL,  roll_aligned, DT);
        float pitch_rate_sp = PID_Update(&pid_pitch_angle, target_pitch + TRIM_PITCH, pitch_aligned, DT);

        float roll_out  = PID_Update(&pid_roll_rate,  roll_rate_sp,  gx_aligned, DT);
        float pitch_out = PID_Update(&pid_pitch_rate, pitch_rate_sp, gy_aligned, DT);
        float yaw_out   = PID_Update(&pid_yaw_rate,   target_yaw,          gz_dps,     DT);
        
        switch (current_state) {
            case OFF: {
                if (drone_on) {
                    Pin_Set_Low(&led_pin);
                    calibrate(&roll_offset, &pitch_offset, &gyro_bias_x, &gyro_bias_y, &gyro_bias_z);
                    Pin_Set_High(&led_pin);
                    current_state = TAKEOFF;
                    throttle = ESC_MIN;
                }
                break;
            }
            case TAKEOFF: {
                throttle += THROTTLE_DELTA_RATE;
                throttle = CLAMP(throttle, ESC_MIN, ESC_MAX);
                
                if ((alt_hold_active && distance_mm > MIN_HOVER)) {
                    current_state = FLIGHT;
                    target_altitude_mm = distance_mm;
                    hover_throttle = throttle - 20.f;
                    printf("Takeoff complete, hover throttle: %.0f, alt: %dmm\n", 
                        hover_throttle, distance_mm);
                }
                
                
                if (throttle >= ESC_MAX - 10) {
                    printf("Takeoff failed � max throttle reached\n");
                    current_state = LANDING;
                    drone_on = false;
                }
                if (!drone_on) {
                    current_state = LANDING;
                }
                break;
            }
            case FLIGHT: {
                if (!drone_on || loop_start > last_packet_received + NO_RX_TIMEOUT) {
                    printf("%d, %lu, %lu, %lu\n", drone_on, loop_start, 
                        last_packet_received, last_packet_received + NO_RX_TIMEOUT);
                    current_state = LANDING;
                    drone_on = false;
                    break;
                }
                if (alt_hold_active) {
                    float alt_error = target_altitude_mm - (float)distance_mm;
                    static float stable_time = 0.0f;

                    if (fabsf(alt_error) < ALT_DEADBAND_MM) {
                        stable_time += DT;
                        if (stable_time > 1.0f) {
                            hover_throttle = hover_throttle * (1.0f - HOVER_LEARN_RATE)
                                           + (throttle - alt_correction_filtered) * HOVER_LEARN_RATE;
                        }
                        alt_correction_filtered *= 0.95f;
                    } else {
                        stable_time = 0.0f;
                    }

                    throttle = hover_throttle + alt_correction_filtered;
                    throttle = CLAMP(throttle, (float)ESC_MIN, (float)ESC_MAX);
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
        
        float roll_pitch_sum = fabsf(roll_out) + fabsf(pitch_out) + fabsf(yaw_out);
        float scale = 1.0f;

        if (roll_pitch_sum > (ESC_MAX - (int32_t)throttle)) {
            scale = (ESC_MAX - (int32_t)throttle) / roll_pitch_sum;
        }

        // M1 = front-right CCW, M2 = front-left CW
        // M3 = rear-left CCW,   M4 = rear-right CW
        int32_t m1 = (int32_t)throttle + (int32_t)(scale * ( roll_out + pitch_out - yaw_out));
        int32_t m2 = (int32_t)throttle + (int32_t)(scale * (-roll_out + pitch_out + yaw_out));
        int32_t m3 = (int32_t)throttle + (int32_t)(scale * (-roll_out - pitch_out - yaw_out));
        int32_t m4 = (int32_t)throttle + (int32_t)(scale * ( roll_out - pitch_out + yaw_out));

        
        
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
            printf("%f %f %f %f %f %f %d\n", target_roll, target_pitch, target_yaw, throttle, target_altitude_mm,  alt_correction, distance_mm);
            printf("R:%.1f P:%.1f | M:%d %d %d %d | G: %f %f %f\n",
                roll_aligned, pitch_aligned, m1, m2, m3, m4, gx_dps, gy_dps, gz_dps);
            print_counter = 0;
        }
        while (SysTick_Get_Microseconds() - loop_start < LOOP_US);
    }
}

#else 

#include "controller-hardware.h"
#include "systick.h"
#include <stdio.h>

static volatile uint32_t last_packet_received = 0;
#define NO_RX_TIMEOUT 1000000
#define TX_TIMEOUT 10000

void main() {
    Controller_Hardware_Init();

    static bool drone_on = false;
    static bool last_start_value = false;
    while (1) {
        uint32_t loop_start = SysTick_Get_Microseconds();
        if (loop_start > last_packet_received + NO_RX_TIMEOUT) {
            drone_on = false;
        }
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
        if (success) {
            last_packet_received = SysTick_Get_Microseconds();
        }
        while (SysTick_Get_Microseconds() - loop_start < TX_TIMEOUT);
    }
}

#endif