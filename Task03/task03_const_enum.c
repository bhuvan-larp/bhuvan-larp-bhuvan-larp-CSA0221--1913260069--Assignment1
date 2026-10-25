/* Task 3 alternative: the same tariff written as enum and const variables.
 * enum   : integer constants, typed int, no storage, visible in debugger.
 * const  : real typed variable (has storage), has scope, type-checked.
 * #define : text replacement, no type, no storage, invisible to the debugger. */
#include <stdio.h>

enum { SLAB1_UNITS = 100, SLAB2_UNITS = 200 };
static const long RATE1_PAISE = 0;
static const long RATE2_PAISE = 225;
static const long RATE3_PAISE = 450;
static const long FIXED_CHARGE_PAISE = 5000L;

static long compute_bill(int units)
{
    long paise = FIXED_CHARGE_PAISE;
    if (units > SLAB2_UNITS) paise += (long)(units - SLAB2_UNITS) * RATE3_PAISE;
    if (units > SLAB1_UNITS)
        paise += (long)((units < SLAB2_UNITS ? units : SLAB2_UNITS) - SLAB1_UNITS) * RATE2_PAISE;
    paise += (long)(units < SLAB1_UNITS ? units : SLAB1_UNITS) * RATE1_PAISE;
    return paise;
}

int main(void)
{
    int tests[] = {0, 100, 150, 200, 350};
    for (int i = 0; i < 5; i++) {
        long p = compute_bill(tests[i]);
        printf("%3d units -> Rs %ld.%02ld\n", tests[i], p / 100, p % 100);
    }
    printf("sizeof(RATE2_PAISE const) = %zu bytes of storage; a #define uses 0\n", sizeof RATE2_PAISE);
    return 0;
}
