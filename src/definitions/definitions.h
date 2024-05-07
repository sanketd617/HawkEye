#define NAME "HawkEye"
#define MIN_PULSE 1000
#define MAX_PULSE 2000
#define WING_COUNT 4
#define HAWK_PINS {13, 27, 26, 25}
#define ALL_WINGS -1
#define FRONT_LEFT_WING 3
#define FRONT_RIGHT_WING 0
#define REAR_LEFT_WING 2
#define REAR_RIGHT_WING 1
#define MAX_WING_THROTTLE 100.0
#define LIMITED_MAX_WING_THROTTLE 15.0
#define MIN_WING_THROTTLE 0.0
#define MAX_ROLL_AND_PITCH_VALUE 90.0
#define LIMITED_MAX_ROLL_AND_AND_PITCH_VALUE 30
#define LIMITED_MIN_ROLL_AND_AND_PITCH_VALUE -30
#define LSB_PER_G 4096.0
#define ACCELERATION_DUE_TO_GRAVITY 9.80665
#define ACCELERATION_SMOOTHENING_FACTOR 0.2
#define RADIANS_TO_DEGREES_RATIO 180.0 / PI
#define DEGREES_TO_RADIANS_RATIO PI / 180.0
#define ROLL_AND_PITCH_CALIBRATION_ALPHA 0.5
#define MOTION_SENSOR_CALIBRATION_INTERVAL 10
#define MOTION_SENSOR_CALIBRATION_SAMPLE_COUNT 100
#define DEFAULT_PITCH 0.0
#define DEFAULT_ROLL 0.0
#define DUALSENSE_MAC_ADDRESS "4C:B9:9B:FB:F5:9A"
#define DEFAULT_CALIBRATION_START_TIME -1
#define CALIBRATION_DURATION_IN_SECONDS 5.0
#define MAX_L2_AND_R2_VALUE 255
#define MAX_JOYSTICK_VALUE 127
#define MIN_JOYSTICK_VALUE -128
#define RAMP_UP_DURATION 2000.0
#define DEFAULT_RAMP_UP_START_TIME -1
#define RAMP_UP_STEP_SIZE (double) LIMITED_MAX_WING_THROTTLE / RAMP_UP_DURATION
#define ROLL_AND_PITCH_DURATION 1000.0
#define ROLL_AND_PITCH_STEP_SIZE (double) LIMITED_MAX_ROLL_AND_AND_PITCH_VALUE / ROLL_AND_PITCH_DURATION
#define WING_SWITCH_PIN 23
