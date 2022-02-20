/*
  NAME:
  Usage of library with all statistics

  DESCRIPTION:
  This sketch demonstrates the use of the "gbj_running" library with
  all type of statistics.
  - As a measured value the sketch utilizes the random integers within
    the range 0 to 1024 and maps them to the float numbers 0.0 ~ 100.0.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#define SKETCH "GBJ_RUNNING_ALL 1.3.0"

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
float demoData;
float filterAvg, filterMed, filterMin, filterMax; // Running statistics

gbj_running runAvg = gbj_running(gbj_running::AVERAGE);
gbj_running runMed = gbj_running(gbj_running::MEDIAN);
gbj_running runMin = gbj_running(gbj_running::MINIMUM);
gbj_running runMax = gbj_running(gbj_running::MAXIMUM);

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
  Serial.println("Data\tAverage\tMedian\tMinimum\tMaximum\tItems");
}

void loop()
{
  demoData = COEF_DEMO * (float)random(SENSOR_DATA_MAX + 1); // Measured data
  filterAvg = runAvg.getValue(demoData);
  filterMed = runMed.getValue(demoData);
  filterMin = runMin.getValue(demoData);
  filterMax = runMax.getValue(demoData);
  Serial.print(demoData);
  Serial.print("\t");
  Serial.print(filterAvg);
  Serial.print("\t");
  Serial.print(filterMed);
  Serial.print("\t");
  Serial.print(filterMin);
  Serial.print("\t");
  Serial.print(filterMax);
  Serial.print("\t");
  Serial.print(runAvg.getSamples());
  Serial.println();
  delay(PERIOD_MEASURE);
}
