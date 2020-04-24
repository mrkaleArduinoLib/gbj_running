/*
  NAME:
  Basic usage of gbj_filter_running library.

  DESCRIPTION:
  This sketch demonstrates the use of running statistics without determining
  optional arguments, so that it uses the default values of them.
  - As a measured value the sketch utilizes the random integers within
    the range 0 to 1023.
  - The sketch calculates just the running average from default number of
    previously calculated values.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#include "gbj_filter_running.h"
#define SKETCH "GBJ_FILTER_RUNNING_AVERAGE 1.1.0"

const unsigned int PERIOD_MEASURE = 3000; // Time in miliseconds between measurements

// Upper limit of random values for mimicking real physical measurement
const unsigned int SENSOR_DATA_MAX = 1023;

// Variables and constants for measurement
unsigned int demoData, filterData;
gbj_filter_running avgRunning = gbj_filter_running();

void setup()
{
  Serial.begin(9600);
  Serial.println(SKETCH);
  Serial.println("Libraries:");
  Serial.println(gbj_filter_running::VERSION);
  Serial.println(gbj_apphelpers::VERSION);
  Serial.println("---");
  // Print header
  Serial.print("Buffer length: ");
  Serial.println(avgRunning.getBufferLen());
  Serial.println("Data\tAverage\tItems");
}

void loop()
{
  demoData = random(SENSOR_DATA_MAX + 1);
  filterData = avgRunning.getStatistic(demoData);
  Serial.print(demoData);
  Serial.print("\t");
  Serial.print(filterData);
  Serial.print("\t");
  Serial.print(avgRunning.getReadings());
  Serial.println();
  delay(PERIOD_MEASURE);
}
