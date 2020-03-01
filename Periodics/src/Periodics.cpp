/***************************************************************************
 * Periodics - manages an array of interval periods to support the execution
 * of recurrent events on a regular, polled basis. Application: Arduino and alikes.
 *
 * MIT License
 *     Copyright (c) 2020 Paul Biesbrouck
 * See file LICENSE included or <https://opensource.org/licenses/MIT>.
 ***************************************************************************/

// SPDX-License-Identifier: MIT

#include    "Periodics.h"

/*================================================================
 * Periodics::check (const char *name)
 * ----------------
 * check the period matching the given name for its period being passed ('passed' = 1).
 * This function clears 'passed' at every call. Hence when it returns '1', all
 * dependendt events must be handled following this one check.
 * Observe the update of 'passed' is done through the global 'refresh' method.
  ================================================================*/
int
Periodics::check (const char *name) {
    period_struct   *period;
    int             passed;

    if ( (period = get (name)) == NULL) {
        return (-1);
    } else {
        passed = period->passed;
        period->passed = 0;
        return (passed);
    }
}

/*================================================================
 * Periodics::check (const int indx)
 * ----------------
 * check the period matching the given index for its period being passed ('passed' = 1).
 * This function clears 'passed' at every call. Hence when it returns '1', all
 * dependendt events must be handled following this one check.
 * Observe the update of 'passed' is done through the global 'refresh' method.
  ================================================================*/
int
Periodics::check (const int indx) {
    period_struct   *period;
    int             passed;

    if ( (period = get (indx)) == NULL) {
        return (-1);
    } else {
        passed = period->passed;
        period->passed = 0;
        return (passed);
    }
}

/*================================================================
 * Periodics::refresh ()
 * ------------------
 * taking the current time, mark the periods matching the given index
 * for its delay being passed => setting 'passed' to 1.
  ================================================================*/
int
Periodics::refresh () {
    int                         i;
    unsigned long               tim = TIMNOW; 
    unsigned long               dif, per, prv;
    
    for (i = 0; i< rows; ++i) {
        prv = periods[i].previous;
        per = periods[i].period;
// ARDUINO - millis() wraps around about every 50days
// 0xFFFFFFFF = 4294967295 milliseconds = 49.71 days
// hence we have to cope with that wrap-around
        dif = DIFTIM(tim, prv);


//DEBUG// my_printf ("i=%d per=%ld prv=%ld tim=%ld dif=%ld\n", i, per, prv, tim, dif);
        if (dif >= per) {
// in order to avoid shifting, the new reference time is calculated on the basis of the previous time.
// Moreover, the refresh might have been delayed - for whatever reason - for a longer time than 'period'
// hence we calculate 'previous' to the latest possible.
            while (dif >= per) {
                prv = prv + per;
                dif = DIFTIM(tim, prv);
            }

            periods[i].previous = prv;
            periods[i].passed = 1;
        }
    }

    return (rows);
}

/*================================================================
 * Periodics::reset ()
 * ----------------
 * reset the whole array of periods (intervals), updating the 'previous' time
 * to the current time and clearing 'passed'.
  ================================================================*/
int
Periodics::reset () {
    int             i;
    unsigned long   tim = TIMNOW;
    
    for (i = 0; i< rows; ++i) {
        periods[i].passed = 0;
        periods[i].previous = tim;
    }

    return (rows);
}

/*================================================================
 * Periodics::reset (const int status)
 * ----------------
 * reset the whole array of periods (intervals), updating the 'previous' time
 * to the current time and *setting* 'passed' to 'status'.
 * Purpose is to start the period measurement from this call onwards.
  ================================================================*/
int
Periodics::reset (const int status) {
    int             i;
    unsigned long   tim = TIMNOW;
    
    for (i = 0; i< rows; ++i) {
        periods[i].passed = status;
        periods[i].previous = tim;
    }

    return (rows);
}

/*================================================================
 * Periodics::set (const char *name, int period)
 * --------------
 * adding a new interval to the array, identified by its 'name'.
  ================================================================*/
int
Periodics::set (const char *name, int period) {
    int             i;
    period_struct   *newArray, *oldArray;	// to cope with adding a new period to the list


	if (rows > 0) {
		for (i = 0; i< rows; ++i) {
            if (strcmp (periods[i].name, name) == 0) {
                periods[i].period = period;
                return (i);
            }
		}
    }
    
// reallocate new array size+1
    newArray = new period_struct[rows+1];     // reallocate array size+1
// duplicate current elements
    for (i = 0; i< rows; ++i) {
        newArray[i] = periods[i];
    }
// initialize new period 
    strncpy (newArray[rows].name, name, 15); newArray[rows].name[15] = '\0';
    newArray[rows].period = period;
    newArray[rows].previous = TIMNOW;
    newArray[rows].passed = 0;

// replace array with newly created
    oldArray = periods;
    periods = newArray;
// and release oldArray
    delete[] oldArray;

    return (++rows);
}

/*================================================================
 * Periodics::~Periodics
 * ---------------------
 * explicit destructor to ensure the array of structures get released.
  ================================================================*/
Periodics::~Periodics () {
	unset ();
}

/*================================================================
 * Periodics::unset ()
 * ----------------
 * called by destructor, releasing the array of structures.
  ================================================================*/
void
Periodics::unset () {
	if (rows > 0) delete[] periods;
	periods = NULL;
	rows = 0;
}

/*================================================================
 * Periodics::print ()
 * ----------------
 * function printing out the entire array of periodss.
  ================================================================*/
void
Periodics::print () {
    int             i;
    
    if (Serial) {
		my_printf ("number of elements: %d\n", rows);
        for (i = 0; i < rows; ++i) {
            my_printf ("%s - period=%d previous=%ld passed=%d\n",
                            periods[i].name, periods[i].period, periods[i].previous, periods[i].passed);
        }
    }
}

/***************************************************************************
 * PRIVATE MEMBER FUNCTIONS                                                *
 ***************************************************************************/

/*================================================================
 * Periodics::get (const char *name)
 * --------------------------------
 * *** PRIVATE MEMBER FUNCTION ***
 * Returns the period (as pointer) matching the given name, otherwise NULL.
  ================================================================*/
period_struct *
Periodics::get (const char *name) {
    int     i;

    if (periods == NULL) return (NULL);     // no single element has yet been added
    for (i = 0; i< rows; ++i) {
        if (strcmp (periods[i].name, name) == 0) {
            return (&periods[i]);
        }
    }

    return (NULL);
}

/*================================================================
 * Periodics::get (const int indx)
 * --------------
 * *** PRIVATE MEMBER FUNCTION ***
 * Returns the period (as pointer) matching the given index, otherwise NULL.
  ================================================================*/
period_struct *
Periodics::get (const int indx) {
    if (periods == NULL) return (NULL);             // no single element has yet been added
    if ( (indx < 1) || (rows < indx)) return (NULL);  // invalid index
    return (&periods[indx-1]);
}

