/*
  NAME:
  Usage of library with all statistics.

  DESCRIPTION:
  This sketch demonstrates the use of Running statistics with determining all
  arguments that it provides.
  - As a measured value the sketch utilizes the random integers within
    the range 0 to 1023, but registers just the values withing range 128 ~ 768
    for demostrating filtering.
  - The sketch calculates all Running statistical types that the library provides.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#include "gbj_running.h"
#define SKETCH "GBJ_RUNNING_ALL 1.2.0"

const unsigned int PERIOD_MEASURE = 3000; // Time in miliseconds between measurements

// Limits of random values for mimicking real physical measurement
const unsigned int SENSOR_DATA_MIN = 0;
const unsigned int SENSOR_DATA_MAX = 1023;

// Valid range of values ensured by filtering
const unsigned int SENSOR_MIN = 128;
const unsigned int SENSOR_MAX = 768;

// Variables and constants for measurement
unsigned int demoData;
unsigned int filterAvg, filterMed, filterMin, filterMax; // Running statistics
gbj_running RunningAvg = gbj_running(gbj_running::AVERAGE, SENSOR_MAX, SENSOR_MIN);
gbj_running RunningMed = gbj_running(gbj_running::MEDIAN, SENSOR_MAX, SENSOR_MIN);
gbj_running RunningMin = gbj_running(gbj_running::MINIMUM, SENSOR_MAX, SENSOR_MIN);
gbj_running RunningMax = gbj_running(gbj_running::MAXIMUM, SENSOR_MAX, SENSOR_MIN);

void setup()
{
  Serial.begin(9600);
  Serial.println(SKETCH);
  Serial.println("Libraries:");
  Serial.println(gbj_running::VERSION);
  Serial.println(gbj_apphelpers::VERSION);
  Serial.println("---");
  Serial.print("Buffer length: ");
  Serial.println(RunningAvg.getBufferLen());
  Serial.print("Valid range: ");
  Serial.print(RunningAvg.getValueMin());
  Serial.print(" ~ ");
  Serial.println(RunningAvg.getValueMax());
  Serial.println("Value => Average Median Min Max Items:");
}

void loop()
{
  demoData = random(SENSOR_DATA_MIN, SENSOR_DATA_MAX + 1);
  filterAvg = RunningAvg.getStatistic(demoData);
  filterMed = RunningMed.getStatistic(demoData);
  filterMin = RunningMin.getStatistic(demoData);
  filterMax = RunningMax.getStatistic(demoData);
  if (demoData >= RunningAvg.getValueMin() && demoData <= RunningAvg.getValueMax())
  {
    Serial.print("*");
  }
  Serial.print(demoData);
  Serial.print(" => ");
  Serial.print(filterAvg);
  Serial.print(" ");
  Serial.print(filterMed);
  Serial.print(" ");
  Serial.print(filterMin);
  Serial.print(" ");
  Serial.print(filterMax);
  Serial.print(" ");
  Serial.print(RunningAvg.getReadings());
  Serial.println();
  delay(PERIOD_MEASURE);
}
