/*
  NAME:
  gbj_filter_running

  DESCRIPTION:
  The library provides calculation of a running statistic value from
  recent sensor readings including the current one that are stored in the
  library instance object in 16-bit resolution.
  - The library calculates running median, average, minimum, and maximum
    as a statistic.
  - One library instance object can calculate running values just for one
    statistic, e.g., median or average, etc.
  - A running value substitutes the current sensor value.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the license GNU GPL v3 http://www.gnu.org/licenses/gpl-3.0.html
  (related to original code) and MIT License (MIT) for added code.

  CREDENTIALS:
  Author: Libor Gabaj
  GitHub: https://github.com/mrkaleArduinoLib/gbj_filter_running.git
*/
#ifndef GBJ_FILTER_RUNNING_H
#define GBJ_FILTER_RUNNING_H

#if defined(__AVR__)
  #if ARDUINO >= 100
    #include "Arduino.h"
  #else
    #include "WProgram.h"
  #endif
  #include <inttypes.h>
#elif defined(PARTICLE)
    #include "Particle.h"
#endif
#include <gbj_apphelpers.h>


// Macro functions
#define divide(a, b) { (uint16_t)round((float)(a)/(float)(b)) }


class gbj_filter_running
{
  public:
    static const String VERSION;

    enum Statistics
    {
      MEDIAN = 1,
      AVERAGE = 2,
      MINIMUM = 3,
      MAXIMUM = 4,
    };

    /*
      Constructor.

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

      valueMax - Maximal valid sensor value for registering.
                 - Data type: non-negative integer
                 - Default value: 65535
                 - Limited range: 0 ~ 65535

      valueMin - Minimal valid sensor value for registering.
                 - Data type: non-negative integer
                 - Default value: 0
                 - Limited range: 0 ~ 65535

      bufferLen - Number of running statistics held in the data buffer used for
                  calculating a new running statistic.
                  - It should be odd number. If it is not, the constructor adds
                    1 to it right before limiting the length in order to make it
                    odd.
                  - The constructor limits the input buffer length to a valid range.
                  - Data type: positive integer
                  - Default value: 5
                  - Limited range: 3 ~ 11

      RETURN:
      Library instance object.
    */
    gbj_filter_running(
        uint8_t statisticType = Statistics::AVERAGE,
        uint16_t valueMax = Limits::FILTER_MAX,
        uint16_t valueMin = Limits::FILTER_MIN,
        uint8_t bufferLen = Limits::BUFFERLEN_DEF);

    /*
      Reset all counters and status flags.

      DESCRIPTION:
      The method initiates all internal counters and status flags of a class
      instance object to default values as they are right after power up of
      a microcontroler.

      PARAMETERS: none

      RETURN: none
    */
    inline void init() { _bufferCnt = 0; };

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
      currentValue - 16-bit value to be used for calculating a new running statistic.
                    - Data type: non-negative integer
                    - Default value: none
                    - Limited range: 0 ~ 65535

      RETURN:
      New running statistic or the recently stored one if the value is not valid.
    */
    uint16_t getStatistic(uint16_t currentValue);
    inline uint16_t getStatistic(void) { return _statisticRecent; };

    // Public getters
    inline uint8_t getStatisticType() { return _statisticType; };

    inline uint8_t getReadings() { return _bufferCnt; };
    inline uint16_t getValueMin() { return _valueMin; };
    inline uint16_t getValueMax() { return _valueMax; };
    //
    static inline uint16_t getFilterMin() { return Limits::FILTER_MIN; };
    static inline uint16_t getFilterMax() { return Limits::FILTER_MAX; };
    //
    inline uint8_t getBufferLen() { return _bufferLen; };
    static inline uint8_t getBufferLenMin() { return Limits::BUFFERLEN_MIN; };
    static inline uint8_t getBufferLenMax() { return Limits::BUFFERLEN_MAX; };
    static inline uint8_t getBufferLenDef() { return Limits::BUFFERLEN_DEF; };

    // Public setters
    inline void setFilterMax(uint16_t valueMax)
    {
      _valueMax = constrain(valueMax, getFilterMin(), getFilterMax());
    };

    inline void setFilterMin(uint16_t valueMin)
    {
      _valueMin = constrain(valueMin, Limits::FILTER_MIN, Limits::FILTER_MAX);
    };

    inline void setFilter(uint16_t valueMax, uint16_t valueMin)
    {
      setFilterMax(valueMax);
      setFilterMin(valueMin);
    };

    inline void setBufferLen(uint8_t bufferLen)
    {
      // Adjust buffer length to odd number
      _bufferLen = bufferLen | 1;
      _bufferLen = constrain(_bufferLen, getBufferLenMin(), getBufferLenMax());
    };


  private:
    enum Limits
    {
      FILTER_MIN = 0x0000, // Minimal valid sensor data
      FILTER_MAX = 0xFFFF, // Maximal valid sensor data
      BUFFERLEN_MIN = 3,   // Minimal valid buffer length
      BUFFERLEN_MAX = 11,  // Maximal valid buffer length
      BUFFERLEN_DEF = 5,   // Default buffer length
    };

    uint16_t _buffer[Limits::BUFFERLEN_MAX]; // Data buffer
    uint16_t _sorter[Limits::BUFFERLEN_MAX]; // Sorting buffer
    uint16_t _valueMin;
    uint16_t _valueMax;
    uint16_t _statisticRecent;
    uint8_t _statisticType;
    uint8_t _bufferLen;
    uint8_t _bufferCnt;

    void shiftRight();
};

#endif
