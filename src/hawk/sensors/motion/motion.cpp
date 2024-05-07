#include <MPU6050.h>
#include <Wire.h>
#include "hawk/sensors/motion/motion.h"
#include "utils/utils.h"
#include "definitions/definitions.h"

MotionSensor::MotionSensor() {
    pitch = DEFAULT_PITCH;
    roll = DEFAULT_ROLL;
    previousAccelerationX = 0;
    previousAccelerationY = 0;
    previousAccelerationZ = ACCELERATION_DUE_TO_GRAVITY;
}

void MotionSensor::initialize() {
    delay(10);
    sensor.initialize();
    delay(10);

    sensor.setFullScaleAccelRange(MPU6050_ACCEL_FS_8);
    sensor.setFullScaleGyroRange(MPU6050_GYRO_FS_500);
}

void MotionSensor::calibrate() {    
    sensor.CalibrateAccel();
    sensor.CalibrateGyro();
}

void MotionSensor::sense() {
    blink(100, 100);

    int16_t rawAccelerationX, rawAccelerationY, rawAccelerationZ;
    sensor.getAcceleration(&rawAccelerationX, &rawAccelerationY, &rawAccelerationZ);
    
    double accelerationX = lowPassFilter(rawAccelerationX / LSB_PER_G * ACCELERATION_DUE_TO_GRAVITY, previousAccelerationX, ACCELERATION_SMOOTHENING_FACTOR);
    double accelerationY = lowPassFilter(rawAccelerationY / LSB_PER_G * ACCELERATION_DUE_TO_GRAVITY, previousAccelerationY, ACCELERATION_SMOOTHENING_FACTOR);
    double accelerationZ = lowPassFilter(rawAccelerationZ / LSB_PER_G * ACCELERATION_DUE_TO_GRAVITY, previousAccelerationZ, ACCELERATION_SMOOTHENING_FACTOR);

    previousAccelerationX = accelerationX;
    previousAccelerationY = accelerationY;
    previousAccelerationZ = accelerationZ;
    
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
