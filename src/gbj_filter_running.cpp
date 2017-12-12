#include "gbj_filter_running.h"


gbj_filter_running::gbj_filter_running(uint8_t runningType, uint16_t valueMax, uint16_t valueMin, uint8_t bufferLen)
{
  _runningType = runningType;
  _bufferLen = bufferLen;
  switch(_runningType)
  {
    case GBJ_FILTER_RUNNING_AVERAGE:
    case GBJ_FILTER_RUNNING_MINIMUM:
    case GBJ_FILTER_RUNNING_MAXIMUM:
      break;

    case GBJ_FILTER_RUNNING_MEDIAN:
      _bufferLen = _bufferLen | 1;
      break;

    default:
      _runningType = GBJ_FILTER_RUNNING_AVERAGE;
      break;
  }
  // Sanitize arguments
  _valueMax  = constrain(valueMax, GBJ_FILTER_RUNNING_MIN, GBJ_FILTER_RUNNING_MAX);
  _valueMin  = constrain(valueMin, GBJ_FILTER_RUNNING_MIN, GBJ_FILTER_RUNNING_MAX);
  swapdata(_valueMin, _valueMax); // Sort valid range values
  _bufferLen = constrain(_bufferLen, GBJ_FILTER_RUNNING_BUFFER_MIN, GBJ_FILTER_RUNNING_BUFFER_MAX);
  init();
}


// Initialize all status variables
void gbj_filter_running::init()
{
  _bufferCnt = 0;
}


/* Register data item into the buffer and return running value of the statistic.
 * The most recent (fresh) statistic is always in the 0 index of the buffer.
 */
uint16_t gbj_filter_running::getStatistic(uint16_t currentValue)
{
  uint16_t statistic = _buffer[0];
  if (currentValue < _valueMin) return statistic;
  if (currentValue > _valueMax) return statistic;
  shiftRight(); // Shift buffer for current value and increase _bufferCnt
  _buffer[0] = currentValue;
  switch(_runningType)
  {
    case GBJ_FILTER_RUNNING_MEDIAN:
      for (uint8_t i = 0; i < _bufferCnt; i++) _sorter[i] = _buffer[i];
      sort();
      // Round down median index
      statistic = _sorter[(_bufferCnt - 1)/2];
      break;

    case GBJ_FILTER_RUNNING_AVERAGE:
      statistic = 0;
      for (uint8_t i = 0; i < _bufferCnt; i++) statistic += _buffer[i];
      // Round up arithmetic mean
      statistic = divide(statistic, _bufferCnt);
      break;

    case GBJ_FILTER_RUNNING_MINIMUM:
      statistic = _buffer[0];
      for (uint8_t i = 1; i < _bufferCnt; i++) statistic = min(statistic, _buffer[i]);
      break;

    case GBJ_FILTER_RUNNING_MAXIMUM:
      statistic = _buffer[0];
      for (uint8_t i = 1; i < _bufferCnt; i++) statistic = max(statistic, _buffer[i]);
      break;
  }
  _buffer[0] = statistic;
  return statistic;
}


//-------------------------------------------------------------------------
// Getters
//-------------------------------------------------------------------------
uint8_t  gbj_filter_running::getBufferLen()   { return _bufferLen; };
uint8_t  gbj_filter_running::getReadings()    { return _bufferCnt; };
uint8_t  gbj_filter_running::getRunningType() { return _runningType; };
uint16_t gbj_filter_running::getValueMin()    { return _valueMin; };
uint16_t gbj_filter_running::getValueMax()    { return _valueMax; };


//-------------------------------------------------------------------------
// Private methods
//-------------------------------------------------------------------------


// Sort array by bubblesort algorithm in ascending order
void gbj_filter_running::sort()
{
  bool again = true;
  for(byte i = 0; i < (_bufferCnt - 1) && again; i++)
  {
    again = false;
    for(byte j = _bufferCnt - 1; j > i; --j)
    {
      if(_sorter[j] < _sorter[j-1])
      {
        uint16_t t = _sorter[j];
        _sorter[j] = _sorter[j-1];
        _sorter[j-1] = t;
        again = true;
      }
    }
  }
}


// Shift array to the right so that 0 index is reserved for the new value
void gbj_filter_running::shiftRight()
{
  for(byte i = _bufferCnt; i > 0 ; --i)
  {
    // Forget the oldest (most right) value in the buffer if it is full
    if (i < _bufferLen) _buffer[i] = _buffer[i-1];
  }
  // Count the freed 0 indexed value. Normally the buffer is full.
  _bufferCnt++;
  _bufferCnt = constrain(_bufferCnt, 0, _bufferLen);
}
