# Periodics
## Purpose
manages an array of interval periods to support the execution
of recurrent events on a regular, polled basis. Application: Arduino and alikes.

## Application
Arduino - and alikes: e.g. ESP8266, etc - with a main loop() {}.

## Use case
Want to be able to execute various functions at different intervals - without
the use of timer(s) and associated interrupt(s). E.g. you want to poll an NTP server
every 10minutes, while reading a sensor every 30seconds. And you don't want
to jeopardize the Wifi servicing functionality of the Wifi module (ESP8266).

The approach (solution) is to store the various required periods in an array,
and polling it regularly (checking millis()), to see whether a (waiting) period
for a given function is passed.

## Core logic
1. Create an entry for the event you want to schedule regularly.
```C++
        Periodics IntervalMgmt;                    // declared (created) as global object 
        // . . .
        // Initialization...
        IntervalMgmt.set ("EVENT", (int) 5000 );   // refresh every 5 seconds (Arduino and alike)
        // - or -
        IntervalMgmt.set ("EVENT", (int) 5 );      // refresh every 5 seconds (Mac, Linux, ...)
        // . . .
        // at a later point in time, you might want to reset (restart) the 'counters' to 'now()'
        IntervalMgmt.reset();   // will reset the reference time to 'now()' for *all* entries
                                // and clear the 'passed' flag
        // . . .
        IntervalMgmt.reset(1);  // will reset the reference time to 'now()' for *all* entries
                                // and set the 'passed' flag (1), which allows for immediate action.
```
2. In your (eternal) main loop, update the schedules (array of intervals/periods).
```C++
        IntervalMgmt.refresh ();
````

3. Check for the planned period passed and handle the event.
```C++
        if  ( IntervalMgmt.check ("EVENT") ) {
            // . . .
            // event handling part
            // . . .
        }
```
The IntervalMgmt.check function clears the 'passed' at every call. Hence when it returns '1', all
dependent events must be handled following this one check.

## Example
1. The main program ['example.cpp'](https://github.com/kanutope/prototyping/blob/master/Periodics/src/example.cpp) demonstrates briefly how to use this module. It can be built against and runs on Mac (Linux) and Arduino (ESP8266).

2. Below the console output of test programma for ESP8266 + BME280:
```
        time=  1583084560 date=Sun Mar  1 17:42:40 2020 press=98834.12  temp=   24.03 humid=   34.39
        time=  1583084620 date=Sun Mar  1 17:43:40 2020 press=98836.81  temp=   24.00 humid=   34.48
        time=  1583084681 date=Sun Mar  1 17:44:41 2020 press=98838.21  temp=   23.96 humid=   34.54
        time=  1583084740 date=Sun Mar  1 17:45:40 2020 press=98842.36  temp=   23.97 humid=   34.58
        time=  1583084800 date=Sun Mar  1 17:46:40 2020 press=98837.67  temp=   23.92 humid=   34.66
```
   which demonstrates how the periodical polling remains in pace with the initial start time - minor or even bigger
   glitches do not shift the periods (e.g. 17:43:40 - 17:44:41 - 17:45:40, ...). And BTW, this test program uses the
   ['roundRobin'](https://github.com/kanutope/prototyping/tree/master/roundRobin) in memory datalogger besides publishing the sensor data onto a Mosquitto broker. :grin:

