<a id="library"></a>

# gbj\_running
The library statistically smooths a data serie by running statistics, namely `average`, `median`, `minimum`, or `maximum`. It calculates a new smoothed value from a couple of previously provided values.

## Fundamental functionality
* The library stores `5` recently provided values, e.g., samples from a sensor reading, and calculates particular statistical value right after it from those values.
* The library instance object acts as a sample cache for recent samples.
* The library does not define a valid range of values to be smoothed. That range and checking values against it should be provided externally.


<a id="dependency"></a>

## Dependency
* **gbj_apphelpers**: Library for generic logics and calculations loaded from file `gbj_apphelpers.h`.

#### Arduino platform
* **Arduino.h**: Main include file for the Arduino SDK version greater or equal to 100.
* **inttypes.h**: Integer type conversions. This header file includes the exact-width integer definitions and extends them with additional facilities provided by the implementation.

#### Espressif platform
* **Arduino.h**: Main include file for the Arduino platform.

#### Particle platform
* **Particle.h**: Includes alternative (C++) data type definitions.
* **math.h**: Includes standard C++ mathematics, needed for fabs().


<a id="tests"></a>

## Unit testing
The subfolder `tests` in the folder `extras`, i.e., `gbj_running/extras/test`, contains testing files, usually just one, with unit tests of library [gbj_running](#library) executable by [Unity](http://www.throwtheswitch.org/unity) test runner. Each testing file should be placed in an individual test folder of a particular project, usually in the structure `test/<testname>/<testfile>`.
* **test_running.cpp**: Test file providing test cases for statistical smoothing calculation and relevant public methods and parameters.


<a id="constants"></a>

## Constants
All constants are embedded into the class as static ones or as enumerations literals.

* **SAMPLES**: Number of recently provided sample values for running statistics. The constant is a literal of the internal enumeration `Params : byte`. The value of the constant is `5`.

### Statistic types
Constants are literals of the internal enumeration `Statistics : byte`.
* **AVERAGE**: Average (arithmetic mean) as a running statistic.
* **MEDIAN**: Median (50% percentile) as a running statistic.
* **MAXIMUM**: Maximum as a running statistic.
* **MINIMUM**: Minimum as a running statistic.


<a id="interface"></a>

## Interface
* [gbj_running()](#gbj_running)
* [init()](#init)

### Getters
* [getValue()](#getValue)
* [getType()](#getType)
* [getSamples()](#getSamples)

#### Setters
* [setAverage()](#setType)
* [setMedian()](#setType)
* [setMinimum()](#setType)
* [setMaximum()](#setType)


<a id="gbj_running"></a>

## gbj\_running()

#### Description
Constructor defines default statistic type for calculation and initiates the internal data buffer acting as the cache within a class instance object, which keeps a serie of provided values, e.g., samples from a sensor's reading, for calculating a running statistic of the statistic type.
* For each type of running statistic as well as each statistical measure (sensor) a separate instance object has to be created.
* Running values of samples from sensor reading reduce excesses and extremes in the physical value measurement.

#### Syntax
    gbj_running(Statistics type)

#### Parameters
* **type**: Optional constant determining type of the default running statistic to be applied at calculations.
  * *Valid values*: one from constants for [statistic types](#constants)
  * *Default value*: gbj\_running::AVERAGE

#### Returns
Object preforming a running statistic calculation.

#### Example
``` cpp
gbj_running runAvg = gbj_running(); // It is equivalent to
gbj_running runAvg = gbj_running(gbj_running::AVERAGE);
```
#### See also
[setAverage(), setMedian(), setMinimum(), setMaximum](#setType)

[Back to interface](#interface)


<a id="init"></a>

## init()

#### Description
The method initiates all internal resources of a class instance object to default values as they are right after creating a library instance object.

#### Syntax
    void init()

#### Parameters
None

#### Returns
None

[Back to interface](#interface)


<a id="getValue"></a>

## getValue()

#### Description
The method is overloaded and returns a result of the recent smoothing calculation.
* If an input argument is provided, it calculates and returns a new running statistic of a type defined in [constructor](#gbj_running) or corresponding [setter](#setType) from the input value and previously stored ones in the cache. This value is stored in the internal cache for the future calculation. At the same time the last value in the cache is removed.
* If no input argument is provided, the method returns the recently calculated running statistic without need of new recalculation.
* If the method without input value is used before the first using the method with input value since initiation either by creating the [instance object](#gbj_running) or calling the method [init()](#init), the `0` is retuned as a default statistic value.
* The method is useful if there is no variable used for the running statistic in an application and the recent statistic is needed.
* The method can be used as a "last good value" at external filtering against a valid range of values.

#### Syntax
    float getValue(float value)
    float getValue()

#### Parameters
* **value**: Value (sample) to be used for calculating a new running statistic.
  * *Valid values*: float
  * *Default value*: none

#### Returns
Running statistic of default statistic type or recently calculated running statistic.

[Back to interface](#interface)


<a id="getType"></a>

## getType()

#### Description
The method returns internal number code representing the statistic type by some of the [constants](#constants) for statistic types.

#### Syntax
    Statistics getType()

#### Parameters
None

#### Returns
Code of actual running statistic type defined by one of constants for [statistic types](#constants).

#### See also
[gbj_running()](#gbj_running)

[Back to interface](#interface)


<a id="getSamples"></a>

## getSamples()

#### Description
The method returns current number of samples in the internal cache, which are going to be used for calculating running statistics.
* The calculation of a running value can be provided before the cache is full. It happens at start of the smoothing process after initialization by either creating the [instance object](#gbj_running) or calling the method [init()](#init). In that case the method returns the number less than the length of the buffer defined by the constant [SAMPLES](#constants).

#### Syntax
    byte getSamples()

#### Parameters
None

#### Returns
Actual number of samples in the cache.

[Back to interface](#interface)


<a id="setType"></a>

## setAverage(), setMedian(), setMinimum(), setMaximum

#### Description
The particular method sets the corresponding statistic type as the default one.
* The running statistic is calculated for that type on without change in the internal cache.
* The methods are useful in conjunction with the constructor without input value.

#### Syntax
    void setAverage()
    void setMedian()
    void setMinimum()
    void setMaximum()

#### Parameters
None

#### Returns
None

#### Example
``` cpp
gbj_running runAvg = gbj_running(); // Defaulted to Average
runAvg.setMedian();
```
#### See also
[gbj_running()](#gbj_running)

[Back to interface](#interface)
