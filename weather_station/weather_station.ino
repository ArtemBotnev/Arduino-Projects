/*
  *“Digital thermometer -  barometer”.
  *The sketch for boards arduino UNO or mini Pro.
  *It’s public software.
  *Created by Artem Botnev.
  *
  *It includes functions of indoor and outdoor air temperature indication 
  *and so atmospheric pressure. Data are rendered on TFT display  1.44’.
  *
  *Outdoor air temperature sensor                             - DALLAS DS18B20
  *Indoor air temperature and atmospheric pressure sensor     - BMP180
  *Display                                                    - TFT 1.44’ color SPI 
  
  *Display pin			Arduino pin
  *Vcc					Vcc
  *GND					GND
  *CS					10
  *RESET				9
  *AO					8
  *SDA					11
  *SCK					13
  *LED					Vcc
  *DS18B20 pin			Arduino pin
  *Vcc					Vcc
  *GND					GND
  *DATA					7
  *BMP180 pin			Arduino pin
  *Vcc					Vcc
  *GND					GND
  *SCL					A5
  *SDA					A4
  *Button pin			Arduino pin
  *button				2
*/

#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <TFT_ILI9163C.h>

#define ONE_WIRE_BUS 7//pin of one wire interface

//colors for the display
#define BLACK         0x0000
#define BLUE          0x001F
#define LIGHT_BLUE  0x689AD3
#define RED           0xF800
#define GREEN         0x07E0
#define CYAN          0x07FF
#define MAGENTA       0xF81F
#define YELLOW        0xFFE0  
#define WHITE         0xFFFF
//pins of spi interface
#define __CS 10
#define __DC 9
#define __RS 8

//Dallas variables
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress thermometerOut = {0x28, 0xFF, 0x84, 0xB6, 0x63,
0x16, 0x04, 0x62}; // address of Dallas sensor
//BMP180 variable
Adafruit_BMP085 bmp;
//display variable
TFT_ILI9163C tft = TFT_ILI9163C(__CS, __RS, __DC);


//general variables
short outTemp, roomTemp, atmPress, lastOutTemp, lastRoomTemp, lastAtmPress;
byte menuIndex, lastIndex; //index menu of display

//display's functions
long outColor(int outTemp){
  if(outTemp < -17){
    return BLUE;
  } else if(outTemp < -7){
    return LIGHT_BLUE;
  } else if(outTemp < 2){
    return WHITE;
  } else if(outTemp < 12){
    return YELLOW;
  } else if(outTemp < 25){
    return GREEN;
  } else {
    return RED;
  }
}

long roomColor(int roomTemp){
  if(roomTemp < 17){
    return BLUE;
  } else if(roomTemp > 23){
    return RED;
  } else return GREEN;
}

long pressColor(int atmPress){
  if(atmPress < 710 || atmPress > 790){
	return RED;
  } else if(atmPress < 730 || atmPress > 770){
	return YELLOW;
  } else return GREEN;
}

void outRoomMenu(int outTemp, int roomTemp){
  tft.fillScreen();
  tft.setCursor(23, 5);
  tft.setTextColor(WHITE);
  tft.setTextSize(1);
  tft.println("Outdoors t, C");
  if(outTemp < 0){
    tft.setCursor(20, 20);
  } else if(outTemp < 10){
    tft.setCursor(50, 20);  
  } else tft.setCursor(35, 20);
  tft.setTextColor(outColor(outTemp));  
  tft.setTextSize(5);
  tft.print(outTemp);
  //////////////////////////////////  
  tft.drawLine(0, 64, 128, 64, WHITE);
  //////////////////////////////////
  tft.setCursor(33, 69);
  tft.setTextColor(WHITE);
  tft.setTextSize(1);
  tft.println("Room t, C");
  if(roomTemp < 0){
    tft.setCursor(20, 84);
  } else if(roomTemp < 10){
    tft.setCursor(50, 84);  
  } else tft.setCursor(35, 84);
  tft.setTextColor(roomColor(roomTemp));
  tft.setTextSize(5);
  tft.print(roomTemp);
}

void outPressMenu(int outTemp, int atmPress){
  tft.fillScreen();
  tft.setCursor(23, 5);
  tft.setTextColor(WHITE);
  tft.setTextSize(1);
  tft.println("Outdoors t, C");
  if(outTemp < 0){
  tft.setCursor(20, 20);
  } else if(outTemp < 10){
  tft.setCursor(50, 20);  
  } else tft.setCursor(35, 20);
  tft.setTextColor(outColor(outTemp));  
  tft.setTextSize(5);
  tft.print(outTemp);
  //////////////////////////////////
  tft.drawLine(0, 64, 128, 64, WHITE);
  //////////////////////////////////
  tft.setCursor(9, 69);
  tft.setTextColor(WHITE);
  tft.setTextSize(1);
  tft.println("Atm.press. p, mmHg");
  tft.setCursor(26, 89);
  tft.setTextColor(pressColor(atmPress));
  tft.setTextSize(4);
  tft.print(atmPress);
}

void roomPressMenu(int roomTemp, int atmPress){
  tft.fillScreen();
  tft.setCursor(33, 5);
  tft.setTextColor(WHITE);
  tft.setTextSize(1);
  tft.println("Room t, C");
  if(roomTemp < 0){
  tft.setCursor(20, 20);
  } else if(roomTemp < 10){
  tft.setCursor(50, 20);  
  } else tft.setCursor(35, 20);
  tft.setTextColor(roomColor(roomTemp));
  tft.setTextSize(5);
  tft.print(roomTemp);
  //////////////////////////////////
  tft.drawLine(0, 64, 128, 64, WHITE);
  //////////////////////////////////
  tft.setCursor(9, 69);
  tft.setTextColor(WHITE);
  tft.setTextSize(1);
  tft.println("Atm.press. p, mmHg");
  tft.setCursor(26, 89);
  tft.setTextColor(pressColor(atmPress));
  tft.setTextSize(4);
  tft.print(atmPress);
}

//general functions
void menuSelector(){
  bool changeOutRoom = 
    ((outTemp != lastOutTemp) || (roomTemp != lastRoomTemp));
  bool changeOutPress =
    ((outTemp != lastOutTemp) || (atmPress != lastAtmPress));
  bool changeRoomPress =
	((roomTemp != lastRoomTemp) || (atmPress != lastAtmPress));
  bool changeIndex = menuIndex != lastIndex;
   
  switch(menuIndex){
	case 0:
		if(changeOutRoom || changeIndex)
			outRoomMenu(outTemp, roomTemp);
		break;
	case 1:
		if(changeOutPress || changeIndex)
			outPressMenu(outTemp, atmPress);
		break;
	default:
		if(changeRoomPress || changeIndex)
			roomPressMenu(roomTemp, atmPress);
	}
	
  lastOutTemp = outTemp;
  lastRoomTemp = roomTemp;
  lastAtmPress = atmPress;
  lastIndex = menuIndex;
}

void indexToggle(){
  menuIndex++;
  menuIndex = menuIndex % 3;
  menuSelector();
}

void setup() {
  sensors.begin();
  sensors.setResolution(thermometerOut, 10);
  bmp.begin();
  tft.begin();
  attachInterrupt(0, indexToggle, RISING);
}

void loop() {
  sensors.requestTemperatures();  
  outTemp = (short)sensors.getTempC(thermometerOut);
  delay(500);
  roomTemp = (short)bmp.readTemperature();
  atmPress = (short)((float)bmp.readPressure() / 133.321995);  
 
  menuSelector();   
  
  delay(60000);  
}
