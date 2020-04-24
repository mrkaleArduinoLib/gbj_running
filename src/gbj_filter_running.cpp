#include "gbj_filter_running.h"
const String gbj_filter_running::VERSION = "GBJ_FILTER_RUNNING 1.1.0";

gbj_filter_running::gbj_filter_running(
  uint8_t statisticType,
  uint16_t valueMax,
  uint16_t valueMin,
  uint8_t bufferLen)
{
  setFilter(valueMax, valueMin);
  setBufferLen(bufferLen);
  init();
  _statisticType = statisticType;
  switch (getStatisticType())
  {
    case Statistics::AVERAGE:
    case Statistics::MINIMUM:
    case Statistics::MAXIMUM:
    case Statistics::MEDIAN:
      break;

    default:
      _statisticType = Statistics::AVERAGE;
      break;
  }
}


/* Register data item into the buffer and return running value of the statistic.
 * The most recent (fresh) sample value is always in the 0 index of the buffer.
 * The most recent (fresh) running statistic is always in the separate attribute.
 */
uint16_t gbj_filter_running::getStatistic(uint16_t currentValue)
{
  // Sanitize input
  if (currentValue < getValueMin() || currentValue > getValueMax())
    return getStatistic();
  // Shift buffer for current value and increase _bufferCnt
  shiftRight();
  _buffer[0] = currentValue;
  switch (getStatisticType())
  {
    case Statistics::MEDIAN:
      for (uint8_t i = 0; i < _bufferCnt; i++)
        _sorter[i] = _buffer[i];
      gbj_apphelpers::sort_buble_asc(_sorter, getReadings());
      // Round down median index
      _statisticRecent = _sorter[(getReadings() - 1) / 2];
      break;

    case Statistics::AVERAGE:
      _statisticRecent = 0;
      for (uint8_t i = 0; i < getReadings(); i++)
        _statisticRecent += _buffer[i];
      // Round up arithmetic mean
      _statisticRecent = divide(_statisticRecent, getReadings());
      break;

    case Statistics::MINIMUM:
      _statisticRecent = currentValue;
      for (uint8_t i = 1; i < getReadings(); i++)
        _statisticRecent = min(_statisticRecent, _buffer[i]);
      break;

    case Statistics::MAXIMUM:
      _statisticRecent = currentValue;
      for (uint8_t i = 1; i < getReadings(); i++)
        _statisticRecent = max(_statisticRecent, _buffer[i]);
      break;
  }
  return getStatistic();
}


// Shift array to the right so that 0 index is reserved for the new value
void gbj_filter_running::shiftRight()
{
  for (byte i = _bufferCnt; i > 0 ; --i)
  {
    // Forget the oldest (most right) value in the buffer if it is full
    if (i < _bufferLen)
      _buffer[i] = _buffer[i-1];
  }
  // Count the freed 0 indexed value. Normally the buffer is full.
  _bufferCnt++;
  _bufferCnt = constrain(_bufferCnt, 0, _bufferLen);
}
