#include "main.h"

// This function get data from the serial port
void serialEvent() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    Serial.println("Received: " + input);
  }
}

// This function is called in the main loop to handle serial events
void handleSerial() {
  serialEvent();
}

// This function send data to the serial port
void sendSerialData(const String &data) {
  Serial.println(data);
}
