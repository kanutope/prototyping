# prototyping
Some prototyping with microboards à la Arduino, ESP8266 and Raspberry PI.
Exploring and investigating the integration with Mosquitto MQTT broker and Home Assistant.

# modules
## [_Periodics_](https://github.com/kanutope/prototyping/tree/master/Periodics)
* Purpose  
manages an array of interval periods to support the execution of recurrent events on a regular, polled basis. Application: Arduino and alikes.

* Use case  
Want to be able to execute various functions at different intervals - without the use of timer(s) and associated interrupt(s). E.g. you want to poll an NTP server every 10 minutes, while reading a sensor every 30 seconds. And you don't want to jeopardize the Wifi servicing functionality of the Wifi module (ESP8266).  

## [_roundRobin_](https://github.com/kanutope/prototyping/tree/master/roundRobin)
* Purpose  
This class manages a series of records (structures), round robin wise:
    - new records are added up until the entire buffer is in use
    - after which space is being reused, on a FIFO basis.

* Application  
Keeping a **datalogger** in memory - e.g. sensor values.  

