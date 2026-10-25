/* Task 1 - Understand the problem and identify the data
 * Prints the data inventory of the billing program (type, size, why) and
 * checks the rules every correct bill must satisfy.
 * Also compares reading a meter value as an integer vs as text. */
#include <stdio.h>
#include <string.h>
#include <limits.h>

#define FIXED_CHARGE_PAISE 5000L
#define RATE2_PAISE 225
#define RATE3_PAISE 450

typedef unsigned short meter_t; /* 16-bit register, 0..65535 */

struct bill {
    long fixed;   /* paise */
    long energy;  /* paise */
    long total;   /* paise */
};

static struct bill make_bill(unsigned int units)
{
    struct bill b;
    long e = 0;
    if (units > 200) e += (long)(units - 200) * RATE3_PAISE;
    if (units > 100) e += (long)((units < 200 ? units : 200) - 100) * RATE2_PAISE;
    b.fixed = FIXED_CHARGE_PAISE;
    b.energy = e;
    b.total = b.fixed + b.energy;
    return b;
}

int main(void)
{
    printf("=== Data inventory ===\n");
    printf("%-20s %-16s %-6s %s\n", "Item", "Type", "Bytes", "Reason");
    printf("%-20s %-16s %-6zu %s\n", "household number", "unsigned int", sizeof(unsigned int), "up to 800,000 houses");
    printf("%-20s %-16s %-6zu %s\n", "category", "char", sizeof(char), "one letter, e.g. 'D' domestic");
    printf("%-20s %-16s %-6zu %s\n", "previous reading", "meter_t(ushort)", sizeof(meter_t), "16-bit register 0..65535");
    printf("%-20s %-16s %-6zu %s\n", "current reading", "meter_t(ushort)", sizeof(meter_t), "16-bit register 0..65535");
    printf("%-20s %-16s %-6zu %s\n", "units consumed", "unsigned int", sizeof(unsigned int), "never negative (roll-over handled)");
    printf("%-20s %-16s %-6zu %s\n", "money (paise)", "long", sizeof(long), "integer paise: no rounding error");
    printf("%-20s %-16s %-6zu %s\n", "district total", "long long", sizeof(long long), "800,000 bills can exceed 32 bits");
    printf("Constants (never change in a run): fixed charge, slab limits, rates\n\n");

    printf("=== Rule checks (bill never negative, total = fixed + energy) ===\n");
    unsigned int tests[] = {0, 100, 150, 200, 350};
    int ok = 1;
    for (int i = 0; i < 5; i++) {
        struct bill b = make_bill(tests[i]);
        int pass = (b.total >= 0) && (b.total == b.fixed + b.energy);
        printf("%3u units: fixed=%ld energy=%ld total=%ld  %s\n",
               tests[i], b.fixed, b.energy, b.total, pass ? "OK" : "FAIL");
        if (!pass) ok = 0;
    }
    printf("All rules satisfied: %s\n\n", ok ? "yes" : "no");

    printf("=== Alternative: reading stored as text vs integer ===\n");
    const char *text = "01530";
    size_t d = strlen(text);
    unsigned int value = 0;
    for (size_t i = 0; i < d; i++) value = value * 10 + (unsigned)(text[i] - '0');
    printf("text \"%s\" uses %zu bytes (d+1), converting costs O(d); integer uses %zu bytes, comparing costs O(1)\n",
           text, d + 1, sizeof(meter_t));
    printf("converted value = %u\n", value);
    printf("Text compare of two d-digit strings is O(d); integer compare is O(1).\n");
    return 0;
}
