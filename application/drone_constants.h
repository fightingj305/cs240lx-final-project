
// Controls constants
#define GYRO_SENSITIVITY    0.070f // from datasheet for 2000 dps range, in dps/LSB
#define COMP_FILTER_ALPHA   0.98f 
#define IMU_ALIGN_ANGLE_DEG  ( 0.6f)

// PID constants
#define LOOP_HZ             400
#define LOOP_US             (1000000 / LOOP_HZ)
#define DT                  (1.0f / LOOP_HZ)

// Flight constants
#define THROTTLE_TAKEOFF    1140
#define MAX_TILT_DEGREES    4
#define MAX_YAW_DEGREES     180
#define THROTTLE_DELTA_RATE 1.f
#define THROTTLE_DOWN_DELTA_RATE 1.25f

#define ALT_CORRECTION_LPF 0.05f
#define HOVER_LEARN_RATE 0.05f
#define MIN_HOVER 20.0
#define ALT_DEADBAND_MM 60.0f


#define THROTTLE_MM_SPEED 5.f

#define TRIM_ROLL  (0.2f)  // negative makes it go more right
#define TRIM_PITCH (0.2f) // negative makes it go more forwards

// Comms constants
#define ON_PACKET_COUNT 10
#define NO_RX_TIMEOUT 1000000 // 1 second