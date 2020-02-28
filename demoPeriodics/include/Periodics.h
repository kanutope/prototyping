/* =========================================================================
Periodics - manages an array of interval periods to support the execution
of recurrent events on a regular, polled basis. Application: Arduino and alikes.

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

#ifndef Periodics_H
#define Periodics_H

#ifdef  ARDUINO
#include    <Arduino.h>

#define     TIMNOW   (millis())

#ifndef		my_print
#define     my_print(str)           Serial.print(str)
#define		my_printf(str, ...)		Serial.printf(str, __VA_ARGS__)
#endif

#else   // MAC, Linux

#include    <stdio.h>
#include    <string.h>
#include    <time.h>

#define     TIMNOW    ((time_t) time(NULL))
#define     Serial  1

#ifndef		my_print
#define     my_print(str)         (int) printf(str)
#define		my_printf(str, ...)   (int) printf(str, __VA_ARGS__)
#endif

#endif

// ARDUINO - millis() wraps around about every 50days
// 0xFFFFFFFF = 4294967295 milliseconds = 49.71 days
// hence we have to cope with that wrap-around
#define     DIFTIM(a,b)     ((((unsigned long long) 0x100000000 + a) - b) % (unsigned long long) 0x100000000);


enum RRtype { RRint, RRlong, RRlonglong, RRfloat, RRdouble, RRinvalid };

typedef struct period_struct {
    char              name[16];     // len: 16
    int               period;       // len:  2
    unsigned long     previous;     // len:  4
    int               passed;       // len:  2
} period_struct;                    // total: 24

// using 'name' as drawback requires 'strcmp' for every operation (method) on an element.
// . . . so be it.
class Periodics {
    public:
        int         check(const char *name);
        int         check(const int  index);
        
        int         set(const char *name, int period);
        int         set(const int period);

        int         refresh();

        int         reset();
        int         reset(const int status);
        void		unset();
        
        void        print();

        Periodics(): rows(0), periods(NULL) { }
        ~Periodics();
		

    private:
        period_struct   *get(const char *name);
        period_struct   *get(const int  indx);
        
        int             rows;
        period_struct   *periods;	// flexible part - must be at the end of the class
};

#endif