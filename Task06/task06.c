/* Task 6 - read and print data correctly.
 * Input is read from stdin: household previous current (one house per line).
 * Run:  ./task06 < input.txt
 * scanf returns the number of items converted; anything other than 3 means invalid input. */
#include <stdio.h>
#include <limits.h>

static long compute_bill(unsigned int units)
{
    long p = 5000L;
    if (units > 200) p += (long)(units - 200) * 450;
    if (units > 100) p += (long)((units < 200 ? units : 200) - 100) * 225;
    return p;
}

int main(void)
{
    /* Wrong-specifier demonstration: %d given a long that does not fit in an int */
    long big = 5000000000L;
    printf("--- wrong specifier test ---\n");
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat"
    printf("long 5000000000 printed with %%d : %d (wrong, undefined behaviour)\n", (int)big);
#pragma GCC diagnostic pop
    printf("long 5000000000 printed with %%ld: %ld (correct)\n\n", big);

    printf("--- specifiers ---\n");
    int i = -7; unsigned u = 42; long l = 123456789L; long long ll = 9876543210LL;
    float f = 1.5f; double d = 2.25; char c = 'D'; const char *s = "Grama Jyothi";
    printf("%%d=%d %%u=%u %%ld=%ld %%lld=%lld %%f=%f %%lf=%lf %%c=%c %%s=%s\n\n", i, u, l, ll, f, d, c, s);

    printf("--- billing from stdin ---\n");
    unsigned int house;
    int prev, curr;
    int line = 0;
    int r;
    while ((r = scanf("%u %d %d", &house, &prev, &curr)) != EOF) {
        line++;
        if (r != 3) {
            printf("line %d: invalid input (scanf returned %d), skipping rest of line\n", line, r);
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) { }
            continue;
        }
        if (prev < 0 || curr < 0 || prev > 65535 || curr > 65535) {
            printf("house %u: reading out of range 0..65535\n", house);
            continue;
        }
        unsigned int units = (unsigned short)(curr - prev);
        long p = compute_bill(units);
        printf("house %u: %u units -> Rs %ld.%02ld\n", house, units, p / 100, p % 100);
    }
    return 0;
}
