
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include "FreeMono9pt7b.h"

#define PIN 4
#define PIN2 5
#define JOYPIN 4

//Making matrixes, 8 high and 1 wide.
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(1, 8, PIN,
  NEO_MATRIX_BOTTOM    + NEO_MATRIX_RIGHT +
  NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);
Adafruit_NeoMatrix matrix2 = Adafruit_NeoMatrix(1, 8, PIN2,
  NEO_MATRIX_BOTTOM    + NEO_MATRIX_RIGHT +
  NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);  

//adding some colors.
const uint16_t colors[] = {
  matrix.Color(255, 0, 0), //red
  matrix.Color(0, 255, 0), //green
  matrix.Color(255, 255, 0), //yellow
  matrix.Color(0, 0, 255), //blue
  matrix.Color(255, 0, 255), //purple
  matrix.Color(0, 255, 255), //cyan
  matrix.Color(255, 255, 255) //white
};

const int textslength = 8;

int textNumber = 0;
int pixelPerChar = 7;
int scrollspeed = 1;
int total_len;
int x = matrix.width();
int pass = 0;
boolean first = true;
int maxDisplacement = 21;  

//joystick stuff
int val = 0;
int Vin= 5;
float Vout= 0;
float R1= 1000; //the resistors value
float R2= 0;
float buffer= 0;

//texts
char outputtexter[textslength][64] = {"",
  "SNILLRIK ",
  "HAPPY NEW YEAR ",
  "DRONESPORTZ ",
  "JOHN SWE FPV ",
  "RSFPV",
  "KLUDDE FPV ",
  "RSKR_FPV "
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
  textNumber = random(1,textslength); //if you have no joystick it pics random text, with many texts you might have to restart many times to get the one you want.
  
}
  
void loop() {
  total_len = strlen(outputtexter[textNumber]) * (pixelPerChar + matrix.width()) ;

  /*val = analogRead(JOYPIN);
  if(val){
    joysticking(val);
  }*/

  //To show text number with LED:s
  if(first){
    //sets the number of the text showing ie 1 is "MY QUAD ROCKS!"
    matrix.fillRect(0,   0, 1, textNumber, colors[0]);
    matrix.show();
    //sets the main delay for haft fast the text scrolls.
    matrix2.fillRect(0, 0, 1, scrollspeed, colors[0]);
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
  delay(scrollspeed);
}

//Not used, but might come in handy to manipulate text, ie upside down.
uint16_t reverseFlow(uint16_t x, uint16_t y) {
  return y * (matrix.width()-x);
}

void joysticking(int val){
    buffer= val * Vin;
    Vout= (buffer)/1024.0;
    buffer= (Vin/Vout) -1;
    R2= R1 * buffer;
    int numberoflights_display = 0;
    Serial.println(R2);
    if(R2<100000){
        matrix.fillScreen(0);
      
      if(R2>7000 && R2<10000){
        Serial.print("delay down");
        Serial.println(scrollspeed);
        if(scrollspeed > 0)
          scrollspeed--;

        numberoflights_display = scrollspeed;
      }
      else if(R2>2000 && R2<3000){
         Serial.print("delay up");
        Serial.println(scrollspeed);
        
        if(scrollspeed < 8)
          scrollspeed++;
          
        numberoflights_display = scrollspeed;
      }
      else if(R2>4000 && R2<6000){
        if(textNumber < textslength-1)
          textNumber++;
          
        numberoflights_display = textNumber;
        Serial.print("Text up");
        Serial.println(outputtexter[textNumber]);
      }
      else if(R2>200 && R2<500){

        if(textNumber >1)
          textNumber--;
          
        numberoflights_display = textNumber;
        Serial.print("text down");
        Serial.println(outputtexter[textNumber]);
      }
      else if(R2>13000 && R2<16000){
        Serial.println("Middle"); 
        matrix.fillScreen(0);
        matrix2.fillScreen(0);  
      } 
      first = true;  
    }

}
