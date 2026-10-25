/* Task 9 - Implement and test the billing program.
 * Input (stdin or file): one line per house:  household previous current
 *   ./task09 < test_input.txt
 * Assumptions (stated in the report):
 *   - the meter register is 16-bit: valid readings are 0..65535
 *   - a drop (current < previous) is accepted as a roll-over only if the
 *     implied consumption is at most MAX_MONTHLY_UNITS; otherwise it is an error
 * All money is in paise (long); district totals use long long. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SLAB1_UNITS 100
#define SLAB2_UNITS 200
#define RATE1_PAISE 0
#define RATE2_PAISE 225
#define RATE3_PAISE 450
#define FIXED_CHARGE_PAISE 5000L
#define MAX_MONTHLY_UNITS 10000U

typedef unsigned short meter_t;

static long compute_bill(unsigned int units)
{
    long paise = FIXED_CHARGE_PAISE;
    if (units > SLAB2_UNITS)
        paise += (long)(units - SLAB2_UNITS) * RATE3_PAISE;
    if (units > SLAB1_UNITS)
        paise += (long)((units < SLAB2_UNITS ? units : SLAB2_UNITS) - SLAB1_UNITS) * RATE2_PAISE;
    paise += (long)(units < SLAB1_UNITS ? units : SLAB1_UNITS) * RATE1_PAISE;
    return paise;
}

/* Alternative design: slabs stored in arrays, loop over them: O(s) per bill. */
static long compute_bill_loop(unsigned int units)
{
    static const unsigned int limit[3] = {SLAB1_UNITS, SLAB2_UNITS, 0xFFFFFFFFU};
    static const long rate[3] = {RATE1_PAISE, RATE2_PAISE, RATE3_PAISE};
    long paise = FIXED_CHARGE_PAISE;
    unsigned int lower = 0;
    for (int s = 0; s < 3 && units > lower; s++) {
        unsigned int upper = units < limit[s] ? units : limit[s];
        paise += (long)(upper - lower) * rate[s];
        lower = limit[s];
    }
    return paise;
}

/* Parses a whole token as a non-negative integer; returns 0 on failure. */
static int parse_reading(const char *tok, long *out)
{
    char *end;
    long v = strtol(tok, &end, 10);
    if (end == tok || *end != '\0') return 0;
    *out = v;
    return 1;
}

int main(void)
{
    char line[128], a[32], b[32], c[32];
    int lineno = 0, valid = 0, invalid = 0;
    long long total = 0;

    printf("%-5s %-8s %-8s %-8s %-6s %-10s %s\n", "line", "house", "prev", "curr", "units", "bill", "status");
    while (fgets(line, sizeof line, stdin) != NULL) {
        lineno++;
        if (sscanf(line, "%31s %31s %31s", a, b, c) != 3) {
            printf("%-5d %-8s %-8s %-8s %-6s %-10s INVALID (need 3 fields)\n", lineno, "-", "-", "-", "-", "-");
            invalid++;
            continue;
        }
        long house, prev, curr;
        if (!parse_reading(a, &house) || !parse_reading(b, &prev) || !parse_reading(c, &curr)) {
            printf("%-5d %-8s %-8s %-8s %-6s %-10s INVALID (not a number)\n", lineno, a, b, c, "-", "-");
            invalid++;
            continue;
        }
        if (prev < 0 || curr < 0 || prev > 65535 || curr > 65535) {
            printf("%-5d %-8ld %-8ld %-8ld %-6s %-10s INVALID (negative or out of range)\n", lineno, house, prev, curr, "-", "-");
            invalid++;
            continue;
        }
        unsigned int units = (meter_t)((meter_t)curr - (meter_t)prev);
        if (curr < prev && units > MAX_MONTHLY_UNITS) {
            printf("%-5d %-8ld %-8ld %-8ld %-6u %-10s INVALID (current < previous, no roll-over)\n", lineno, house, prev, curr, units, "-");
            invalid++;
            continue;
        }
        long p = compute_bill(units);
        if (p != compute_bill_loop(units)) {
            printf("INTERNAL ERROR: array/loop design disagrees for %u units\n", units);
            return 1;
        }
        printf("%-5d %-8ld %-8ld %-8ld %-6u Rs %ld.%02ld %s\n", lineno, house, prev, curr, units,
               p / 100, p % 100, (curr < prev) ? "OK (roll-over)" : "OK");
        total += p;
        valid++;
    }
    printf("\nvalid bills: %d, invalid lines: %d\n", valid, invalid);
    printf("total of valid bills: Rs %lld.%02lld\n", total / 100, total % 100);
    return 0;
}
