/* Task 8 - follow the program through the compilation stages.
 * See build_stages.sh. This file has a macro, a comment and an include so
 * that the preprocessor output is visibly different from the source. */
#include <stdio.h>

#define FIXED_CHARGE_PAISE 5000L /* Rs 50 */
#define RUPEES_TO_PAISE(r) ((r) * 100L)

long bill_for(int units)
{
    return FIXED_CHARGE_PAISE + RUPEES_TO_PAISE(units > 100 ? units - 100 : 0);
}

int main(void)
{
    long p = bill_for(150);
    printf("150 units -> Rs %ld.%02ld\n", p / 100, p % 100);
    return 0;
}
