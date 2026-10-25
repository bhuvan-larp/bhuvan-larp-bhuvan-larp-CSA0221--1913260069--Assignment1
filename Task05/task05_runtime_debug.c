/* Task 5 alternative: debug controlled by a run-time variable.
 * The test "if (debug)" runs on every call (O(1) extra time per call) and the
 * debug code always stays in the executable (extra space), unlike #ifdef DEBUG. */
#include <stdio.h>
#include "tariff.h"

static int debug = 1; /* change at run time */

long compute_bill(int units)
{
    long paise = FIXED_CHARGE_PAISE;
    if (debug) printf("[debug] units=%d\n", units);
    if (units > SLAB2_UNITS) paise += (long)(units - SLAB2_UNITS) * RATE3_PAISE;
    if (units > SLAB1_UNITS)
        paise += (long)((units < SLAB2_UNITS ? units : SLAB2_UNITS) - SLAB1_UNITS) * RATE2_PAISE;
    paise += (long)(units < SLAB1_UNITS ? units : SLAB1_UNITS) * RATE1_PAISE;
    if (debug) printf("[debug] bill=%ld paise\n", paise);
    return paise;
}

int main(void)
{
    long p = compute_bill(150);
    printf("150 units -> Rs %ld.%02ld\n", p / 100, p % 100);
    return 0;
}
