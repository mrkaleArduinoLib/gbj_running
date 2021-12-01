/*
  NAME:
  Unit tests of library "gbj_running" for smoothing.

  DESCRIPTION:
  The test suite provides test cases for calculating filtered value for two
  consecutive values.
  - The test runner is Unity Project - ThrowTheSwitch.org.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#include <Arduino.h>
#include <unity.h>
#include "gbj_running.h"

// Valid range of values ensured by filtering
const unsigned int SENSOR_FILTER_MIN = 128;
const unsigned int SENSOR_FILTER_MAX = 768;
const unsigned int SMOOTH_SAMPLES = 5;

uint16_t sample[] = {423, 753, 217, 42, 898, 712, 728, 510, 835, 77};
// Full valid samples at sample = 728, index = 6
uint16_t statistic;

void test_running_average(void)
{
    gbj_running Filter = gbj_running(gbj_running::AVERAGE,
        SENSOR_FILTER_MAX, SENSOR_FILTER_MIN, SMOOTH_SAMPLES);
    // Running medians {423, 588, 464, 464, 464, 526, 567}
    byte i = 0;
    while (Filter.getReadings() < SMOOTH_SAMPLES)
       statistic = Filter.getStatistic(sample[i++]);
    TEST_ASSERT_EQUAL_INT16(567, Filter.getStatistic());
}

void test_running_median(void)
{
    gbj_running Filter = gbj_running(gbj_running::MEDIAN,
        SENSOR_FILTER_MAX, SENSOR_FILTER_MIN, SMOOTH_SAMPLES);
    // Running medians {423, 423, 423, 423, 423, 423, 712}
    byte i = 0;
    while (Filter.getReadings() < SMOOTH_SAMPLES)
       statistic = Filter.getStatistic(sample[i++]);
    TEST_ASSERT_EQUAL_INT16(712, Filter.getStatistic());
}

void test_running_minimum(void)
{
    gbj_running Filter = gbj_running(gbj_running::MINIMUM,
        SENSOR_FILTER_MAX, SENSOR_FILTER_MIN, SMOOTH_SAMPLES);
    // Running medians {423, 423, 217, 217, 217, 217, 217}
    byte i = 0;
    while (Filter.getReadings() < SMOOTH_SAMPLES)
       statistic = Filter.getStatistic(sample[i++]);
    TEST_ASSERT_EQUAL_INT16(217, Filter.getStatistic());
}

void test_running_maximum(void)
{
    gbj_running Filter = gbj_running(gbj_running::MAXIMUM,
        SENSOR_FILTER_MAX, SENSOR_FILTER_MIN, SMOOTH_SAMPLES);
    // Running medians {423, 753, 753, 753, 753, 753, 753}
    byte i = 0;
    while (Filter.getReadings() < SMOOTH_SAMPLES)
       statistic = Filter.getStatistic(sample[i++]);
    TEST_ASSERT_EQUAL_INT16(753, Filter.getStatistic());
}


void setup() {
    UNITY_BEGIN();

    RUN_TEST(test_running_average);
    RUN_TEST(test_running_median);
    RUN_TEST(test_running_minimum);
    RUN_TEST(test_running_maximum);

    UNITY_END();
}

void loop(){}