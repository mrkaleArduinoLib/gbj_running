#include "gbj_running.h"
const String gbj_running::VERSION = "GBJ_RUNNING 1.2.0";

gbj_running::gbj_running(
  uint8_t statisticType,
  uint16_t valueMax,
  uint16_t valueMin,
  uint8_t bufferLen)
{
  setFilter(valueMax, valueMin);
  setBufferLen(bufferLen);
  init();
  statisticType_ = statisticType;
  switch (getStatisticType())
  {
    case Statistics::AVERAGE:
    case Statistics::MEDIAN:
    case Statistics::MINIMUM:
    case Statistics::MAXIMUM:
      break;

    default:
      statisticType_ = Statistics::AVERAGE;
      break;
  }
}


/* Register data item into the buffer and return running value of the statistic.
 * The most recent (fresh) sample value is always in the 0 index of the buffer.
 * The most recent (fresh) running statistic is always in the separate attribute.
 */
uint16_t gbj_running::getStatistic(uint16_t currentValue)
{
  if (currentValue < getValueMin() || currentValue > getValueMax())
    return getStatistic();
  // Make place for current value in the data buffer
  shiftRight();
  buffer_[0] = currentValue;
  switch (getStatisticType())
  {
    case Statistics::AVERAGE:
      statisticRecent_ = 0;
      for (uint8_t i = 0; i < getReadings(); i++)
        statisticRecent_ += buffer_[i];
      // Round up arithmetic mean
      statisticRecent_ = divide(statisticRecent_, getReadings());
      break;

    case Statistics::MEDIAN:
      uint16_t *sorter;
      sorter = (uint16_t *)calloc(getReadings(), sizeof(uint16_t));
      for (uint8_t i = 0; i < getReadings(); i++)
        sorter[i] = buffer_[i];
      gbj_apphelpers::sort_buble_asc(sorter, getReadings());
      // Round down median index
      statisticRecent_ = sorter[(getReadings() - 1) / 2];
      free(sorter);
      break;

    case Statistics::MINIMUM:
      statisticRecent_ = currentValue;
      for (uint8_t i = 1; i < getReadings(); i++)
        statisticRecent_ = min(statisticRecent_, buffer_[i]);
      break;

    case Statistics::MAXIMUM:
      statisticRecent_ = currentValue;
      for (uint8_t i = 1; i < getReadings(); i++)
        statisticRecent_ = max(statisticRecent_, buffer_[i]);
      break;
  }
  return getStatistic();
}


// Shift array to the right so that 0 index is reserved for the new value
void gbj_running::shiftRight()
{
  for (byte i = getReadings(); i > 0; --i)
  {
    // Forget the oldest (most right) value in the buffer if it is full
    if (i < getBufferLen())
      buffer_[i] = buffer_[i-1];
  }
  // Count the freed 0 indexed value. Normally the buffer is full.
  bufferCnt_++;
  bufferCnt_ = constrain(getReadings(), 0, getBufferLen());
}
