typedef struct {
    float kp, ki, kd;
    float integral;
    float prev_error;
    float integral_limit;
} PID;

float PID_Update(PID *pid, float setpoint, float measured, float dt) {
    float error = setpoint - measured;
    pid->integral += error * dt;
    if (pid->integral >  pid->integral_limit) pid->integral =  pid->integral_limit;
    if (pid->integral < -pid->integral_limit) pid->integral = -pid->integral_limit;
    float derivative = (error - pid->prev_error) / dt;
    pid->prev_error = error;
    return pid->kp * error + pid->ki * pid->integral + pid->kd * derivative;
}

#include <stdio.h>
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif
void test_calibration() {
  
    // Level baseline
    printf("Place level, hold still...\n");
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

    // Tilted forward baseline
    printf("Tilt forward, hold still...\n");
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

    // The misalignment angle is how much roll appears when pitching forward
    float delta_roll  = roll_fwd  - roll_level;
    float delta_pitch = pitch_fwd - pitch_level;

    // Alignment angle = atan2 of cross-coupling
    float align_angle = atan2f(delta_roll, -delta_pitch) * 180.0f / M_PI;
    printf("Align angle: %.4f degrees\n", align_angle);;
    printf("Delta R:%.2f P:%.2f\n", delta_roll, delta_pitch);
    printf("Align angle: %.4f degrees\n", align_angle);

    while (1);
}

#define ESC_MIN             1000
#define ESC_MAX             2000

void esc_calibration(){
    printf("Starting calibration process\n");
    TIM1_Set_Duty_Cycle(TIM_CHANNEL_1, ESC_MAX);
    TIM1_Set_Duty_Cycle(TIM_CHANNEL_2, ESC_MAX);
    TIM1_Set_Duty_Cycle(TIM_CHANNEL_3, ESC_MAX);
    TIM1_Set_Duty_Cycle(TIM_CHANNEL_4, ESC_MAX);
    // ESC config
    SysTick_Delay_Milliseconds(6000);
    printf("Should be on");
    TIM1_Set_Duty_Cycle(TIM_CHANNEL_1, ESC_MIN);
    TIM1_Set_Duty_Cycle(TIM_CHANNEL_2, ESC_MIN);
    TIM1_Set_Duty_Cycle(TIM_CHANNEL_3, ESC_MIN);
    TIM1_Set_Duty_Cycle(TIM_CHANNEL_4, ESC_MIN);
    
    while(1);
}