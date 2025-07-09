#ifndef SERIAL_H
#define SERIAL_H

#include "main.h"

// This function gets data from the serial port
void serialEvent();
// This function is called in the main loop to handle serial events
void handleSerial();

#endif // SERIAL_H