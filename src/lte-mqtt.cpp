// #include <HardwareSerial.h>
// #include "lte-mqtt.h"
// #include "main.h"

// HardwareSerial simSerial(2);

// String topic = "esp32/data";
// String payload = "{\"temp\":26.5,\"hum\":60}";

// void sim_at_wait() {
//   delay(100);
//   while (simSerial.available()) {
//     Serial.write(simSerial.read());
//   }
// }

// bool sim_at_cmd(String cmd, int wait_ms) {
//   Serial.println(">> " + cmd);
//   simSerial.println(cmd);
//   delay(wait_ms);
//   sim_at_wait();
//   return true;
// }

// void power_on_module() {
//   pinMode(SIM_PWR, OUTPUT);
//   digitalWrite(SIM_PWR, LOW);
//   delay(1000);
//   digitalWrite(SIM_PWR, HIGH);
//   delay(5000);  // Đợi module lên
// }

// void mqtt_connect() {
//   sim_at_cmd("AT+CMQTTSTART", 2000);
//   sim_at_cmd("AT+CMQTTACCQ=0,\"esp32client\"", 1000);
//   sim_at_cmd("AT+CMQTTCONNECT=0,\"tcp://broker.hivemq.com:1883\",60,1", 3000);
// }

// void mqtt_publish(String topic, String message) {
//   sim_at_cmd("AT+CMQTTTOPIC=0," + String(topic.length()));
//   simSerial.print(topic); simSerial.write(0x0D);  // CR
//   delay(500);

//   sim_at_cmd("AT+CMQTTPAYLOAD=0," + String(message.length()));
//   simSerial.print(message); simSerial.write(0x0D);
//   delay(500);

//   sim_at_cmd("AT+CMQTTPUB=0,1,60");
// }


