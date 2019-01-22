//
// Created by Artem Botnev on 04/01/19.
//

/**
 * For TFT display 1.44’
 * renders measured parameters' menu
 *
 * Display pin			UNO board pin
 * Vcc					Vcc
 * GND					GND
 * CS					10
 * RESET				8
 * AO					9
 * SDA					11
 * SCK					13
 * LED					Vcc
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include <SPI.h>
#include <TFT_ILI9163C.h>

extern "C" {
    #include "colors.h"
};

// uno, pro-mini pins
#define __CS 10
#define __DC 9
#define __RS 8

// texts size
#define TITLE_TEXT_SIZE 1
#define VALUE_TEXT_SIZE 5

// titles
#define OUT_TEMPER_TITLE    "Outdoors t, C"
#define ROOM_TEMPER_TITLE   "Room t, C"
#define OUT_HUM_TITLE       "Outdoors h, %"
#define ROOM_HUM_TITLE      "Room h, %"
#define PRESSURE_TITLE      "Atm.press. p, mmHg"

class Display {

public:
    Display();

    void begin();

    void drawTemperatureMenu(int8_t outTemperature, int8_t roomTemperature);

    void drawHumidityMenu(uint8_t outHumidity, uint8_t roomHumidity);

    void drawAtmPressureMenu(uint16_t pressure);

private:
    void drawTopMenu(int8_t value, const char *header, uint16_t (*value_color)(int8_t));

    void drawBottomMenu(int8_t value, const char *header, uint16_t (*value_color)(int8_t));
};

#endif
