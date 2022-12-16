#include <stdio.h>
#include <stdlib.h>

#include <Elegoo_GFX.h>    // Core graphics library
#include <Elegoo_TFTLCD.h> // Hardware-specific library
#include <SD.h>
#include <SPI.h>
#include "Tile.h"

#include <Adafruit_NeoPixel.h>

// Setting Pins
#define LONGPIN 41
#define LINEA 42
#define LINEB 43
#define LINEC 44
#define LINED 45
#define LINEE 46
#define LINEF 47
#define LINEG 48
#define LINEH 49
#define LINEI 50
#define LINEJ 51
#define TITLE 52

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

// Assign human-readable names to some common 16-bit color values:
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED2     0xF800
#define GREEN   0x07E0
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

Elegoo_TFTLCD tft2(LCD_CS2, LCD_CD2, LCD_WR2, LCD_RD2, LCD_RESET2);
Elegoo_TFTLCD tft1(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
Elegoo_TFTLCD tft3(LCD_CS3, LCD_CD3, LCD_WR3, LCD_RD3, LCD_RESET3);

const int buttons[6][3] ={{22,23,24},{25,26,40},{28,29,30},{31,32,33},{34,35,36},{37,38,39}};            // GPIO 22 for the button


 
// House.compat = {.5,.5,.5,.5,1,5};

 //SmHouse.Num = 2;
//  Factory.compat[] ={3,3,3,3,3,1};
 //City.compat = {1,1,1,1,5};
 //Solar.compat = {-3,-3,-3,-3,-3,-3};
 //Empty.compat = {0,0,0,0,0,0};







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
  screen.println("PLACE A\n TILE");
  screen.println("TO BEGIN");
  screen.drawRect(2,127,230,154,YELLOW);
  screen.setCursor(10,125);
  screen.setTextColor(WHITE);    
  screen.setTextSize(2);
  screen.println("Power lines carry\n up to 380K \nVolts!");
  screen.println("This gets\ntransformed to 230 \nVolts when \n delivered to homes");
}

void Directions_2(Elegoo_TFTLCD screen){

  screen.fillScreen(DARKGREY);
  screen.setTextColor(YELLOW);    
  screen.setTextSize(4);
  screen.println("PLACE A\n TILE");
  screen.println("TO BEGIN");
  screen.setCursor(16,133);
  screen.setTextColor(LIGHTGREY);   
  screen.setTextSize(2);
  screen.println("3 Phase power uses 3\n power lines for high \n power density at the \n same amperage.");
  screen.setCursor(16,255);
  screen.setTextSize(2);
  screen.println("PLACE A TILE ON\n THE 3 PHASE LINE TO\n SEE WHAT 3 PHASE IS USED FOR");

}
void Directions_3(Elegoo_TFTLCD screen){

  screen.fillScreen(DARKGREY);
  screen.setTextColor(YELLOW);    
  screen.setTextSize(4);
  screen.println("PLACE A\n TILE");
  screen.println("TO BEGIN");
  screen.drawLine(14, 115, 226, 115, YELLOW);
  screen.drawLine(14, 293, 226, 293, YELLOW);
  screen.setCursor(20,145);
  screen.setTextColor(LIGHTGREY);    
  screen.setTextSize(2);
  screen.println("Power consumers and\n   producers can be\n placed on the same line\n to form a smaller power\n      grid");

}

void not_enough_power_Factory(Elegoo_TFTLCD screen){
  screen.fillScreen(RED);
  screen.setTextColor(DARKRED);    
  screen.setTextSize(4);
  screen.println("NOT ENOUGH");
  screen.println("POWER");
  screen.drawRect(2,127,230,154,DARKRED);
  screen.setCursor(10,125);
  screen.setCursor(20,145);
  screen.setTextColor(WHITE);    
  screen.setTextSize(2);
  screen.println("Factories need a LOT\n of power! \n Find a place \nwhere the Factory \ncould get more \npower than this.");
}

void not_enough_power_double(Elegoo_TFTLCD screen){
  screen.fillScreen(RED);
  screen.setTextColor(DARKRED);    
  screen.setTextSize(4);
  screen.println("NOT ENOUGH");
  screen.println("POWER");
  screen.drawRect(2,127,230,112,DARKRED);
  screen.setCursor(10,125);
  screen.setCursor(20,145);
  screen.setTextColor(WHITE);    
  screen.setTextSize(2);
  screen.println("These consumers can't \n share 1 Phase Power");
   screen.setCursor(16,255);
  screen.setTextColor(DARKRED);    
  screen.setTextSize(2);
  screen.println("SCAN THE QR CODE TO LEARN\nMORE ABOUT POWER CONSUMERS");
}
void microgrid(Elegoo_TFTLCD screen){
  screen.fillScreen(LIGHTGREEN);
  screen.setTextColor(DARKGREEN);
  screen.setTextSize(2);
  screen.println("You created a"); 
  screen.setTextSize(3);
  screen.println("MICROGRID");
  screen.drawLine(14, 115, 226, 115, DARKDARKGREY);
  screen.drawLine(14, 293, 226, 293, DARKDARKGREY);
  screen.setCursor(10,125);
  screen.setCursor(20,145);
  screen.setTextColor(WHITE);    
  screen.setTextSize(2);
  screen.println("This is when a smaller plant\n powers a consumer seperately\n from the main grid");
  screen.setCursor(16,255);
  screen.setTextColor(DARKDARKGREY);    
  screen.setTextSize(2);
  screen.println("SCAN THE QR CODE TO LEARN\nMORE ABOUT MICROGRIDS");
}

void too_much_power(Elegoo_TFTLCD screen){
  screen.fillScreen(YELLOW);
  screen.setTextColor(LIGHTGREY);    
  screen.setTextSize(4);
  screen.println("TOO MUCH");
  screen.println("POWER");
  screen.fillRect(2,112,230,127,LIGHTGREY);
  screen.setCursor(10,125);
  screen.setCursor(20,145);
  screen.setTextColor(DARKGREY);    
  screen.setTextSize(2);
  screen.println("This consumer doesn't\n need 3 Phase Power,\n find a better spot for\n this consumer's needs");
   screen.setCursor(16,255);
  screen.setTextColor(LIGHTGREY);    
  screen.setTextSize(2);
  screen.println("SCAN THE QR CODE TO LEARN\nMORE ABOUT 3 PHASE POWER");
}

void city_1PP(Elegoo_TFTLCD screen){
  screen.fillScreen(LIGHTGREEN);
  screen.setTextColor(DARKGREEN);    
  screen.setTextSize(4);
  screen.println("CORRECT!");
  //screen.fillRect(2,127,230,112,DARKGREEN);
  screen.setCursor(10,125);
  screen.setCursor(20,145);
  screen.setTextColor(WHITE);    
  screen.setTextSize(2);
  screen.println("Commercial districts can use 1\n Phase Power but they\n consume more than residential \n districts");
   screen.setCursor(16,255);
}

void solar_farm_1PP(Elegoo_TFTLCD screen){
  screen.fillScreen(LIGHTGREEN);
  screen.setTextColor(DARKGREEN);    
  screen.setTextSize(4);
  screen.println("CORRECT!");
  // //screen.fillRect(2,127,230,112,DARKGREEN);
  // screen.setCursor(10,125);
  // screen.setCursor(20,145);
  screen.setTextColor(WHITE);    
  screen.setTextSize(2);
  screen.println("Solar Farms produce 3\n Phase Power, but this\n can be transformed\n into 1 Phase with \ntransformers and substations");
   screen.setCursor(16,255);
  screen.setTextColor(DARKDARKGREY);    
  screen.setTextSize(2);
  screen.println("SCAN THE QR CODE TO LEARN\nMORE ABOUT SOLAR POWER");
}

void house_1PP(Elegoo_TFTLCD screen){
  screen.fillScreen(LIGHTGREEN);
  screen.setTextColor(DARKGREEN);    
  screen.setTextSize(4);
  screen.println("CORRECT!");
  // screen.fillRect(2,127,230,112,DARKGREEN);
  // screen.setCursor(10,125);
  // screen.setCursor(20,145);
  screen.setTextColor(WHITE);    
  screen.setTextSize(2);
  screen.println("Neighborhoods consume\n less power than other\n consumers");
   screen.setCursor(16,255);
  screen.setTextColor(DARKDARKGREY);    
  screen.setTextSize(2);
  screen.println("SCAN THE QR CODE TO LEARN\nMORE ABOUT 1 PHASE POWER");
}

void smart_house_1PP(Elegoo_TFTLCD screen){
  screen.fillScreen(LIGHTGREEN);
  screen.setTextColor(DARKGREEN);    
  screen.setTextSize(4);
  screen.println("CORRECT!");
  // screen.fillRect(2,127,230,112,DARKGREEN);
  // screen.setCursor(10,125);
  // screen.setCursor(20,145);
  screen.setTextColor(WHITE);    
  screen.setTextSize(2);
  screen.println("Solar homes can provide\n their own energy and\n take from the grid as\n needed");
   screen.setCursor(16,255);
  screen.setTextColor(DARKDARKGREY);    
  screen.setTextSize(2);
  screen.println("SCAN THE QR CODE TO LEARN\nMORE ABOUT SOLAR POWER");
}

void factory_3PP(Elegoo_TFTLCD screen){
  screen.fillScreen(LIGHTGREEN);
  screen.setTextColor(DARKGREEN);    
  screen.setTextSize(4);
  screen.println("CORRECT!");
  // screen.fillRect(2,127,230,112,DARKGREEN);
  // screen.setCursor(10,125);
  // screen.setCursor(20,145);
  screen.setTextColor(WHITE);    
  screen.setTextSize(2);
  screen.println("Factories use 3 Phase\n Power to efficently\n operate powerfull\n machinery.");
   screen.setCursor(16,255);
  screen.setTextColor(DARKDARKGREY);    
  screen.setTextSize(2);
  screen.println("SCAN THE QR CODE TO LEARN\nMORE ABOUT 3 PHASE POWER");
}

void solar_farm_3PP(Elegoo_TFTLCD screen){
  screen.fillScreen(LIGHTGREEN);
  screen.setTextColor(DARKGREEN);    
  screen.setTextSize(4);
  // screen.println("CORRECT!");
  // screen.fillRect(2,127,230,112,DARKGREEN);
  // screen.setCursor(10,125);
  // screen.setCursor(20,145);
  screen.setTextColor(WHITE);    
  screen.setTextSize(2);
  screen.println("Solar farms provide/n 3 Phase Power to/n the grid");
  screen.println("This is transformed/n and used by consumers");
   screen.setCursor(16,255);
  screen.setTextColor(DARKDARKGREY);    
  screen.setTextSize(2);
  screen.println("SCAN THE QR CODE TO LEARN\nMORE ABOUT SOLAR POWER");
}

void smart_house_3PP(Elegoo_TFTLCD screen){
  screen.fillScreen(YELLOW);
  screen.setTextColor(LIGHTGREY);    
  screen.setTextSize(4);
  screen.println("CANT PRODUCE");
  screen.println("ENOUGH POWER");
  // screen.fillRect(2,112,230,127,LIGHTGREY);
  // screen.setCursor(10,125);
  // screen.setCursor(20,145);
  screen.setTextColor(DARKGREY);    
  screen.setTextSize(2);
  screen.println("The Solar Home doesn't \nproduce enough extra \npower to provide 3 \nPhase Power to the grid");
  screen.setCursor(16,255);
  screen.setTextColor(LIGHTGREY);    
  screen.setTextSize(2);
  screen.println("SCAN THE QR CODE TO LEARN\nMORE ABOUT SOLAR POWER");
}

void house_house(Elegoo_TFTLCD screen){
  screen.fillScreen(LIGHTGREEN);
  screen.setTextColor(DARKGREEN);    
  screen.setTextSize(4);
  screen.println("CORRECT!");
  // screen.fillRect(2,127,230,112,DARKGREEN);
  // screen.setCursor(10,125);
  // screen.setCursor(20,145);
  screen.setTextColor(WHITE);    
  screen.setTextSize(2);
  screen.println("Multiple Neighborhoods can share\n 1 Phase Power\nNeighborhoods use les\ns power than other\n consumers");
   screen.setCursor(16,255);
  screen.setTextColor(DARKDARKGREY);    
  screen.setTextSize(2);
  screen.println("SCAN THE QR CODE TO LEARN\nMORE ABOUT 1 PHASE POWER");
}

void house_smhouse(Elegoo_TFTLCD screen){
  screen.fillScreen(LIGHTGREEN);
  screen.setTextColor(DARKGREEN);    
  screen.setTextSize(4);
  screen.println("CORRECT!");
  screen.fillRect(2,127,230,112,DARKGREEN);
  screen.setCursor(10,125);
  screen.setCursor(20,145);
  screen.setTextColor(WHITE);    
  screen.setTextSize(2);
  screen.println("Here the Neighborhood can\n get power from the grid,\n and the solar homes can\n buy supplimental power\n as needed");
   screen.setCursor(16,255);
  screen.setTextColor(DARKDARKGREY);    
  screen.setTextSize(2);
  screen.println("SCAN THE QR CODE TO LEARN\nMORE ABOUT 1 PHASE POWER");
}

void city_smhouse(Elegoo_TFTLCD screen){
  screen.fillScreen(LIGHTGREEN);
  screen.setTextColor(DARKGREEN);    
  screen.setTextSize(4);
  screen.println("CORRECT!");
  screen.fillRect(2,127,230,112,DARKGREEN);
  screen.setCursor(10,125);
  screen.setCursor(20,145);
  screen.setTextColor(WHITE);    
  screen.setTextSize(2);
  screen.println("Here the Commercial District\n can get power from the grid,\n and the solar homes can\n buy supplimental power\n as needed");
  screen.setCursor(16,255);
  screen.setTextColor(DARKDARKGREY);    
  screen.setTextSize(2);
  screen.println("SCAN THE QR CODE TO LEARN\nMORE ABOUT 1 PHASE POWER");
}

void solfarm_smhouse(Elegoo_TFTLCD screen){
  screen.fillScreen(LIGHTGREEN);
  screen.setTextColor(DARKGREEN);    
  screen.setTextSize(4);
  screen.println("CORRECT!");
  screen.fillRect(2,127,230,112,DARKGREEN);
  screen.setCursor(10,125);
  screen.setCursor(20,145);
  screen.setTextColor(WHITE);    
  screen.setTextSize(2);
  screen.println("Here both power producers\n can sell to the grid,\n or store power as needed");
   screen.setCursor(16,255);
  screen.setTextColor(DARKDARKGREY);    
  screen.setTextSize(2);
  screen.println("SCAN THE QR CODE TO LEARN\nMORE ABOUT SOLAR POWER");
}

void tft_call(Elegoo_TFTLCD screen, String message){
screen.fillScreen(BLUE);
screen.setTextColor(WHITE);    screen.setTextSize(1);
screen.println(message);
return;
}

 //Tile House, SmHouse, Factory, City, Solar;
// Tile House;
// House.Num = 4;
// House.compat[0] = .5;
// House.compat[1] = .5;
// House.compat[2] = .5;
// House.compat[3] = .5;
// House.compat[4] = 1;
// House.compat[5] = 5;


// class Tile{
//   public:
//     int Num;
//     //int position;
//     float compat[6]={0,0,0,0,0,0};
//     //boolean works = NULL;
//     Tile::Tile(int Num, float compat[]) {
//   // Use 'this->' to make the difference between the
//   // 'pin' attribute of the class and the
//   // local variable 'pin' created from the parameter.
//   this->Num = Num;
//   this->compat[0]=compat[0];
//   this->compat[1]=compat[1];
//   this->compat[2]=compat[2];
//   this->compat[3]=compat[3];
//   this->compat[4]=compat[4];
//   this->compat[5]=compat[5];
// }
// };



// Setting Lines
// #LEDs, pin, Color(don't change)
// Need 6 Interupts (User Caused)
Adafruit_NeoPixel long_strip = Adafruit_NeoPixel(122, LONGPIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel LINE1 = Adafruit_NeoPixel(8, LINEA, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel LINE2 = Adafruit_NeoPixel(3, LINEB, NEO_GRB + NEO_KHZ800); // Tile 1
Adafruit_NeoPixel LINE3 = Adafruit_NeoPixel(6, LINEC, NEO_GRB + NEO_KHZ800); // Tile 2
Adafruit_NeoPixel LINE4 = Adafruit_NeoPixel(9, LINED, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel LINE5 = Adafruit_NeoPixel(3, LINEE, NEO_GRB + NEO_KHZ800); // Tile 3
Adafruit_NeoPixel LINE6 = Adafruit_NeoPixel(3, LINEF, NEO_GRB + NEO_KHZ800); // Tile 4 
Adafruit_NeoPixel LINE7 = Adafruit_NeoPixel(13, LINEG, NEO_GRB + NEO_KHZ800); // Tile 5
Adafruit_NeoPixel LINE8 = Adafruit_NeoPixel(2, LINEH, NEO_GRB + NEO_KHZ800); // Tile 6
Adafruit_NeoPixel LINE9 = Adafruit_NeoPixel(2, LINEI, NEO_GRB + NEO_KHZ800); // Tile 6
Adafruit_NeoPixel LINE10 = Adafruit_NeoPixel(3, LINEJ, NEO_GRB + NEO_KHZ800); // Tile 6
Adafruit_NeoPixel title = Adafruit_NeoPixel(52, TITLE, NEO_GRB + NEO_KHZ800);

//SHOULD BE GLOBALS
// array to store tile names at locations
  //array to store tile #'s at locations
  int numMap[6]= {0,0,0,0,0,0};
  //store tile compat at each location
  float locationCompat[6]= {0,0,0,0,0,0};
  const float compat[7][6] = {{0,0,0,0,0,0},{1,1,1,1,5},{-1.5,-1.5,-1.5,-1.5,-1,4},{3,3,3,3,3,1},{.5,.5,.5,.5,1,5},{0,0,0,0,0,0},{-3,-3,-3,-3,-3,-3}};
  int Powerlines[]= {0,0,0,0,0,0,0,0,0};
int button_slot(int slot[]){
  int first, second, third;
  int oldval=0,a=0;
  while(a<10){
  first= digitalRead(slot[0]);
  second = digitalRead(slot[1]);
  third = digitalRead(slot[2]);
  if ((first+second*2+third*4)==oldval){
    a++;
    oldval = (first+second*2+third*4);
  }
  else{
    a=0;
  }
}
return (first+second*2+third*4);
}

void LED_out(int slot){
  Powerlines[slot] = compat_to_direc(locationCompat[slot]);
if(slot<4){
 switch(slot){
  case(0):
  case(1):
  Powerlines[7] = compat_to_direc(locationCompat[0]+locationCompat[1]);

  break;
  case(2):
  case(3):
  Powerlines[8] = compat_to_direc(locationCompat[2]+locationCompat[3]);
   
 }
}
}

void disp_out(int slot){

if(slot==0||slot==1){  
  tft1.fillScreen(BLACK);
  if (Powerlines[7]==3){
    not_enough_power_double(tft1);
  }
  else if(Powerlines[7]==0){
    microgrid(tft1);
  }
  else{
   int unique = numMap[0]^ numMap[1];
   switch(unique){
    case(0):
    house_house(tft1);
    break;
    case(3):
    city_smhouse(tft1);
    break;
    case(4):
    solfarm_smhouse(tft1);
    break;
    case(6):
    house_smhouse(tft1);
    break;
    default:
    Directions_1(tft1);
   }
    
  }
}
else if(slot==2||slot==3){
    tft2.fillScreen(BLACK);
    if (Powerlines[8]==3){
    not_enough_power_double(tft2);
  }
  else if(Powerlines[8]==0){
    microgrid(tft2);
  }
  else{
   int unique = numMap[0]^ numMap[1];
   switch(unique){
    case(0):
    house_house(tft1);
    break;
    case(3):
    city_smhouse(tft1);
    break;
    case(4):
    solfarm_smhouse(tft1);
    break;
    case(6):
    house_smhouse(tft1);
    break;
    default:
    Directions_1(tft1);
   }
  }
}
else if(slot==5){
  tft3.fillScreen(BLACK);
  switch(numMap[5]){
    case(1):
    // city
    city_1PP(tft3);
    break;
    case(2):
    //smart house
    smart_house_1PP(tft3);
    break;
    case(3):
    // factory
    not_enough_power_Factory(tft3);
    break;
    case(4):
    //house
    house_1PP(tft3);
    break;
    case(6):
    //solar farm
    solar_farm_1PP(tft3);
    break;
    default:
      //empty (0)
      Directions_3(tft3);
  }
}
else if(slot ==6){
  tft3.fillScreen(BLACK);
  switch(numMap[6]){
        case(1):
    // city
    too_much_power(tft3);
    break;
    case(2):
    //smart house
    smart_house_3PP(tft3);
    break;
    case(3):
    // factory
    factory_3PP(tft3);
    break;
    case(4):
    //house
    too_much_power(tft3);
    break;
    case(6):
    //solar farm
    solar_farm_3PP(tft3);
    break;
    default:
      //empty (0)
      Directions_3(tft3);
  }

}
}


int compat_to_direc(float compat){
int direc;
 if(compat<=1&&compat>=-1.5&&compat!=0)
  return 1;
if(compat<=-3)
  return -1;
if(compat==-2||compat==-2.5)
  return 0;
if(compat>=1.5)
  return 3;
  else{
 Serial.println("invalid compat val");
 return(-999);
 }
 }

 void longFlow(uint32_t c, uint8_t wait, uint16_t boop) { // Specifically only for the main long line
  // Set start points
  uint16_t sml_start = 36;
  uint16_t med_start = 37;
  uint16_t lrg_start = 122;
  uint16_t sml_boop, med_boop, lrg_boop; // Boops

  
  sml_boop = sml_start + boop; // Sets boop, main, line
  med_boop = med_start - boop;
  lrg_boop = lrg_start + boop;

  for (uint16_t i=0; i<=48; i++) { // Cycle through at least the longest strand
    if (sml_start >= 0) // Small Line
      long_strip.setPixelColor(sml_start, c); // Signal line
      if (sml_boop <= 36) // Only runs when "boop" is on the correct line
        long_strip.setPixelColor(sml_boop, long_strip.Color(0, 0, 0)); // Sets main line
      sml_start--;
      sml_boop = sml_start + boop;

    if (med_start <= 76) // Medium Line
      long_strip.setPixelColor(med_start, c);
      long_strip.setPixelColor(med_boop, long_strip.Color(0, 0, 0));
      med_start++;
      med_boop = med_start - boop;

    if (lrg_start >= 77) // Large Line
      //lrg_boop = lrg_start + boop; // Sets boop, main, line
      long_strip.setPixelColor(lrg_start, c); // Signal line
      if (lrg_boop <= 122) // Only runs when "boop" is on the correct line
        long_strip.setPixelColor(lrg_boop, long_strip.Color(0, 0, 0)); // Sets main line
      lrg_start--;
      lrg_boop = lrg_start + boop;

    long_strip.show();
    delay(wait);
  }
}

void Flow(Adafruit_NeoPixel line, uint32_t c, int mode, uint16_t boop) { // Stops after being triggered once, probably useful for interrupts
  uint16_t numPixels = line.numPixels();
  uint16_t line_boop;
if(mode ==1){
  for(uint16_t i=0; i<= numPixels; i++) { // Instead of working like this, ask it to trigger every line instead of having to specify, useful on LINE1, LINE4, and LINE7
    line.setPixelColor(i, c);
    line_boop = i-boop;
    line.setPixelColor(line_boop, line.Color(0, 0, 0));
    line.show();
    delay(50);
  }
  for (uint16_t j = 0; j<=boop; j++) { // Clears boop
    line.setPixelColor(line_boop, line.Color(0, 0, 0));
    line_boop++;
    line.show();
    delay(50);
  }
}
else if(mode == -1){// reverse
  for(uint16_t i=numPixels; i>= 0; i--) {
    line.setPixelColor(i, c);
    line_boop = i+boop;
    line.setPixelColor(line_boop, line.Color(0, 0, 0));
    line.show();
    delay(50);
  }
  for (uint16_t j = 0; j<=boop; j++) {
    line.setPixelColor(line_boop, line.Color(0, 0, 0));
    line_boop++;
    line.show();
    delay(50);
  }
  
}
else if(mode ==0 ){//off
  for(uint16_t i=numPixels; i>= 0; i--) {
    line.setPixelColor(line_boop, line.Color(0, 0, 0));
    line.show();
    delay(50);
  }  
}
else if(mode == 3){
    for(uint16_t i=numPixels; i>= 0; i--) {
    line.setPixelColor(line_boop, line.Color(127, 0, 0));
    line.show();
    delay(50);
  }
}
}

void playLEDs(void){
  longFlow(long_strip.Color(127, 127, 0), 50, 6);
    Flow(LINE1, LINE1.Color(127, 200, 0), Powerlines[1], 3);
    Flow(LINE2, LINE1.Color(127, 200, 0), Powerlines[2], 3);
    Flow(LINE3, LINE1.Color(127, 200, 0), Powerlines[3], 3);
    Flow(LINE4, LINE1.Color(127, 200, 0), Powerlines[4], 3);
    Flow(LINE5, LINE1.Color(127, 200, 0), Powerlines[5], 3);
    Flow(LINE6, LINE1.Color(127, 200, 0), Powerlines[6], 3);
    Flow(LINE7, LINE1.Color(127, 200, 0), Powerlines[7], 3);
    Flow(LINE8, LINE1.Color(127, 200, 0), Powerlines[8], 3);
  
}

void setup() {
  //array to store tile #'s at locations
  //numMap
  //store tile compat at each location
  //locationCompat
  long_strip.begin();
  long_strip.setBrightness(50);
  long_strip.show(); // Initialize all pixels to 'off'

  LINE1.begin();
  LINE1.setBrightness(50);
  LINE1.show(); // Initialize all pixels to 'off'

  LINE2.begin();
  LINE2.setBrightness(50);
  LINE2.show(); // Initialize all pixels to 'off'

  LINE3.begin();
  LINE3.setBrightness(50);
  LINE3.show(); // Initialize all pixels to 'off'

  LINE4.begin();
  LINE4.setBrightness(50);
  LINE4.show(); // Initialize all pixels to 'off'

  LINE5.begin();
  LINE5.setBrightness(50);
  LINE5.show(); // Initialize all pixels to 'off'

  LINE6.begin();
  LINE6.setBrightness(50);
  LINE6.show(); // Initialize all pixels to 'off'

  LINE7.begin();
  LINE7.setBrightness(50);
  LINE7.show(); // Initialize all pixels to 'off'

  LINE8.begin();
  LINE8.setBrightness(50);
  LINE8.show(); // Initialize all pixels to 'off'

  LINE9.begin();
  LINE9.setBrightness(50);
  LINE9.show(); // Initialize all pixels to 'off'

  LINE10.begin();
  LINE10.setBrightness(50);
  LINE10.show(); // Initialize all pixels to 'off'

  title.begin();
  title.setBrightness(50);
  title.show(); // Initialize all pixels to 'off'


  Serial.begin(9600);
  //LED STRANDS




  /*off = 000
 * 1= 001
 * 2= 010
 * 3= 011
 * 4= 100
 * 5= 101 *NA*
 * 6=110
 * invalid = 111
 */



  Serial.begin(9600);
  LCD_init(tft1);
  LCD_init(tft2);
  LCD_init(tft3);
  delay(1000);
  
  //intialize GPIO button inputs
   int i;
   for(i=22; i<39; i++){
      pinMode(i, INPUT);
   }

}
void loop() 
{

  longFlow(long_strip.Color(127, 127, 0), 50, 6);
  // put your main code here, to run repeatedly:
  int i,num;
  //Tile tileName;
  for(i=0;i<6;i++)
  {
  num = button_slot(buttons[i]);  //number pressed in buttons
  if(num!= numMap[i]){// if the slot value changed
    numMap[i] = num; //puts tile nuber value in number map
    //tileMap[i] = tileName; //puts tile name in location array
    locationCompat[i] = compat[num][i];
    Serial.println(num);
    LED_out(i);
    disp_out(i);
    }
    playLEDs();
  }
}

