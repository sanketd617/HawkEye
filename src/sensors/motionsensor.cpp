#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "motionsensor.h"
#include "utils/utils.h"

MotionSensor::MotionSensor() {
    
}

void MotionSensor::calibrate() {
    Serial.println("Calibrating accelerometer and gyro sensor..");

    Wire.begin();
    delay(10);
    sensor.begin();
    delay(10);
    
    sensor.setAccelerometerRange(MPU6050_RANGE_8_G);
    sensor.setGyroRange(MPU6050_RANGE_500_DEG);
    sensor.setFilterBandwidth(MPU6050_BAND_21_HZ);

    float ax = 0, ay = 0, az = 0, gx = 0, gy = 0, gz = 0;
    
    for (int i = 0; i < CALIBRATION_SAMPLE_COUNT; i++) {
        sensors_event_t a, g, temp;
        sensor.getEvent(&a, &g, &temp);
        ax += a.acceleration.x;
        ay += a.acceleration.y;
        az += a.acceleration.z;
        gx += g.gyro.x;
        gy += g.gyro.y;
        gz += g.gyro.z;
        blink(CALIBRATION_INTERVAL, CALIBRATION_INTERVAL);
    }
    
    accelerationOffsetX = ax / (CALIBRATION_SAMPLE_COUNT * CALIBRATION_INTERVAL);
    accelerationOffsetY = ay / (CALIBRATION_SAMPLE_COUNT * CALIBRATION_INTERVAL);
    accelerationOffsetZ = az / (CALIBRATION_SAMPLE_COUNT * CALIBRATION_INTERVAL);

    gyroOffsetX = gx / (CALIBRATION_SAMPLE_COUNT * CALIBRATION_INTERVAL);
    gyroOffsetY = gy / (CALIBRATION_SAMPLE_COUNT * CALIBRATION_INTERVAL);
    gyroOffsetZ = gz / (CALIBRATION_SAMPLE_COUNT * CALIBRATION_INTERVAL);

    float accelerationX = accelerationOffsetX;
    float accelerationY = accelerationOffsetY;
    float accelerationZ = accelerationOffsetZ;
    
    float gyroX = gyroOffsetX * PI / 180.0;
    float gyroY = gyroOffsetY * PI / 180.0;
    float gyroZ = gyroOffsetZ * PI / 180.0;

    float pitchAcceleration = atan2(-accelerationX, sqrt(accelerationY * accelerationY + accelerationZ * accelerationZ)) * 180.0 / PI;
    float rollAcceleration = atan2(accelerationY, accelerationZ) * 180.0 / PI;

    float pitchGyro = pitchAcceleration + gyroX * CALIBRATION_INTERVAL;
    float rollGyro = rollAcceleration + gyroY * CALIBRATION_INTERVAL;

    pitchOffset = CALIBRATION_ALPHA * pitchGyro + (1 - CALIBRATION_ALPHA) * pitchAcceleration;
    rollOffset = CALIBRATION_ALPHA * rollGyro + (1 - CALIBRATION_ALPHA) * rollAcceleration;
    Serial.println("Calibration complete for accelerometer and gyro sensor..");
}

void MotionSensor::sense() {
    blink(100, 100);

    sensors_event_t a, g, temp;
    sensor.getEvent(&a, &g, &temp);

    float deltaTime = (millis() - startTime);
    startTime = millis();

    float accelerationX = a.acceleration.x - accelerationOffsetX;
    float accelerationY = a.acceleration.y - accelerationOffsetY;
    float accelerationZ = a.acceleration.z - accelerationOffsetZ;
    
    float gyroX = (g.gyro.x - gyroOffsetX) * PI / 180.0;
    float gyroY = (g.gyro.y - gyroOffsetY) * PI / 180.0;
    float gyroZ = (g.gyro.z - gyroOffsetZ) * PI / 180.0;

    float pitchAcceleration = atan2(-accelerationX, sqrt(accelerationY * accelerationY + accelerationZ * accelerationZ)) * 180.0 / PI;
    float rollAcceleration = atan2(accelerationY, accelerationZ) * 180.0 / PI;

    float pitchGyro = pitchAcceleration + gyroX * deltaTime;
    float rollGyro = rollAcceleration + gyroY * deltaTime;

    pitch = CALIBRATION_ALPHA * pitchGyro + (1 - CALIBRATION_ALPHA) * pitchAcceleration - pitchOffset;
    roll = CALIBRATION_ALPHA * rollGyro + (1 - CALIBRATION_ALPHA) * rollAcceleration - rollOffset;
}

float MotionSensor::getRoll() {
    return roll;
}

float MotionSensor::getPitch() {
    return pitch;
}
