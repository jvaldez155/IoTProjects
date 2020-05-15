/*
 * Project CapstoneProject
 * Description:
 * Author: John Valdez
 * Date: 5/5/2020
 */
#include <Seeed_HM330X.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <JsonParserGeneratorRK.h>
#include <OneButton.h>
#include <SparkFun_Qwiic_Twist_Arduino_Library.h>

/****************SetUp HM330X**************/
HM330X sensor;
uint8_t buf[30];
int PM1;
int PM25;
int PM10;
int c = 0;
const char* str[] = {"sensor num: ", "PM1.0 concentration(CF=1,Standard particulate matter,unit:ug/m3): ",
                     "PM2.5 concentration(CF=1,Standard particulate matter,unit:ug/m3): ",
                     "PM10 concentration(CF=1,Standard particulate matter,unit:ug/m3): ",
                     "PM1.0 concentration(Atmospheric environment,unit:ug/m3): ",
                     "PM2.5 concentration(Atmospheric environment,unit:ug/m3): ",
                     "PM10 concentration(Atmospheric environment,unit:ug/m3): ",
                    };

HM330XErrorCode print_result(const char* str, uint16_t value) {
    if (NULL == str) {
        return ERROR_PARAM;
    }
    Serial.print(str);
    Serial.println(value);
    return NO_ERROR1;
}

/*parse buf with 29 uint8_t-data*/
HM330XErrorCode parse_result(uint8_t* data) {
    uint16_t value = 0;
    if (NULL == data) {
        return ERROR_PARAM;
    }
    for (int i = 1; i < 8; i++) {
        value = (uint16_t) data[i * 2] << 8 | data[i * 2 + 1];
        print_result(str[i - 1], value);
    }
    return NO_ERROR1;
}

HM330XErrorCode parse_result_value(uint8_t* data) {
    if (NULL == data) {
        return ERROR_PARAM;
    }
    for (int i = 0; i < 28; i++) {
        Serial.print(data[i], HEX);
        Serial.print("  ");
        if ((0 == (i) % 5) || (0 == i)) {
            Serial.println("");
        }
    }
    uint8_t sum = 0;
    for (int i = 0; i < 28; i++) {
        sum += data[i];
    }
    if (sum != data[28]) {
        Serial.println("wrong checkSum!!!!");
    }
    Serial.println("");
    return NO_ERROR1;
}
/********************** SetUp Oled Display **********************/
#define OLED_ADDR  0x3C
Adafruit_SSD1306 display(-1);     // the -1 means there is no reset pin
#define SSD1306_128_64
String pm1;
String pm25;
String pm10;
/*********************** OneButton SetUP  *************************/
OneButton menu(A5, false);
OneButton menuB(A4, false);
/************Declare Variables*************/
TWIST twist;
int encPosition;//= twist.getCount();
int status;
int ledR = D6;
int ledY = D7;
int ledG = D8;
int buzzer = D5;
int pm1Llast = 50;
int pm1Hlast = 100;
int pm25Llast = 50;
int pm25Hlast = 100;
int pm10Llast = 50;
int pm10Hlast = 100;
bool menuState = false;
int pm1L;
int pm1H;
int pm25L;
int pm25H;
int pm10L;
int pm10H;
byte menuCount = 0;

void setup() {
  Serial.begin(9600);
  delay(1000);
  Serial.println("----------------------------------------------------------");

  Serial.println("Serial start");
  if (sensor.init()) {
    Serial.println("HM330X init failed!!!");  digitalWrite(buzzer, HIGH);
            Serial.println("lalala");
    while (1);
    }
    Wire.begin();
    display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
    display.clearDisplay();
    display.display();
    display.setRotation(2);

    display.setCursor(3,0);
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.print("Dust Reading");
    display.display();

    menu.attachClick(menuDown);
    menu.attachDuringLongPress(menuOpen);
    menu.setPressTicks(1500);
    menuB.attachClick(menuUp);
    twist.begin();
 pm1L = 50;
 pm1H = 100;
 pm25L = 50;
 pm25H = 100;
 pm10L = 50;
 pm10H = 100;
 pm1Llast = 50;
 pm1Hlast = 100;
 pm25Llast = 50;
 pm25Hlast = 100;
 pm10Llast = 50;
 pm10Hlast = 100;
twist.setCount(pm1Llast);
encPosition = twist.getCount();
    pm1 = "PM1";
    pm25 = "PM25";
    pm10 = "PM10";
    pinMode(ledR, OUTPUT);
    pinMode(ledY,OUTPUT);
    pinMode(ledG,OUTPUT);
    pinMode(buzzer,OUTPUT);
    delay(2000);
}

void loop() {
    for(int b = 0; b < 120; b++) {
        if (sensor.read_sensor_value(buf, 29)) {
            Serial.println("HM330X read result failed!!!");
            digitalWrite(buzzer, HIGH);
            Serial.println("lalala");
        }
        Serial.println("Starting Display");
        parse_result_value(buf);
        Serial.println("Printing Concentrations");
        parse_result(buf);
        Serial.println("PM2.5 Data");
        PM1 = (uint16_t) buf[5 *2] << 8 | buf[5 * 2 +1];
        PM25 = (uint16_t) buf[6 * 2] << 8 | buf[6 * 2 + 1];
        PM10 = (uint16_t) buf[7 *2] << 8 | buf[7 * 2 + 1];
        print_result(str[6 - 1], PM25); menu.tick();Parameters();
        Serial.println("");Serial.println(PM1);Serial.println(PM25);Serial.println(PM10);
        printDust(pm1,pm25,pm10);
        if(PM25 > 0 && PM25 <= pm25L) {
            digitalWrite(ledR, LOW);
            digitalWrite(ledY, LOW);
            digitalWrite(ledG, HIGH);
            digitalWrite(D5, LOW);menu.tick();Parameters();
        }
        if(PM25 > pm25L && PM25 <= pm25H) {
            digitalWrite(ledR, LOW);
            digitalWrite(ledY, HIGH);
            digitalWrite(ledG, LOW);
            digitalWrite(D5, LOW);menu.tick();Parameters();
        }
        if(PM25 > pm25H) {
            digitalWrite(ledR, HIGH);
            digitalWrite(ledY, LOW);
            digitalWrite(ledG, LOW);
            digitalWrite(D5, HIGH);menu.tick();
            Serial.print("lastPm25h");Serial.println(pm25H);
        }Serial.println(menuCount);Parameters();
        delay(500);
    }menu.tick();Parameters();
    createEventPayLoad (PM1, PM25, PM10) ;
}

void printDust(String pm1, String pm25, String pm10) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.print("PM1.0: ");
    display.setTextSize(2);
    display.print(PM1);
    display.setTextSize(1);
    display.println("ug/m3");
    display.setCursor(0,25);
    display.print("PM2.5: ");
    display.setTextSize(2);
    display.print(PM25);
    display.setTextSize(1);
    display.println("ug/m3");
    display.setCursor(0,50);
    display.print("PM10:  ");
    display.setTextSize(2);
    display.print(PM10);
    display.setTextSize(1);
    display.println("ug/m3");
    display.display();
}

void createEventPayLoad (int p1 , int p25 , int p10 ) {
  JsonWriterStatic<256> jw; {
    JsonWriterAutoObject obj(&jw);

    jw.insertKeyValue("Dust1", p1);
    jw.insertKeyValue("Dust25", p25);
    jw.insertKeyValue("Dust10", p10);
  }
  Particle.publish("DustReadings", jw.getBuffer(),PRIVATE) ;
}

void Parameters() {
    encPosition = twist.getCount();
    while(menuState == true){
        Serial.println("Im in the menu");
        menuCheck();
        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(10, 0);
        display.print("pm1.0 Low:");
        display.setCursor(75, 0);
        display.println(pm1Llast);
    menu.tick();menuB.tick();
        display.setCursor(10, 10);
        display.println("pm1.0 High:");
        display.setCursor(75, 10);
        display.println(pm1Hlast);
    menu.tick();menuB.tick();
        display.setCursor(10, 20);
        display.println("pm2.5 Low:");
        display.setCursor(75, 20);
        display.println(pm25Llast);
    menu.tick();menuB.tick();
        display.setCursor(10, 30);
        display.print("pm2.5 High:");
        display.setCursor(75, 30);
        display.println(pm25Hlast);
    menu.tick();menuB.tick();
        display.setCursor(10, 40);
        display.println("pm10 Low:");
        display.setCursor(75, 40);
        display.println(pm10Llast);
    menu.tick();menuB.tick();
        display.setCursor(10, 50);
        display.println("pm10 High:");
        display.setCursor(75, 50);
        display.println(pm10Hlast);
    menu.tick();menuB.tick();
        display.setCursor(2, menuCount * 10);
        display.println(">");
        display.display();
    }
}

void menuCheck() {
  if (menuCount < 0) {
    menuCount = 5;
  }
  if (menuCount >= 6) {
    menuCount = 0;
  }
  if (menuCount == 0) {encPosition = twist.getCount();  
    encPosition = twist.getCount();
    pm1L = encPosition;
    pm1Llast = pm1L;
  }
  if (menuCount == 1) {encPosition = twist.getCount();  
    encPosition = twist.getCount();
    pm1H = encPosition;
    pm1Hlast = pm1H;
  }
  if (menuCount == 2) {encPosition = twist.getCount();  
    encPosition = twist.getCount();
    pm25L = encPosition;
    pm25Llast = pm25L;
  }
  if (menuCount == 3) {encPosition = twist.getCount();  
    encPosition = twist.getCount();
    pm25H = encPosition;
    pm25Hlast = pm25H;
  }
  if (menuCount == 4) {encPosition = twist.getCount(); 
    encPosition = twist.getCount();
    pm10L = encPosition;
    pm10Llast = pm10L;
  }
  if (menuCount == 5) {encPosition = twist.getCount();  
    encPosition = twist.getCount();
    pm10H = encPosition;
    pm10Hlast = pm10H;
  }
}

void menuDown() {menuCount++;
    if(menuCount > 5) menuCount = 0;
    switch(menuCount){
        case 0: 
            twist.setCount(pm1Llast);
            break;
        case 1: 
            twist.setCount(pm1Hlast);
            break;
         case 2: 
            twist.setCount(pm25Llast);
            break;
        case 3: 
            twist.setCount(pm25Hlast);
            break;
         case 4: 
            twist.setCount(pm10Llast);
            break;
        case 5: 
            twist.setCount(pm10Hlast);
            break;      
    }
}
void menuUp() {menuCount--;
    if(menuCount < 0) menuCount = 5;
    switch(menuCount){
        case 0: 
            twist.setCount(pm1Llast);
            break;
        case 1: 
            twist.setCount(pm1Hlast);
            break;
         case 2: 
            twist.setCount(pm25Llast);
            break;
        case 3: 
            twist.setCount(pm25Hlast);
            break;
         case 4: 
            twist.setCount(pm10Llast);
            break;
        case 5: 
            twist.setCount(pm10Hlast);
            break;      
    }
}
void menuOpen() {
    menuState = !menuState;
    if(menuState == true){
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(40,20);
    display.println("MENU");
    display.display();}
    else
    {   display.clearDisplay();
        display.setCursor(30,20);
        display.setTextSize(2);
        display.println("RETURN");
        display.display();}
    delay(1500);}
