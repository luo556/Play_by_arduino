#include "FastLED.h"                                          // FastLED library.
#include <SoftwareSerial.h>
#include <Wire.h>
//#include "DFRobot_TCS34725.h"
#include <Servo.h> 
#include <time.h>
//灯光
#define LED_DT 3                                             // Data pin to connect to the strip.
#define COLOR_ORDER BGR                                      // It's GRB for WS2812 and BGR for APA102.
#define LED_TYPE WS2812                                       // Using APA102, WS2812, WS2801. Don't forget to modify LEDS.addLeds to suit.
#define NUM_LEDS 60                                           // Number of LED's.
int *mode_serial;
int *tep_rec;
uint8_t max_bright = 128;                                     // Overall brightness definition. It can be changed on the fly.
struct CRGB leds[NUM_LEDS]; 

int *count
//声音
SoftwareSerial voiceserial(10, 11);


//颜色传感
//DFRobot_TCS34725 tcs = DFRobot_TCS34725(&Wire, TCS34725_ADDRESS,TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);


//舵机
Servo myservo; 
int pos = 0; 



void setup() {
  myservo.attach(9); 
  voiceserial.begin(9600);
  Serial.begin(9600);                                        // Initialize serial port for debugging.
  delay(1000);                                                // Soft startup to ease the flow of electrons.
  volume(0x1A);
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);         // For WS2812B
 
  
  FastLED.setBrightness(max_bright);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);               // FastLED Power management set at 5V, 500mA
  //myservo.attach(9); 
}


void loop(){
  *mode_serial = Serial.read();
  if(*mode_serial == '4'){
    *tep_rec = 1;
  }

//默认状态
if(*tep_rec ==0){
      
   light_mode_1();
   play(0x03);
   }

   
  if( *tep_rec== 1){
    if(*mode_serial == '1'){
      light_mode_2();
      play(0x06);
      delay(2000);
      *tep_rec = 0;
    }
    
    else if(*mode_serial == '2'){
    light_mode_3();;
       play(0x01);
       delay(2000);
      *tep_rec = 0
    }
    else if(*mode_serial == '3'){
 
      light_mode_4();
      play(0x02);
       delay(2000);
      *tep_rec = 0
    }
   else if(*mode_serial == '0'){
 
      light_mode_5();
      play(0x02);
      *tep_rec = 0;
      *count--;
    }
    if( *count==0){
      *count = 200;
      *tep_rec = 0;
    }
  
    }

  }
  


//mode1 默认 2 胜利 3 失败 4 平局
void light_mode_1(){
fill_solid(leds, 60, CHSV(30,255, 255));
  FastLED.show();
}
void light_mode_2(){
  fill_solid(leds, 60, CHSV(30,255, 255));
  FastLED.show();
}
void light_mode_3(){
  fill_solid(leds, 60, CHSV(50,255, 255));
  FastLED.show();
}
void light_mode_4(){
  fill_solid(leds, 60, CHSV(90,255, 255));
  FastLED.show();
}
void light_mode_4(){
  fill_solid(leds, 60, CHSV(90,255, 255));
  FastLED.show();
}


void play(unsigned char Track)
{
  unsigned char play[6] = {0xAA,0x07,0x02,0x00,Track,Track+0xB3};//0xB3=0xAA+0x07+0x02+0x00,即最后一位为校验和
   voiceserial.write(play,6);
}
void volume( unsigned char vol)
{
  unsigned char volume[5] = {0xAA,0x13,0x01,vol,vol+0xBE};//0xBE=0xAA+0x13+0x01,即最后一位为校验和
     voiceserial.write(volume,5);
 }

void duoji(){
      for(pos = 0; pos < 180; pos += 1){    // 舵机从0°转到180°，每次增加1°          
      myservo.write(pos);           // 给舵机写入角度   
      delay(15);                    // 延时15ms让舵机转到指定位置
   }
    for(pos = 180; pos>=1; pos-=1) {    // 舵机从180°转回到0°，每次减小1°                               
       myservo.write(pos);        // 写角度到舵机     
       delay(15);                 // 延时15ms让舵机转到指定位置
    } 
}
