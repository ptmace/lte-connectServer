#include "main.h"


// #include <HardwareSerial.h>

// #define simSerial               Serial2
// #define MCU_SIM_BAUDRATE        115200
// #define MCU_SIM_TX_PIN              17
// #define MCU_SIM_RX_PIN              16
// #define MCU_SIM_EN_PIN              15

// // Please update number before test
// #define PHONE_NUMBER                "+84349305014"

// void sim_at_wait()
// {
//     delay(100);
//     while (simSerial.available()) {
//         Serial.write(simSerial.read());
//     }
// }

// bool sim_at_cmd(String cmd){
//     simSerial.println(cmd);
//     sim_at_wait();
//     return true;
// }

// bool sim_at_send(char c){
//     simSerial.write(c);
//     return true;
// }

// void sent_sms()
// {
//     sim_at_cmd("AT+CMGF=1");
//     String temp = "AT+CMGS=\"";
//     temp += (String)PHONE_NUMBER;
//     temp += "\"";
//     sim_at_cmd(temp);
//     sim_at_cmd("ESP32-SIM7600X From linhkienthuduc.com");

//     // End charactor for SMS
//     sim_at_send(0x1A);
// }

// void call()
// {
//     String temp = "ATD";
//     temp += PHONE_NUMBER;
//     temp += ";";
//     sim_at_cmd(temp); 

//     delay(20000);

//     // Hang up
//     sim_at_cmd("ATH"); 
// }

// void setup() 
// {
//     /*  Power enable  */
//     pinMode(MCU_SIM_EN_PIN,OUTPUT); 
//     digitalWrite(MCU_SIM_EN_PIN,LOW);

//     delay(20);
//     Serial.begin(115200);
//     Serial.println("\n\n\n\n-----------------------\nSystem started!!!!");

//     // Delay 8s for power on
//     delay(8000);
//     simSerial.begin(MCU_SIM_BAUDRATE, SERIAL_8N1, MCU_SIM_RX_PIN, MCU_SIM_TX_PIN);

//     // Check AT Command
//     sim_at_cmd("AT");

//     // Product infor
//     sim_at_cmd("ATI");

//     // Check SIM Slot
//     sim_at_cmd("AT+CPIN?");

//     // Check SMS Center Address
//     sim_at_cmd("AT+CSCA?"); 

//     // Check Network Registration
//     sim_at_cmd("AT+CREG?"); 

//     // Check Signal Quality
//     sim_at_cmd("AT+CSQ");

//     sim_at_cmd("AT+CIMI"); 

//     sim_at_cmd("AT+CGDCONT=1,\"IP\",\"v-internet\""); // Set APN (Viettel: v-internet, Mobifone: m-wap) 

//     sim_at_cmd("AT+CGACT=1,1"); // Activate PDP context

//     sim_at_cmd("AT+CGPADDR=1"); // Get IP Address


// //   
//     //sim_at_cmd("AT+CMQTTSSLCFG?"); // Check MQTT SSL configuration

//     //sim_at_cmd("AT+CFSGFLS"); // Check upload files

//     //sim_at_cmd("AT+CMQTTSSLCFG=0,1,\"AmazonRootCA1.pem\",\"device-cert.pem\",\"private-key.pem\"");
    
//     sim_at_cmd("AT+NETOPEN"); //Mở socket kết nối mạng

//     sim_at_cmd("AT+NETSTATUS");
    
//     sim_at_cmd("AT+CMQTTSTART");
//     sim_at_cmd("AT+CMQTTACCQ=0,\"test-lte\"");
//     //sim_at_cmd("AT+CMQTTCONNECT=0,\"ssl://ah5lyhi48b5xm-ats.iot.ap-southeast-1.amazonaws.com:8883\",60,1");

//     sim_at_cmd("AT+CMQTTCONNECT=0,\"tcp://vierone.anyengarden.com.vn:1883\",60,1,\"vierone-demo\",\"123456aB@\"");

//     sim_at_cmd("AT+CMQTTTOPIC=0,31");  // số ký tự của topic
//     sim_at_cmd("vierone/node-id/device_update");

//     sim_at_cmd("AT+CMQTTPAYLOAD=0,11"); // 11 ký tự của "Hello World"
//     sim_at_cmd("Hello World");

//     sim_at_cmd("AT+CMQTTPUB=0,1,60");  // QoS 1, Retained 0, Timeout 60s

//     sim_at_cmd("AT+CMQTTDISC=0,60");
//     delay(1000);
//     sim_at_cmd("AT+CMQTTREL=0");
//     delay(1000);
//     sim_at_cmd("AT+CMQTTSTOP");
//     delay(1000);
//     sim_at_cmd("AT+NETCLOSE");
//     delay(1000);



//     pinMode(2,OUTPUT); 
//     digitalWrite(2,HIGH);

//     sent_sms();

//     // Delay 5s
//     delay(500);   

//     call();
// }

// void loop() 
// {     
//     if (Serial.available()){
//         char c = Serial.read();
//         simSerial.write(c);
//     }
//     sim_at_wait();
// }

#include <HardwareSerial.h>

#define simSerial               Serial2
#define MCU_SIM_BAUDRATE        115200
#define MCU_SIM_TX_PIN          17
#define MCU_SIM_RX_PIN          16
#define MCU_SIM_EN_PIN          15

#define PHONE_NUMBER            "+84349305014"

// Topic và Payload
#define MQTT_TOPIC              "vierone/node-id/device_update"
#define MQTT_PAYLOAD            "{\"msg\":\"hello test\"}"

void sim_at_wait() {
    delay(200);
    while (simSerial.available()) {
        Serial.write(simSerial.read());
    }
}

void sim_at_cmd(const String &cmd) {
    simSerial.println(cmd);
    sim_at_wait();
}

void sim_at_send_raw(const String &raw) {
    simSerial.print(raw);
    simSerial.write(0x0D); // gửi CR
    sim_at_wait();
}

void send_mqtt() {
    sim_at_cmd("AT+CMQTTSTART");
    delay(500);

    sim_at_cmd("AT+CMQTTACCQ=0,\"test-lte\"");
    delay(500);

    sim_at_cmd("AT+CMQTTCONNECT=0,\"tcp://vierone.anyengarden.com.vn:1883\",60,1,\"vierone-demo\",\"123456aB@\"");
    delay(1000);

    // Send Topic
    sim_at_cmd("AT+CMQTTTOPIC=0," + String(strlen(MQTT_TOPIC)));
    delay(200);
    sim_at_send_raw(MQTT_TOPIC);
    delay(500);

    // Send Payload
    sim_at_cmd("AT+CMQTTPAYLOAD=0," + String(strlen(MQTT_PAYLOAD)));
    delay(200);
    sim_at_send_raw(MQTT_PAYLOAD);
    delay(500);

    // Publish QoS 0 (khuyến khích)
    sim_at_cmd("AT+CMQTTPUB=0,0,60");
    delay(1000);

    // Disconnect
    sim_at_cmd("AT+CMQTTDISC=0,60");
    delay(500);
    sim_at_cmd("AT+CMQTTREL=0");
    delay(500);
    sim_at_cmd("AT+CMQTTSTOP");
}

void setup() {
    pinMode(MCU_SIM_EN_PIN, OUTPUT);
    digitalWrite(MCU_SIM_EN_PIN, LOW);

    Serial.begin(115200);
    Serial.println("\nSystem started!!!!");

    delay(8000);  // Đợi SIM module khởi động

    simSerial.begin(MCU_SIM_BAUDRATE, SERIAL_8N1, MCU_SIM_RX_PIN, MCU_SIM_TX_PIN);

    sim_at_cmd("AT");
    sim_at_cmd("ATI");
    sim_at_cmd("AT+CPIN?");
    sim_at_cmd("AT+CSCA?");
    sim_at_cmd("AT+CREG?");
    sim_at_cmd("AT+CSQ");
    sim_at_cmd("AT+CIMI");

    sim_at_cmd("AT+CGDCONT=1,\"IP\",\"v-internet\"");
    sim_at_cmd("AT+CGACT=1,1");
    sim_at_cmd("AT+CGPADDR=1");

    delay(500);
    sim_at_cmd("AT+NETOPEN");
    delay(500);

    send_mqtt();  // Gửi tin nhắn MQTT
}

void loop() {
    if (Serial.available()) {
        simSerial.write(Serial.read());
    }
    if (simSerial.available()) {
        Serial.write(simSerial.read());
    }
}
