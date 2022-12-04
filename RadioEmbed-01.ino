//https://github.com/VolosR/TEmbedFMRadio
//https://www.youtube.com/watch?v=bg2Ysrh85Ek&t=12s

#include <Wire.h>
#include <TEA5767.h> //https://github.com/big12boy/TEA5767
//#include <FastLED.h>
//#include "TFT_eSPI.h"
#include <RotaryEncoder.h> //https://github.com/mathertel/RotaryEncoder
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735

//TFT_eSPI tft = TFT_eSPI();
//TFT_eSprite spr = TFT_eSprite(&tft);

#define PIN_IN1 4
#define PIN_IN2 3

//#define NUM_LEDS 7
//#define DATA_PIN 42
//#define CLOCK_PIN 45
//CRGB leds[NUM_LEDS];

RotaryEncoder encoder(PIN_IN1, PIN_IN2, RotaryEncoder::LatchMode::TWO03);

#define TFT_CS         6
  #define TFT_RST        7                                            
  #define TFT_DC         8
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

#define color1 0xC638
#define color2  0xC638

int value=980;
int minimal=880;
int maximal=1080;
int strength=0;
String sta[6]={"96.6","101.0","89.4","106,5","98.2","92.4"};

float freq=0.00;
TEA5767 radio = TEA5767();

bool muted=0;
int deb=0;

void setup() {
  
 // pinMode(46, OUTPUT);
 // digitalWrite(46, HIGH);

  //tft.begin();
  tft.initR(INITR_BLACKTAB);
 // tft.writecommand(0x11);
  tft.fillScreen(ST77XX_BLACK);
  tft.setRotation(3);
 // tft.fillScreen(TFT_BLACK);

 // pinMode(15, OUTPUT);
 // digitalWrite(15, HIGH);

  pinMode(2, INPUT_PULLUP);
 //// FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);

  Wire.begin();
    

 // spr.createSprite(320,170);
 // spr.setTextDatum(4);
 // spr.setSwapBytes(true);
 // spr.setFreeFont(&Orbitron_Light_24);
 // spr.setTextColor(color1,TFT_BLACK);

 // leds[0] = CRGB::Red;
//  leds[1] = CRGB::White;
 // leds[2] = CRGB::Red;
 // leds[3] = CRGB::Green;
 // leds[4] = CRGB::Red;
 // leds[5] = CRGB::Blue;
 // leds[6] = CRGB::Red;
  //FastLED.show();
  drawSprite();

}

void readEncoder() {

  static int pos = 0;
  encoder.tick();

  if(digitalRead(2)==0){
    if(deb==0){
      deb=1;
      muted=!muted;
      radio.setMuted(muted);
      drawSprite();
      delay(200);
    }
  }else deb=0;
  
  int newPos = encoder.getPosition();
  if (pos != newPos) {
    
    if(newPos>pos)
    value=value-1;
      if(newPos<pos)
      value=value+1;
    
    pos = newPos;
    
    drawSprite();
  } 

}

void drawSprite()
{
freq=value/10.00;
if(muted==false)
radio.setFrequency(freq);

strength=radio.getSignalLevel();
tft.fillScreen(ST77XX_BLACK);
tft.setTextColor(ST77XX_CYAN,ST77XX_BLACK);
tft.setTextSize(3);
tft.setCursor(5, 25);
tft.print(freq);
tft.setTextSize(1);
tft.setCursor(130,30); tft.print("MHz");
//,1,160,64,7);
//spr.setFreeFont(&Orbitron_Light_24);
tft.setCursor(5,3);
tft.setTextColor(ST77XX_ORANGE,ST77XX_BLACK);
tft.print("TEA5767 - FM Radio");
tft.setTextColor(ST77XX_WHITE,ST77XX_BLACK);
//tft.setCursor(38, 14);
//tft.print("STATIONS");
//tft.drawRoundRect(1,1,76,110,4,0xAD55); //chenar
tft.drawRoundRect(93,71,64,16,4,ST77XX_WHITE);

tft.drawRect(290,6,20,9,ST77XX_WHITE);
tft.fillRect(291,7,12,7,0x34CD);
tft.fillRect(310,8,2,5,ST77XX_WHITE);

//tft.setTextFont(0);
tft.setTextColor(0xBEDF,ST77XX_BLACK);
//for(int i=0;i<6;i++){
//  tft.setCursor(38, 32+(i*12));
//tft.print(sta[i]);
//tft.fillCircle(16,31+(i*12),2,0xFBAE);
//}
tft.setTextColor(ST77XX_WHITE,ST77XX_BLACK);
tft.setCursor(5,64);
tft.print("SEMNAL:");
tft.setCursor(5,82);
tft.print("MUTE");
tft.fillRoundRect(40,76,20,20,3,0xCC40);

if(muted==1)
tft.fillCircle(50,85,6,ST77XX_WHITE);

//nivel semnal
for(int i=0;i<strength;i++)
tft.fillRect(60+(i*4),65-(i*1),2,4+(i*1),0x3526);



tft.fillTriangle(76,84,80,94,84,84,ST77XX_RED);

  
 int temp=value-20;
 for(int i=0;i<40;i++)
 {
 if((temp%10)==0){
  tft.drawLine(i*4,128,i*4,110,color1);
  
  tft.drawLine((i*4)+1,128,(i*4)+1,110,color1);
  tft.setCursor(i*4,98);
  //tft.print(temp/10.0,1,i*8,130,2);
  tft.print (temp/10.0);
  }
 else if((temp%5)==0 && (temp%10)!=0)
 {tft.drawLine(i*4,128,i*4,115,color1);
 tft.drawLine((i*4)+1,128,(i*4)+1,115,color1);
 //spr.drawFloat(temp/10.0,1,i*8,144);
 }
 else
  {tft.drawLine(i*4,128,i*4,118,color1);}
 
  temp=temp+1;
 }
 tft.setCursor(100, 75);
tft.print("Stereo: "+String(radio.isStereo()));




tft.drawLine(80,84,80,128,ST77XX_RED);
//tft.pushSprite(0,0);

}

void loop() { 

  readEncoder();
}
