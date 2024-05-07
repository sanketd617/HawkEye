#include <MPU6050.h>
#include <Wire.h>
#include "hawk/sensors/motion/motion.h"
#include "utils/utils.h"
#include "definitions/definitions.h"

MotionSensor::MotionSensor() {
    pitch = DEFAULT_PITCH;
    roll = DEFAULT_ROLL;
}

void MotionSensor::initialize() {
    delay(10);
    sensor.initialize();
    delay(10);

    sensor.setFullScaleAccelRange(MPU6050_GYRO_FS_500);
    sensor.setFullScaleGyroRange(MPU6050_ACCEL_FS_8);
}

void MotionSensor::calibrate() {    
    sensor.CalibrateAccel();
    sensor.CalibrateGyro();
}

void MotionSensor::sense() {
    blink(100, 100);

    int16_t ax, ay, az;
    sensor.getAcceleration(&ax, &ay, &az);
    
    // Convert raw accelerometer data to g-force; 16384 LSB per g
    double accelerationX = ax / LSB_PER_G;
    double accelerationY = ay / LSB_PER_G;
    double accelerationZ = az / LSB_PER_G;
    
    // Calculate pitch and roll angles in degrees
    pitch = atan2(-accelerationX, sqrt(accelerationY * accelerationY + accelerationZ * accelerationZ)) * RADIANS_TO_DEGREES_RATIO;
    roll = atan2(accelerationY, accelerationZ) * RADIANS_TO_DEGREES_RATIO;
}

double MotionSensor::getRoll() {
    return roll;
}

double MotionSensor::getPitch() {
    return pitch;
}
