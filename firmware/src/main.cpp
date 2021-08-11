#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoOTA.h>
#include <CREDENTIALS.h>

#include "pins.h"
#include "LogicData.h"


#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))
#define CYCLE_TIME_US 1000  // 1 ms

int btn_pins[] = {BTN_UP, BTN_DOWN};
const int btn_pressed_state = HIGH;// when we read high, button is pressed
const uint32_t debounce_time = 50;
const uint32_t double_time = 500;

enum Actions { UpSingle, UpDouble, DownSingle, DownDouble };
bool latch_up = false;
bool latch_down = false;

#define BTN_COUNT ARRAY_SIZE(btn_pins)
int8_t btn_last_state[BTN_COUNT] = {-1};
int8_t btn_last_double[BTN_COUNT] = {-1};
uint32_t debounce[BTN_COUNT] = {0};
uint32_t btn_last_on[BTN_COUNT] = {0};


//last_signal is just the last time input was read from buttons or from controller
//If we haven't seen anything from either in a bit, stop moving
uint32_t last_signal = 0;
uint32_t signal_giveup_time = 2000;


LogicData ld(-1);
WiFiClient espClient;
PubSubClient client(espClient);

volatile unsigned long gCurTimeUs = 0;
volatile unsigned long gLastTimeUs = 0;

//-- Buffered mode parses input words and sends them to output separately
void ICACHE_RAM_ATTR logicDataPin_ISR() {
  ld.PinChange(HIGH == digitalRead(LOGICDATA_RX));
}

uint8_t highTarget = 120;
uint8_t lowTarget = 80;

volatile uint8_t height;
uint8_t target;

bool checkTime()
{
    gCurTimeUs = micros();
    if (gCurTimeUs - gLastTimeUs >= CYCLE_TIME_US)
    {
        gLastTimeUs = gCurTimeUs;
        return true;
    }
    return false;
}


void callback(char *topic, byte *payload, unsigned int length)
{
    String receivedtopic = topic;
    int gTargetHeight;

    String value = "";
    for (unsigned int i = 0; i < length; i++)
    {
        value += (char)payload[i];
    }

    if (receivedtopic == "deskcontrol/setheight")
    {
       gTargetHeight = value.toInt();
       if (gTargetHeight>height)
       {
          latch_up = true;
          target = gTargetHeight;
          last_signal = millis();
       }
       if (gTargetHeight<height)
       {
          latch_down = true;
          target = gTargetHeight;
          last_signal = millis();
       }
          
    }
}

void initWiFi()
{
    WiFi.mode(WIFI_STA);
    WiFi.persistent(false);
    WiFi.config(ip, dns, gateway, subnet);
    WiFi.setAutoReconnect(true);
    WiFi.begin(WIFI_SSID, WIFI_PASS); //Connect to the WiFi network
}

void initOTA()
{
    ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
            type = "sketch";
        else // U_SPIFFS
            type = "filesystem";

        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
        Serial.println("Start updating " + type);
    });
    ArduinoOTA.onEnd([]() {
        Serial.println("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR)
            Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR)
            Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR)
            Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR)
            Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR)
            Serial.println("End Failed");
    });
    ArduinoOTA.begin();
}

long lastReconnectAttempt = 0;
boolean mqttReconnect()
{
    if (client.connect("DeskClient"))
    {
        client.subscribe("deskcontrol/setheight");
    }
    return client.connected();
}

void initMQTT()
{
    if (!client.connected())
    {
        long now = millis();
        if (now - lastReconnectAttempt > 5000)
        {
            lastReconnectAttempt = now;
            // Attempt to reconnect
            if (mqttReconnect())
            {
                lastReconnectAttempt = 0;
            }
        }
    }
    else
    {
        // Client connected

        client.loop();
    }
}

long lastPublish = 0;
byte lastHeight = 0;
void publishHeight()
{
    long now = millis();
    if (lastHeight != height && now - lastPublish > 5000)
    {
        lastPublish = now;
        lastHeight = height;
        char heightBuffer[3];
        sprintf(heightBuffer, "%d", height);
        client.publish("deskcontrol/currentheight", heightBuffer, true);
    }
}


void setup() {

  pinMode(BTN_UP, INPUT);
  pinMode(BTN_DOWN, INPUT);
  pinMode(LOGICDATA_RX, INPUT);

  pinMode(ASSERT_UP, OUTPUT);
  pinMode(ASSERT_DOWN, OUTPUT);

  Serial.begin(115200);

  logicDataPin_ISR();
  attachInterrupt(digitalPinToInterrupt(LOGICDATA_RX), logicDataPin_ISR, CHANGE);

  ld.Begin();
  initWiFi();
  initOTA();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  lastReconnectAttempt = 0;
  gLastTimeUs = micros();

  Serial.println("Robodesk v2.1  build: " __DATE__ " " __TIME__);
}

// Record last time the display changed
// sets globals height and last_signal
void check_display() {
  static uint32_t prev = 0;
  uint32_t msg = ld.ReadTrace();
  char buf[80];
  if (msg) {
    uint32_t now = millis();
    sprintf(buf, "%6ums %s: %s", now - prev, ld.MsgType(msg), ld.Decode(msg));
    Serial.println(buf);
    prev=now;
  }

  // Reset idle-activity timer if display number changes or if any other display activity occurs (i.e. display-ON)
  if (ld.IsNumber(msg)) {
    auto new_height = ld.GetNumber(msg);
    if (new_height == height) {
      return;
    }
    height = new_height;
  }
  if (msg)
    last_signal = millis();
}



void transitionState(enum Actions action) {
  switch(action) {
    case UpSingle:
    case DownSingle:
      if (latch_up || latch_down) {
        Serial.println("Breaking latches");
        latch_up = false;
        latch_down = false;
      }
      break;
    case UpDouble:
      latch_up = true;
      target = highTarget;
      Serial.print("Latching UP ");
      Serial.println(target);
      break;
    case DownDouble:
      latch_down = true;
      target = lowTarget;
      Serial.print("Latching Down ");
      Serial.println(target);
      break;
  }
}

void move() {
  //btn_last_state has current buttons pressed
  if(btn_last_state[0] && btn_last_state[1]) {
    //both buttons pressed, do nothing
    //do nothing
  } else if(btn_last_state[0]) {
    digitalWrite(ASSERT_UP, HIGH);
    return;
  } else if(btn_last_state[1]) {
    digitalWrite(ASSERT_DOWN, HIGH);
    return;
  } else if (!latch_up && !latch_down) {
    //if not latch, do nothing
    digitalWrite(ASSERT_UP, LOW);
    digitalWrite(ASSERT_DOWN, LOW);
    return;
  }

  if (latch_up && latch_down) {
    Serial.println("Latch up and latch down set, this is an issue");
    while(true) ;
  }
  if(millis() - last_signal > signal_giveup_time) {
    Serial.println("Haven't seen input in a while, turning everything off for safety");
    digitalWrite(ASSERT_UP, LOW);
    digitalWrite(ASSERT_DOWN, LOW);
    latch_up = false;
    latch_down = false;
    while(true) ;
  }

  if(height != target) {
    //should be moving
    digitalWrite(latch_up ? ASSERT_UP : ASSERT_DOWN, HIGH);
    digitalWrite(!latch_up ? ASSERT_UP : ASSERT_DOWN, LOW);
    return;
  } else {
    //hit target
    digitalWrite(ASSERT_UP, LOW);
    digitalWrite(ASSERT_DOWN, LOW);
    latch_up = false;
    latch_down = false;
    Serial.print("Hit target ");
    Serial.println(target);
    return;
  }
}


void loop() {

  ArduinoOTA.handle();
  initMQTT();

  // sets global height and last_signal from logicdata serial
  check_display();

  for(uint8_t i=0; i < ARRAY_SIZE(btn_pins); ++i) {
    int btn_state = digitalRead(btn_pins[i]);
    if((btn_state == btn_pressed_state) != btn_last_state[i] && millis() - debounce[i] > debounce_time) {
      //change state
      btn_last_state[i] = (btn_state == btn_pressed_state);
      debounce[i] = millis();
      last_signal = debounce[i];

      if(btn_last_state[i]) {
        if(millis() - btn_last_on[i] < double_time) {
          //double press
          Serial.print("double press ");
          Serial.println(i == 0 ? "up" : "down");
          transitionState(i == 0 ? UpDouble : DownDouble);
        } else {
          btn_last_on[i] = debounce[i];
          //single press
          Serial.print("single press ");
          Serial.println(i == 0 ? "up" : "down");
          transitionState(i == 0 ? UpSingle : DownSingle);
        }
      }// endif pressed
    }
  }
  
  move();
  publishHeight();

}
