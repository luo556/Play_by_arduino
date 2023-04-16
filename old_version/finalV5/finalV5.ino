#include "FastLED.h"                                          // FastLED library.
#include <SoftwareSerial.h>
#include <Wire.h>
#include<math.h>
//#include "DFRobot_TCS34725.h"
#include <Servo.h> 
#include <time.h>
//灯光
#define LED_DT 3                                             // Data pin to connect to the strip.
#define COLOR_ORDER GRB                                      // It's GRB for WS2812 and BGR for APA102.
#define LED_TYPE WS2812                                       // Using APA102, WS2812, WS2801. Don't forget to modify LEDS.addLeds to suit.
#define NUM_LEDS 40                                           // Number of LED's.
//int mode_serial;
int *tep_rec;
uint8_t max_bright = 128;                                     // Overall brightness definition. It can be changed on the fly.
struct CRGB leds[NUM_LEDS]; 

int *count;
//声音
SoftwareSerial voiceserial(10, 11);
char mode_serial;

//颜色传感
//DFRobot_TCS34725 tcs = DFRobot_TCS34725(&Wire, TCS34725_ADDRESS,TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);


//舵机
Servo myservo; 
int pos = 0; 



  void setup() {
  myservo.attach(2); 
  voiceserial.begin(9600);
  Serial.begin(9600);                                        // Initialize serial port for debugging.
  delay(1000);                                                // Soft startup to ease the flow of electrons.
  volume(0x1A);
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);         // For WS2812B
 
  
  FastLED.setBrightness(max_bright);
        // FastLED Power management set at 5V, 500mA
  //myservo.attach(9); 
  //*mode_serial = 0;
}


void loop(){
  play(0x01);
  delay(5000);
  mode_serial = Serial.read();
 // control_duo();

  if(mode_serial == '1'){
    play(0x01);
    light_mode_2();
    
  }
  else if(mode_serial == '2'){
    play(0x02);
    light_mode_2();
  }
  else if(mode_serial == '3'){
    play(0x03);
    light_mode_2();
  }
  else if(mode_serial == '4'){
    play(0x04);
    light_mode_3();
  }
  else if(mode_serial == '5'){
    play(0x05);
    light_mode_3();
  }
  else if(mode_serial == '6'){
    play(0x06);
    light_mode_3();
  }
   else if(mode_serial == '7'){
    play(0x07);
    light_mode_4();
  }
  else if(mode_serial == '8'){
    play(0x08);
    light_mode_4();
  }
  else if(mode_serial == '9'){
    play(0x09);
    light_mode_4();
  }
  else{
   //play(0x09);
    light_mode_1();
       for (pos = 0; pos <= 180; pos ++) { // 0°到180°
    // in steps of 1 degree
    myservo.write(pos);              // 舵机角度写入
    delay(10);                       // 等待转动到指定角度
  }
  for (pos = 180; pos >= 0; pos --) { // 从180°到0°
    myservo.write(pos);              // 舵机角度写入
    delay(10);                       // 等待转动到指定角度
  }
  }  
  
  

  }
  


//mode1 默认 2 胜利 3 失败 4 平局
void light_mode_1() {
  uint8_t bpm = 30;
  uint8_t inner = beatsin8(bpm, NUM_LEDS/4, NUM_LEDS/4*3);    // Move 1/4 to 3/4
  uint8_t outer = beatsin8(bpm, 0, NUM_LEDS-1);               // Move entire length
  uint8_t middle = beatsin8(bpm, NUM_LEDS/3, NUM_LEDS/3*2);   // Move 1/3 to 2/3

  leds[middle] = CRGB::Red;
  leds[inner] = CRGB::Blue;
  leds[outer] = CRGB::Green;

  nscale8(leds,NUM_LEDS,110);                             // Fade the entire array. Or for just a few LED's, use  nscale8(&leds[2], 5, fadeval);
  FastLED.show();
} // dot_beat()
//red


void light_mode_2(){
  fill_solid(leds, 15, CRGB(255,0,0));
  FastLED.show();
  delay(2000);
}
//green
void light_mode_3(){
  fill_solid(leds+16, 15, CRGB(0,255,0));
  FastLED.show();
  delay(2000);
}
//blue
void light_mode_4(){
  fill_solid(leds+29, 10, CRGB(0,0,255));
  FastLED.show();
  delay(2000);
}
void light_mode_5(){
  fill_solid(leds,40, CRGB(255, 255, 255));
  FastLED.show();
  delay(500);
}


void play(unsigned char Track)
{
  unsigned char play[6] = {0xAA,0x07,0x02,0x00,Track,Track+0xB3};//0xB3=0xAA+0x07+0x02+0x00,即最后一位为校验和
   voiceserial.write(play,6);
}
void volume(unsigned char vol)
{
  unsigned char volume[5] = {0xAA,0x13,0x01,vol,vol+0xBE};//0xBE=0xAA+0x13+0x01,即最后一位为校验和
     voiceserial.write(volume,5);
 }
