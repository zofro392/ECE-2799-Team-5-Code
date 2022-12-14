// IMPORTANT: ELEGOO_TFTLCD LIBRARY MUST BE SPECIFICALLY
// CONFIGURED FOR EITHER THE TFT SHIELD OR THE BREAKOUT BOARD.
// SEE RELEVANT COMMENTS IN Elegoo_TFTLCD.h FOR SETUP.
//Technical support:goodtft@163.com

#include <Elegoo_GFX.h>    // Core graphics library
#include <Elegoo_TFTLCD.h> // Hardware-specific library
#include <SD.h>
#include <SPI.h>


// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define PIN_SD_CS 53 // Elegoo SD shields and modules: pin 10

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

#define LCD_RESET2 A9
#define LCD_CS2 A8 // Chip Select goes to Analog 3
#define LCD_CD2 A7 // Command/Data goes to Analog 2
#define LCD_WR2 A6 // LCD Write goes to Analog 1
#define LCD_RD2 A5 // LCD Read goes to Analog 0


#define LCD_RESET3 A14
#define LCD_CS3 A13 // Chip Select goes to Analog 3
#define LCD_CD3 A12 // Command/Data goes to Analog 2
#define LCD_WR3 A11 // LCD Write goes to Analog 1
#define LCD_RD3 A10 // LCD Read goes to Analog 0

// When using the BREAKOUT BOARD only, use these 8 data lines to the LCD:
// For the Arduino Uno, Duemilanove, Diecimila, etc.:
//   D0 connects to digital pin 8  (Notice these are
//   D1 connects to digital pin 9   NOT in order!)
//   D2 connects to digital pin 2
//   D3 connects to digital pin 3
//   D4 connects to digital pin 4
//   D5 connects to digital pin 5
//   D6 connects to digital pin 6
//   D7 connects to digital pin 7
// For the Arduino Mega, use digital pins 22 through 29
// (on the 2-row header at the end of the board).

// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED2     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW2  0xFFE0
#define WHITE   0xFFFF


#define DARKGREEN 0x03E6
#define DARKDARKGREY 0x528A
#define DARKGREY 0x6B8D
#define LIGHTGREEN 0x7EAA
#define DARKRED 0x8841
#define MEDGREY 0xA534
#define LIGHTGREY 0xD6BA
#define RED 0xEA8A
#define ORANGE 0xFDCA
#define YELLOW 0xFECA 

int colorarr[] = {DARKGREEN, DARKDARKGREY,DARKGREY, LIGHTGREEN, DARKRED, MEDGREY, LIGHTGREY,RED,ORANGE,YELLOW};
Elegoo_TFTLCD tft2(LCD_CS2, LCD_CD2, LCD_WR2, LCD_RD2, LCD_RESET2);
Elegoo_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
Elegoo_TFTLCD tft3(LCD_CS3, LCD_CD3, LCD_WR3, LCD_RD3, LCD_RESET3);
// If using the shield, all control and data lines are fixed, and
// a simpler declaration can optionally be used:
// Elegoo_TFTLCD tft;

#define MAX_BMP         10                      // bmp file num
#define FILENAME_LEN    20                      // max file name length

//File bmpFile;
#define BUFFPIXEL       60                      // must be a divisor of 240 
#define BUFFPIXEL_X3    180                     // BUFFPIXELx3

void LCD_init(Elegoo_TFTLCD screen){
  screen.reset();
  uint16_t identifier = screen.readID();
  if(identifier == 0x9341) {
    Serial.println(F("Found ILI9341 LCD driver"));
  } else if(identifier==0x0101)
  {     
      identifier=0x9341;
       Serial.println(F("Found 0x9341 LCD driver"));
  }else {
    Serial.print(F("Unknown LCD driver chip: "));
    Serial.println(identifier, HEX);
    identifier=0x9341;
   
  }
  screen.begin(identifier);
  screen.fillScreen(BLUE);
  screen.setTextColor(WHITE);    
  screen.setTextSize(3);
  screen.println("Screen Initailized");
  delay(1000);
  return;
}
void Directions_1(Elegoo_TFTLCD screen){

  screen.fillScreen(DARKGREY);
  screen.setTextColor(YELLOW);    
  screen.setTextSize(5);
  screen.println("PLACE A TILE");
  screen.println("TO BEGIN");
  screen.drawRect(2,127,230,154,YELLOW);
  screen.setCursor(10,125);
  screen.setTextColor(WHITE);    
  screen.setTextSize(2);
  screen.println("Power lines carry up to\n   380k Volts!");
  screen.println("This gets transformed\n to just 230 Volts when \n delivered to homes");
}

void Directions_2(Elegoo_TFTLCD screen){

  screen.fillScreen(DARKGREY);
  screen.setTextColor(YELLOW);    
  screen.setTextSize(4);
  screen.println("PLACE A TILE");
  screen.println("TO BEGIN");
  screen.fillRect(1,127,240,112,WHITE);
  screen.setCursor(16,133);
  screen.setTextColor(DARKGREY);    
  screen.setTextSize(3);
  screen.println("3 Phase power uses 3\n power lines for high \n power density at the \n same amperage.");
  screen.setCursor(16,255);
  screen.setTextColor(LIGHTGREY);    
  screen.setTextSize(2);
  screen.println("PLACE A TILE ON THE 3 PHASE LINE TO\n SEE WHAT 3 PHASE IS USED FOR");

}
void Directions_3(Elegoo_TFTLCD screen){

  screen.fillScreen(DARKGREY);
  screen.setTextColor(YELLOW);    
  screen.setTextSize(4);
  screen.println("PLACE A TILE");
  screen.println("TO BEGIN");
  screen.drawLine(14, 115, 226, 115, YELLOW);
  screen.drawLine(14, 293, 226, 293, YELLOW);
  screen.setCursor(20,145);
  screen.setTextColor(LIGHTGREY);    
  screen.setTextSize(2);
  screen.println("Power consumers and\n   producers can be\n placed on the same line\n to form a smaller power\n      grid");

}

void tft_call(Elegoo_TFTLCD screen, String message){
screen.fillScreen(BLUE);
screen.setTextColor(WHITE);    screen.setTextSize(1);
screen.println(message);
return;
}
void clearscreen(Elegoo_TFTLCD screen){
  screen.fillScreen(BLACK);
}


void setup(void) {
  Serial.begin(9600);
  LCD_init(tft);
  LCD_init(tft2);
  LCD_init(tft3);
  delay(1000);
}

void loop(void) {

  tft.begin(0x9341);
  tft.fillScreen(BLACK);
  Directions_1(tft);  
  delay(1000);
  tft2.begin(0x9341);
  tft2.fillScreen(BLACK);
  Directions_2(tft2);  
  delay(1000);
  tft3.begin(0x9341);
  tft3.fillScreen(BLACK);
  Directions_3(tft3);
  delay(1000);

}
