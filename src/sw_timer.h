// Software timer for ESP32
#ifndef SW_TIMER_H
#define SW_TIMER_H

#include "main.h"

#define TIMEOUT_SEND 1000 // 1 second timeout for sending data
#define TIMEOUT_SERIAL 500 // 500 ms timeout for serial events
extern unsigned long lastSendTime;

bool isTimerExpired(unsigned long interval);

#endif // SW_TIMER_H