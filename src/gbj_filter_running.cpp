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
    case Statistics::MEDIAN:
    case Statistics::MINIMUM:
    case Statistics::MAXIMUM:
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
  if (currentValue < getValueMin() || currentValue > getValueMax())
    return getStatistic();
  // Make place for current value in the data buffer
  shiftRight();
  _buffer[0] = currentValue;
  switch (getStatisticType())
  {
    case Statistics::AVERAGE:
      _statisticRecent = 0;
      for (uint8_t i = 0; i < getReadings(); i++)
        _statisticRecent += _buffer[i];
      // Round up arithmetic mean
      _statisticRecent = divide(_statisticRecent, getReadings());
      break;

    case Statistics::MEDIAN:
      uint16_t *sorter;
      sorter = (uint16_t *)calloc(getReadings(), sizeof(uint16_t));
      for (uint8_t i = 0; i < getReadings(); i++)
        sorter[i] = _buffer[i];
      gbj_apphelpers::sort_buble_asc(sorter, getReadings());
      // Round down median index
      _statisticRecent = sorter[(getReadings() - 1) / 2];
      free(sorter);
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
  for (byte i = getReadings(); i > 0; --i)
  {
    // Forget the oldest (most right) value in the buffer if it is full
    if (i < getBufferLen())
      _buffer[i] = _buffer[i-1];
  }
  // Count the freed 0 indexed value. Normally the buffer is full.
  _bufferCnt++;
  _bufferCnt = constrain(getReadings(), 0, getBufferLen());
}
