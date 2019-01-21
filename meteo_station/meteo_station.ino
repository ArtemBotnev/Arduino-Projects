
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <DFRobot_SHT20.h>

#include "display.h"

#define SENSOR_DELAY 100
#define SCREEN_DELAY 2800

Adafruit_BME280 bme;
DFRobot_SHT20 sht20;

Display display;

void setup() {
    bme.begin();
    sht20.initSHT20();
    display.begin();
}

void loop() {
    readTemperatureAndShow();
    readHumidityAndShow();
    readAtmPressureAndShow();
}

void readTemperatureAndShow() {
    int8_t roomTemper = (int8_t) bme.readTemperature();
    delay(SENSOR_DELAY);

    int8_t outTemper = (int8_t) sht20.readTemperature();
    delay(SENSOR_DELAY);

    display.drawTemperatureMenu(outTemper, roomTemper);
    delay(SCREEN_DELAY);
}

void readHumidityAndShow() {
    uint8_t roomHumidity = (uint8_t) bme.readHumidity();
    delay(SENSOR_DELAY);

    uint8_t outHumidity = (uint8_t) sht20.readHumidity();
    delay(SENSOR_DELAY);

    display.drawHumidityMenu(outHumidity, roomHumidity);
    delay(SCREEN_DELAY);
}

void readAtmPressureAndShow() {
    // mmhg
    uint16_t pressure = (uint16_t) (bme.readPressure() * 0.0075F);
    delay(2 * SENSOR_DELAY);

    display.drawAtmPressureMenu(pressure);
    delay(SCREEN_DELAY);
}
