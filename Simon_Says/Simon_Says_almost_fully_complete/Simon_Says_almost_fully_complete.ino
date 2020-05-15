/*
 * Project: Simon Says nearly finished
 * Author: John Valdez
 * Date: 4-7-2020
 */
#include<Adafruit_NeoPixel.h>
#include<colors.h>
#include<Adafruit_GFX.h>
#include<Adafruit_SSD1306.h>
#include<Fonts/FreeSerifBoldItalic9pt7b.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET 4
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, & Wire, OLED_RESET);

#define LED_PIN 17
#define NUM_PIXELS 16 
Adafruit_NeoPixel pixels(NUM_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

const int buttonB = 1;
const int buttonR = 4;
const int buttonG = 5;
const int buttonY = 7;

int simonArray[15];
int s;
int userArray[15];
int u;
int colorArray[] = {blue,red,green,yellow};
int c;
int i;
int k;
int setMax = 15;
bool lose;
bool buttonState;

char message[]=("TO START, PRESS RED BUTTON");
char hello[]=("HELLO IM SIMON, LETS PLAY A GAME");
int x;

void setup() {
  // put your setup code here, to run once:
  if(!display.begin(SSD1306_SWITCHCAPVCC,0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't procede, loop forever
  }
  pixels.begin();
  pixels.clear();
  pixels.show();

  Serial.begin(9600);
  delay(100);
  Serial.println("Ready to Start");

  pinMode(buttonB,INPUT);
  pinMode(buttonR,INPUT);
  pinMode(buttonG,INPUT);
  pinMode(buttonY,INPUT);
 

  randomSeed(analogRead(0));      // use floating input to seed the random number generator
  Serial.print("Random Seed = ");Serial.println(analogRead(0));

  Wire.begin();
  display.setTextColor(SSD1306_WHITE);
  x = display.width();
  display.clearDisplay();
  display.display();
  display.setRotation(2);
  
  i = 0;
  lose = false;
  x=150;
  while(digitalRead(buttonR) == false) {
    pixels.clear();
    pixels.show();   
    preGame();
    Serial.println(digitalRead(buttonR));
  }
  while(digitalRead(buttonR)==true){
    x=150;
  }
  for(int s=0;s<425;s++){
    startGame();
    if(digitalRead(buttonR)==true){
      s=425;
    }
  }
  beginGame();
  First();
}

void loop() {
  // put your main code here, to run repeatedly:
  pixels.clear();
  pixels.show();
  delay(1000);

  simonArray[i] = random(0,4);          // add random color to simonArray
  displaySimon(simonArray,i+1);         // display sequence on neoPixels for user to follow

  display.clearDisplay();
  display.display();
  
  for(k=0;k<i+1;k++) {                  // for loop that has user follow all the colors displayed above
    userArray[k] = -1;                  // initialize userArray element to an invalid entry
    while(userArray[k]==-1) {           // while element is still invalid, wait for user input from getinput()
      userArray[k]=getinput();
    }
    delay(250);
    Serial.print("Button ");
    Serial.print(userArray[k]);
    Serial.println(" was pressed");
    if(userArray[k]!=simonArray[k]) {   // compare user input to corresponding element in simonArray
      Serial.println("You loose!");
      endGame();
      lose=true;
      break;                            // if user enters invalid entry, break out of FOR loop 
    }
    else {
      Serial.println("Correct");
    }
  }

  if(i==setMax-1 || lose==true) {      // if user reaches max or enters invalid entry then reset game
    displayReset(lose);
    lose=false;
    for(k=0;k<5;k++) {
      simonArray[k]=0;
      i=0;
    }
  }
  else {
    i++;
  }
}
    
void preGame(){
  display.clearDisplay();
  display.setFont(&FreeSerifBoldItalic9pt7b);
  display.setTextWrap(false);
  int minX = -21*strlen(message);
  display.setCursor(x,25);
  display.setTextSize(2);  
  display.setTextColor(SSD1306_WHITE);
  display.print(message);
  display.display();
  x=x-2; 
  if(x<minX)x=display.width();
}

void startGame(){
  display.clearDisplay();
  display.setFont();
  display.setTextWrap(false);
  int minX = -35*strlen(hello);
  display.setTextSize(3);
  display.setCursor(x,5);
  display.print(hello);
  display.display();
  x=x-2;
  if(x<minX)x=display.width();
}

void beginGame(){
  display.clearDisplay();
  display.display();
  for(c=0; c<4;c++){
    for(int p=0; p<17;p++){
      pixels.fill(colorArray[c],p,6);
      pixels.setBrightness(20);
      pixels.show();
      delay(20);
      pixels.clear();
      pixels.show();
      delay(10);
    }
    delay(200);
  }
}

void First() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(13,0);
  display.println(" I'LL GO");
  display.print("   FIRST");
  display.display();
  delay(1750);
}

void displaySimon(int simon[15], int n) {
  int j;
  for(j=0;j<n;j++){
    pixels.clear();
    switch(simon[j]) {
      case 0:
        pixels.setPixelColor(0,blue);        // note, my pixels are hooked up after the PixelRing, so they are 15->12 instead of 0->3
        break;
      case 1:
        pixels.setPixelColor(1,red);
        break;
      case 2:
        pixels.setPixelColor(2,green);
        break;
      case 3:
        pixels.setPixelColor(3,yellow);
        break;
    }
    pixels.show();
    delay(500);
    pixels.clear();
    pixels.show();
    delay(300);
  }
}

int getinput() {                                 // see if a button is pressed
  int userinput=-1;                              // default, return invalid entry if no button is pressed
  if(digitalRead(buttonB)==true) {
    userinput=0;
    Serial.println("Blue was pressed");
    pixels.setPixelColor(0,blue);
  }
  if(digitalRead(buttonR)==true) {
    userinput=1;
    Serial.println("Red was pressed");
    pixels.setPixelColor(1,red);  
  }
  if(digitalRead(buttonG)==true) {
    userinput=2;
    Serial.println("Green was pressed");
    pixels.setPixelColor(2,green);  
  }
  if(digitalRead(buttonY)==true) {
    userinput=3;
    Serial.println("Yellow was pressed");
    pixels.setPixelColor(3,yellow);  
  }
  pixels.show();
  pixels.clear();
  return userinput;
}

void displayReset(bool lose) {                        // reset game after displaying message based on winning or losing
  int j;
  int thecolor;
  if(lose==true) {
    thecolor=maroon;
    Serial.println("Resetting");
    for(j=0;j<4;j++) {                                  // flash green for winning, orange for losing
      pixels.fill(thecolor,0,16);
      pixels.show();
      delay(500);
      pixels.clear();
      pixels.show();
      delay(500);
    }
  }
  else {
    Serial.println("CONGRATULATIONS - YOU WIN!!!!!");
    winner();
    for(j=0;j<200;j++){
      youWIN();
      delay(50);
    }
  }
  x=100;
  while(digitalRead(buttonR) == false) {
    pixels.clear();
    pixels.show();   
    preGame();
    Serial.println(digitalRead(buttonR));
  }
  while(digitalRead(buttonR)==true){
    x=150;
  }
  for(int s=0;s<425;s++){
    startGame();
    if(digitalRead(buttonR)==true){
      s=425;
    }
  }
  beginGame();
  First();
}

void endGame(){
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10,6);
  display.println("YOU LOSE!");
  display.display();
}
void winner(){
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(6,6);
  display.println("!!WINNER!!");
  display.display();
}

void youWIN(){
 // pixels.setBrightness(15);
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
  pixels.setBrightness(10);
  pixels.show();
}
