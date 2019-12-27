
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include "FreeMono9pt7b.h"

#define PIN 4
#define PIN2 5
 
// MATRIX DECLARATION:
// Parameter 1 = width of NeoPixel matrix
// Parameter 2 = height of matrix
// Parameter 3 = pin number (most are valid)
// Parameter 4 = matrix layout flags, add together as needed:
//   NEO_MATRIX_TOP, NEO_MATRIX_BOTTOM, NEO_MATRIX_LEFT, NEO_MATRIX_RIGHT:
//     Position of the FIRST LED in the matrix; pick two, e.g.
//     NEO_MATRIX_TOP + NEO_MATRIX_LEFT for the top-left corner.
//   NEO_MATRIX_ROWS, NEO_MATRIX_COLUMNS: LEDs are arranged in horizontal
//     rows or in vertical columns, respectively; pick one or the other.
//   NEO_MATRIX_PROGRESSIVE, NEO_MATRIX_ZIGZAG: all rows/columns proceed
//     in the same order, or alternate lines reverse direction; pick one.
//   See example below for these values in action.
// Parameter 5 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
 // Det var alltså 32, 8 och PIN som start. 
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(1, 8, PIN,
  NEO_MATRIX_BOTTOM    + NEO_MATRIX_RIGHT +
  NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);
Adafruit_NeoMatrix matrix2 = Adafruit_NeoMatrix(1, 8, PIN2,
  NEO_MATRIX_BOTTOM    + NEO_MATRIX_RIGHT +
  NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);  
 //PIXEL_TYPE
const uint16_t colors[] = {
  matrix.Color(255, 0, 0), 
  matrix.Color(0, 255, 0), 
  matrix.Color(255, 255, 0),
  matrix.Color(0, 0, 255), 
  matrix.Color(255, 0, 255), 
  matrix.Color(0, 255, 255), 
  matrix.Color(255, 255, 255)
};
int maxDisplacement = 21;  
int textNumber = 0;
int pixelPerChar = 7;
const int textslength = 4; //one more than texts in outputtexter
int speed = 1;
int x = matrix.width();
int pass = 0;
boolean first = true;

/*
char outputtexter[textslength][64] = {"",
  "QWAD KWADDERS ",
  "ROTOR RIOT ",
  "JOHN SWE FPV ",
  "WOOHOO ",
  "DRONESPORTZ.SE ",
  "MERRY CHRISTMAS AND A HAPPY NEW YEAR "
 };*/
/*char outputtexter[textslength][64] = {"",
  "MERRY CHRISTMAS AND A HAPPY NEW YEAR ",
  "HAPPY NEW YEAR "
};*/
char outputtexter[textslength][64] = {"",
  "SNILLRIK ",
  "QWAD KWADDERS ",
  "HAPPY NEW YEAR "
};

void setup() {
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setFont(&FreeMono9pt7b);
  matrix.setBrightness(255);
  matrix.setTextColor(colors[0]); 

  matrix2.begin();
  matrix2.setTextWrap(false);
  matrix2.setFont(&FreeMono9pt7b);
  matrix2.setBrightness(255);
  matrix2.setTextColor(colors[0]); 
  //matrix2.setRemapFunction(reverseFlow);
  Serial.begin(9600);
  randomSeed(analogRead(0));
  textNumber = random(1,textslength);
}
  
void loop() {
  int total_len = strlen(outputtexter[textNumber]) * (pixelPerChar + matrix.width()) ;

  //To show text number with LED:s
  if(first){
    matrix.fillRect(0, 0, 1, textNumber, colors[0]);
    matrix.show();
    matrix2.fillRect(0, 0, 1, textNumber, colors[0]);
    matrix2.show();    
    delay(1000);
    first = false;
  }  
  
  matrix.fillScreen(0);
  matrix2.fillScreen(0);
  matrix.setCursor(x, 0);
  matrix2.setCursor(-1*(total_len+x), 0);
  
  matrix.print(outputtexter[textNumber]);
  matrix2.print(outputtexter[textNumber]);
  maxDisplacement = strlen(outputtexter[textNumber]) * (pixelPerChar + matrix.width());
  
  if(--x < -maxDisplacement) 
  {
    x = matrix.width();
 
    if(++pass >= 7) 
      pass = 0;
      
    matrix.setTextColor(colors[pass]);
    matrix2.setTextColor(colors[pass]);

  }
    
  matrix.show();
  matrix2.show();
  delay(speed);
}

//Not really used, but might come in handy to manipulate text, ie upside down.
uint16_t reverseFlow(uint16_t x, uint16_t y) {
  return y * (matrix.width()-x);
}
