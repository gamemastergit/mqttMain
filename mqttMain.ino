#include <SPI.h>
#include <Ethernet2.h>
#include <PubSubClient.h>
#include <ButtonDebounce.h>


const int RELAY0 = 3;
const int RELAY1 = 2;

const int OUTPUT0 = 4;
const int OUTPUT1 = 5;
const int OUTPUT2 = 6;
const int OUTPUT3 = 7;
const int OUTPUT4 = 11;
const int OUTPUT5 = 12;

const int INPUT0 = A0;
const int INPUT1 = A1;
const int INPUT2 = A2;
const int INPUT3 = A3;
const int INPUT4 = A4;
const int INPUT5 = A5;


ButtonDebounce button0(INPUT0, 250);
ButtonDebounce button1(INPUT1, 250);
ButtonDebounce button2(INPUT2, 250);
ButtonDebounce button3(INPUT3, 250);
ButtonDebounce button4(INPUT4, 250);
ButtonDebounce button5(INPUT5, 250);

#define LED LED_BUILTIN

#define HALL 6

byte mac[]    = {  0xDE, 0xEF, 0xAA, 0xBD, 0xFE, 0xEE };
IPAddress ip(192, 168, 1, 166);
IPAddress server(192, 168, 1, 101);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println("Got a message!");
  String strTopic = String((char*)topic);
  payload[length] = '\0';
  String s = String((char*)payload);
  int value = s.toInt();


  if (strTopic == "rtf/main/simon") {
    Serial.println("Turning simon on");
    Serial.println(value);
    digitalWrite(OUTPUT3, value);
  }
  if (strTopic == "rtf/main/chicken") {
    Serial.println("Dropping Chicken");
    Serial.println(value);
    digitalWrite(OUTPUT0, value);
  }
  if (strTopic == "rtf/main/balls") {
    Serial.println("Dropping Balls");
    Serial.println(value);
    digitalWrite(RELAY1, value);
  }
  if (strTopic == "rtf/main/ladder") {
    Serial.println("Opening Ladder");
    Serial.println(value);
    digitalWrite(RELAY0, value);
  }
  if (strTopic == "rtf/main/waffle") {
    Serial.println("Opening Waffle");
    Serial.println(value);
    digitalWrite(OUTPUT3, value);
  }
   if (strTopic == "rtf/main/beaconLight") {
    Serial.println("Powering beaconLight");
    Serial.println(value);
    digitalWrite(OUTPUT1, value);
  }
    /*if (strTopic == "rtf/main/startcolor") {
    Serial.println("Starting the color sensor");
    Serial.println(value);
    digitalWrite(OUTPUT2, value);
  }*/
    if (strTopic == "rtf/main/dump") {
    Serial.println("Starting the dump");
    Serial.println(value);
    digitalWrite(OUTPUT5, value);
  }
    if (strTopic == "rtf/main/hotwheels") {
    Serial.println("Starting the hotwheels");
    Serial.println(value);
    digitalWrite(OUTPUT4, value);
  }

}

EthernetClient ethClient;
PubSubClient mqttClient(server, 1883, callback, ethClient);

void reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttClient.connect("FactoryController")) {
      Serial.println("connected");
      // Once connected, publish an announcement...


      mqttClient.subscribe("rtf/main/chicken");
      mqttClient.subscribe("rtf/main/balls");
      mqttClient.subscribe("rtf/main/ladder");
      mqttClient.subscribe("rtf/main/waffle");
      mqttClient.subscribe("rtf/main/beaconLight");
      mqttClient.subscribe("rtf/main/startcolor");
      mqttClient.subscribe("rtf/main/hotwheels");
      mqttClient.subscribe("rtf/main/dump");
      mqttClient.subscribe("serverRoomBAC/get/input2");
      mqttClient.subscribe("rtf/main/simon");
    } else {
      Serial.print("failed, rc=");
      digitalWrite(LED, HIGH);
      delay(500);
      digitalWrite(LED, LOW);
      Ethernet.begin(mac, ip);
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(57600);
  pinMode(OUTPUT0, OUTPUT);
  pinMode(OUTPUT1, OUTPUT);
  pinMode(OUTPUT2, OUTPUT);
  pinMode(OUTPUT3, OUTPUT);
  pinMode(OUTPUT4, OUTPUT);
  pinMode(OUTPUT5, OUTPUT);
  pinMode(RELAY0, OUTPUT);
  pinMode(RELAY1, OUTPUT);
  pinMode(INPUT0, INPUT);
  pinMode(INPUT1, INPUT);
  pinMode(INPUT2, INPUT);
  pinMode(INPUT3, INPUT);
  pinMode(INPUT4, INPUT);
  pinMode(INPUT5, INPUT);
  pinMode(LED, OUTPUT);
  //digitalWrite(OUTPUT2, HIGH);


  Ethernet.begin(mac, ip);
  // Allow the hardware to sort itself out
  delay(1500);
}

int i0 = 0;
int i1 = 0;
int i2 = 0;
int i3 = 0;
int i4 = 0;
int i5 = 0;



void loop()
{
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();

  button0.update();
  button1.update();
  button2.update();
  button3.update();
  button4.update();
  button5.update();


  if (button0.state() != i0) {
    if (button0.state()) {
      Serial.println("Start Button Pressed");
      mqttClient.publish("rtf/main/start", "1");
      i0 = button0.state();
    } else {
      Serial.println(button0.state());
      mqttClient.publish("rtf/main/start", "0");
      i0 = button0.state();
    }

  }
  if (button1.state() != i1) {
    if (button1.state()) {
      Serial.println("Bolt Puzzle Completed");
      mqttClient.publish("rtf/main/bolt", "1");
      i1 = button1.state();
    } else {
      mqttClient.publish("rtf/main/bolt", "0");
      i1 = button1.state();
    }


  }
  if (button2.state() != i2) {
    if (button2.state()) {
      Serial.println("Color Puzzle Completed");
      mqttClient.publish("rtf/main/color", "1");
      mqttClient.publish("serverRoomBAC/set/output3", "1");
      i2 = button2.state();
    } else {
      mqttClient.publish("rtf/main/color", "0");
      i2 = button2.state();
    }

  }
  if (button3.state() != i3) {

    if (button3.state()) {
      Serial.println("OPEN Completed");
      mqttClient.publish("rtf/main/open", "1");
      i3 = button3.state();
    } else {
      mqttClient.publish("rtf/main/open", "0");
      i3 = button3.state();
    }
  }
  if (button4.state() != i4) {

    if (button4.state()) {
      Serial.println("Ball hands pushed");
      mqttClient.publish("rtf/main/ballhands", "1");
      i4 = button4.state();
    } else {
      mqttClient.publish("rtf/main/ballhands", "0");
      i4 = button4.state();
    }

  }
  if (button5.state() != i5) {

    if (button5.state()) {
      Serial.println("Simon puzzle completed... Game ending");
      mqttClient.publish("rtf/main/endGame", "1");
      i5 = button5.state();
    } else {
      mqttClient.publish("rtf/main/endGame", "0");
      i5 = button5.state();
    }
  }
}
