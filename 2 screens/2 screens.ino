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
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

Elegoo_TFTLCD tft2(LCD_CS2, LCD_CD2, LCD_WR2, LCD_RD2, LCD_RESET2);
Elegoo_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
Elegoo_TFTLCD tft3(LCD_CS3, LCD_CD3, LCD_WR3, LCD_RD3, LCD_RESET3);
// If using the shield, all control and data lines are fixed, and
// a simpler declaration can optionally be used:
// Elegoo_TFTLCD tft;

#define MAX_BMP         10                      // bmp file num
#define FILENAME_LEN    20                      // max file name length

const int __Gnbmp_height = 320;                 // bmp hight
const int __Gnbmp_width  = 240;                 // bmp width

unsigned char __Gnbmp_image_offset  = 0;        // offset

int __Gnfile_num = 5;                           // num of file

char __Gsbmp_files[3][FILENAME_LEN] =           // add file name here
{
"Directions_1.bmp",
"Directions_2.bmp",
"Directions_3.bmp",
};
//File bmpFile;
#define BUFFPIXEL       60                      // must be a divisor of 240 
#define BUFFPIXEL_X3    180                     // BUFFPIXELx3

void bmpdraw(Elegoo_TFTLCD screen,File f, int x, int y)
{
    f.seek(__Gnbmp_image_offset);

    uint32_t time = millis();

    uint8_t sdbuffer[BUFFPIXEL_X3];                 // 3 * pixels to buffer

    for (int i=0; i< __Gnbmp_height; i++) {
        for(int j=0; j<(240/BUFFPIXEL); j++) {
            f.read(sdbuffer, BUFFPIXEL_X3);
            
            uint8_t buffidx = 0;
            int offset_x = j*BUFFPIXEL;
            unsigned int __color[BUFFPIXEL];
            
            for(int k=0; k<BUFFPIXEL; k++) {
                __color[k] = sdbuffer[buffidx+2]>>3;                        // read
                __color[k] = __color[k]<<6 | (sdbuffer[buffidx+1]>>2);      // green
                __color[k] = __color[k]<<5 | (sdbuffer[buffidx+0]>>3);      // blue
                
                buffidx += 3;
            }

	    for (int m = 0; m < BUFFPIXEL; m ++) {
              screen.drawPixel(m+offset_x, i,__color[m]);
	    }
        }
    }
    
    Serial.print(millis() - time, DEC);
    Serial.println(" ms");
}

boolean bmpReadHeader(File f) 
{
    // read header
    uint32_t tmp;
    uint8_t bmpDepth;
    
    if (read16(f) != 0x4D42) {
        // magic bytes missing
        return false;
    }

    // read file size
    tmp = read32(f);
    Serial.print("size 0x");
    Serial.println(tmp, HEX);

    // read and ignore creator bytes
    read32(f);

    __Gnbmp_image_offset = read32(f);
    Serial.print("offset ");
    Serial.println(__Gnbmp_image_offset, DEC);

    // read DIB header
    tmp = read32(f);
    Serial.print("header size ");
    Serial.println(tmp, DEC);
    
    int bmp_width = read32(f);
    int bmp_height = read32(f);
    
    if(bmp_width != __Gnbmp_width || bmp_height != __Gnbmp_height)  {    // if image is not 320x240, return false
        return false;
    }

    if (read16(f) != 1)
    return false;

    bmpDepth = read16(f);
    Serial.print("bitdepth ");
    Serial.println(bmpDepth, DEC);

    if (read32(f) != 0) {
        // compression not supported!
        return false;
    }

    Serial.print("compression ");
    Serial.println(tmp, DEC);

    return true;
}

/*********************************************/
// These read data from the SD card file and convert them to big endian
// (the data is stored in little endian format!)

// LITTLE ENDIAN!
uint16_t read16(File f)
{
    uint16_t d;
    uint8_t b;
    b = f.read();
    d = f.read();
    d <<= 8;
    d |= b;
    return d;
}

// LITTLE ENDIAN!
uint32_t read32(File f)
{
    uint32_t d;
    uint16_t b;

    b = read16(f);
    d = read16(f);
    d <<= 16;
    d |= b;
    return d;
}

void tft_call(Elegoo_TFTLCD screen, String message){
screen.fillScreen(BLUE);
screen.setTextColor(WHITE);    screen.setTextSize(1);
screen.println(message);
return;
}
void tft_show_bmp(Elegoo_TFTLCD screen, char file[FILENAME_LEN]){

  File  bmpFile = SD.open(file);
  if (! bmpFile) {
    Serial.println("didnt find image");
    while (1);
  }
  if(! bmpReadHeader(bmpFile)) {
            Serial.println("bad bmp");
            screen.setTextColor(WHITE);    screen.setTextSize(1);
            screen.println("bad bmp");
            return;
        }
        bmpdraw(screen, bmpFile, 0, 0);

         bmpFile.close();
  delay(1000);
  delay(1000);
  return;
}

void setup(void) {
  Serial.begin(9600);
  tft.reset();
  tft2.reset();
  tft3.reset();
  uint16_t identifier = tft.readID();
   
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
  
  tft.begin(identifier);
  tft.fillScreen(BLUE);
  
  
  uint16_t identifier2 = tft2.readID();
    if(identifier2 == 0x9341) {
    Serial.println(F("Found ILI9341 LCD driver"));
  } else if(identifier2==0x0101)
  {     
      identifier2=0x9341;
       Serial.println(F("Found 0x9341 LCD driver"));
  }else {
    Serial.print(F("Unknown LCD driver chip: "));
    Serial.println(identifier2, HEX);
    identifier2=0x9341;
   
  }
  
  tft2.begin(identifier2);
  tft2.fillScreen(BLUE);

    uint16_t identifier3 = tft3.readID();
   
  if(identifier3 == 0x9341) {
    Serial.println(F("Found ILI9341 LCD driver"));
  } else if(identifier3==0x0101)
  {     
      identifier3=0x9341;
       Serial.println(F("Found 0x9341 LCD driver"));
  }else {
    Serial.print(F("Unknown LCD driver chip: "));
    Serial.println(identifier3, HEX);
    identifier3=0x9341;
   
  }
  
  tft3.begin(identifier3);
  tft3.fillScreen(BLUE);
  
    pinMode(53, OUTPUT);
   
  if (!SD.begin(53)) {
    Serial.println("initialization failed!");
    tft.setCursor(0, 0);
    tft.setTextColor(WHITE);    
    tft.setTextSize(1);
    tft.println("SD Card Init fail.");   
  }else
  Serial.println("initialization done."); 
}

void loop(void) {
     
tft_call(tft, "tft1 called");
Serial.println("tft1 returned");
delay(1000);
tft_call(tft2, "tft2 called");
Serial.println("tft 2 returned");   
delay(1000);
tft_call(tft3, "tft3 called");
Serial.println("tft 3 returned");   
delay(1000);


}



