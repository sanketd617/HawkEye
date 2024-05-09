#include <MPU6050.h>
#include <Wire.h>
#include "hawk/sensors/motion/motion.h"
#include "utils/utils.h"
#include "definitions/definitions.h"

MotionSensor::MotionSensor() {
    initialize();
}

void MotionSensor::initialize() {
    delay(10);
    sensor.initialize();
    delay(10);

    sensor.setFullScaleAccelRange(MPU6050_ACCEL_FS_8);
    sensor.setFullScaleGyroRange(MPU6050_GYRO_FS_500);

    startTime = UNDEFINED;
    previousAccelerationX = UNDEFINED;
    previousAccelerationY = UNDEFINED;
    previousAccelerationZ = UNDEFINED;
    previousGyroscopeX = UNDEFINED;
    previousGyroscopeY = UNDEFINED;
    previousGyroscopeZ = UNDEFINED;
    pitchFromGyroscope = UNDEFINED;
    rollFromGyroscope = UNDEFINED;
    pitchFromAccelerometer = UNDEFINED;
    rollFromAccelerometer = UNDEFINED;
}

void MotionSensor::calibrate() {    
    initialize();
    sensor.CalibrateAccel();
    sensor.CalibrateGyro();
}

void MotionSensor::sense() {
    int16_t rawAccelerationX, rawAccelerationY, rawAccelerationZ, rawRotationRateX, rawRotationRateY, rawRotationRateZ;
    sensor.getAcceleration(&rawAccelerationX, &rawAccelerationY, &rawAccelerationZ);
    sensor.getRotation(&rawRotationRateX, &rawRotationRateY, &rawRotationRateZ);
    
    double accelerationX = lowPassFilter(rawAccelerationX / LSB_PER_G * ACCELERATION_DUE_TO_GRAVITY, previousAccelerationX, SMOOTHENING_FACTOR);
    double accelerationY = lowPassFilter(rawAccelerationY / LSB_PER_G * ACCELERATION_DUE_TO_GRAVITY, previousAccelerationY, SMOOTHENING_FACTOR);
    double accelerationZ = lowPassFilter(rawAccelerationZ / LSB_PER_G * ACCELERATION_DUE_TO_GRAVITY, previousAccelerationZ, SMOOTHENING_FACTOR);

    previousAccelerationX = accelerationX;
    previousAccelerationY = accelerationY;
    previousAccelerationZ = accelerationZ;

    if (pitchFromGyroscope == UNDEFINED) {
        rollFromGyroscope =  atan2(accelerationY, accelerationZ) * RADIANS_TO_DEGREES_RATIO;
        pitchFromGyroscope = atan2(-accelerationX, sqrt(accelerationY * accelerationY + accelerationZ * accelerationZ)) * RADIANS_TO_DEGREES_RATIO;
        startTime = millis();
    } else {
        double deltaTime = (millis() - startTime) / 1000.0;
        startTime = millis();
        rollFromGyroscope = getRoll() + lowPassFilter(deltaTime * rawRotationRateX / LSB_PER_DEGREE_PER_S, previousGyroscopeX, SMOOTHENING_FACTOR);
        pitchFromGyroscope = getPitch() + lowPassFilter(deltaTime * rawRotationRateY / LSB_PER_DEGREE_PER_S, previousGyroscopeY, SMOOTHENING_FACTOR);
        previousGyroscopeX = lowPassFilter(deltaTime * rawRotationRateX / LSB_PER_DEGREE_PER_S, previousGyroscopeX, SMOOTHENING_FACTOR);
        previousGyroscopeY = lowPassFilter(deltaTime * rawRotationRateY / LSB_PER_DEGREE_PER_S, previousGyroscopeY, SMOOTHENING_FACTOR);
        previousGyroscopeZ = lowPassFilter(deltaTime * rawRotationRateZ / LSB_PER_DEGREE_PER_S, previousGyroscopeZ, SMOOTHENING_FACTOR);
    }

    pitchFromAccelerometer = atan2(-accelerationX, sqrt(accelerationY * accelerationY + accelerationZ * accelerationZ)) * RADIANS_TO_DEGREES_RATIO;
    rollFromAccelerometer = atan2(accelerationY, accelerationZ) * RADIANS_TO_DEGREES_RATIO;
}

double MotionSensor::getRoll() {
    return lowPassFilter(rollFromAccelerometer, rollFromGyroscope, 0.02);
}

double MotionSensor::getPitch() {
    return lowPassFilter(pitchFromAccelerometer, pitchFromGyroscope, 0.02);
}
