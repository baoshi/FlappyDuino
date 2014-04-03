#include <avr/pgmspace.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

 
 
// pin definition for the LCD
#define cs   10
#define dc   8
#define rst  9  

#define TOP_MARGIN 16
#define MAX_X 159
#define MAX_Y 127

#define SKY_COLOR 0x065F

volatile boolean new_reading = 0;
volatile int distance = 0;

#define bird_x 30 // X position of the bird
#define bird_w 18
#define bird_h 14
// Bird_down, 18x14
uint8_t bird_down_image0[8]PROGMEM = // 6, 4
  {0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00};
uint8_t bird_down_image1[18] PROGMEM = // 4, 9
  {0X00,0X00,0X00,0X00,0XE0,0XFF,0XE0,0XFF,0X00,0X00,0XFF,0XFF,0X00,0X00,0X00,0X00,0X00,0X00};
uint8_t bird_down_image2[20] PROGMEM = // 3, 10
  {0X00,0X00,0XE0,0XFF,0X00,0X00,0XE0,0XFF,0XE0,0XFF,0X00,0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X00,0X00};
uint8_t bird_down_image3[28] PROGMEM = // 1, 14
  {0X00,0X00,0X00,0X00,0XE0,0XFF,0XE0,0XFF,0XE0,0XFF,0X00,0X00,0XE0,0XFF,0X00,0X00,0XFF,0XFF,0X00,0X00,0X00,0X00,0XFF,0XFF,0X00,0X00,0X00,0X00};
uint8_t bird_down_image4[30] PROGMEM = // 0, 15
  {0X00,0X00,0XE0,0XFF,0XE0,0XFF,0XE0,0XFF,0XE0,0XFF,0XE0,0XFF,0XE0,0XFF,0X00,0X00,0X00,0X00,0XFF,0XFF,0X00,0X00,0X00,0X00,0XFF,0XFF,0XFF,0XFF,0X00,0X00};
uint8_t bird_down_image5[30] PROGMEM = // 0, 15
  {0X00,0X00,0XE0,0XFF,0XE0,0XFF,0XE0,0XFF,0XE0,0XFF,0XE0,0XFF,0XE0,0XFF,0XE0,0XFF,0X00,0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X00,0X00};
uint8_t bird_down_image6[34] PROGMEM = // 0, 17
  {0X00,0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE0,0XFF,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00};
uint8_t bird_down_image7[36] PROGMEM = // 0, 18
  {0X00,0X00,0X07,0XFF,0X07,0XFF,0X07,0XFF,0X07,0XFF,0X07,0XFF,0X07,0XFF,0XE0,0XFF,0X00,0X00,0X89,0XF3,0X89,0XF3,0X89,0XF3,0X89,0XF3,0X89,0XF3,0X89,0XF3,0X89,0XF3,0X89,0XF3,0X00,0X00};
uint8_t bird_down_image8[36] PROGMEM = // 0, 18
  {0X00,0X00,0X07,0XFF,0X07,0XFF,0X07,0XFF,0X07,0XFF,0X07,0XFF,0X07,0XFF,0XE0,0XFF,0X00,0X00,0X89,0XF3,0X89,0XF3,0X89,0XF3,0X89,0XF3,0X89,0XF3,0X89,0XF3,0X89,0XF3,0X89,0XF3,0X00,0X00};
uint8_t bird_down_image9[32] PROGMEM = // 1, 16
  {0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XE0,0XFF,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00};
uint8_t bird_down_image10[32] PROGMEM = // 1, 16
  {0X00,0X00,0X00,0XFC,0X00,0XFC,0X00,0XFC,0X00,0XFC,0X00,0XFC,0X00,0XFC,0X00,0XFC,0X00,0XFC,0X89,0XF3,0X89,0XF3,0X89,0XF3,0X89,0XF3,0X89,0XF3,0X89,0XF3,0X00,0X00};
uint8_t bird_down_image11[32] PROGMEM = // 1, 16
  {0X00,0X00,0X00,0XFC,0X00,0XFC,0X00,0XFC,0X00,0XFC,0X00,0XFC,0X00,0XFC,0X00,0XFC,0X00,0XFC,0X89,0XF3,0X89,0XF3,0X89,0XF3,0X89,0XF3,0X89,0XF3,0X89,0XF3,0X00,0X00};
uint8_t bird_down_image12[28] PROGMEM = // 2, 14
  {0X00,0X00,0X00,0X00,0X00,0XFC,0X00,0XFC,0X00,0XFC,0X00,0XFC,0X00,0XFC,0X00,0XFC,0X89,0XF3,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00};
uint8_t bird_down_image13[18] PROGMEM = // 4, 9
  {0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X5F,0X06,0X5F,0X06};

// Bird_up, 18x14
uint8_t bird_up_image0[8] PROGMEM = // 6, 4
  {0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00};
uint8_t bird_up_image1[18] PROGMEM = // 4, 9
  {0X00,0X00,0X00,0X00,0XE0,0XFF,0XE0,0XFF,0X00,0X00,0XFF,0XFF,0X00,0X00,0X00,0X00,0X00,0X00};
uint8_t bird_up_image2[20] PROGMEM = // 3, 10
  {0X00,0X00,0XE0,0XFF,0XE0,0XFF,0XE0,0XFF,0XE0,0XFF,0X00,0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X00,0X00};
uint8_t bird_up_image3[28] PROGMEM = // 1, 14
  {0X00,0X00,0X00,0X00,0XE0,0XFF,0XE0,0XFF,0XE0,0XFF,0XE0,0XFF,0XE0,0XFF,0X00,0X00,0XFF,0XFF,0X00,0X00,0X00,0X00,0XFF,0XFF,0X00,0X00,0X00,0X00};
uint8_t bird_up_image4[30] PROGMEM = // 0, 15
  {0X00,0X00,0XE0,0XFF,0XE0,0XFF,0XE0,0XFF,0XE0,0XFF,0XE0,0XFF,0XE0,0XFF,0XE0,0XFF,0X00,0X00,0XFF,0XFF,0X00,0X00,0X00,0X00,0XFF,0XFF,0XFF,0XFF,0X00,0X00};
uint8_t bird_up_image5[30] PROGMEM = // 0, 15
  {0X00,0X00,0XE0,0XFF,0XE0,0XFF,0XE0,0XFF,0XE0,0XFF,0XE0,0XFF,0XE0,0XFF,0XE0,0XFF,0X00,0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X00,0X00};
uint8_t bird_up_image6[34] PROGMEM = // 0, 17
  {0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00};
uint8_t bird_up_image7[36] PROGMEM = // 0, 18
  {0X00,0X00,0X07,0XFF,0X07,0XFF,0X07,0XFF,0X07,0XFF,0X07,0XFF,0X07,0XFF,0X07,0XFF,0X00,0X00,0X89,0XF3,0X89,0XF3,0X89,0XF3,0X89,0XF3,0X89,0XF3,0X89,0XF3,0X89,0XF3,0X89,0XF3,0X00,0X00};
uint8_t bird_up_image8[36] PROGMEM = // 0, 18
  {0X00,0X00,0X07,0XFF,0X07,0XFF,0X07,0XFF,0X07,0XFF,0X07,0XFF,0X07,0XFF,0X07,0XFF,0X00,0X00,0X89,0XF3,0X89,0XF3,0X89,0XF3,0X89,0XF3,0X89,0XF3,0X89,0XF3,0X89,0XF3,0X89,0XF3,0X00,0X00};
uint8_t bird_up_image9[32] PROGMEM = // 1, 16
  {0X00,0X00,0X07,0XFF,0X07,0XFF,0X07,0XFF,0X07,0XFF,0X07,0XFF,0X00,0X00,0X00,0XFC,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00};
uint8_t bird_up_image10[32] PROGMEM = // 1, 16
  {0X00,0X00,0X07,0XFF,0X07,0XFF,0X07,0XFF,0X07,0XFF,0X00,0X00,0X00,0XFC,0X00,0XFC,0X00,0XFC,0X89,0XF3,0X89,0XF3,0X89,0XF3,0X89,0XF3,0X89,0XF3,0X89,0XF3,0X00,0X00};
uint8_t bird_up_image11[32] PROGMEM = // 1, 16
  {0X00,0X00,0X07,0XFF,0X07,0XFF,0X00,0X00,0X00,0X00,0X00,0XFC,0X00,0XFC,0X00,0XFC,0X00,0XFC,0X89,0XF3,0X89,0XF3,0X89,0XF3,0X89,0XF3,0X89,0XF3,0X89,0XF3,0X00,0X00};
uint8_t bird_up_image12[28] PROGMEM = // 2, 14
  {0X00,0X00,0X00,0X00,0X00,0XFC,0X00,0XFC,0X00,0XFC,0X00,0XFC,0X00,0XFC,0X00,0XFC,0X89,0XF3,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00};
uint8_t bird_up_image13[18] PROGMEM = // 4, 9
  {0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00};


#define pipe_w 22
#define pipe_h 80
uint8_t pipe_image[3520] PROGMEM = { // Pipe, 22 x 80
  0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,
  0XE4,0X53,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE4,0X53,
  0XE4,0X53,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE4,0X53,
  0XE4,0X53,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE4,0X53,
  0XE4,0X53,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE4,0X53,
  0XE4,0X53,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE4,0X53,
  0XE4,0X53,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE4,0X53,
  0XE4,0X53,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE4,0X53,
  0XE4,0X53,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE4,0X53,
  0XE4,0X53,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE5,0X75,0XE4,0X53,
  0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
  0X5F,0X06,0XE4,0X53,0XF1,0XDF,0XF1,0XDF,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0X4B,0X9F,0XE4,0X53,0X4B,0X9F,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0XE4,0X53,0X5F,0X06,
};


// Position of the pipe (left edge)
int pipe_x;
uint8_t pipe_type; // left clip / normal / right clip
int gap_t, gap_b, gap_l, gap_r;  // rectangle (inclusive) of the gap between two pipes
  
// Position of the bird (center)
int bird_y;
// Score
int score;
// is game over?
boolean game_over;


Adafruit_ST7735 tft = Adafruit_ST7735(cs, dc, rst);

void draw_led(boolean on)
{
  tft.fillCircle(150, TOP_MARGIN / 2 - 1, 4, (on ? ST7735_GREEN : ST7735_RED));
}


void draw_score(int score)
{
  tft.setCursor(42, 4);
  tft.print(score); 
}


void draw_sub_pipe_top(int x, int y, int sub_x, int w, int h)
{
  int stride = pipe_w * 2;
  uint8_t *start, *cur;
  int row, col;
  start = pipe_image + (h - 1) * stride;
  tft.setAddrWindow(x, y, x + w - 1, y + h - 1);
  for (row = 0; row < h; ++row)
  {
    cur = start + sub_x * 2;
    for (col = 0; col < w; ++col)
    {
      tft.pushColor(pgm_read_word(cur));
      cur += 2;
    }
    start -= stride;
  }  
}


void draw_sub_pipe_bottom(int x, int y, int sub_x, int w, int h)
{
  // Draw pipe upside down
  int stride = pipe_w * 2;
  uint8_t *start, *cur;
  int row, col;
  start = pipe_image;
  tft.setAddrWindow(x, y, x + w - 1, y + h - 1);
  for (row = 0; row < h; ++row)
  {
    cur = start + sub_x * 2;
    for (col = 0; col < w; ++col)
    {
      tft.pushColor(pgm_read_word(cur));
      cur += 2;
    }
    start += stride;
  }
}


// This function draws a pair of pipes
void draw_pipe()
{
  int bottom_pipe_h, top_pipe_h;
  int sub_image_start_x, sub_image_w, sub_image_h;

  top_pipe_h = gap_t - TOP_MARGIN;
  bottom_pipe_h = MAX_Y - gap_b;
  
  if (pipe_type == 2)
  {
    // right side of the pipe is out of screen
    // draw bottom pipe
    sub_image_start_x = 0;
    sub_image_w = gap_r - gap_l + 1;
    sub_image_h = bottom_pipe_h;
    draw_sub_pipe_bottom(gap_l, gap_b + 1, sub_image_start_x, sub_image_w, sub_image_h);
    // draw top pipe
    sub_image_h = top_pipe_h;
    draw_sub_pipe_top(gap_l, TOP_MARGIN, sub_image_start_x, sub_image_w, sub_image_h);
  }
  else if (pipe_type == 1)
  {
    // left side of the pipe is out of screen
    // draw bottom pipe
    sub_image_start_x = pipe_w - gap_r - 1;
    sub_image_w = pipe_w - sub_image_start_x;
    sub_image_h = bottom_pipe_h;
    draw_sub_pipe_bottom(0, gap_b + 1, sub_image_start_x, sub_image_w, sub_image_h);
    // draw top pipe
    sub_image_h = top_pipe_h;
    draw_sub_pipe_top(0, TOP_MARGIN, sub_image_start_x, sub_image_w, sub_image_h);
  }
  else
  {
    // draw full pipe
    // draw bottom pipe
    sub_image_start_x = 0;
    sub_image_w = pipe_w;
    sub_image_h = bottom_pipe_h;
    draw_sub_pipe_bottom(gap_l, gap_b + 1, sub_image_start_x, sub_image_w, sub_image_h);
    // draw top pipe
    sub_image_h = top_pipe_h;
    draw_sub_pipe_top(gap_l, TOP_MARGIN, sub_image_start_x, sub_image_w, sub_image_h);
  }
}


// Draw bird up or down
void draw_bird(int bird_y, boolean dir, int x = bird_x)
{
  int i;
  if (!dir)
  {
    tft.setAddrWindow(x + 6, bird_y, x + 9, bird_y + 1);
    ++bird_y;
    for (i = 0; i < 4; ++i) tft.pushColor(pgm_read_word(bird_down_image0 + i + i));
    tft.setAddrWindow(x + 4, bird_y, x + 12, bird_y + 1);
    ++bird_y;
    for (i = 0; i < 9; ++i) tft.pushColor(pgm_read_word(bird_down_image1 + i + i));
    tft.setAddrWindow(x + 3, bird_y, x + 12, bird_y + 1);
    ++bird_y;
    for (i = 0; i < 10; ++i) tft.pushColor(pgm_read_word(bird_down_image2 + i + i));
    tft.setAddrWindow(x + 1, bird_y, x + 14, bird_y + 1);
    ++bird_y;
    for (i = 0; i < 14; ++i) tft.pushColor(pgm_read_word(bird_down_image3 + i + i));
    tft.setAddrWindow(x, bird_y, x + 14, bird_y + 1);
    ++bird_y;
    for (i = 0; i < 15; ++i) tft.pushColor(pgm_read_word(bird_down_image4 + i + i));
    tft.setAddrWindow(x, bird_y, x + 14, bird_y + 1);
    ++bird_y;
    for (i = 0; i < 15; ++i) tft.pushColor(pgm_read_word(bird_down_image5 + i + i));
    tft.setAddrWindow(x, bird_y, x + 16, bird_y + 1);
    ++bird_y;
    for (i = 0; i < 17; ++i) tft.pushColor(pgm_read_word(bird_down_image6 + i + i));
    tft.setAddrWindow(x, bird_y, x + 17, bird_y + 1);
    ++bird_y;
    for (i = 0; i < 18; ++i) tft.pushColor(pgm_read_word(bird_down_image7 + i + i));
    tft.setAddrWindow(x, bird_y, x + 17, bird_y + 1);
    ++bird_y;
    for (i = 0; i < 18; ++i) tft.pushColor(pgm_read_word(bird_down_image8 + i + i));
    tft.setAddrWindow(x + 1, bird_y, x + 16, bird_y + 1);
    ++bird_y;
    for (i = 0; i < 16; ++i) tft.pushColor(pgm_read_word(bird_down_image9 + i + i));
    tft.setAddrWindow(x + 1, bird_y, x + 16, bird_y + 1);
    ++bird_y;
    for (i = 0; i < 16; ++i) tft.pushColor(pgm_read_word(bird_down_image10 + i + i));
    tft.setAddrWindow(x + 1, bird_y, x + 16, bird_y + 1);
    ++bird_y;
    for (i = 0; i < 16; ++i) tft.pushColor(pgm_read_word(bird_down_image11 + i + i));
    tft.setAddrWindow(x + 2, bird_y, x + 15, bird_y + 1);
    ++bird_y;
    for (i = 0; i < 14; ++i) tft.pushColor(pgm_read_word(bird_down_image12 + i + i));
    tft.setAddrWindow(x + 4, bird_y, x + 10, bird_y + 1);
    ++bird_y;
    for (i = 0; i < 7; ++i) tft.pushColor(pgm_read_word(bird_down_image13 + i + i));
  }
  else
  {
    tft.setAddrWindow(x + 6, bird_y, x + 9, bird_y + 1);
    ++bird_y;
    for (i = 0; i < 4; ++i) tft.pushColor(pgm_read_word(bird_up_image0 + i + i));
    tft.setAddrWindow(x + 4, bird_y, x + 12, bird_y + 1);
    ++bird_y;
    for (i = 0; i < 9; ++i) tft.pushColor(pgm_read_word(bird_up_image1 + i + i));
    tft.setAddrWindow(x + 3, bird_y, x + 12, bird_y + 1);
    ++bird_y;
    for (i = 0; i < 10; ++i) tft.pushColor(pgm_read_word(bird_up_image2 + i + i));
    tft.setAddrWindow(x + 1, bird_y, x + 14, bird_y + 1);
    ++bird_y;
    for (i = 0; i < 14; ++i) tft.pushColor(pgm_read_word(bird_up_image3 + i + i));
    tft.setAddrWindow(x, bird_y, x + 14, bird_y + 1);
    ++bird_y;
    for (i = 0; i < 15; ++i) tft.pushColor(pgm_read_word(bird_up_image4 + i + i));
    tft.setAddrWindow(x, bird_y, x + 14, bird_y + 1);
    ++bird_y;
    for (i = 0; i < 15; ++i) tft.pushColor(pgm_read_word(bird_up_image5 + i + i));
    tft.setAddrWindow(x, bird_y, x + 16, bird_y + 1);
    ++bird_y;
    for (i = 0; i < 17; ++i) tft.pushColor(pgm_read_word(bird_up_image6 + i + i));
    tft.setAddrWindow(x, bird_y, x + 17, bird_y + 1);
    ++bird_y;
    for (i = 0; i < 18; ++i) tft.pushColor(pgm_read_word(bird_up_image7 + i + i));
    tft.setAddrWindow(x, bird_y, x + 17, bird_y + 1);
    ++bird_y;
    for (i = 0; i < 18; ++i) tft.pushColor(pgm_read_word(bird_up_image8 + i + i));
    tft.setAddrWindow(x + 1, bird_y, x + 16, bird_y + 1);
    ++bird_y;
    for (i = 0; i < 16; ++i) tft.pushColor(pgm_read_word(bird_up_image9 + i + i));
    tft.setAddrWindow(x + 1, bird_y, x + 16, bird_y + 1);
    ++bird_y;
    for (i = 0; i < 16; ++i) tft.pushColor(pgm_read_word(bird_up_image10 + i + i));
    tft.setAddrWindow(x + 1, bird_y, x + 16, bird_y + 1);
    ++bird_y;
    for (i = 0; i < 16; ++i) tft.pushColor(pgm_read_word(bird_up_image11 + i + i));
    tft.setAddrWindow(x + 2, bird_y, x + 15, bird_y + 1);
    ++bird_y;
    for (i = 0; i < 14; ++i) tft.pushColor(pgm_read_word(bird_up_image12 + i + i));
    tft.setAddrWindow(x + 4, bird_y, x + 10, bird_y + 1);
    ++bird_y;
    for (i = 0; i < 7; ++i) tft.pushColor(pgm_read_word(bird_up_image13 + i + i));
  }
}


// partially clean the pipe image (draw blue color)
void clear_old_pipe(void)
{
  // partially clean old pipe
  tft.fillRect(gap_r - 1, TOP_MARGIN, 2, gap_t - TOP_MARGIN, SKY_COLOR);
  tft.fillRect(gap_r - 1, gap_b + 1, 2, MAX_Y - gap_b, SKY_COLOR);
}


// given an old bird draw at last_bird_y, clean only portion of pixels unnecessary for the new bird
void clear_old_bird(int last_bird_y, int movement)
{
  switch (movement)
  {
    case -13:
      tft.drawFastHLine(bird_x + 4, last_bird_y + 1, 9, SKY_COLOR);
      tft.drawFastHLine(bird_x + 3, last_bird_y + 2, 10, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 3, 14, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 4, 15, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 5, 15, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 6, 17, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 7, 18, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 8, 18, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 9, 16, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 10, 16, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 11, 16, SKY_COLOR);
      tft.drawFastHLine(bird_x + 2, last_bird_y + 12, 14, SKY_COLOR);
      tft.drawFastHLine(bird_x + 4, last_bird_y + 13, 7, SKY_COLOR);
      break;
    case -12:
      tft.drawFastHLine(bird_x + 11, last_bird_y + 1, 2, SKY_COLOR);
      tft.drawFastHLine(bird_x + 3, last_bird_y + 2, 10, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 3, 14, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 4, 15, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 5, 15, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 6, 17, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 7, 18, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 8, 18, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 9, 16, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 10, 16, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 11, 16, SKY_COLOR);
      tft.drawFastHLine(bird_x + 2, last_bird_y + 12, 14, SKY_COLOR);
      tft.drawFastHLine(bird_x + 4, last_bird_y + 13, 7, SKY_COLOR);
      break;
    case -11:
      tft.drawFastHLine(bird_x + 3, last_bird_y + 2, 1, SKY_COLOR); tft.drawFastHLine(bird_x + 11, last_bird_y + 2, 2, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 3, 14, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 4, 15, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 5, 15, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 6, 17, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 7, 18, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 8, 18, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 9, 16, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 10, 16, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 11, 16, SKY_COLOR);
      tft.drawFastHLine(bird_x + 2, last_bird_y + 12, 14, SKY_COLOR);
      tft.drawFastHLine(bird_x + 4, last_bird_y + 13, 7, SKY_COLOR);
      break;
    case -10:
      tft.drawFastHLine(bird_x + 1, last_bird_y + 3, 3, SKY_COLOR); tft.drawFastHLine(bird_x + 11, last_bird_y + 3, 4, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 4, 15, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 5, 15, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 6, 17, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 7, 18, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 8, 18, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 9, 16, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 10, 16, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 11, 16, SKY_COLOR);
      tft.drawFastHLine(bird_x + 2, last_bird_y + 12, 14, SKY_COLOR);
      tft.drawFastHLine(bird_x + 4, last_bird_y + 13, 7, SKY_COLOR);
      break;
    case -9:
      tft.drawFastHLine(bird_x + 1, last_bird_y + 3, 1, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 4, 4, SKY_COLOR); tft.drawFastHLine(bird_x + 11, last_bird_y + 4, 4, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 5, 15, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 6, 17, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 7, 18, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 8, 18, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 9, 16, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 10, 16, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 11, 16, SKY_COLOR);
      tft.drawFastHLine(bird_x + 2, last_bird_y + 12, 14, SKY_COLOR);
      tft.drawFastHLine(bird_x + 4, last_bird_y + 13, 7, SKY_COLOR);
    case -8:
      tft.drawFastHLine(bird_x, last_bird_y + 4, 2, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 5, 4, SKY_COLOR); tft.drawFastHLine(bird_x + 11, last_bird_y + 5, 4, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 6, 17, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 7, 18, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 8, 18, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 9, 16, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 10, 16, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 11, 16, SKY_COLOR);
      tft.drawFastHLine(bird_x + 2, last_bird_y + 12, 14, SKY_COLOR);
      tft.drawFastHLine(bird_x + 4, last_bird_y + 13, 7, SKY_COLOR);
      break;
    case -7:
      tft.drawFastHLine(bird_x, last_bird_y + 4, 1, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 5, 2, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 6, 4, SKY_COLOR); tft.drawFastHLine(bird_x + 11, last_bird_y + 6, 6, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 7, 18, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 8, 18, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 9, 16, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 10, 16, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 11, 16, SKY_COLOR);
      tft.drawFastHLine(bird_x + 2, last_bird_y + 12, 14, SKY_COLOR);
      tft.drawFastHLine(bird_x + 4, last_bird_y + 13, 7, SKY_COLOR);
      break;
    case -6:
      tft.drawFastHLine(bird_x, last_bird_y + 4, 1, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 5, 1, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 6, 2, SKY_COLOR); tft.drawFastHLine(bird_x + 16, last_bird_y + 6, 1, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 7, 4, SKY_COLOR); tft.drawFastHLine(bird_x + 11, last_bird_y + 7, 7, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 8, 18, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 9, 16, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 10, 16, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 11, 16, SKY_COLOR);
      tft.drawFastHLine(bird_x + 2, last_bird_y + 12, 14, SKY_COLOR);
      tft.drawFastHLine(bird_x + 4, last_bird_y + 13, 7, SKY_COLOR);
      break;
    case -5:
      tft.drawFastHLine(bird_x, last_bird_y + 4, 1, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 5, 1, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 6, 1, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 7, 2, SKY_COLOR); tft.drawFastHLine(bird_x + 16, last_bird_y + 7, 2, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 8, 4, SKY_COLOR); tft.drawFastHLine(bird_x + 11, last_bird_y + 8, 7, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 9, 16, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 10, 16, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 11, 16, SKY_COLOR);
      tft.drawFastHLine(bird_x + 2, last_bird_y + 12, 14, SKY_COLOR);
      tft.drawFastHLine(bird_x + 4, last_bird_y + 13, 7, SKY_COLOR);
      break;
    case -4:
      tft.drawFastHLine(bird_x, last_bird_y + 5, 1, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 6, 1, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 7, 1, SKY_COLOR); tft.drawFastHLine(bird_x + 17, last_bird_y + 7, 1, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 8, 2, SKY_COLOR); tft.drawFastHLine(bird_x + 16, last_bird_y + 8, 2, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 9, 3, SKY_COLOR); tft.drawFastHLine(bird_x + 11, last_bird_y + 9, 6, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 10, 16, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 11, 16, SKY_COLOR);
      tft.drawFastHLine(bird_x + 2, last_bird_y + 12, 14, SKY_COLOR);
      tft.drawFastHLine(bird_x + 4, last_bird_y + 13, 7, SKY_COLOR);
      break;
    case -3:
      tft.drawFastHLine(bird_x, last_bird_y + 6, 1, SKY_COLOR);    
      tft.drawFastHLine(bird_x, last_bird_y + 7, 1, SKY_COLOR); tft.drawFastHLine(bird_x + 17, last_bird_y + 7, 1, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 8, 1, SKY_COLOR); tft.drawFastHLine(bird_x + 17, last_bird_y + 8, 1, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 9, 1, SKY_COLOR); tft.drawFastHLine(bird_x + 16, last_bird_y + 9, 1, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 10, 3, SKY_COLOR); tft.drawFastHLine(bird_x + 11, last_bird_y + 10, 6, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 11, 16, SKY_COLOR);
      tft.drawFastHLine(bird_x + 2, last_bird_y + 12, 14, SKY_COLOR);
      tft.drawFastHLine(bird_x + 4, last_bird_y + 13, 7, SKY_COLOR);
      break;
    case -2:
      tft.drawFastHLine(bird_x, last_bird_y + 7, 1, SKY_COLOR); tft.drawFastHLine(bird_x + 17, last_bird_y + 7, 1, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 8, 1, SKY_COLOR); tft.drawFastHLine(bird_x + 17, last_bird_y + 8, 1, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 10, 1, SKY_COLOR); tft.drawFastHLine(bird_x + 16, last_bird_y + 10, 1, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 11, 3, SKY_COLOR); tft.drawFastHLine(bird_x + 11, last_bird_y + 11, 6, SKY_COLOR);
      tft.drawFastHLine(bird_x + 2, last_bird_y + 12, 14, SKY_COLOR);
      tft.drawFastHLine(bird_x + 4, last_bird_y + 13, 7, SKY_COLOR);
      break;
    case -1: // up by 1 pixel
      tft.drawFastHLine(bird_x, last_bird_y + 8, 1, SKY_COLOR); tft.drawFastHLine(bird_x + 17, last_bird_y + 8, 1, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 11, 1, SKY_COLOR); tft.drawFastHLine(bird_x + 16, last_bird_y + 11, 1, SKY_COLOR);
      tft.drawFastHLine(bird_x + 2, last_bird_y + 12, 2, SKY_COLOR); tft.drawFastHLine(bird_x + 11, last_bird_y + 12, 5, SKY_COLOR);
      tft.drawFastHLine(bird_x + 4, last_bird_y + 13, 7, SKY_COLOR);
      break;
    case 1:  // down by 1 pixel
      tft.drawFastHLine(bird_x + 6, last_bird_y, 4, SKY_COLOR);
      tft.drawFastHLine(bird_x + 4, last_bird_y + 1, 2, SKY_COLOR); tft.drawFastHLine(bird_x + 10, last_bird_y + 1, 3, SKY_COLOR);
      tft.drawFastHLine(bird_x + 3, last_bird_y + 2, 1, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 3, 2, SKY_COLOR); tft.drawFastHLine(bird_x + 13, last_bird_y + 3, 2, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 4, 1, SKY_COLOR);
      tft.drawFastHLine(bird_x + 15, last_bird_y + 6, 2, SKY_COLOR);
      tft.drawFastHLine(bird_x + 17, last_bird_y + 7, 1, SKY_COLOR);
    break;
    case 2:
      tft.drawFastHLine(bird_x + 6, last_bird_y, 4, SKY_COLOR);
      tft.drawFastHLine(bird_x + 4, last_bird_y + 1, 9, SKY_COLOR);
      tft.drawFastHLine(bird_x + 3, last_bird_y + 2, 3, SKY_COLOR); tft.drawFastHLine(bird_x + 10, last_bird_y + 2, 3, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 3, 3, SKY_COLOR); tft.drawFastHLine(bird_x + 13, last_bird_y + 3, 2, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 4, 3, SKY_COLOR); tft.drawFastHLine(bird_x + 13, last_bird_y + 4, 2, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 5, 1, SKY_COLOR);
      tft.drawFastHLine(bird_x + 15, last_bird_y + 6, 2, SKY_COLOR);
      tft.drawFastHLine(bird_x + 15, last_bird_y + 7, 3, SKY_COLOR);
      tft.drawFastHLine(bird_x + 17, last_bird_y + 8, 1, SKY_COLOR);
      break;
    case 3:
      tft.drawFastHLine(bird_x + 6, last_bird_y, 4, SKY_COLOR);
      tft.drawFastHLine(bird_x + 4, last_bird_y + 1, 9, SKY_COLOR);
      tft.drawFastHLine(bird_x + 3, last_bird_y + 2, 10, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 3, 5, SKY_COLOR); tft.drawFastHLine(bird_x + 10, last_bird_y + 3, 5, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 4, 4, SKY_COLOR); tft.drawFastHLine(bird_x + 13, last_bird_y + 4, 2, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 5, 3, SKY_COLOR); tft.drawFastHLine(bird_x + 13, last_bird_y + 5, 2, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 6, 1, SKY_COLOR); tft.drawFastHLine(bird_x + 15, last_bird_y + 6, 2, SKY_COLOR);
      tft.drawFastHLine(bird_x + 15, last_bird_y + 7, 3, SKY_COLOR);
      tft.drawFastHLine(bird_x + 15, last_bird_y + 8, 3, SKY_COLOR);
      break;
    case 4:
      tft.drawFastHLine(bird_x + 6, last_bird_y, 4, SKY_COLOR);
      tft.drawFastHLine(bird_x + 4, last_bird_y + 1, 9, SKY_COLOR);  
      tft.drawFastHLine(bird_x + 3, last_bird_y + 2, 10, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 3, 14, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 4, 6, SKY_COLOR); tft.drawFastHLine(bird_x + 10, last_bird_y + 4, 5, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 5, 4, SKY_COLOR); tft.drawFastHLine(bird_x + 13, last_bird_y + 5, 2, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 6, 3, SKY_COLOR); tft.drawFastHLine(bird_x + 13, last_bird_y + 6, 4, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 7, 1, SKY_COLOR); tft.drawFastHLine(bird_x + 15, last_bird_y + 7, 3, SKY_COLOR);
      tft.drawFastHLine(bird_x + 15, last_bird_y + 8, 3, SKY_COLOR);
      tft.drawFastHLine(bird_x + 15, last_bird_y + 9, 2, SKY_COLOR);
      break;
    case 5:
      tft.drawFastHLine(bird_x + 6, last_bird_y, 4, SKY_COLOR);
      tft.drawFastHLine(bird_x + 4, last_bird_y + 1, 9, SKY_COLOR);  
      tft.drawFastHLine(bird_x + 3, last_bird_y + 2, 10, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 3, 14, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 4, 15, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 5, 6, SKY_COLOR); tft.drawFastHLine(bird_x + 10, last_bird_y + 5, 5, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 6, 4, SKY_COLOR); tft.drawFastHLine(bird_x + 13, last_bird_y + 6, 4, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 7, 3, SKY_COLOR); tft.drawFastHLine(bird_x + 13, last_bird_y + 7, 5, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 8, 1, SKY_COLOR); tft.drawFastHLine(bird_x + 15, last_bird_y + 8, 3, SKY_COLOR);
      tft.drawFastHLine(bird_x + 15, last_bird_y + 9, 2, SKY_COLOR);
      tft.drawFastHLine(bird_x + 15, last_bird_y + 10, 2, SKY_COLOR);
      break;
    case 6:
      tft.drawFastHLine(bird_x + 6, last_bird_y, 4, SKY_COLOR);
      tft.drawFastHLine(bird_x + 4, last_bird_y + 1, 9, SKY_COLOR);  
      tft.drawFastHLine(bird_x + 3, last_bird_y + 2, 10, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 3, 14, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 4, 15, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 5, 15, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 6, 6, SKY_COLOR); tft.drawFastHLine(bird_x + 10, last_bird_y + 6, 7, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 7, 4, SKY_COLOR); tft.drawFastHLine(bird_x + 13, last_bird_y + 7, 5, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 8, 3, SKY_COLOR); tft.drawFastHLine(bird_x + 13, last_bird_y + 8, 5, SKY_COLOR);
      tft.drawFastHLine(bird_x + 15, last_bird_y + 9, 2, SKY_COLOR);
      tft.drawFastHLine(bird_x + 15, last_bird_y + 10, 2, SKY_COLOR);
      tft.drawFastHLine(bird_x + 15, last_bird_y + 11, 2, SKY_COLOR);
      break;
    case 7:
      tft.drawFastHLine(bird_x + 6, last_bird_y, 4, SKY_COLOR);
      tft.drawFastHLine(bird_x + 4, last_bird_y + 1, 9, SKY_COLOR);  
      tft.drawFastHLine(bird_x + 3, last_bird_y + 2, 10, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 3, 14, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 4, 15, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 5, 15, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 6, 17, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 7, 6, SKY_COLOR); tft.drawFastHLine(bird_x + 10, last_bird_y + 7, 8, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 8, 4, SKY_COLOR); tft.drawFastHLine(bird_x + 13, last_bird_y + 8, 5, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 9, 2, SKY_COLOR); tft.drawFastHLine(bird_x + 13, last_bird_y + 9, 4, SKY_COLOR);
      tft.drawFastHLine(bird_x + 15, last_bird_y + 10, 2, SKY_COLOR);
      tft.drawFastHLine(bird_x + 15, last_bird_y + 11, 2, SKY_COLOR);
      tft.drawFastHLine(bird_x + 15, last_bird_y + 12, 1, SKY_COLOR);
      break;
    case 8:
      tft.drawFastHLine(bird_x + 6, last_bird_y, 4, SKY_COLOR);
      tft.drawFastHLine(bird_x + 4, last_bird_y + 1, 9, SKY_COLOR);  
      tft.drawFastHLine(bird_x + 3, last_bird_y + 2, 10, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 3, 14, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 4, 15, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 5, 15, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 6, 17, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 7, 18, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 8, 6, SKY_COLOR); tft.drawFastHLine(bird_x + 10, last_bird_y + 8, 8, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 9, 3, SKY_COLOR); tft.drawFastHLine(bird_x + 13, last_bird_y + 9, 4, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 10, 2, SKY_COLOR); tft.drawFastHLine(bird_x + 13, last_bird_y + 10, 4, SKY_COLOR);
      tft.drawFastHLine(bird_x + 15, last_bird_y + 11, 2, SKY_COLOR);
      tft.drawFastHLine(bird_x + 15, last_bird_y + 12, 1, SKY_COLOR);
      break;
    case 9:
      tft.drawFastHLine(bird_x + 6, last_bird_y, 4, SKY_COLOR);
      tft.drawFastHLine(bird_x + 4, last_bird_y + 1, 9, SKY_COLOR);  
      tft.drawFastHLine(bird_x + 3, last_bird_y + 2, 10, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 3, 14, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 4, 15, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 5, 15, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 6, 17, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 7, 18, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 8, 18, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 9, 5, SKY_COLOR); tft.drawFastHLine(bird_x + 10, last_bird_y + 9, 7, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 10, 3, SKY_COLOR); tft.drawFastHLine(bird_x + 13, last_bird_y + 10, 4, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 11, 2, SKY_COLOR); tft.drawFastHLine(bird_x + 13, last_bird_y + 11, 4, SKY_COLOR);
      tft.drawFastHLine(bird_x + 15, last_bird_y + 12, 1, SKY_COLOR);
      break;
    case 10:
      tft.drawFastHLine(bird_x + 6, last_bird_y, 4, SKY_COLOR);
      tft.drawFastHLine(bird_x + 4, last_bird_y + 1, 9, SKY_COLOR);  
      tft.drawFastHLine(bird_x + 3, last_bird_y + 2, 10, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 3, 14, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 4, 15, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 5, 15, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 6, 17, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 7, 18, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 8, 18, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 9, 16, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 10, 5, SKY_COLOR); tft.drawFastHLine(bird_x + 10, last_bird_y + 10, 7, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 11, 3, SKY_COLOR); tft.drawFastHLine(bird_x + 13, last_bird_y + 11, 4, SKY_COLOR);
      tft.drawFastHLine(bird_x + 2, last_bird_y + 12, 1, SKY_COLOR); tft.drawFastHLine(bird_x + 13, last_bird_y + 12, 3, SKY_COLOR);
      break;
    case 11:
      tft.drawFastHLine(bird_x + 6, last_bird_y, 4, SKY_COLOR);
      tft.drawFastHLine(bird_x + 4, last_bird_y + 1, 9, SKY_COLOR);  
      tft.drawFastHLine(bird_x + 3, last_bird_y + 2, 10, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 3, 14, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 4, 15, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 5, 15, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 6, 17, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 7, 18, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 8, 18, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 9, 16, SKY_COLOR);    
      tft.drawFastHLine(bird_x + 1, last_bird_y + 10, 16, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 11, 5, SKY_COLOR); tft.drawFastHLine(bird_x + 10, last_bird_y + 11, 7, SKY_COLOR);
      tft.drawFastHLine(bird_x + 2, last_bird_y + 12, 2, SKY_COLOR); tft.drawFastHLine(bird_x + 13, last_bird_y + 12, 3, SKY_COLOR);
      break;
    case 12:
      tft.drawFastHLine(bird_x + 6, last_bird_y, 4, SKY_COLOR);
      tft.drawFastHLine(bird_x + 4, last_bird_y + 1, 9, SKY_COLOR);  
      tft.drawFastHLine(bird_x + 3, last_bird_y + 2, 10, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 3, 14, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 4, 15, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 5, 15, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 6, 17, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 7, 18, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 8, 18, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 9, 16, SKY_COLOR);    
      tft.drawFastHLine(bird_x + 1, last_bird_y + 10, 16, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 11, 16, SKY_COLOR);
      tft.drawFastHLine(bird_x + 2, last_bird_y + 12, 4, SKY_COLOR); tft.drawFastHLine(bird_x + 10, last_bird_y + 12, 6, SKY_COLOR);
      break;
    case 13:
      tft.drawFastHLine(bird_x + 6, last_bird_y, 4, SKY_COLOR);
      tft.drawFastHLine(bird_x + 4, last_bird_y + 1, 9, SKY_COLOR);  
      tft.drawFastHLine(bird_x + 3, last_bird_y + 2, 10, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 3, 14, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 4, 15, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 5, 15, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 6, 17, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 7, 18, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 8, 18, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 9, 16, SKY_COLOR);    
      tft.drawFastHLine(bird_x + 1, last_bird_y + 10, 16, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 11, 16, SKY_COLOR);
      tft.drawFastHLine(bird_x + 3, last_bird_y + 12, 14, SKY_COLOR);
      tft.drawFastHLine(bird_x + 4, last_bird_y + 13, 2, SKY_COLOR); tft.drawFastHLine(bird_x + 10, last_bird_y + 13, 1, SKY_COLOR);
      break;
    default: // just erase the old bird
      tft.drawFastHLine(bird_x + 6, last_bird_y, 4, SKY_COLOR);
      tft.drawFastHLine(bird_x + 4, last_bird_y + 1, 9, SKY_COLOR);  
      tft.drawFastHLine(bird_x + 3, last_bird_y + 2, 10, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 3, 14, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 4, 15, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 5, 15, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 6, 17, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 7, 18, SKY_COLOR);
      tft.drawFastHLine(bird_x, last_bird_y + 8, 18, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 9, 16, SKY_COLOR);    
      tft.drawFastHLine(bird_x + 1, last_bird_y + 10, 16, SKY_COLOR);
      tft.drawFastHLine(bird_x + 1, last_bird_y + 11, 16, SKY_COLOR);
      tft.drawFastHLine(bird_x + 3, last_bird_y + 12, 14, SKY_COLOR);
      tft.drawFastHLine(bird_x + 4, last_bird_y + 13, 7, SKY_COLOR);
  }
}


// display a shameless splash screen
void splash()
{
  tft.fillRect(0, 0, MAX_X + 1, MAX_Y + 1, ST7735_WHITE);
  tft.fillRect(0, 10, 140, 20, ST7735_RED);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextWrap(false);
  tft.setTextSize(2);
  tft.setCursor(5, 13);
  tft.print("#ArduinoD14");
  tft.setTextColor(SKY_COLOR);
  tft.setCursor(14, 53);
  tft.print("FLAPPYDUINO");
  draw_bird(80, true, 71);
  tft.setTextColor(ST7735_BLACK);
  tft.setCursor(62, 116);
  tft.setTextSize(1);
  tft.print("Baoshi(@ba0sh1)");
  delay(2000);
}


void game_start()
{
  tft.fillRect(0, 0, MAX_X + 1, MAX_Y + 1, SKY_COLOR);
  tft.drawCircle(131, 40, 9, ST7735_WHITE);
  tft.drawCircle(131, 88, 9, ST7735_WHITE);
  tft.drawRect(120, 29, 23, 23, ST7735_WHITE);
  tft.drawRect(120, 57, 23, 45, ST7735_WHITE);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(1);
  tft.setCursor(5, 29);
  tft.print("Hover hand over");
  tft.setCursor(5, 39);
  tft.print("right side to");
  tft.setCursor(5, 49);
  tft.print("start ---->");
  tft.setCursor(5, 105);
  tft.print("Visit me:");
  tft.setCursor(5, 115);
  tft.print("http://www.ba0sh1.com");

  tft.setTextSize(3);
  tft.setTextColor(ST7735_WHITE, SKY_COLOR);
  boolean wait_finish = false;
  while (!wait_finish)
  {
    while (distance <= 300) {};
    unsigned long now = millis();
    unsigned long wait = now + 2999;
    while (now < wait)
    {
        int counter = (wait - now) / 1000 + 1;
        tft.setCursor(45, 68);
        tft.print(counter);
        delay(100);
        if (distance <= 300)
        {
          tft.fillRect(45, 68, 30, 30, SKY_COLOR);
          break;
        }
        now = millis();
    }
    if (now >= wait)
      wait_finish = true;
  }
  // initial scene  
  tft.fillRect(0, 0, MAX_X + 1, TOP_MARGIN - 1, ST7735_BLUE);
  tft.drawFastHLine(0, TOP_MARGIN - 1, MAX_X + 1, ST7735_BLACK);
  tft.fillRect(0, TOP_MARGIN, MAX_X + 1, MAX_Y - TOP_MARGIN + 1, SKY_COLOR);
  tft.setTextColor(ST7735_WHITE, ST7735_BLUE);
  tft.setTextWrap(false);
  tft.setTextSize(1);
  tft.setCursor(5, 4);
  tft.print("Score:");
  score = 0;
  pipe_x = -pipe_w - 1;
  bird_y = tft.height() / 2;
  game_over = false; 
}


void game_loop()
{
  static int last_distance = 0;
  static float a = 0;
  static float v = 0;
  static int gap_h = 45;
  static unsigned long last_up = millis();
  static int exercise_count = 50;  // for the first 50 frames we run without pipe
  
  boolean exercising;
   
  int loop_start, loop_end, loop_delay;
  int gap_y;
  int last_bird_y;
  
  loop_start = millis();
  
  if (exercise_count <= 0)
  {
    exercising = false;
  }
  else
  {
    --exercise_count;
    exercising = true;
  }
  // advance pipe
  if (!exercising)
  {
    if (pipe_x <= -pipe_w)
    {
      // The pipe already moved out of screen, create new pipe
      pipe_x = MAX_X;
      gap_y = random(TOP_MARGIN + 40, MAX_Y - 40);
      gap_t = gap_y - (gap_h / 2) + 1;
      gap_b = gap_t + gap_h - 1;
    }
    else
    {
      clear_old_pipe();
      pipe_x -= 2;
    }
    if (pipe_x > MAX_X - pipe_w + 1)
    {
      // right side of the pipe exceed screen
      pipe_type = 2;  // right clip
      gap_l = pipe_x;
      gap_r = MAX_X;
    }
    else if (pipe_x < 0)
    {
      // left side of the pipe exceed screen
      pipe_type = 1; // left clip
      gap_l = 0;
      gap_r = pipe_w + pipe_x - 1;
    }
    else
    {
      // pipe within screen
      pipe_type = 0;
      gap_l = pipe_x;
      gap_r = gap_l + pipe_w - 1;
    }
    
    // update score
    if (pipe_x + pipe_w == bird_x + 1)  // because pipe_x is odd number and bird_x and pipe_w are even number
    {
      ++score;
    }
  }
    
  // dertermine if we have a "flap" action, and adjust velocity (v) and acceleration (a)
  if (new_reading)
  {
    // Serial.print(last_distance); Serial.print(", "); Serial.println(distance);
    if ((distance > 350) && (distance - last_distance > 100))
    {
      unsigned long now = millis();
      Serial.println(now - last_up);
      if (now - last_up > 150)  // prevent 2  consecutive "up" actions due to noise
      {
        Serial.println("up");
        draw_led(true);
        if (v >= 0) 
          v = -0.7;
        a = -0.2;
        last_up = now;      
      }
    }
    else
    {
      draw_led(false);
      a = 0.1;
    }
    last_distance = distance;
    new_reading = 0;
  }
  v = v + a;
  if ((bird_y == MAX_Y - bird_h + 1) && (v > 0))
    v = 0;
  if ((bird_y == TOP_MARGIN) && (v < 0))
    v = 0;
  
  // update bird location
  last_bird_y = bird_y;
  bird_y = (int)(bird_y + v);
  if (bird_y > MAX_Y - bird_h + 1 ) { bird_y = MAX_Y - bird_h + 1; }
  if (bird_y < TOP_MARGIN) { bird_y = TOP_MARGIN; }
  if (last_bird_y != bird_y)
  {
    clear_old_bird(last_bird_y, bird_y - last_bird_y);
  }
  
  if (!exercising)
  {
    draw_pipe();
    draw_score(score);
  }
  draw_bird(bird_y, (v < 0));
  
  // Collision detection, very bird shape dependent, change bird shape with caution!
  boolean hit = false;
  // is bird drop to bottom?
  if (bird_y == MAX_Y - bird_h + 1)
  {
    Serial.println("Hit bottom");
    hit = true;
  }
  if (!exercising)  // No pipe during exercising
  {
    // check top pipe
    int dist = bird_y - gap_t;
    // is bird hit top pipe above cap?
    if (dist < -19) // (bird_y + 8 < gap_t - 11)  // bird at top pipe above cap?
    {
      if ((bird_x + bird_w - 1 >= gap_l + 1) && (bird_x < gap_r - 1))
      {
        Serial.println("Hit top pipe");
        hit = true;
      }
    }
    // is bird hit top pipe cap?
    else if ((dist >= -19) && (dist <= -8)) //((bird_y + 8 >= gap_t - 11) && (bird_y + 7 <= gap_t - 1)) 
    {
      if ((bird_x + bird_w - 1 >= gap_l) && (bird_x <= gap_r))
      {
        Serial.println("Hit top cap");
        hit = true;
      }
    }
    else
    {
      switch (dist)
      {
        case -7:
          if ((bird_x + bird_w - 2 >= gap_l) && (bird_x <= gap_r))
          {
            Serial.println("Hit top cap");
            hit = true;
          }
          break;
        case -6:
        case -5:
          if ((bird_x + bird_w - 4 >= gap_l) && (bird_x <= gap_r))
          {
            Serial.println("Hit top cap");
            hit = true;
          }
          break;
        case -4:
          if ((bird_x + bird_w - 4 >= gap_l) && (bird_x <= gap_r - 1))
          {
            Serial.println("Hit top cap");
            hit = true;
          }
          break;
        case -3:
          if ((bird_x + bird_w - 6 >= gap_l) && (bird_x <= gap_r - 3))
          {
            Serial.println("Hit top cap");
            hit = true;
          }
          break;
        case -2:
          if ((bird_x + bird_w - 6 >= gap_l) && (bird_x <= gap_r - 4))
          {
            Serial.println("Hit top cap");
            hit = true;
          }
          break;
        case -1:
          if ((bird_x + bird_w - 9 >= gap_l) && (bird_x <= gap_r - 6))
          {
            Serial.println("Hit top cap");
            hit = true;
          }
          break;
      }
    }
    // check bottom pipe
    dist = bird_y - gap_b;
    // is bird hit bottom pipe below cap? (pic 2)
    if (dist > 4) // bird at bottom pipe below cap
    {
      if ((bird_x + bird_w - 1 >= gap_l + 1) && (bird_x < gap_r - 1))
      {
        Serial.println("Hit bottom pipe");
        hit = true;
      }
    }
    // is bird hit bottom pipe cap?
    else if ((dist <= 4) && (dist >= -7))
    {
      if ((bird_x + bird_w - 1 >= gap_l) && (bird_x <= gap_r))
      {
        Serial.println("Hit bottom cap");
        hit = true;
      }
    }
    else
    {
      switch (dist)
      {
        case -8:
        case -9:
        case -10:
          if ((bird_x + bird_w - 1 >= gap_l + 1) && (bird_x <= gap_r - 1))
          {
            Serial.println("Hit bottom cap");
            hit = true;
          }
          break;
        case -11:
          if ((bird_x + bird_w - 1 >= gap_l + 2) && (bird_x <= gap_r - 2))
          {
            Serial.println("Hit bottom cap");
            hit = true;
          } 
          break;  
        case -12:
          if ((bird_x + bird_w - 1 >= gap_l + 7) && (bird_x <= gap_r - 4))
          {
            Serial.println("Hit bottom cap");
            hit = true;
          } 
          break;  
      }
    }
    
    if (hit)
    {
      tft.invertDisplay(true);
      delay(100);
      tft.invertDisplay(false);
      delay(100);
      tft.invertDisplay(true);
      delay(100);
      tft.invertDisplay(false);
      delay(100);
      tft.invertDisplay(true);
      delay(100);
      tft.invertDisplay(false);
      delay(100);
      tft.invertDisplay(true);
      delay(100);
      tft.invertDisplay(false);
      delay(1000);
      game_over = true;
      a = 0;
      v = 0;
      exercise_count = 50;
    }
  }
  loop_end = millis();
  loop_delay = 40 - loop_end + loop_start;
  if (loop_delay > 0) 
    delay(loop_delay);
}


void setup()
{
  // initialize the serial port
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  
  randomSeed(analogRead(2));
  
  // initialize the display
  tft.initR(INITR_BLACKTAB);   // black tab panel
  tft.setRotation(3);

  cli();  // clear interrupts
  
  // ADC setup
  // clear ADLAR in ADMUX (0x7C) to right-adjust the result
  // ADCL will contain lower 8 bits, ADCH upper 2 (in last two bits)
  ADMUX &= B11011111;
  // Set REFS1..0 in ADMUX (0x7C) to change reference voltage to the
  // proper source (01)
  ADMUX |= B01000000; // Reference at AVCC with external cap at AREF pin
  // Clear MUX3..0 in ADMUX (0x7C) in preparation for setting the analog
  // input
  ADMUX &= B11110000;
  ADMUX |= B00000000; // ADC0
  
  ADCSRA = (1 << ADEN) | (1 << ADATE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
  ADCSRB = (1 << ADTS2) | (1 << ADTS0);  // timer1 compare match B to trigger
  ADCSRA |= (1 << ADIE);    // Enable ADC interrupt
  
  // Timer setup
  TCCR1A = 0;
  TCCR1B  = (1<<WGM12);   // CTC, prescaler 256
  //TIMSK1 |= (1<<OCIE1B);  //  enable timer 1 compareB interrupt 
  OCR1A   = 3215;        //  set value for 20Hz 
  OCR1B   = 3215;      //  set value for 20Hz 
  TCNT1 = 0;
  TCCR1B |= (1<<CS12);
  sei();  // enable interrupt
  
  splash();
}


void loop()
{
  game_start();
  while (!game_over)
    game_loop();
}


// Interrupt service routine for the ADC completion
ISR(ADC_vect)
{
  //clear timer compare match flag
  TIFR1 = (1 << OCF1B);
  // Must read low first
  distance = ADCL | (ADCH << 8);
  new_reading = 1;
}
