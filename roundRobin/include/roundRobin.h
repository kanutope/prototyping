/* =========================================================================
roundRobin - manages series of values or structures round robin wise, for
data logging purpose.

    Copyright (C) 2020  Paul Biesbrouck

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
========================================================================= */

#ifndef roundRobin_H
#define roundRobin_H

#ifdef  ARDUINO
#include    <Arduino.h>
#include    <stdlib.h>

#ifndef		my_print
#define     my_print(str)           Serial.printf(str)
#define		my_printf(str, ...)		Serial.printf(str, __VA_ARGS__)
#endif

#define     TIMNOW                  (millis ())

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

typedef struct dataRecord {
    unsigned long   time;               // len 4
    char            dateStamp[32];      // len 32
    int             pressure;           // len 2
    int             temperature;        // len 2
    int             humidity;           // len 2

    void            print ();
} dataRecord;


class roundRobin {
    public:
        int         add (dataRecord record);
        int         get (dataRecord *record);
        int         first(dataRecord *record);
        int         next(dataRecord *record);

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
