/*
  NAME:
  Demonstration of the basic running statistics filtering

  DESCRIPTION:
  This sketch demonstrates usage of the "gbj_running" library.
  - As a measured value the sketch utilizes the random integers within
    the range 0 to 1024 and maps them to the float numbers 0.0 ~ 100.0.
  - For demonstration purposes each data point is output.
  - The sketch calculates just the running average.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#define SKETCH "GBJ_RUNNING_AVERAGE 1.3.0"

#if defined(__AVR__)
  #include <Arduino.h>
  #include <inttypes.h>
#elif defined(ESP8266) || defined(ESP32)
  #include <Arduino.h>
#elif defined(PARTICLE)
  #include <Particle.h>
#endif
#include "gbj_running.h"

const unsigned int PERIOD_MEASURE = 3000;
const unsigned int SENSOR_DATA_MAX = 1023;
const float COEF_DEMO = 100.0 / float(SENSOR_DATA_MAX);
float demoData, filterData;

gbj_running avgRunning = gbj_running();

void setup()
{
  Serial.begin(115200);
  delay(2000);
  Serial.print("Sketch: ");
  Serial.println(SKETCH);
  Serial.println("Libraries:");
  Serial.println(gbj_running::VERSION);
  Serial.println(gbj_apphelpers::VERSION);
  Serial.println("---");
  // Print header
  Serial.println("Data\tAverage\tItems");
}

void loop()
{
  demoData = COEF_DEMO * (float)random(SENSOR_DATA_MAX + 1); // Measured data
  filterData = avgRunning.getValue(demoData); // Filtered data
  Serial.print(demoData);
  Serial.print("\t");
  Serial.print(filterData);
  Serial.print("\t");
  Serial.print(avgRunning.getSamples());
  Serial.println();
  delay(PERIOD_MEASURE);
}
