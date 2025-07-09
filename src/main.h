#ifndef MAIN_H
#define MAIN_H

#include "Arduino.h"
#include "serial.h"
#include "sw_timer.h"
#include <MPU6050.h>
#include <Wire.h>

#define SCL_PIN 25
#define SDA_PIN 26

extern MPU6050 mpu; // Create an instance of the MPU6050 class

#endif