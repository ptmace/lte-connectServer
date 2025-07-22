// #include "main.h"

// // This function get data from the serial port
// void serialEvent() {
//   if (isTimerExpired(TIMEOUT_SEND)) {
//     int16_t ax, ay, az, gx, gy, gz;
//     mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
//     Serial.print("Accel: ");
//     Serial.print("X: "); Serial.print(ax);  
//     Serial.print(" Y: "); Serial.print(ay);
//     Serial.print(" Z: "); Serial.println(az);
//     Serial.print("Gyro: ");
//     Serial.print("X: "); Serial.print(gx);
//     Serial.print(" Y: "); Serial.print(gy);
//     Serial.print(" Z: "); Serial.println(gz);
//   }
// }

// // This function is called in the main loop to handle serial events
// void handleSerial() {
//   serialEvent();
// }