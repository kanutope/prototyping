# RoundRobin
## Purpose
This class manages a series of records (structures), round robin wise:
    - new records are added up until the entire buffer is in use
    - after which space is being reused, on a FIFO basis.
  
## Application
Keeping a datalogger in memory - e.g. sensor values.

## Core logic
1. Create the buffer first, with the proper maximum size.
```C++
        roundRobin Pressure ((const int) 20);      // declared (created) as global object
```
2. In your main loop, add records to the buffer created.
```C++
	dataRecord 		rec;
        // . . .
        // fill the record with your data
        rec.pressure    = <value>;
        rec.temperature = <value>;
        rec.humidity    = <value>;
        // . . .
        // and store the record
        Pressure.add(rec);
````

3. After having added or while adding more records, you can loop over the present set.
```C++
	dataRecord 		rec;
        // . . .
        // one can print to stdout (Serial) the entire buffer of records
        Pressure.print ();
        // . . .
        // one can loop along the entire buffer and retrieve all records
        i = Pressure.first(&rec);
        while (i > -1) {
                rec.print();    // and even print to stdout (Serial) one record
                i = Pressure.next(&rec);
        }
```  
## Example
The main program ['example.ccp'](https://github.com/kanutope/prototyping/blob/master/roundRobin/src/example.cpp) demonstrates briefly how to use this module. It can be built against and runs on Mac (Linux) and Arduino (ESP8266).