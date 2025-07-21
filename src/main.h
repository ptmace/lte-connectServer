#ifndef __MAIN_H__
#define __MAIN_H__

#define TINY_GSM_MODEM_SIM7600
#define TINY_GSM_USE_GPRS true
#define TINY_GSM_USE_PPP true

#include <Arduino.h>
#include <TinyGsmClient.h>
#include <PubSubClient.h>

// Modem Pinout
#define MODEM_TX    17
#define MODEM_RX    16
#define MODEM_PWR   15
#define MODEM_BAUD  115200
#define SERIAL_MONITOR      Serial
#define SERIAL_AT           Serial2

// MQTT Broker Config
#define MQTT_SERVER "vierone.anyengarden.com.vn"
#define MQTT_PORT   1883
#define MQTT_USER   "vierone-demo"
#define MQTT_PASS   "123456aB@"
#define MQTT_TOPIC  "vierone/node-id/device_update"

// Forward Declaration
void modemPowerOn();
void mqttReconnect();

extern HardwareSerial serialAT;
extern TinyGsm modem;
extern TinyGsmClient gsmClient;
extern PubSubClient mqtt;

#endif  // __MAIN_H__
