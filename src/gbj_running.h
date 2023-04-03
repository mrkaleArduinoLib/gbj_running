/*
  NAME:
  gbj_running

  DESCRIPTION:
  The library provides calculation of a running statistic value for a float
  measure for the provided value and previous ones stored in the library
  instance object.
  - The library utilizes maximal 5 values recently provided for calculation
    of a running value.
  - The library calculates running median, average, minimum, and maximum
    as a statistic.
  - One library instance object can calculate running values just for one
    statistic type, e.g., median or average, etc.
  - A running value substitutes the current measure's value.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the license GNU GPL v3
  http://www.gnu.org/licenses/gpl-3.0.html (related to original code) and MIT
  License (MIT) for added code.

  CREDENTIALS:
  Author: Libor Gabaj
  GitHub: https://github.com/mrkaleArduinoLib/gbj_running.git
*/
#ifndef GBJ_RUNNING_H
#define GBJ_RUNNING_H

#if defined(__AVR__)
  #include <Arduino.h>
  #include <inttypes.h>
#elif defined(ESP8266)
  #include <Arduino.h>
#elif defined(ESP32)
  #include <Arduino.h>
#elif defined(PARTICLE)
  #include "Particle.h"
  #include "math.h"
#endif
#include "gbj_apphelpers.h"

class gbj_running
{
public:
  enum Statistics : byte
  {
    AVERAGE,
    MEDIAN,
    MINIMUM,
    MAXIMUM,
  };

  enum Params : byte
  {
    SAMPLES = 5,
  };

  /*
    Constructor

    DESCRIPTION:
    Constructor creates the data buffer within a class instance object, which
    holds a series of running values of particular statistical type.
    - For each type of running statistic as well as each statistical variable
      (sensor) a separate instance object has to be created with separated
      data buffer.
    - Running values of sensor readings or smoothed sensor readings reduce
      excesses and extremes in the physical value measurement.
    - The class may be used for original digital data as well, where
      the running statistical processing is desirable.
    - The constructor has all arguments defaulted. If some argument after
      some defaulted arguments should have a specific value, use corresponding
      constants in place of those defaulted arguments.

    PARAMETERS:
    statisticType - Type of the running statistic to be calculated.
                    - It has to be one of statistic types defined by constants
                      in the enumeration Statistics.
                    - Data type: positive integer
                    - Default value: AVERAGE
                    - Limited range: AVERAGE, MEDIAN, MAXIMUM, MINIMUM

    RETURN:
    Library instance object.
  */
  inline gbj_running(Statistics type = Statistics::AVERAGE)
  {
    init();
    type_ = type;
    switch (type_)
    {
      case Statistics::AVERAGE:
      case Statistics::MEDIAN:
      case Statistics::MINIMUM:
      case Statistics::MAXIMUM:
        break;

      default:
        type_ = Statistics::AVERAGE;
        break;
    }
  }

  /*
    Reset all counters and status flags.

    DESCRIPTION:
    The method initiates all internal counters and status flags of a class
    instance object to default values as they are right after power up of
    a microcontroler.

    PARAMETERS: none

    RETURN: none
  */
  inline void init()
  {
    samples_ = 0;
    value_ = 0;
  };

  /*
    Calculate new running statistic for current sensor value or get last one.

    DESCRIPTION:
    The method calculates and returns a new running statistic from current
    input value and previous values and stores it in the data buffer for the
    future calculation.
    - The method is overloaded.
    - If the input argument is provided, the method calculates new running
      statistic.
    - If not input argument is provided, the method returns recent calculated
      statistic value.
    - The method accepts only values within the valid range defined in the
      constructor.

    PARAMETERS:
    currentValue - Value to be used for calculating a new running statistic.
      - Data type: float
      - Default value: none
      - Limited range: real numbers

    RETURN:
    New running statistic or the recently calculated one.
  */
  inline float getValue(void) { return value_; };
  float getValue(float value);
  inline byte getSamples() { return samples_; };
  inline Statistics getType() { return type_; };

  // Setters
  inline void setAverage(void) { type_ = Statistics::AVERAGE; }
  inline void setMedian(void) { type_ = Statistics::MEDIAN; }
  inline void setMinimum(void) { type_ = Statistics::MINIMUM; }
  inline void setMaximum(void) { type_ = Statistics::MAXIMUM; }

private:
  Statistics type_;
  byte samples_;
  float buffer_[Params::SAMPLES];
  float value_;

  void shiftRight();
};

#endif
