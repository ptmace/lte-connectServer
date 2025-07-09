#include "main.h"

MPU6050 mpu; // Create an instance of the MPU6050 class

void setup() {
  // Set up serial communication at 115200 baud rate
  Serial.begin(115200);

  // Check if the serial port is available
  if (!Serial) {
    // If the serial port is not available, print an error message
    Serial.println("Serial port not available!");
    return; // Exit setup if serial is not available
  }
  else{
    // If the serial port is available, print a success message
    Serial.println("Serial port initialized successfully!");
  }

  // Initialize the MPU6050 sensor
  Wire.begin(SDA_PIN, SCL_PIN); // Start I2C communication
  mpu.initialize(); // Initialize the MPU6050 sensor
  if (!mpu.testConnection()) {
    // If the MPU6050 connection gitfails, print an error message
    Serial.println("MPU6050 connection failed!");
  } else {
    // If the MPU6050 connection is successful, print a success message
    Serial.println("MPU6050 connection successful!");
  }
}

void loop() {
  // Serial communication is handled in the serialEvent function
  handleSerial();
}