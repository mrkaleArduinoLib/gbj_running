<a id="library"></a>
# gbjFilterRunning
The library provides calculation of a running statistic value from recent sensor readings including the current one that are stored in the library instance object in 16-bit resolution, i.e., median, average, minimum, and maximum.


<a id="dependency"></a>
## Dependency

#### Particle platform
- **Particle.h**: Includes alternative (C++) data type definitions.

#### Arduino platform
- **Arduino.h**: Main include file for the Arduino SDK version greater or equal to 100.
- **WProgram.h**: Main include file for the Arduino SDK version less than 100.
- **inttypes.h**: Integer type conversions. This header file includes the exact-width integer definitions and extends them with additional facilities provided by the implementation.


<a id="constants"></a>
## Constants
- **GBJ\_FILTER\_RUNNING\_VERSION**: Name and semantic library version.
- **GBJ\_FILTER\_RUNNING\_MIN**: Minimal valid value for sensor data.
- **GBJ\_FILTER\_RUNNING\_MAX**: Maximal valid value for sensor data.
- **GBJ\_FILTER\_RUNNING\_BUFFER\_MIN**: Minimal number of values for running statistic.
- **GBJ\_FILTER\_RUNNING\_BUFFER\_MAX**: Maximal number of values for running statistic.
- **GBJ\_FILTER\_RUNNING\_BUFFER\_DEF**: Default number of values for running statistic.
- **GBJ\_FILTER\_RUNNING\_MEDIAN**: Median (50% percentile) as a running statistic.
- **GBJ\_FILTER\_RUNNING\_AVERAGE**: Average (arithmetic mean) as a running statistic.
- **GBJ\_FILTER\_RUNNING\_MINIMUM**: Minimum as a running statistic.
- **GBJ\_FILTER\_RUNNING\_MAXIMUM**: Maximum as a running statistic.


<a id="interface"></a>
## Interface
- [gbj_filter_running()](#gbj_filter_running)
- [init()](#init)
- [getStatistic()](#getStatistic)

#### Getters
- [getRunningType()](#getRunningType)
- [getValueMin()](#getValueRange)
- [getValueMax()](#getValueRange)
- [getBufferLen()](#getBufferLen)
- [getReadings()](#getReadings)
- [getLastStatistic()](#getLastStatistic)


<a id="gbj_filter_running"></a>
## gbj_filter_running()
#### Description
Constructor creates the data buffer within a class instance object, which holds a series of running values of particular statistical type.
- For each type of running statistic as well as each statistical variable (sensor) a separate instance object has to be created with separated data buffer.
- Running values of sensor readings or smoothed sensor readings reduce excesses and extremes in the physical value measurement.
- The class may be used for original digital data as well, where the running statistical processing is desirable.

#### Syntax
    gbj_filter_running(uint8_t runningType, uint16_t valueMax, uint16_t valueMin, uint8_t bufferLen);

#### Parameters
<a id="prm_runningType"></a>
- **runningType**: Type of the running statistic to be calculated.
  - *Valid values*: [GBJ\_FILTER\_RUNNING\_MEDIAN](#constants), [GBJ\_FILTER\_RUNNING\_AVERAGE](#constants), [GBJ\_FILTER\_RUNNING\_MINIMUM](#constants), [GBJ\_FILTER\_RUNNING\_MAXIMUM](#constants)
  - *Default value*: [GBJ\_FILTER\_RUNNING\_AVERAGE](#constants)

<a id="prm_valueMax"></a>
- **valueMax**: Maximal valid sensor value for registering.
  - *Valid values*: non-negative integer 0 to 65365 ([GBJ\_FILTER\_RUNNING\_MIN ~ GBJ\_FILTER\_RUNNING\_MAX](#constants))
  - *Default value*: 65365 ([GBJ\_FILTER\_RUNNING\_MAX](#constants))

<a id="prm_valueMin"></a>
- **valueMin**: Minimal valid sensor value for registering.
  - *Valid values*: non-negative integer 0 to 65365 ([GBJ\_FILTER\_RUNNING\_MIN ~ GBJ\_FILTER\_RUNNING\_MAX](#constants))
  - *Default value*: 0 ([GBJ\_FILTER\_RUNNING\_MIN](#constants))

<a id="prm_bufferLen"></a>
- **bufferLen**: Number of running statistics held in the data buffer used for calculating a new running statistic.
  - The constructor limits the input buffer length to a valid range.
  - For median the buffer length should be odd number. If it is not, the constructor adds 1 to it right before limiting the length in order to make it odd.
  - *Valid values*: positive integer in the range 3 to 11 ([GBJ\_FILTER\_RUNNING\_BUFFER\_MIN ~ GBJ\_FILTER\_RUNNING\_BUFFER\_MAX](#constants))
  - *Default value*: 5 ([GBJ\_FILTER\_RUNNING\_BUFFER\_DEF](#constants))

#### Returns
Object preforming a running statistic calculation.

#### Example
The constructor has all arguments defaulted. The constructor instance without any parameters is equivalent to an instance with all arguments set by corresponding constant with default value:

``` cpp
gbj_filter_running RunningAvg = gbj_filter_running(); // It is equivalent to
gbj_filter_running RunningAvg = gbj_filter_running(GBJ_FILTER_RUNNING_AVERAGE, GBJ_FILTER_RUNNING_MAX, GBJ_FILTER_RUNNING_MIN, GBJ_FILTER_RUNNING_BUFFER_DEF);
```

If some argument after some defaulted arguments should have a specific value, use corresponding constants in place of those defaulted arguments, e.g.

``` cpp
gbj_filter_running RunningAvg = gbj_filter_running(GBJ_FILTER_RUNNING_AVERAGE, GBJ_FILTER_RUNNING_MAX, GBJ_FILTER_RUNNING_MIN, 11); // Specific buffer length
```

Typical usage is just with setting a desired statistical type in the constructor:

``` cpp
gbj_filter_running RunningAvg = gbj_filter_running(); // Only default arguments - running average
gbj_filter_running RunningMed = gbj_filter_running(GBJ_FILTER_RUNNING_MEDIAN, 1023);      // Running median with maximal value filtering
gbj_filter_running RunningMax = gbj_filter_running(GBJ_FILTER_RUNNING_MAXIMUM, 768, 16);  // Running maximum with valid value range filtering
```

[Back to interface](#interface)


<a id="init"></a>
## init()
#### Description
The method initiates all internal counters and status flags of a class instance object to default values as they are right after power up of a microcontroller.

#### Syntax
    void init();

#### Parameters
None

#### Returns
None

[Back to interface](#interface)


<a id="getStatistic"></a>
## getStatistic()
#### Description
The method calculates and returns a new running statistic of a type defined in constructor parameter [runningType](#prm_runningType) from the current input value and previous values and stores it in the data buffer for the future calculation.

#### Syntax
    uint16_t getStatistic(uint16_t currentValue);

#### Parameters
<a id="prm_currentValue"></a>
- **currentValue**: 16-bit value to be used for calculating a new running statistic.
  - *Valid values*: non-negative integer 0 ~ 65535, but restricted by current filter range
  - *Default value*: none

#### Returns
Running statistic or recently stored running statistic, if the input value is outside of valid range.

#### See also
[gbj_filter_running()](#gbj_filter_running)

[getLastStatistic()](#getLastStatistic)

[Back to interface](#interface)


<a id="getRunningType"></a>
## getRunningType()
#### Description
The method returns internal number code (defined by corresponding library macro constant) of running statistic type for the current instance object.

#### Syntax
    uint8_t getRunningType();

#### Parameters
None

#### Returns
Code of actual running statistic type defined by one constant from the list [GBJ\_FILTER\_RUNNING\_MEDIAN, GBJ\_FILTER\_RUNNING\_AVERAGE, GBJ\_FILTER\_RUNNING\_MINIMUM, GBJ\_FILTER\_RUNNING\_MAXIMUM](#constants).

#### See also
[gbj_filter_running()](#gbj_filter_running)

[Back to interface](#interface)


<a id="getValueRange"></a>
## getValueMax(), getValueMin()
#### Description
The particular method returns set minimal or maximal value valid for calculating.

#### Syntax
    uint16_t getValueMax();
    uint16_t getValueMin();

#### Parameters
None

#### Returns
Actual minimal or maximal value of the valid data range.

#### See also
[gbj_filter_running()](#gbj_filter_running)

[Back to interface](#interface)


<a id="getBufferLen"></a>
## getBufferLen()
#### Description
The method returns current length of the data buffer used for calculation.
- Usually the returned value is the same as length put to the constructor parameter [bufferLen](#prm_bufferLen).
- If class has adjusted or limited the input buffer length, the method returns the actual length.
- The method is useful, if the length has been put to the constructor as a numeric literal and there is no variable of the length to use it in other statements.

#### Syntax
    uint8_t getBufferLen();

#### Parameters
None

#### Returns
Actual length of the data buffer.

#### See also
[gbj_filter_running()](#gbj_filter_running)

[getReadings()](#getReadings)

[Back to interface](#interface)


<a id="getReadings"></a>
## getReadings()
#### Description
The method returns number of running values of a statistic in the data buffer, which are going to be used for calculating a new one.
- The calculation of a new running value can be provided before the data buffer is full. In that case the method returns the number less than the length of the data buffer set in the constructor parameter [bufferLen](#prm_bufferLen).
- After a while after initialization the returned value is same as the length of the data buffer.

#### Syntax
    uint8_t getReadings();

#### Parameters
None

#### Returns
Actual number of running values of a statistic in the data buffer.

#### See also
[getBufferLen()](#getBufferLen)

[Back to interface](#interface)


<a id="getLastStatistic"></a>
## getLastStatistic()
#### Description
The method returns the recently calculated running statistics without need of new measured value (sample) or recalculation.
- The same effect can be achieved by inputting the value outside the initial filter in the method [getStatistic()](#getStatistic), if the valid range is narrower than the value space of the [input data](#prm_currentValue).
- The method is useful if there is no variable used for the running statistic in an application and the recent statistic is needed.

#### Syntax
    uint16_t getLastStatistic();

#### Parameters
None

#### Returns
Recently calculated running statistic.

#### See also
[getStatistic()](#getStatistic)

[Back to interface](#interface)
