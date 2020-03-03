/***************************************************************************
 * Periodics - manages an array of interval periods to support the execution
 * of recurrent events on a regular, polled basis. Application: Arduino and alikes.
 *
 * MIT License
 *     Copyright (c) 2020 Paul Biesbrouck
 * See file LICENSE included or <https://opensource.org/licenses/MIT>.
 ***************************************************************************/

// SPDX-License-Identifier: MIT

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

#define PERIOD_NAM_LEN     16

typedef struct period_struct {
    char              name[PERIOD_NAM_LEN];    // len: 16
    int               period;           // len:  2
    unsigned long     previous;         // len:  4
    int               passed;           // len:  2
} period_struct;                    // total: 24

// using 'name' as drawback requires 'strcmp' for every operation (method) on an element.
// . . . so be it.
class Periodics {
    public:
        int         check (const char *name);
        int         check (const int  index);
        
        int         set (const char *name, int period);
        int         set (const int period);

        int         refresh ();

        int         reset ();
        int         reset (const int status);
        void		unset ();
        
        void        print();

        Periodics(): rows (0), periods (NULL) { }
        ~Periodics();
		

    private:
        period_struct   *get (const char *name);
        period_struct   *get (const int  indx);
        
        int             rows;
        period_struct   *periods;	// flexible part - must be at the end of the class
};

#endif
