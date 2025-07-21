#include <main.h>
#include <HardwareSerial.h>


HardwareSerial serialAT(2);
TinyGsm modem(serialAT);
TinyGsmClient gsmClient(modem);
PubSubClient mqtt(gsmClient);


void modemPowerOn() {
    pinMode(MODEM_PWR, OUTPUT);
    digitalWrite(MODEM_PWR, LOW);
    delay(1000);
}

void setup() {
    Serial.begin(115200);
    delay(10);

    modemPowerOn();

    serialAT.begin(MODEM_BAUD, SERIAL_8N1, MODEM_RX, MODEM_TX);

    Serial.println("[Modem] Restarting...");
    modem.restart();

    Serial.println("[Modem] Waiting for network...");
    if (!modem.waitForNetwork()) {
        Serial.println("[Modem] Network failed!");
        while (true);
    }

    Serial.println("[Modem] Connecting to APN...");
    if (!modem.gprsConnect("m-wap", "", "")) {
        Serial.println("[Modem] GPRS connect failed!");
        while (true);
    }

    Serial.print("[Modem] IP address: ");
    Serial.println(modem.localIP());

    mqtt.setServer(MQTT_SERVER, MQTT_PORT);

    Serial.println("[MQTT] Connecting to broker...");
    if (!mqtt.connect("test-lte-client", MQTT_USER, MQTT_PASS)) {
        Serial.println("[MQTT] Failed to connect");
        while (true);
    }

    Serial.println("[MQTT] Connected! Publishing...");
    mqtt.publish(MQTT_TOPIC, "{\"msg\":\"hi from zybike with luv\"}");
    mqtt.disconnect();
}

void mqttReconnect() {
    while (!mqtt.connected()) {
        Serial.println("[MQTT] Attempting reconnect...");
        if (mqtt.connect("test-lte-client", MQTT_USER, MQTT_PASS)) {
            Serial.println("[MQTT] Connected!");
            mqtt.publish(MQTT_TOPIC, "{\"msg\":\"hi again from reconnect\"}");
        } else {
            Serial.print("[MQTT] Failed, rc=");
            Serial.print(mqtt.state());
            Serial.println(" Retry in 5s...");
            delay(5000);
        }
    }
}

void loop() {
    if (!modem.isNetworkConnected()) {
        Serial.println("[Modem] Network lost. Reconnecting...");
        if (!modem.waitForNetwork(30000L)) {
            Serial.println("[Modem] Reconnect failed");
            delay(10000);
            return;
        }
        Serial.println("[Modem] Reconnected to network");
    }

    if (!modem.isGprsConnected()) {
        Serial.println("[Modem] GPRS lost. Reconnecting...");
        if (!modem.gprsConnect("m-wap", "", "")) {
            Serial.println("[Modem] GPRS reconnect failed");
            delay(10000);
            return;
        }
        Serial.println("[Modem] GPRS reconnected");
    }

    if (!mqtt.connected()) {
        mqttReconnect();
    }

    mqtt.loop();
}
