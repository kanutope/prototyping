/***************************************************************************
 * roundRobin - manages series of values or structures round robin wise, for
 * data logging purpose.
 *
 * MIT License
 *     Copyright (c) 2020 Paul Biesbrouck
 * See file LICENSE included or <https://opensource.org/licenses/MIT>.
 ***************************************************************************/

// SPDX-License-Identifier: MIT

#ifndef roundRobin_H
#define roundRobin_H

#ifdef  ARDUINO
#include    <Arduino.h>
#include    <stdlib.h>

#ifndef		my_print
#define     my_print(str)           Serial.printf(str)
#define		my_printf(str, ...)		Serial.printf(str, __VA_ARGS__)
#endif

#else   // MAC, Linux, et alii

#include    <stdio.h>
#include    <string.h>
#include    <time.h>
#include    <stdlib.h>

#define     Serial  1

#ifndef		my_print
#define     my_print(str)           (int) printf(str)
#define		my_printf(str, ...)     (int) printf(str, __VA_ARGS__)
#endif

#define     TIMNOW                  ((time_t) time (NULL))

#endif

// len=59 buf = {"time":4123456789,"Pa":102399.99,"°C":-01.99,"%h":  0.00}
// const char 		*JSON_fmt = "{\"time\":%10lu,\"Pa\":%9.2f,\"*C\":%6.2f,\"%%h\":%6.2f}";
#define	ROUNDROBIN_JSON_LEN		60		// buffer length required for JSON conversion

typedef char jsonBuffer[ROUNDROBIN_JSON_LEN];

typedef struct dataRecord {
    unsigned long   time;               // len 4
    float           pressure;           // len 4
    float           temperature;        // len 4
    float           humidity;           // len 4

    void            print ();
    void            *json (jsonBuffer);    // requires a string with the required lenght 
} dataRecord;


class roundRobin {
    public:
        int         add (dataRecord record);
        int         get (dataRecord *record);
        int         first (dataRecord *record);
        int         next (dataRecord *record);

        int         reset ();
        void		unset ();
        void        print ();
        
        roundRobin (const int maxValues);
        
        ~roundRobin ();
		

    private:
        roundRobin () : count (-1), indx(-1), last (-1), wrap (0), records (NULL) {};     // to avoid pirates
        int             first();
        int             next();
        inline int incr (const int ptr) { return((ptr+1) %count); };


        int				count;
        int             indx;
        int				last;
        int             wrap;
		dataRecord  	*records;
};

#endif
