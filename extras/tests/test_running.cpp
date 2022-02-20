/*
  NAME:
  Unit tests of library "gbj_running" for smoothing

  DESCRIPTION:
  The test suite provides test cases for calculating filtered value for
  consecutive values.
  - The test runner is Unity Project - ThrowTheSwitch.org.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#include "gbj_apphelpers.h"
#include "gbj_running.h"
#include <Arduino.h>
#include <unity.h>

//******************************************************************************
// Preparation
//******************************************************************************
const float SAMPLE_LIST[] = { 423, 753, 217, 42, 898, 712, 728, 510, 835, 77 };
const byte SAMPLES = sizeof(SAMPLE_LIST) / sizeof(SAMPLE_LIST[0]);
const byte BUFFER_LEN = gbj_running::SAMPLES;

//******************************************************************************
// Tests
//******************************************************************************
void test_average(void)
{
  float valActual, valExpected;
  gbj_running smooth = gbj_running();
  for (byte i = 0; i < SAMPLES; i++)
  {
    // Algorithm
    int jStart = max(0, i + 1 - BUFFER_LEN);
    valExpected = 0;
    for (byte j = jStart; j <= i; j++)
    {
      valExpected += SAMPLE_LIST[j];
    }
    valExpected /= i + 1 - jStart;
    // Testee
    valActual = smooth.getValue(SAMPLE_LIST[i]);
    String msg = "i=" + String(i);
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(valExpected, valActual, msg.c_str());
  }
}

void test_median(void)
{
  float valActual, valExpected;
  float *sorter;
  gbj_running smooth = gbj_running();
  smooth.setMedian();
  sorter = (float *)calloc(BUFFER_LEN, sizeof(float));
  for (byte i = 0; i < SAMPLES; i++)
  {
    // Algorithm
    int jStart = max(0, i + 1 - BUFFER_LEN);
    byte samples = 0;
    for (byte j = jStart, k = 0; j <= i; j++, k++)
    {
      sorter[k] = SAMPLE_LIST[j];
      samples++;
    }
    gbj_apphelpers::sort_buble_asc(sorter, samples);
    if (samples % 2)
    {
      valExpected = sorter[(samples - 1) / 2];
    }
    else
    {
      valExpected = (sorter[samples / 2 - 1] + sorter[samples / 2]) / 2.0;
    }
    // Testee
    valActual = smooth.getValue(SAMPLE_LIST[i]);
    String msg = "i=" + String(i);
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(valExpected, valActual, msg.c_str());
  }
  free(sorter);
}

void test_minimum(void)
{
  float valActual, valExpected;
  gbj_running smooth = gbj_running();
  smooth.setMinimum();
  for (byte i = 0; i < SAMPLES; i++)
  {
    // Algorithm
    int jStart = max(0, i + 1 - BUFFER_LEN);
    valExpected = SAMPLE_LIST[jStart];
    for (byte j = jStart + 1; j <= i; j++)
    {
      valExpected = min(valExpected, SAMPLE_LIST[j]);
    }
    // Testee
    valActual = smooth.getValue(SAMPLE_LIST[i]);
    String msg = "i=" + String(i);
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(valExpected, valActual, msg.c_str());
  }
}

void test_maximum(void)
{
  float valActual, valExpected;
  gbj_running smooth = gbj_running();
  smooth.setMaximum();
  for (byte i = 0; i < SAMPLES; i++)
  {
    // Algorithm
    int jStart = max(0, i + 1 - BUFFER_LEN);
    valExpected = SAMPLE_LIST[jStart];
    for (byte j = jStart + 1; j <= i; j++)
    {
      valExpected = max(valExpected, SAMPLE_LIST[j]);
    }
    // Testee
    valActual = smooth.getValue(SAMPLE_LIST[i]);
    String msg = "i=" + String(i);
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(valExpected, valActual, msg.c_str());
  }
}

//******************************************************************************
// Run
//******************************************************************************
void setup()
{
  delay(2000);
  UNITY_BEGIN();

  RUN_TEST(test_average);
  RUN_TEST(test_median);
  RUN_TEST(test_minimum);
  RUN_TEST(test_maximum);

  UNITY_END();
}

void loop() {}