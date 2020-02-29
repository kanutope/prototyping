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

/***************************************************************************
  This object manages series of values or structures, round robin wise
    - new values are added up until the entire list is in use
    - after which space is being reused, on a FIFO basis.
  
  Use case
  ========
  Keeping a datalogger in memory - e.g. sensor values.
***************************************************************************/

#include    "roundRobin.h"

/***************************************************************************
 * roundRobin::roundRobin (const int maxValues)
 * ======================
 * Only valid / applicable constructor - taking the maximum size of the buffer.
***************************************************************************/
roundRobin::roundRobin (const int maxValues)
 : count (-1), indx(-1), last (-1), wrap (0), records (NULL)  {
	if (maxValues <= 1) {
		my_printf("whoops, this will not work with maxValues=%d\n", maxValues);
	} else {
		count = maxValues;
		last = -1;
		records = new dataRecord[maxValues];
	}
}

/***************************************************************************
 * roundRobin::~roundRobin ()
 * =======================
 * explicit destructor to ensure the array of structures is released.
***************************************************************************/
roundRobin::~roundRobin () {
	unset ();
}

/***************************************************************************
 * roundRobin::unset ()
 * =================
 * called by destructor, releasing the buffer (array of structures).
***************************************************************************/
void
roundRobin::unset () {
	if (count > 0) delete[] records;
	records = NULL;
	count = -1;
}

/***************************************************************************
 * roundRobin::add (dataRecord record)
 * ===============
 * add an element (dataRecord) to the buffer.
***************************************************************************/
int
roundRobin::add (dataRecord record) {
	int		prv;

	prv = last;
	last = incr(last);

	if (wrap == 0) {					// first round
		if (last < prv) wrap = 1;		// first wrap around
	}

	records[last] = record;
	return (last);
}

/***************************************************************************
 * roundRobin::get (dataRecord *record)
 * ===============
 * add an element (dataRecord) to the buffer.
***************************************************************************/
int
roundRobin::get (dataRecord *record) {
	if (last < 0) {
		return (-1);
	} else {
		*record = records[last];
		return (last);
	}
}

void
dataRecord::print () {
	my_printf("time=%12lu date=%s press=%8.2f  temp=%8.2f humid=%8.2f\n"
						, time
						, dateStamp
						, pressure / 100.0F
						, temperature / 100.0F
						, humidity / 100.0F
						);
}

void
roundRobin::print () {
	int		i;
	if (last < 0) {
		my_print("*** roundRobin::print () - No single value added yet to buffer ***\n");
	} else {
		if (wrap == 0) i = -1;		// first round
		else i = last;				// second and next round

		do {
			i = incr(i);
			records[i].print ();
		} while (i != last);
	}

}

/***************************************************************************
 * roundRobin::first (dataRecord *record)
 * =================
 * initializes a loop along all records stored and returns the first one.
 * Return value (-1) indicates an invalid boundary condition.
***************************************************************************/
int
roundRobin::first (dataRecord *record) {
/*--------------------------------------------------------------------------
If an invalid boundary condition is met - e.g. buffer still empty -
the variable 'record' remains unchanged - hence unpredictable.
--------------------------------------------------------------------------*/
	int		i;

	if ((i = first()) > -1) *record = records[i];
	return(i);
}

/***************************************************************************
 * roundRobin::next (dataRecord *record)
 * ================
 * Retrieves the subsequent next records from a loop initiated by 'first'.
 * Return value (-1) indicates no more records - or an invalid boundary condition.
***************************************************************************/
int
roundRobin::next (dataRecord *record) {
/*--------------------------------------------------------------------------
Provided the loop was correctly initiated by first, the next record is
returned until the most recent one. The next call will then return (-1).
In that case the variable 'record' remains unchanged - hence unpredictable.
--------------------------------------------------------------------------*/
	int		i;

	if ((i = next()) > -1) *record = records[i];
	return(i);
}
	
/* =========================================================================
=== PRIVATE MEMBER FUNCTIONS                                             ===          
============================================================================ */

/***************************************************************************
 * roundRobin::first ()
 * =================
 * initializes the private member 'indx' for looping records already added.
***************************************************************************/
int
roundRobin::first () {
/*--------------------------------------------------------------------------
t0 -              last == -1    => indx = -1     // before all
t1 -              last == -1    => indx = -1     // at creation (instantiation)
t2 - warp == 0    last ==  0    => indx =  0     // after adding first record
t3 - warp == 0    last ==  n    => indx =  0     // first round adding records
t4 - warp == 1    last ==  n    => incr(last)    // wrapped around, buffer full
--------------------------------------------------------------------------*/
	if (last == -1)     indx = -1;
	else if (wrap == 0) indx = 0;
	else                indx = incr(last);

	return(indx);
}

/***************************************************************************
 * roundRobin::next ()
 * ================
 * set 'indx' to the next record, and returns -1 if there are no more.
***************************************************************************/
int
roundRobin::next () {
/*--------------------------------------------------------------------------
 * 
 * t0 -              last == -1    => indx = -1     // before all
 * t1 -              last == -1    => indx = -1     // at creation (instantiation)
 * t2 - warp == 0    last ==  0    => indx = -1     // first record already returned by 'first()'
 *                                                  // No more records available.
 * t3,t4             last != indx  => incr(indx)    // first round adding records
 * t3,t4             last == indx  => indx = -1     // first round adding records
--------------------------------------------------------------------------*/
	if (last == -1)                      indx = -1;
	else if ((wrap == 0) && (last == 0)) indx = -1; // first record already previously returned by 'first'
	else if (indx != last)               indx = incr(indx);
	else                                 indx = -1;

	return(indx);
}
