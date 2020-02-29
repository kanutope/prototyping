/* =========================================================================
GNU All-Permissive License
--------------------------
Copying and distribution of this file, with or without modification,
are permitted in any medium without royalty provided the copyright
notice and this notice are preserved.  This file is offered as-is,
without any warranty.
========================================================================= */

/***************************************************************************
 * Sample main program demonstrating the use and features of the RoundRobin module.
 * It can be built against and runs on Mac (Linux) and Arduino (ESP8266).
***************************************************************************/

#include  "roundRobin.h"

roundRobin Pressure ((const int) 16);

// some defines for testing purpose
#ifdef  ARDUINO
#define	EPOCH(x)	    (x/1000 + 1582905719)	// Epoch 20200228-16040159 = 1582905719
#define	INITRAND(x)		randomSeed (x)
#define	RANDOM		    (random (0x80000000))    // just for testing purpose
#else	// MAC, Linux, et alii
#define	EPOCH(x)		(x)						// no conversion on Mac, Linux et alii
#define	INITRAND(x)		srandom (x)
#define	RANDOM			(random ())        // just for testing purpose
#endif

void
setup () {
	dataRecord 		last;

#ifdef ARDUINO
	Serial.begin (115200);
#endif

	INITRAND (TIMNOW);
  
	if (Pressure.get (&last) < 0) my_print("*** No single value added yet to buffer ***\n");
	else last.print ();
}

#define	maxCnt	10		// used to control Mac (Linux) implementation, i.e. to avoid endless loop.

int		counter = 0;
int		stop    = 0;

void
loop () {
	int				i;
	time_t  		tClock;
	float			denom = 0x80000000;

	dataRecord 		rec;

	if (counter < maxCnt) {
		tClock = TIMNOW;
		
		rec.time = EPOCH(tClock) + 30*counter;
		strcpy (rec.dateStamp, ctime ((const time_t *) &rec.time));
		rec.dateStamp[strlen (rec.dateStamp)-1] = '\0';						// cutting off the last character '\n'
		rec.pressure    = 100 * (1010.0F + 12 * (0.5 - RANDOM / denom));	// add some random portion
		rec.temperature = 100 * (10.0F + 12 * (0.5 - RANDOM / denom));  	// add some random portion
		rec.humidity    = 100 * (100.0F * RANDOM / denom);					// add some random portion
		Pressure.add(rec);
	} else if (counter == maxCnt) {
		my_print("*** Calling class print() function\n");
		Pressure.print ();
	} else if (counter == (maxCnt+1)) {
		my_print("*** Looping the buffer one by one\n");
		i = Pressure.first(&rec);
		while (i > -1) {
			my_printf("*** Printing record[%d]\n", i);
			rec.print();
			i = Pressure.next(&rec);
		}
	} else {
		stop = 1;
	}

	counter++;
}

#ifndef		ARDUINO

int
main () {
	setup ();
	while (stop == 0) loop();
	return (0);
}

#endif