#include <Adafruit_NeoPixel.h>
#include <OneButton.h>
#include <colors.h>
// Define NeoPixel constants

const int Pin = 17;
const int NUMPIXELS = 19;

Adafruit_NeoPixel pixels(NUMPIXELS, Pin, NEO_GRB + NEO_KHZ800);
OneButton button1(1,false);


const int pixeldelay=50;
const int start_pixel = 0; //the first pixel is 0
int chase = LOW;
int buttonState = false;
int runn;
int onoff;

int counter = 0;
int colorarray[] = {teal, maroon, yellow, green, navy, purple, silver};
int arraysize = 7;
int cycle;

int i = 0; // counter variable

void setup() {
  // initialize Serial Monitor)
  Serial.begin(9600);
  delay(1000);     //pause for Serial Monitor
  Serial.println ("Starting NeoPixel");
  Serial.print("buttonState = ");
  Serial.println(buttonState);

  button1.attachClick(click1);
  button1.attachDoubleClick(doubleclick1);
  button1.attachLongPressStart(longPressStart1);
 // button1.attachLongPressStop(longPressStop1);
  button1.setClickTicks(250);
  button1.setPressTicks(750);
  // initialize NeoPixels
  pixels.begin();
  pixels.clear();
 
 }
void loop() {
   button1.tick();
  // for loop to reference each pixel
  for(i=0; i<NUMPIXELS; i++) {
    
   if (onoff = buttonState) {
    Serial.println("Lighting up pixel ");
    Serial.println (i);
    pixels.fill(colorarray[counter],0,16);
    button1.tick();
    pixels.setBrightness (30);
    pixels.show();     
    button1.tick();
    } 
    else 
    {
      pixels.clear();
      pixels.show();
      button1.tick();
      }
 
    if (runn = chase) {
      pixels.setBrightness (30);
      pixels.setPixelColor(i,colorarray[cycle]);
      pixels.setPixelColor(19-i,colorarray[cycle]);
      pixels.show();
    button1.tick();
    }
    delay(pixeldelay);    //Pause befoe next pass through loop
  }
}

 void click1() {
 buttonState = !buttonState;
 Serial.print("buttonState = ");
 Serial.println(buttonState);
}
  
void doubleclick1 () {
  Serial.print("chase = ");
  Serial.println(chase);
  if(chase = !chase){
    cycle++;}
  if(cycle == arraysize){
    cycle = 0;
  }
}

void longPressStart1() {
  counter++;
  if(counter == arraysize){
    counter = 0;
  }
}
