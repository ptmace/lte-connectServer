#include "main.h"

// MPU6050 mpu; // Create an instance of the MPU6050 class

// void setup() {
//   // Set up serial communication at 115200 baud rate
//   Serial.begin(115200);

//   // Check if the serial port is available
//   if (!Serial) {
//     // If the serial port is not available, print an error message
//     Serial.println("Serial port not available!");
//     return; // Exit setup if serial is not available
//   }
//   else{
//     // If the serial port is available, print a success message
//     Serial.println("Serial port initialized successfully!");
//   }

//   // Initialize the MPU6050 sensor
//   Wire.begin(SDA_PIN, SCL_PIN); // Start I2C communication
//   mpu.initialize(); // Initialize the MPU6050 sensor
//   if (!mpu.testConnection()) {
//     // If the MPU6050 connection gitfails, print an error message
//     Serial.println("MPU6050 connection failed!");
//   } else {
//     // If the MPU6050 connection is successful, print a success message
//     Serial.println("MPU6050 connection successful!");
//   }

//   Serial2.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
  
//   Serial.println("ESP32-A7682S bắt đầu...");
//   Serial2.println("AT"); // Gửi lệnh AT
//   delay(5000);           // Đợi module trả lời

//   // // Đọc toàn bộ dữ liệu trả về (dù có hay không)
//   // while (Serial2.available()) {
//   //     char c = Serial2.read();
//   //     Serial.print(c);
//   // }
//   // Serial.println("Đã đọc xong Serial2.");
// }

// void loop() {
//   // Serial communication is handled in the serialEvent function
//   // handleSerial();
//   // Nhận từ Serial2, in ra Serial
//   while (Serial2.available()) {
//     char c = Serial2.read();
//     Serial.print(c);
//   }
//   // Nhận từ Serial Monitor, gửi sang Serial2
//   // while (Serial.available()) {
//   //   char c = Serial.read();
//   //   Serial2.print(c);
//   // }
// }
// #define RXD2 16
// #define TXD2 17
// #define EN_GPIO 32
// #define BUILTIN_LED 2

// String at_commands[] = {
//   "AT+CPIN?\r\n",
//   "AT\r\n",
//   "AT+CSQ\r\n",
//   "AT+CGDCONT=1,\"IP\",\"M2MISAFE\"\r\n",
//   "AT+CGACT=1,1\r\n",
//   "AT+CNTP=\"asia.pool.ntp.org\",0\r\n",
//   "AT+CNTP\r\n",
//   "AT+CCLK?\r\n",
//   "AT+CLBS=1\r\n",
//   "AT+CMQTTSTART\r\n",
//   "AT+CMQTTACCQ=0,\"CLIENT4G\"\r\n",
//   "AT+CMQTTCFG=\"argtopic\",0,1,1\r\n",
//   "AT+CMQTTCONNECT=0,\"tcp://test.mosquitto.org:1883\",60,1\r\n",
//   "AT+CMQTTSUB=0,\"4GBOARD_SIMCOMM/cmd\",2,1\r\n"
// };

// int current_cmd = 0;
// bool mqtt_connected = false;
// unsigned long last_send = 0;

// void sendAT(const String &cmd) {
//   Serial2.print(cmd);
//   Serial.print(">> ");
//   Serial.print(cmd);
// }

// void restartA7682S() {
//   digitalWrite(EN_GPIO, HIGH);
//   delay(2000);
//   digitalWrite(EN_GPIO, LOW);
//   delay(20000);
// }

// void setup() {
//   Serial.begin(115200);
//   Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  
//   pinMode(BUILTIN_LED, OUTPUT);
//   pinMode(EN_GPIO, OUTPUT);

//   Serial.println("ESP32 + A7682S started");
//   restartA7682S();

//   sendAT(at_commands[current_cmd]);
// }

// String rx_buffer = "";

// void loop() {
//   // Đọc phản hồi từ module
//   while (Serial2.available()) {
//     char c = Serial2.read();
//     rx_buffer += c;
//     Serial.print(c);
//   }

//   // Kiểm tra OK để gửi tiếp lệnh
//   if (rx_buffer.indexOf("OK") >= 0) {
//     rx_buffer = "";

//     current_cmd++;
//     if (current_cmd < sizeof(at_commands) / sizeof(at_commands[0])) {
//       delay(1000);
//       sendAT(at_commands[current_cmd]);
//     } else {
//       mqtt_connected = true;
//       digitalWrite(BUILTIN_LED, HIGH);
//     }
//   }

//   // Nếu đã kết nối MQTT → gửi dữ liệu JSON
//   if (mqtt_connected && millis() - last_send > 10000) {
//     String payload = "{\"device\":\"esp32\",\"rssi\":\"99\",\"data\":{\"COUNT\":\"123\",\"LATITUDE\":\"21.x\",\"LONGITUDE\":\"105.x\"}}";
//     String pub_cmd = "AT+CMQTTPUB=0,\"esp32/topic\",1," + String(payload.length()) + "\r\n";

//     sendAT(pub_cmd);
//     delay(100);
//     sendAT(payload + "\r\n");

//     last_send = millis();
//   }
// }
#include <HardwareSerial.h>

#define simSerial               Serial2
#define MCU_SIM_BAUDRATE        115200
#define MCU_SIM_TX_PIN              17
#define MCU_SIM_RX_PIN              16
#define MCU_SIM_EN_PIN              15

// Please update number before test
#define PHONE_NUMBER                "+8498*****"

void sim_at_wait()
{
    delay(100);
    while (simSerial.available()) {
        Serial.write(simSerial.read());
    }
}

bool sim_at_cmd(String cmd){
    simSerial.println(cmd);
    sim_at_wait();
    return true;
}

bool sim_at_send(char c){
    simSerial.write(c);
    return true;
}

void sent_sms()
{
    sim_at_cmd("AT+CMGF=1");
    String temp = "AT+CMGS=\"";
    temp += (String)PHONE_NUMBER;
    temp += "\"";
    sim_at_cmd(temp);
    sim_at_cmd("ESP32-SIM7600X From linhkienthuduc.com");

    // End charactor for SMS
    sim_at_send(0x1A);
}

void call()
{
    String temp = "ATD";
    temp += PHONE_NUMBER;
    temp += ";";
    sim_at_cmd(temp); 

    delay(20000);

    // Hang up
    sim_at_cmd("ATH"); 
}

void setup() 
{
    /*  Power enable  */
    pinMode(MCU_SIM_EN_PIN,OUTPUT); 
    digitalWrite(MCU_SIM_EN_PIN,LOW);

    delay(20);
    Serial.begin(115200);
    Serial.println("\n\n\n\n-----------------------\nSystem started!!!!");

    // Delay 8s for power on
    delay(8000);
    simSerial.begin(MCU_SIM_BAUDRATE, SERIAL_8N1, MCU_SIM_RX_PIN, MCU_SIM_TX_PIN);

    // Check AT Command
    sim_at_cmd("AT");

    // Product infor
    sim_at_cmd("ATI");

    // Check SIM Slot
    sim_at_cmd("AT+CPIN?");

    // Check Signal Quality
    sim_at_cmd("AT+CSQ");

    sim_at_cmd("AT+CIMI");

    pinMode(2,OUTPUT); 
    digitalWrite(2,HIGH);

    sent_sms();

    // Delay 5s
    delay(5000);   

    call();
}

void loop() 
{     
    if (Serial.available()){
        char c = Serial.read();
        simSerial.write(c);
    }
    sim_at_wait();
}