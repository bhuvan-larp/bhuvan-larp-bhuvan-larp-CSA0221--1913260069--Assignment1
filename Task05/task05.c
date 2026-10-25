/* Task 5 - header file, include guards and conditional compilation.
 *   normal build: gcc -Wall -Wextra task05.c -o task05
 *   debug build : gcc -Wall -Wextra -DDEBUG task05.c -o task05_debug
 * #include "tariff.h" searches the current folder first (own header);
 * #include <stdio.h> searches the system include folders. */
#include <stdio.h>
#include "tariff.h"
#include "tariff.h" /* second include is harmless thanks to the include guard */

long compute_bill(int units)
{
    long paise = FIXED_CHARGE_PAISE;
#ifdef DEBUG
    printf("[DEBUG] units=%d fixed=%ld\n", units, paise);
#endif
    if (units > SLAB2_UNITS) {
        long part = (long)(units - SLAB2_UNITS) * RATE3_PAISE;
        paise += part;
#ifdef DEBUG
        printf("[DEBUG] slab3 charge=%ld\n", part);
#endif
    }
    if (units > SLAB1_UNITS) {
        long part = (long)((units < SLAB2_UNITS ? units : SLAB2_UNITS) - SLAB1_UNITS) * RATE2_PAISE;
        paise += part;
#ifdef DEBUG
        printf("[DEBUG] slab2 charge=%ld\n", part);
#endif
    }
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
    return 0;
}
