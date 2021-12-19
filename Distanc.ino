#include "ssd1306.h"
#include "nano_gfx.h"
#include <Wire.h>
#include <VL53L0X.h>
#define button A0
unsigned char buffer[8];
VL53L0X sensor;
int distance = 0;
int lagori[5] = {0, 0, 0, 0, 0};
int8_t counter = 0;
void setup()
{
  ssd1306_128x64_i2c_init();
  ssd1306_setFixedFont(ssd1306xled_font6x8);
  ssd1306_clearScreen();
  ssd1306_printFixed(0,  0, "   ", STYLE_NORMAL);
  ssd1306_printFixed(0,  10, "VL53L0X Distance", STYLE_NORMAL);
  ssd1306_printFixed(0,  25, "Reading: ", STYLE_NORMAL);
  Wire.begin();
  pinMode(button, INPUT);
  sensor.setTimeout(500);
  if (!sensor.init())
  {
    ssd1306_clearScreen();
    ssd1306_printFixed(0,  25, "Failed to detect and initialize sensor!", STYLE_NORMAL);
    while (1);
  }
  sensor.startContinuous();
}

void loop()
{
  distance = sensor.readRangeContinuousMillimeters();
  if (distance <= 20) {
    //ssd1306_printFixed(0,  40, "                      ", STYLE_NORMAL);
    ssd1306_printFixed(0,  40, "Object too close!    ", STYLE_NORMAL);
  }
  else if (distance >= 8000) {
    //ssd1306_printFixed(0,  40, "                      ", STYLE_NORMAL);
    ssd1306_printFixed(0,  40, "Object too far!      ", STYLE_NORMAL);
  }
  else {
    ssd1306_printFixed(0,  25, "Reading:   ", STYLE_NORMAL);
    dtostrf(distance, 6, 0, buffer);
    ssd1306_printFixed(45,  25, buffer, STYLE_NORMAL);
    ssd1306_printFixed(90,  25, "mm", STYLE_NORMAL);
    ssd1306_printFixed(0,  40, "                      ", STYLE_NORMAL);
  }
  if (sensor.timeoutOccurred()) {
    ssd1306_printFixed(0,  40, "SENSOR TIMEOUT!", STYLE_NORMAL);
  }
  if (digitalRead(button)) {
    if (counter < 5) {
      lagori[counter] = distance;
      counter++;
      ssd1306_printFixed(0,  40, "                      ", STYLE_NORMAL);
      ssd1306_printFixed(0,  40, "Largori set", STYLE_NORMAL);
      delay(400);
    }
    else {
      ssd1306_printFixed(0,  40, "                      ", STYLE_NORMAL);
      ssd1306_printFixed(0,  40, "Largori  all set", STYLE_NORMAL);
      char num[20];
      sprintf(num, "%u,%u,%u,%u,%u",lagori[0],lagori[1],lagori[2],lagori[3],lagori[4]);
      ssd1306_printFixed(0,  50, num, STYLE_NORMAL);
      delay(5000);
      ssd1306_printFixed(0,  50, "                      ", STYLE_NORMAL);
      counter = 0;
    }
  }
  delay(10);
}
