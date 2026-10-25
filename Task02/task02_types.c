/* Task 2 - Data types: sizes, ranges, and a float vs paise precision problem */
#include <stdio.h>
#include <limits.h>
#include <float.h>

int main(void)
{
    printf("%-20s %-6s %-26s %s\n", "Type", "Bytes", "Min", "Max");
    printf("%-20s %-6zu %-26d %d\n", "char", sizeof(char), CHAR_MIN, CHAR_MAX);
    printf("%-20s %-6zu %-26d %d\n", "short", sizeof(short), SHRT_MIN, SHRT_MAX);
    printf("%-20s %-6zu %-26d %u\n", "unsigned short", sizeof(unsigned short), 0, (unsigned)USHRT_MAX);
    printf("%-20s %-6zu %-26d %d\n", "int", sizeof(int), INT_MIN, INT_MAX);
    printf("%-20s %-6zu %-26d %u\n", "unsigned int", sizeof(unsigned int), 0, UINT_MAX);
    printf("%-20s %-6zu %-26ld %ld\n", "long", sizeof(long), LONG_MIN, LONG_MAX);
    printf("%-20s %-6zu %-26lld %lld\n", "long long", sizeof(long long), LLONG_MIN, LLONG_MAX);
    printf("%-20s %-6zu %-26e %e\n", "float", sizeof(float), (double)FLT_MIN, (double)FLT_MAX);
    printf("%-20s %-6zu %-26e %e\n", "double", sizeof(double), DBL_MIN, DBL_MAX);
    printf("float digits of precision: %d, double: %d\n\n", FLT_DIG, DBL_DIG);

    /* District sufficiency: 800,000 bills of up to about Rs 5,000 = 4e11 paise */
    long long district_paise = 800000LL * 500000LL;
    printf("District worst case %lld paise: fits int? %s, long (64-bit)? %s\n\n",
           district_paise,
           (district_paise <= INT_MAX) ? "yes" : "no",
           (district_paise <= LONG_MAX) ? "yes" : "no");

    /* Roll-over of a 16-bit register */
    unsigned short r = 65535;
    r++;
    printf("unsigned short 65535 + 1 = %u (wraps to 0)\n\n", (unsigned)r);

    /* Float vs paise: add 4000 bills of Rs 162.50 plus a Rs 0.10 bill each */
    float f_total = 0.0f;
    long p_total = 0;
    for (int i = 0; i < 4000; i++) {
        f_total += 162.10f;       /* Rs 162.10 */
        p_total += 16210L;        /* 16210 paise */
    }
    printf("Expected exact total : Rs 648400.00\n");
    printf("float rupees total   : Rs %.2f\n", (double)f_total);
    printf("long paise total     : Rs %ld.%02ld\n", p_total / 100, p_total % 100);
    printf("float error          : Rs %.2f\n", (double)f_total - 648400.0);
    return 0;
}
