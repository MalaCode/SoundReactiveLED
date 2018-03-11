// testcolors demo for Adafruit RGBmatrixPanel library.
// Renders 512 colors on our 16x32 RGB LED matrix:
// http://www.adafruit.com/products/420
// Library supports 4096 colors, but there aren't that many pixels!  :)

// Written by Limor Fried/Ladyada & Phil Burgess/PaintYourDragon
// for Adafruit Industries.
// BSD license, all text above must be included in any redistribution.

#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library
#include <SoftwareSerial.h>


#define CLK 8  // MUST be on PORTB! (Use pin 11 on Mega)
#define LAT A3
#define OE  9
#define A   A0
#define B   A1
#define C   A2
#define left 0
#define middle 15
#define right 31

RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);
uint8_t r=0, g=0, b=0;
int i = 16; 
int j = 15;
int k;
int currVal = 15;

boolean inc;

enum displayStates {on, off} displayState;
//enum readStates {on, off} readState;

SoftwareSerial mySerial (10,11);


void increaseSize(int j)
{
      for (; currVal > j; currVal--)
      {
        matrix.drawLine(middle,currVal,middle,14,matrix.Color333(7, 0, 0));
      }
      int temp = currVal;
      for (int l = middle; l <= right; l++)
      {
        if(temp >= 15)
        {break;}
        matrix.drawLine(l,temp,l,14,matrix.Color333(7, 0, 0));     
        temp++;  
      }
      temp = currVal;
      for (int l = middle; l >= left; l--)
      {
        if(temp >= 15)
        {break;}
        matrix.drawLine(l,temp,l,14,matrix.Color333(7, 0, 0));       
        temp++;
      }
//      delay(3);
//      matrix.drawLine(middle,0,middle,14,matrix.Color333(0, 0, 0));
//      temp = currVal;
//      for (int l = middle; l < right; l++)
//      {
//        if(temp >= 15)
//        {break;}
//        matrix.drawLine(l,14,l,temp,matrix.Color333(0, 0, 0));       
//        temp++;
//      }
//      temp = currVal;
//      for (int l = middle; l >= left; l--)
//      {
//        if(temp >= 15)
//        {break;}
//        matrix.drawLine(l,temp,l,14,matrix.Color333(0, 0, 0));       
//        temp++;
//      }
}

void decreaseSize(int j)
{
      for (; currVal < j; currVal++)
      {
        matrix.drawLine(middle,currVal,middle,14,matrix.Color333(7, 0, 0));
      }
      int temp = currVal;
      for (int l = middle; l <= right; l++)
      {
        if(temp >= 15)
        {break;}
        matrix.drawLine(l,temp,l,14,matrix.Color333(7, 0, 0));     
        temp++;  
      }
      temp = currVal;
      for (int l = middle; l >= left; l--)
      {
        if(temp >= 15)
        {break;}
        matrix.drawLine(l,temp,l,14,matrix.Color333(7, 0, 0));       
        temp++;
      }
//      delay(3);
//      matrix.drawLine(middle,0,middle,15,matrix.Color333(0, 0, 0));
//      temp = currVal;
//      for (int l = middle; l < right; l++)
//      {
//        if(temp >= 15)
//        {break;}
//        matrix.drawLine(l,14,l,temp,matrix.Color333(0, 0, 0));       
//        temp++;
//      }
//      temp = currVal;
//      for (int l = middle; l >= left; l--)
//      {
//        if(temp >= 15)
//        {break;}
//        matrix.drawLine(l,temp,l,14,matrix.Color333(0, 0, 0));       
//        temp++;
//      }
}

void setup() {
    Serial.begin(9600);
    while(!Serial);
    matrix.begin();
  //  matrix.drawFastVLine(14,0,3,matrix.Color333(7, 0, 0));
}
unsigned char test;
bool othertest = true;
unsigned int final;
void loop() {
  matrix.drawLine(0, 15, 31, 15, matrix.Color333(7, 0, 0));

  if(Serial.available())
  {
      final = Serial.read();
      matrix.drawLine(middle,0,middle,14,matrix.Color333(0, 0, 0));
      int temp = currVal;
      for (int l = middle; l < right; l++)
      {
        if(temp >= 15)
        {break;}
        matrix.drawLine(l,14,l,temp,matrix.Color333(0, 0, 0));       
        temp++;
      }
      temp = currVal;
      for (int l = middle; l >= left; l--)
      {
        if(temp >= 15)
        {break;}
        matrix.drawLine(l,temp,l,14,matrix.Color333(0, 0, 0));       
        temp++;
      }
    currVal = 15;
    for (; currVal > final; currVal--)
    {
      matrix.drawLine(middle,currVal,middle,14,matrix.Color333(7, 0, 0));
    }
   temp = currVal;
    for (int l = middle; l <= right; l++)
    {
      if(temp >= 15)
      {break;}
      matrix.drawLine(l,temp,l,14,matrix.Color333(7, 0, 0));     
      temp++;  
    }
    temp = currVal;
    for (int l = middle; l >= left; l--)
    {
      if(temp >= 15)
      {break;}
      matrix.drawLine(l,temp,l,14,matrix.Color333(7, 0, 0));       
      temp++;
    }
  }

}

