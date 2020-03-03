/***************************************************************************
 * Sample main program demonstrating the use and features of the Periodics module.
 * It can be built against and runs on Mac (Linux) and Arduino (ESP8266).
 *
 * MIT License
 *     Copyright (c) 2020 Paul Biesbrouck
 * See file LICENSE included or <https://opensource.org/licenses/MIT>.
 ***************************************************************************/

// SPDX-License-Identifier: MIT

#include    "Periodics.h"

Periodics IntervalMgmt;

void setup () {
#ifdef ARDUINO
  Serial.begin(115200);
// initialize the interval polling mechanisme
  IntervalMgmt.set ("EVENT1", (int) 5000 );   // refresh every 5 seconds
  IntervalMgmt.set ("EVENT2", (int) 4000 );   // update every 4 seconds
#else   // MAC, Linux
// initialize the interval polling mechanisme
  IntervalMgmt.set ("EVENT1", (int) 5 );      // refresh every 5 seconds
  IntervalMgmt.set ("EVENT2", (int) 4 );      // update every 4 seconds
#endif

// for debugging purpose
  IntervalMgmt.print();
}

unsigned long long cycle = 0;

void loop () {
#define BUF_LEN   32
  char    buf[BUF_LEN];
  time_t  tClock;


    ++cycle;
    IntervalMgmt.refresh ();

    tClock = TIMNOW;
#ifdef ARDUINO
    snprintf(buf, BUF_LEN, "%16lu\n", tClock / 1000);
#else   // MAC, Linux
    snprintf(buf, BUF_LEN, "%s", ctime(&tClock));    // btw, ctime ends with a '\n'
#endif

    if  ( IntervalMgmt.check ("EVENT1") ) {
      my_printf ("\n%16llu Event1     passed at %s", cycle, buf);
    }

    if  ( IntervalMgmt.check ("EVENT2") ) {
      my_printf ("\n%16llu     Event2 passed at %s", cycle, buf);
    }
}

#ifndef ARDUINO

/*
struct timespec {
    time_t tv_sec;	/ * seconds * /
    long tv_nsec;	/ * nanoseconds * /
};
*/

int
main () {
    int   i;
    timespec  req, got;

    setup ();

    req.tv_sec = 0;               // seconds
    req.tv_nsec = 100*1000*1000;  // nanoseconds

    while (1) {
      loop ();
      my_print(".");
      fflush(stdout);
// sleep 100nsec = 0,1sec
      nanosleep (&req, &got);
    }

    return (0);
}

#endif
