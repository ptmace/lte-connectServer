#include "sw_timer.h"

unsigned long lastSendTime = 0; // Initialize the last send time

bool isTimerExpired(unsigned long interval) {
    unsigned long currentTime = millis();
    if (currentTime - lastSendTime >= interval) {
        lastSendTime = currentTime; // Update the last send time
        return true; // Timer has expired
    }
    return false; // Timer has not expired
}