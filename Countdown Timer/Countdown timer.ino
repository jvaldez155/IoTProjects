/*
 * Project: V02_2_timer
 * Description: timer that counts down from SerialInput value
 * Auther: John Valdez
 * Date: 3-29-2020
 */

 /* ASSIGNMENT 
  *  You will need one button and the Neopixels for this assignment. 
  *  You are going to modify your timer code. 
  *     Create a countdown timer
  *     Use the SerialMonitor to input the starting value for the timer.
  *     Push the button once to start the countdown.
  *     When you get to zero, turn on or flash the NeoPixels show that time has expired
  *     Optional: write to the OLED display or use the NeoPixels to show the last 10 seconds counting down
  */

 
 #include<Wire.h>
 #include<ACROBOTIC_SSD1306.h>
 #include<Adafruit_NeoPixel.h>
 #include<colors.h>
 const int Pin = 17;
 const int NUMPIXELS = 16;

 Adafruit_NeoPixel pixels(NUMPIXELS, Pin, NEO_GRB + NEO_KHZ800);

 const int start_pixel = 0;
 
 int button1=1;
 String readyGo;
 int startTime;
 int elapsedTime;
 float currentTime;
 int lastState;
 String inString;
 int myValue;
 int inChar;
 float countdown;
 String timer;
 int finished;
 String complete;
 int count;
 String readyset;
 String starting;
 float almost;
 int clearall;
 int i;
 int counter;
 int flashes;
 int c;


 void setup() {
  Serial.begin(9600);
  delay(500);
  Serial.println("Serial monitor ready");
  Serial.println("Beginning String to Int(): ");
  inString = ""; // Ensure the string is blank to begin with
 
 // initailize OLED display
 Wire.begin();
  oled.init();
  oled.clearDisplay();

  pixels.begin();
  pixels.clear();
  pixels.show();

  readyset = "Ready,Set...";
  readyGo = " Go!";
  timer = countdown;
  complete = finished;
  starting = almost;
  
  pinMode(button1, INPUT);
  Serial.println("Systems Ready...Wanna Play a Game?");
 }

void loop() {

  while(digitalRead(button1)==LOW){     // Loops and reads the input until the button is pressed
 //   Serial.print(c);Serial.println(myValue); 
    if(c == clearall && c < myValue){
      pixels.clear();
      pixels.show();
    }
    else
    {
      for(i=0;i<myValue;i++){       
        if(i<myValue){
          pixels.setBrightness(50); 
          pixels.fill(maroon,0,16);
          pixels.show();
     //     Serial.print(c);Serial.println(i);Serial.println(myValue);
        }
      }
    }       
    while(Serial.available()>0) {   // Read serial input
      inChar = Serial.read();     
      Serial.print("buttonState");Serial.println(button1);     
        if (isDigit(inChar)) {
          inString+=(char)inChar;     // Convert incoming bute to char and add to string
        }
        if (inChar == '\n') {         // n represents the byte for newline (or enter)          
          myValue=inString.toInt();
          Serial.print("Value:");
          Serial.println(myValue);
          Serial.print("String: ");
          Serial.println(inString);
          inString ="";               //Reset string for next input
          clearall++;
          Serial.print(button1);
          oled_start(readyset, starting);
          pixels.fill(red,0,16);
          pixels.show();        
        }
    }
  } 
    startTime=millis();   //sets startTime to where ever the system clock is
    Serial.println("Timer Starting...");

      while (digitalRead(button1)==HIGH) {  // wait until button is realeased
        Serial.println("Waiting for button to be released");
        Serial.print("buttonState ");Serial.println(button1);
      }
   //   Serial.print("buttonState");Serial.println(button1);
  //   Serial.print("Waiting to press ");
    currentTime = (millis()-startTime)/1000.0;
  //   Serial.println(currentTime);
    countdown = myValue-currentTime;
   
    while(digitalRead(button1)==LOW && countdown > 0 ){  // wait until button is pressed a second time
   // Serial.print("buttonState");Serial.println(button1);
   // Serial.print("Waiting to press ");
      currentTime = (millis()-startTime)/1000.0;
      Serial.println(currentTime);
      countdown = myValue-currentTime; 
        if(countdown > 10) {
          Serial.print("Time left.... "); Serial.println(countdown);
          pixels.setBrightness(60);
          pixels.fill(green,0,16);
          pixels.show();
  
          oled_write(readyGo, timer);
          delay(10);
        }
        else
        {
          if(countdown<10 && countdown>3){
            pixels.setBrightness(40);
            pixels.fill(yellow,0,16);
            pixels.show();
        
            oled_write(readyGo, timer);
            delay(10);
          }
          else
          {
            pixels.clear();
            pixels.show();  
            delay(489);
            pixels.setBrightness(10);
            pixels.fill(yellow,0,16);
            pixels.show();
            delay(489);
          
            oled_write(readyGo, timer);
          }
        }
    }
    
    oled_write2(complete);
        
    counter = 250;    
    for(flashes = 0; flashes<counter; flashes++){ 
      timerfinished();
      delay(50);
    }
   pixels.clear();
   pixels.show();
   c++;
}

void oled_start(String readyset, String starting){
  oled.clearDisplay();
  oled.setTextXY(0,3);
  oled.putString(readyset);
  oled.setTextXY(4,5);
  oled.putString(myValue);
  oled.setTextXY(4,8);
  oled.putString(" secs");
}

void oled_write(String readyGo, String timer){
 // oled.clearDisplay();
  oled.setTextXY(1,6);
  oled.putString(readyGo);
  oled.setTextXY(4,2);
  oled.putString(countdown);
}
void oled_write2(String complete) {
  oled.clearDisplay();
  oled.setTextXY(2,1);
  oled.putString("!!!FINISHED!!!");
  oled.setTextXY(4,3);
  oled.putString("---0.00---");
}

void timerfinished(){
    pixels.setPixelColor(0, random(0,255),random(0,255),random(0,255));
    pixels.setPixelColor(1, random(0,255),random(0,255),random(0,255));
    pixels.setPixelColor(2, random(0,255),random(0,255),random(0,255));
    pixels.setPixelColor(3, random(0,255),random(0,255),random(0,255));
    pixels.setPixelColor(4, random(0,255),random(0,255),random(0,255));
    pixels.setPixelColor(5, random(0,255),random(0,255),random(0,255));
    pixels.setPixelColor(6, random(0,255),random(0,255),random(0,255));
    pixels.setPixelColor(7, random(0,255),random(0,255),random(0,255));
    pixels.setPixelColor(8, random(0,255),random(0,255),random(0,255));
    pixels.setPixelColor(9, random(0,255),random(0,255),random(0,255));
    pixels.setPixelColor(10, random(0,255),random(0,255),random(0,255));
    pixels.setPixelColor(11, random(0,255),random(0,255),random(0,255));
    pixels.setPixelColor(12, random(0,255),random(0,255),random(0,255));
    pixels.setPixelColor(13, random(0,255),random(0,255),random(0,255));
    pixels.setPixelColor(14, random(0,255),random(0,255),random(0,255));
    pixels.setPixelColor(15, random(0,255),random(0,255),random(0,255));
    pixels.setPixelColor(16, random(0,255),random(0,255),random(0,255));
    pixels.show();
  

}
