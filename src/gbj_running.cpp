#include "gbj_running.h"

/* Register data item into the buffer and return running value of the statistic.
 * The most recent (fresh) sample value is always in the 0 index of the buffer.
 * The most recent (fresh) running statistic is always in the separate
 * attribute.
 */
float gbj_running::getValue(float value)
{
  shiftRight(); // Make place for input value in the data buffer
  buffer_[0] = value;
  switch (type_)
  {
    case Statistics::AVERAGE:
      value_ = 0;
      for (byte i = 0; i < samples_; i++)
      {
        value_ += buffer_[i];
      }
      if (samples_)
      {
        value_ /= samples_;
      }
      break;

    case Statistics::MEDIAN:
      float *sorter;
      sorter = (float *)calloc(samples_, sizeof(float));
      for (byte i = 0; i < samples_; i++)
      {
        sorter[i] = buffer_[i];
      }
      gbj_apphelpers::sort_buble_asc(sorter, samples_);
      // Return middle item at odd items in the buffer
      if (samples_ % 2)
      {
        value_ = sorter[(samples_ - 1) / 2];
      }
      // Calculate from two middle items at event items in the buffer
      else
      {
        value_ =
          (sorter[samples_ / 2 - 1] + sorter[samples_ / 2]) / 2.0;
      }
      free(sorter);
      break;

    case Statistics::MINIMUM:
      value_ = value;
      for (byte i = 1; i < samples_; i++)
      {
        value_ = min(value_, buffer_[i]);
      }
      break;

    case Statistics::MAXIMUM:
      value_ = value;
      for (byte i = 1; i < samples_; i++)
      {
        value_ = max(value_, buffer_[i]);
      }
      break;
  }
  return value_;
}

// Shift array to the right so that 0 index is reserved for the new value
void gbj_running::shiftRight()
{
  for (byte i = samples_; i > 0; --i)
  {
    // Forget the oldest (most right) value in the buffer if it is full
    if (i < Params::SAMPLES)
      buffer_[i] = buffer_[i - 1];
  }
  // Count the freed 0 indexed value. Normally the buffer is full.
  samples_++;
  samples_ = constrain(samples_, 0, Params::SAMPLES);
}
