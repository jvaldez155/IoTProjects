/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/Users/John/Documents/IoTClass/particle-02a-jvaldez155/MoistureSensor/src/MoistureSensor.ino"
/*
 * Project MoistureSensor
 * Description: Take readings from Sensor and display on OLED
 * Author: John Valdez
 * Date: 4/14/2020
 */

#include<Particle.h>
#include<Wire.h>
#include<Adafruit_SSD1306.h>
#include<Adafruit_GFX.h>

#include <Adafruit_MQTT.h>

// This #include statement was automatically added by the Particle IDE. 
#include "Adafruit_MQTT/Adafruit_MQTT.h" 
#include "Adafruit_MQTT/Adafruit_MQTT_SPARK.h" 
#include "Adafruit_MQTT/Adafruit_MQTT.h" 

void setup();
void loop();
void checkTime();
void printDisplay(String reading);
void buttonState();
#line 20 "c:/Users/John/Documents/IoTClass/particle-02a-jvaldez155/MoistureSensor/src/MoistureSensor.ino"
#define OLED_ADDR  0x3C
Adafruit_SSD1306 display(-1);     // the -1 means there is no reset pin

#define SSD1306_128_64

/************************* Adafruit.io Setup *********************************/ 
#define AIO_SERVER      "io.adafruit.com" 
#define AIO_SERVERPORT  1883                   // use 8883 for SSL 
#define AIO_USERNAME  "jvaldez155"
#define AIO_KEY       "aio_lYlK40eeCJC7hH9ce04e8Brky4dD"


/************ Global State (you don't need to change this!) ***   ***************/ 
TCPClient TheClient;   

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details. 
Adafruit_MQTT_SPARK mqtt(&TheClient,AIO_SERVER,AIO_SERVERPORT,AIO_USERNAME,AIO_KEY); 


/****************************** Feeds ***************************************/ 
// Setup a feed called <object> for publishing. 
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname> 

Adafruit_MQTT_Publish M1 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/<Moisture");
Adafruit_MQTT_Publish T1 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/<Temperture");

int delayTime=1000;
int sensor = A5;
char currentTime[25];
char current[9];
String t;
String t2;
String reading;
int moisture;
int Moisture;

Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/LED_On");
int button;


// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.
  Serial.begin(9600);
  mqtt.subscribe(&onoffbutton);
  pinMode(A2,OUTPUT);
  pinMode(sensor,INPUT);
  Wire.begin();

  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.display();
  reading = "moisture";
  delay(delayTime);
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // The core of your code will likely live here.
  //moisture = analogRead(sensor);
  Moisture = analogRead(sensor);;
  if(mqtt.Update()){
      M1.publish(Moisture);
  }
      
  for(int b=0; b<120;b++){
    moisture = analogRead(sensor);
//    Serial.println(analogRead(sensor));
//    checkTime();
//    printDisplay(reading);
 /*   Adafruit_MQTT_Subscribe *subscription;
    while ((subscription = mqtt.readSubscription(950))) {  // do this loop for 1 seconds
      if (subscription == &onoffbutton) {
        button = atoi((char *)onoffbutton.lastread);   //convert adafruit string to int
        Serial.printf("Button State is %i",button);
        analogWrite(A2,button);
      }
    }*/
    buttonState();
    analogRead(sensor);
    Serial.println(analogRead(sensor));
    checkTime();
    printDisplay(reading);
      
    
  }
}

void checkTime() {
  t=Time.timeStr();
  t.toCharArray(currentTime,25);
  t2=t.substring(11,19);
  t2.toCharArray(current,9);
    Time.zone(-6);    // Set Time Zone to MDT (UTC - 7)
    unsigned long cur = millis();
    // Request time sync from Particle Device Cloud
    Particle.syncTime();
    //Wait to receive time from Particle Device Cloud
    waitUntil(Particle.syncTimeDone);
    // Check if synchronized successfully
    if(Particle.timeSyncedLast() >= cur){
      // Print current time
      Serial.printlnf("Current time is %s",current);//Time.timeStr().c_str());
    }      
}
void printDisplay(String reading) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(2,6);
  display.print("Moisture =");
  display.setCursor(35,25);
  display.print(moisture);display.println("");
  display.setCursor(2,50);
  display.print("@ ");
  display.println(current);
  display.display();
}

void buttonState(){
     Adafruit_MQTT_Subscribe *subscription;
    while ((subscription = mqtt.readSubscription(950))) {  // do this loop for 1 seconds
      if (subscription == &onoffbutton) {
        button = atoi((char *)onoffbutton.lastread);   //convert adafruit string to int
        Serial.printf("Button State is %i",button);
        analogWrite(A2,button);
          }
        }
}