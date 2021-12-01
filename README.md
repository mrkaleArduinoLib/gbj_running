<a id="library"></a>

# gbjFilterRunning
The library provides calculation of a running statistic value from recent sensor readings including the current one that are stored in the library instance object in 16-bit resolution, i.e., average, median, minimum, and maximum.


<a id="dependency"></a>

## Dependency

#### Particle platform
* **Particle.h**: Includes alternative (C++) data type definitions.

#### Arduino platform
* **Arduino.h**: Main include file for the Arduino SDK version greater or equal to 100.
* **WProgram.h**: Main include file for the Arduino SDK version less than 100.
* **inttypes.h**: Integer type conversions. This header file includes the exact-width integer definitions and extends them with additional facilities provided by the implementation.

#### Custom Libraries
* **gbjAppHelpers**: Custom library loaded from the file *gbj\_apphelpers.h* for a generic application logic.


<a id="constants"></a>

## Constants
All constants are embedded into the class as static ones.

* **gbj\_running::VERSION**: Name and semantic version of the library.
* **gbj\_running::AVERAGE**: Average (arithmetic mean) as a running statistic.
* **gbj\_running::MEDIAN**: Median (50% percentile) as a running statistic.
* **gbj\_running::MAXIMUM**: Maximum as a running statistic.
* **gbj\_running::MINIMUM**: Minimum as a running statistic.


<a id="interface"></a>

## Interface
* [gbj_running()](#gbj_running)
* [init()](#init)
* [getStatistic()](#getStatistic)

#### Setters
* [setFilter()](#setFilter)
* [setFilterMin()](#setFilter)
* [setFilterMax()](#setFilter)
* [setBufferLen()](#setBufferLen)

#### Getters
* [getStatistic()](#getStatistic)
* [getStatisticType()](#getStatisticType)
* [getValueMin()](#getValueRange)
* [getValueMax()](#getValueRange)
* [getFilterMin()](#getFilterRange)
* [getFilterMax()](#getFilterRange)
* [getBufferLen()](#getBufferLen)
* [getBufferLenDef()](#getBufferLenStatic)
* [getBufferLenMin()](#getBufferLenStatic)
* [getBufferLenMax()](#getBufferLenStatic)
* [getReadings()](#getReadings)


<a id="gbj_running"></a>

## gbj_running()

#### Description
Constructor creates the data buffer within a class instance object, which holds a series of running values of particular statistical type.
* For each type of running statistic as well as each statistical variable (sensor) a separate instance object has to be created with separated data buffer.
* Running values of sensor readings or smoothed sensor readings reduce excesses and extremes in the physical value measurement.
* The class may be used for original digital data as well, where the running statistical processing is desirable.

#### Syntax
    gbj_running(uint8_t runningType, uint16_t valueMax, uint16_t valueMin, uint8_t bufferLen)

#### Parameters
<a id="prm_statisticType"></a>
* **statisticType**: Constant defining type of the running statistic to be calculated.
  * *Valid values*: [gbj\_running::AVERAGE, gbj\_running::MEDIAN, gbj\_running::MAXIMUM, gbj\_running::MINIMUM](#constants)
  * *Default value*: [gbj\_running::AVERAGE](#constants)

<a id="prm_valueMax"></a>
* **valueMax**: Maximal valid sensor value for registering.
  * *Valid values*: non-negative integer 0 to 65365
  * *Default value*: 65365

<a id="prm_valueMin"></a>
* **valueMin**: Minimal valid sensor value for registering.
  * *Valid values*: non-negative integer 0 to 65365
  * *Default value*: 0

<a id="prm_bufferLen"></a>
* **bufferLen**: Number of running statistics held in the data buffer used for calculating a new running statistic.
  * The constructor limits the input buffer length to a valid range.
  * The buffer length should be odd number. If it is not, the constructor adds 1 to it right before limiting the length in order to make it odd.
  * *Valid values*: positive integer in the range 3 to 11
  * *Default value*: 5

#### Returns
Object preforming a running statistic calculation.

#### Example
The constructor has all arguments defaulted. The constructor instance without any parameters is equivalent to an instance with all arguments set by corresponding static getters for default values:

``` cpp
gbj_running RunningAvg = gbj_running(); // It is equivalent to
gbj_running RunningAvg = gbj_running(
  gbj_running::AVERAGE,
  gbj_smoothing::getFilterMax(), gbj_smoothing::getFilterMin(),
  gbj_smoothing::getBufferLenDef());
```

If some argument after some defaulted arguments should have a specific value, use corresponding getter in place of those defaulted arguments, e.g.

``` cpp
gbj_running RunningAvg = gbj_running(gbj_running::AVERAGE,
  gbj_smoothing::getFilterMax(), gbj_smoothing::getFilterMin(), 11); // Specific buffer length
```

Typical usage is just with setting a desired statistical type in the constructor:

``` cpp
gbj_running RunningAvg = gbj_running(); // Only default arguments - running average
gbj_running RunningMed = gbj_running(gbj_running::MEDIAN, 1023);      // Running median with maximal value filtering
gbj_running RunningMax = gbj_running(gbj_running::MAXIMUM, 768, 16);  // Running maximum with valid value range filtering
```

#### See also
[setFilterMax()](#setFilter)

[setFilterMin()](#setFilter)

[setFilter()](#setFilter)

[Back to interface](#interface)


<a id="init"></a>

## init()

#### Description
The method initiates all internal counters and status flags of a class instance object to default values as they are right after power up of a microcontroller.

#### Syntax
    void init()

#### Parameters
None

#### Returns
None

[Back to interface](#interface)


<a id="getStatistic"></a>

## getStatistic()

#### Description
The method is overloaded.
* If an input argument is provided, it calculates and returns a new running statistic of a type defined in constructor parameter [statisticType](#prm_statisticType) from the current input value and previous values and stores it in the data buffer for the future calculation.
* If no input argument is provided,the method returns the recently calculated running statistics without need of new measured value (sample) or recalculation.
  * This variant of the method is declared as a static getter.
  * The same effect can be achieved by inputting the value outside the current filter.
  * The method is useful if there is no variable used for the running statistic in an application and the recent statistic is needed.

#### Syntax
    uint16_t getStatistic(uint16_t currentValue)
    uint16_t getStatistic()

#### Parameters
<a id="prm_currentValue"></a>
* **currentValue**: 16-bit value to be used for calculating a new running statistic.
  * *Valid values*: non-negative integer 0 ~ 65535, but restricted by current filter range
  * *Default value*: none

#### Returns
Running statistic or recently stored running statistic.

#### See also
[gbj_running()](#gbj_running)

[Back to interface](#interface)


<a id="getStatisticType"></a>

## getStatisticType()

#### Description
The method returns internal number code (defined by corresponding library constant) of running statistic type for the current instance object.

#### Syntax
    uint8_t getStatisticType()

#### Parameters
None

#### Returns
Code of actual running statistic type defined by one of constants [gbj\_running::AVERAGE, gbj\_running::MEDIAN, gbj\_running::MAXIMUM, gbj\_running::MINIMUM](#constants).

#### See also
[gbj_running()](#gbj_running)

[Back to interface](#interface)


<a id="setFilter"></a>
## setFilter(), setFilterMin(), setFilterMax()
#### Description
The corresponding method redefines minimal, maximal, or both valid values for registered sensor data set in the constructor defined there by default or explicitly.

#### Syntax
    void setFilter(uint16_t valueMax, uint16_t valueMin)
    void setFilterMin(uint16_t valueMin)
    void setFilterMax(uint16_t valueMax)

#### Parameters

* **valueMax**: Maximal valid sensor value for registering.
  * *Valid values*: as in the constructor argument [valueMax](#prm_valueMax)
  * *Default value*: as in the constructor argument [valueMax](#prm_valueMax)


* **valueMin**: Minimal valid sensor value for registering.
  * *Valid values*: as in the constructor argument [valueMin](#prm_valueMin)
  * *Default value*: as in the constructor argument [valueMin](#prm_valueMin)

#### Returns
None

#### See also
[getValueMin()](#getValueRange)

[getValueMax()](#getValueRange)

[gbj_smoothing()](#gbj_smoothing)

[registerData()](#registerData)

[Back to interface](#interface)


<a id="getValueRange"></a>

## getValueMin(), getValueMax()

#### Description
The corresponding method returns currently set minimal or maximal value valid for calculation.

#### Syntax
    uint16_t getValueMin()
    uint16_t getValueMax()

#### Parameters
None

#### Returns
Actual minimal or maximal value of the valid data range.

#### See also
[setFilter()](#setFilter)

[setFilterMin()](#setFilter)

[setFilterMax()](#setFilter)

[Back to interface](#interface)


<a id="getFilterRange"></a>

## getFilterMin(), getFilterMax()

#### Description
The corresponding static method returns hardcoded limit of minimal or maximal value valid for calculation.

#### Syntax
    uint16_t getFilterMin()
    uint16_t getFilterMax()

#### Parameters
None

#### Returns
Minimal or maximal limit of the valid data range.

#### See also
[getValueMin()](#getValueRange)

[getValueMax()](#getValueRange)

[Back to interface](#interface)


<a id="setBufferLen"></a>

## setBufferLen()

#### Description
The method redefines length of the data buffer used for registering sensor data, i.e., the number of data samples used for calculating a running statistic set in the constructor and defined there by default or explicitly.

#### Syntax
    void setBufferLen(uint8_t bufferLen)

#### Parameters
* **bufferLen**: Number of 16-bit values, which a statistical value is going to be calculated from.
  * *Valid values*: as in the constructor argument [bufferLen](#prm_bufferLen)
  * *Default value*: as in the constructor argument [bufferLen](#prm_bufferLen)

#### Returns
None

#### See also
[getBufferLen()](#getBufferLen)

[gbj_running()](#gbj_running)

[Back to interface](#interface)


<a id="getBufferLen"></a>

## getBufferLen()

#### Description
The method returns current length of the data buffer used for calculation.
* Usually the returned value is the same as length put to the constructor parameter [bufferLen](#prm_bufferLen).
* If class has adjusted or limited the input buffer length, the method returns the actual length.
* The method is useful, if the length has been put to the constructor as a numeric literal and there is no variable of the length to use it in other statements.

#### Syntax
    uint8_t getBufferLen()

#### Parameters
None

#### Returns
Actual length of the data buffer.

#### See also
[setBufferLen()](#setBufferLen)

[gbj_running()](#gbj_running)

[getReadings()](#getReadings)

[Back to interface](#interface)


<a id="getBufferLenStatic"></a>

## getBufferLenDef(), getBufferLenMin(), getBufferLenMax()

#### Description
The corresponding static method returns hardcoded default value or valid limit of length of the data buffer used for registering sensor data.

#### Syntax
    uint8_t getBufferLenDef()
    uint8_t getBufferLenMin()
    uint8_t getBufferLenMax()

#### Parameters
None

#### Returns
Default, minimal, or maximal length of the data buffer.

#### See also
[getBufferLen()](#getBufferLen)

[Back to interface](#interface)


<a id="getReadings"></a>

## getReadings()

#### Description
The method returns number of running values of a statistic in the data buffer, which are going to be used for calculating a new one.
* The calculation of a new running value can be provided before the data buffer is full. In that case the method returns the number less than the length of the data buffer set in the constructor parameter [bufferLen](#prm_bufferLen).
* After a while after initialization the returned value is same as the length of the data buffer.

#### Syntax
    uint8_t getReadings()

#### Parameters
None

#### Returns
Actual number of running values of a statistic in the data buffer.

#### See also
[setBufferLen()](#setBufferLen)

[getBufferLen()](#getBufferLen)

[gbj_running()](#gbj_running)

[Back to interface](#interface)
