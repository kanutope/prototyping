# prototyping
Some prototyping with microboards à la Arduino, ESP8266 and Raspberry PI.
Exploring and investigating the integration with Mosquitto MQTT broker and Home Assistant.

# modules
## _Periodics_
* Purpose  
manages an array of interval periods to support the execution of recurrent events on a regular, polled basis. Application: Arduino and alikes.

* Use case  
Want to be able to execute various functions at different intervals - without the use of timer(s) and associated interrupt(s). E.g. you want to poll an NTP server every 10 minutes, while reading a sensor every 30 seconds. And you don't want to jeopardize the Wifi servicing functionality of the Wifi module (ESP8266).  

More to come (soon?).
