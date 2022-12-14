#include <stdio.h>
#include <stdlib.h>

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

// Assign human-readable names to some common 16-bit color values:
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

Elegoo_TFTLCD tft2(LCD_CS2, LCD_CD2, LCD_WR2, LCD_RD2, LCD_RESET2);
Elegoo_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
Elegoo_TFTLCD tft3(LCD_CS3, LCD_CD3, LCD_WR3, LCD_RD3, LCD_RESET3);

const int buttons[2][3] ={{22,23,24},{25,26,27},{28,29,30},{31,32,33},{34,35,36},{37,38,39}};            // GPIO 22 for the button


//tile object/struct is created and init
struct Tile{
  int Num;
  int position;
  float compat[6];
  boolean works = NULL;
  
  
} ;//House, SmHouse, Factory, City, Solar, Empty


//LED STRANDS
int MainLine;

 Tile House, SmHouse, Factory, City, Solar;
 House.Num = 4;
 House.compat = {.5,.5,.5,.5,1,5};

 SmHouse.Num = 2;
 SmHouse.compat = {-1.5,-1.5,-1.5,-1.5,1,4};

 Factory.Num = 3;
 Factory.compat ={3,3,3,3,3,1};

 City.Num = 1;
 City.compat = {1,1,1,1,5};

 Solar.Num = 6;
 Solar.compat = {-3,-3,-3,-3,-3,-3};

 Empty.Num = 0;
 Empty.compat = {0,0,0,0,0,0};


/*off = 000
 * 1= 001
 * 2= 010
 * 3= 011
 * 4= 100
 * 5= 101 *NA*
 * 6=110
 * invalid = 111
 */

int PowerLines[]= {0,0,0,0,0,0,0,0,0];

//SHOULD BE GLOBALS
// array to store tile names at locations
Tile tileMap[]={Empty,Empty,Empty,Empty,Empty,Empty};
//array to store tile #'s at locations
int numMap[] = {0,0,0,0,0,0};
//store tile compat at each location
float locationCompat[]= {0,0,0,0,0,0};


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

void setup() {

  Serial.begin(9600);
  
  
//intialize GPIO button inputs
   int i;
   for(i=22; i<39; i++){
      pinMode(i, INPUT);
   }

}
void loop() 
{
  // put your main code here, to run repeatedly:
  int i,num;
  Tile tileName;
  for(i=0;i<6;i++)
  {
  num = button_slot(buttons[i]);  //number pressed in buttons
  if(num!= numMap[i]){// if the slot value changed
    numMap[i] = num; //puts tile nuber value in number map
    tileName = getTile(num);
    tileMap[i] = tileName; //puts tile name in location array
    locationCompat[i] = tileName.compat[i];
    Serial.println(num);
    LED_out(tileName,i);
    disp_out(i);
    }
    runLEDs();
  delay(100);                    // wait
  }
}
int button_slot(int slot[]){
  int first, second, third;
  first= digitalRead(slot[0]);
  second = digitalRead(slot[1]);
  third = digitalRead(slot[2]);
  return (first+second*2+third*4);
}

Tile getTile ( int num){
  switch(num){
    case(1):
    return(City);
    case(2):
    return(SmHouse);
    case(3):
    return(Factory);
    case(4):
    return (House);
    case(6):
    return(Solar);
    case(0):
    return(Empty);
    default:
      Serial.println("invalid tile #");
      return NULL; 
    }
}

void LED_out(Tile peice, int slot){
  Powerlines(slot+1) = compat_to_direc(locationCompat[i]);
if(slot<4){
 switch(slot){
  case(0|1):
  Powerlines[7] = compat_to_direc(locationCompat[0]+locationCompat[1]);

  break;
  case(2|3):
  Powerlines[8] = compat_to_direc(locationCompat[2]+locationCompat[3]);
   
 }
}
}

void disp_out(int slot){
if(slot==0||slot==1){
  if (powerLine[7]==3){
    not_enough_power(tft1);
  }
  else if(powerline[7]==0){
    microgrid(tft1);
  }
  else{
    // check all unique cases and display tft1
  }
}
else if(slot==2||slot==3){
    if (powerLine[8]==3){
    not_enough_power(tft2);
  }
  else if(powerline[8]==0){
    microgrid(tft2);
  }
  else{
    // check all unique cases and display tft2
  }
}
else if(slot==5){
  switch(peice){
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
    not_enough_power(tft3);
    break;
    case(4):
    //house
    house_1PP(tft3);
    break;
    case(6):
    //solar farm
    SolarFarm_1PP(tft3);
    break;
    default{
      //empty (0)
      Directions_3(tft3);
    }
  }
}
else if(slot ==6){
  switch(peice){
        case(1):
    // city
    city_3PP(tft3);
    break;
    case(2):
    //smart house
    smart_house_3PP(tft3);
    break;
    case(3):
    // factory
    Factory_3PP(tft3);
    break;
    case(4):
    //house
    too_much_power(tft3);
    break;
    case(6):
    //solar farm
    SolarFarm_3PP(tft3);
    break;
    default{
      //empty (0)
      Directions_3(tft3);
    }
  }
}
}

int compat_to_direc(float compat){
int direc;
 switch(compat){
  case(-1.5|-1|-.5|.5|1):
  direc = 1;
  break;
  case(-4.5|-3):
  direc=-1;
  break;
  case(-2.5|-2|0):
  direc = 0;
  break;
  case(1.5|2.5|3.5|3|4|5)
  direc=3;
  break;
 default:
 Serial.println("invalid compat val");
 return(-999);
 }
 return direc;
 }
