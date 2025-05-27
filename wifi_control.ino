#include <ESP8266WiFi.h>
#include <MQTT.h>
//L298N pinout:
//left Motor
const int ENA = 14; //D5->GPIO14
const int IN1 = 0;//D4->GPIO0
const int IN2 = 2;//D3->GPIO2

//right motor
const int ENB = 12; //D6->GPIO12
const int IN3 = 13;//D8->GPIO15
const int IN4 = 15;//D7->GPIO13

String Payload = "";

const char ssid[] = "WIFINAME";
const char pass[] = "WIFIPASS";

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("arduino", "mqtt-test", "<MQTT KEY HERE>")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.subscribe("/hello");
  // client.unsubscribe("/hello");
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
  Payload=payload;

}

void setup() {

  pinMode(ENA,OUTPUT);
  pinMode(ENB,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  Serial.begin(115200);
  WiFi.begin(ssid, pass);

  // use shiftr MQTT
  client.begin("mqtt-test.cloud.shiftr.io", net);
  client.onMessage(messageReceived);

  connect();
}

void loop() {
  client.loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (!client.connected()) {
    connect();
  }
/*
  // publish a message roughly every second.
  if (millis() - lastMillis > 1000) {
    lastMillis = millis();
    client.publish("/hello", Payload);
  }
*/
  if (Payload=="forward"){
    MotorForward();
  }
  if (Payload=="backward"){
    MotorBackward();
  }
  if (Payload=="stop"){
    MotorStop();
  }
  if (Payload=="left"){
    TurnLeft();
  }
  if (Payload=="right"){
    TurnRight();
  }
  
  
}





void MotorForward(void)   
{
  //ENA=HIGH, IN1=LOW, IN2=HIGH
   //ENB=HIGH, IN3=LOW, IN4=HIGH

  //Left Motor
  digitalWrite(ENA,HIGH);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);

  //right motor
  digitalWrite(ENB,HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  
  
}

/********************************************* BACKWARD *****************************************************/
void MotorBackward(void)   
{
   //EN=HIGH, IN1=HIGH, IN2=LOW for both motors

  //Left Motor
  digitalWrite(ENA,HIGH);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);

  //right motor
  digitalWrite(ENB,HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  
}

/********************************************* TURN LEFT *****************************************************/
void TurnLeft(void)   
{
    //EN=HIGH, IN1=LOW, IN2=HIGH , IN3=LOW, IN4=HIGH (opposite)

  //Left Motor
  digitalWrite(ENA,HIGH);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);

  //right motor
  digitalWrite(ENB,HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  
}

/********************************************* TURN RIGHT *****************************************************/
void TurnRight(void)   
{
  //EN=HIGH, IN1=HIGH, IN2=LOW , IN3=LOW, IN4=HIGH (opposite)

  //Left Motor
  digitalWrite(ENA,HIGH);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);

  //right motor
  digitalWrite(ENB,HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

}

/********************************************* STOP *****************************************************/
void MotorStop(void)   
{
    //LOW motor stop, so all LOW except enable
  //Left Motor
  digitalWrite(ENA,HIGH);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);

  //right motor
  digitalWrite(ENB,HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

}
