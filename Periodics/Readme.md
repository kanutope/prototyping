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
The main program ['example.cpp'](https://github.com/kanutope/prototyping/blob/master/Periodics/src/example.cpp) demonstrates briefly how to use this module. It can be built against and runs on Mac (Linux) and Arduino (ESP8266)
