
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include "FreeMono9pt7b.h"

//OLED
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define PIN 4
#define PIN2 5
#define JOYPIN 1

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

const int textslength = 7;

int textNumber = 0;
int pixelPerChar = 7;
int scrollspeed = 5;
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
  "PHOTOGRAPHY ",
  "FARS DAG ",
  "STOCKHOLM ",
  "LENSBALL ",
  "SWEe0fp9fm3e9aq5 "
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

//OLED
 if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.invertDisplay(true);
  display_text();    // Draw 'stylized' characters
  //display.invertDisplay(true);
  
}
  
void loop() {
  total_len = strlen(outputtexter[textNumber]) * (pixelPerChar + matrix.width()) ;

  val = analogRead(JOYPIN);
  if(val){
    joysticking(val);
  }

  //To show text number with LED:s
  if(first){
    //sets the number of the text showing ie 1 is "MY QUAD ROCKS!"
    matrix.fillRect(0,   0, 1, textNumber, colors[0]);
    matrix.show();
    //sets the main delay for haft fast the text scrolls.
    matrix2.fillRect(0, 0, 1, scrollspeed, colors[0]);
    matrix2.show();    
    delay(100);
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
  //display_text();
  delay(scrollspeed);
}

void display_text(void) {
  display.clearDisplay();

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.print(F("Speed:"));
  display.print(8-scrollspeed);
  
  display.setCursor(64,0);
  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
  display.println(F("Rainbow"));
  display.setCursor(0,8);             // Start at top-left corner
  display.setTextColor(SSD1306_WHITE);
  display.print(F("Txt "));
  display.print(textNumber);
  display.print(" of ");
  display.print(textslength-1);
  display.setCursor(64,8);
  display.println(F("Test: more"));  

  //display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
  //display.println(3.141592);
  display.setTextSize(2);             // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.println(outputtexter[textNumber]);
  display.display();

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
    
    if(R2<100000){
      Serial.println(R2);
        matrix.fillScreen(0);
      
      if(R2>300 && R2<600){
        Serial.print("delay down");
        Serial.println(scrollspeed);
        if(scrollspeed > 0)
          scrollspeed--;

        numberoflights_display = scrollspeed;
      }
      else if(R2>1200 && R2<1800){
         Serial.print("delay up");
        Serial.println(scrollspeed);
        
        if(scrollspeed < 8)
          scrollspeed++;
          
        numberoflights_display = scrollspeed;
      }
      else if(R2>700 && R2<1100){
        if(textNumber < textslength-1)
          textNumber++;
          
        numberoflights_display = textNumber;
        Serial.print("Text up");
        Serial.println(outputtexter[textNumber]);
      }
      else if(R2>2100 && R2<2500){

        if(textNumber >1)
          textNumber--;
          
        numberoflights_display = textNumber;
        Serial.print("text down");
        Serial.println(outputtexter[textNumber]);
      }
      else if(R2>3500 && R2<3700){
        Serial.println("Middle"); 
        matrix.fillScreen(0);
        matrix2.fillScreen(0);  
      } 
      first = true;  

      display_text();
    }

}
