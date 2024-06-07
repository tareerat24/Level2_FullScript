#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebSrv.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include "DHT.h"
#define DHTPIN 23  //pin DHT 23
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);  // 0x27 or 0x3F

#define Buzzer_Pin 18
#define LDR_Pin 34
int LDR_input = 0, lcd_mode = 0;
float temp, humi, ldr;
bool alarm_state = 0;

//-----------------------------------------------Set your device name and password.--------------------------------------//
const char *ssid = "SMV";
const char *password = "77777777";
//-----------------------------------------------------------------------------------------------------------------------//

//-------------------------------------------------edit Parameter-------------------------------------------------------//
int light_alarm = 400;  //*****แก้ระดับแสง
int temp_alarm = 35;    //******แก้อุณหภูมิ

unsigned long previousMillis1 = 0, previousMillis2 = 0;
const long lcd_refresh = 1000;

const int TrigPin = 5;   // Connect the trigger pin of first Ultrasonic sensor to Pin 34 of arduino
const int EchoPin = 35;  // Connect the eco pin of first Ultrasonic sensor to Pin 35 of arduino
long duration, Ultra;
int colision_distance = 8;
bool auto_move = false;

//--------------------------------------Define AGV------------------------------------------//
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define UP_LEFT 5
#define UP_RIGHT 6
#define DOWN_LEFT 7
#define DOWN_RIGHT 8
#define TURN_LEFT 9
#define TURN_RIGHT 10
#define STOP 0
#define ON_BUZZER 11
#define OFF_BUZZER 12

#define FRONT_LEFT_MOTOR 3
#define FRONT_RIGHT_MOTOR 0
#define BACK_RIGHT_MOTOR 1
#define BACK_LEFT_MOTOR 2

#define L_ENA 4   // Adjust speed  //motor1
#define L_ENB 19  // Adjust speed //motor2
#define R_ENA 2   // Adjust speed //motor3
#define R_ENB 15  // Adjust speed //motor4

#define FORWARD 1
#define BACKWARD -1

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}

struct MOTOR_PINS {
  int pinIN1;
  int pinIN2;
};

std::vector<MOTOR_PINS> motorPins = {
  { 12, 13 },  // FRONT_RIGHT_MOTOR
  { 32, 33 },  // BACK_RIGHT_MOTOR
  { 27, 14 },  // FRONT_LEFT_MOTOR
  { 25, 26 },  // BACK_LEFT_MOTOR
  { 19, 4 },   // LEFT_MOTOR_SPEED
  { 15, 2 },   // RIGHT_MOTOR_SPEED
};

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

//////////////////////////////////////////////////////
String LCDtext0 = "idektep";       //16 charactor ข้อความบรรทัด0
String LCDtext1 = "robo expoler";  //16 charactor ข้อความบรรทัด1
//////////////////////////////////////////////////////////

void setUpPinModes() {
  for (int i = 0; i < motorPins.size(); i++) {
    pinMode(motorPins[i].pinIN1, OUTPUT);
    pinMode(motorPins[i].pinIN2, OUTPUT);
    rotateMotor(i, STOP);
  }
}

void setup(void) {
  setUpPinModes();
  Serial.begin(115200);
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);
  initWiFi();
  Serial.print("RRSI: ");
  Serial.println(WiFi.RSSI());
  server.on("/", HTTP_GET, handleRoot);
  server.onNotFound(handleNotFound);
  ws.onEvent(onWebSocketEvent);
  server.addHandler(&ws);
  server.begin();
  Serial.println("HTTP server started");
  dht.begin();
  lcd.begin();
  lcd.display();    //เปิดหน้าจอ
  lcd.backlight();  //เปิดไฟ backlight
  lcd.clear();      //ล้างหน้าจอ
  pinMode(Buzzer_Pin, OUTPUT);
  pinMode(LDR_Pin, INPUT);
  digitalWrite(Buzzer_Pin, LOW);
  lcd.noBacklight();
}
void loop() {
  sensor_fn();
  lcd_fn();
  ws.cleanupClients();
}
