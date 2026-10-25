/* Task 11 - final recommended design.
 * Types   : meter_t = unsigned short, units = unsigned int, money = long paise,
 *           district totals = long long paise.
 * Tariff  : defaults compiled in with #define; optional tariff.cfg overrides them
 *           at start-up (no recompile needed). Format: "NAME value" per line. */
#include <stdio.h>
#include <string.h>
#include <time.h>

#define DEF_FIXED 5000L
#define DEF_RATE1 0L
#define DEF_RATE2 225L
#define DEF_RATE3 450L
#define SLAB1_UNITS 100
#define SLAB2_UNITS 200

typedef unsigned short meter_t;

_Static_assert(sizeof(long long) >= 8, "long long must hold district totals");
_Static_assert(sizeof(meter_t) == 2, "meter register must be 16-bit");

static long fixed_charge = DEF_FIXED, r1 = DEF_RATE1, r2 = DEF_RATE2, r3 = DEF_RATE3;

static int load_config(const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f) return 0;
    char key[32]; long v; int n = 0;
    while (fscanf(f, "%31s %ld", key, &v) == 2) {
        if (!strcmp(key, "FIXED_PAISE")) fixed_charge = v;
        else if (!strcmp(key, "RATE1_PAISE")) r1 = v;
        else if (!strcmp(key, "RATE2_PAISE")) r2 = v;
        else if (!strcmp(key, "RATE3_PAISE")) r3 = v;
        else continue;
        n++;
    }
    fclose(f);
    return n;
}

static long compute_bill(unsigned int units)
{
    long p = fixed_charge;
    if (units > SLAB2_UNITS) p += (long)(units - SLAB2_UNITS) * r3;
    if (units > SLAB1_UNITS) p += (long)((units < SLAB2_UNITS ? units : SLAB2_UNITS) - SLAB1_UNITS) * r2;
    p += (long)(units < SLAB1_UNITS ? units : SLAB1_UNITS) * r1;
    return p;
}

static void show(const char *label)
{
    unsigned int tests[] = {0, 100, 150, 200, 350};
    printf("%s\n", label);
    for (int i = 0; i < 5; i++) {
        long p = compute_bill(tests[i]);
        printf("  %3u units -> Rs %ld.%02ld\n", tests[i], p / 100, p % 100);
    }
}

int main(void)
{
    printf("Recommended types: meter_t=unsigned short(%zu) units=unsigned int(%zu) money=long(%zu) district=long long(%zu)\n\n",
           sizeof(meter_t), sizeof(unsigned int), sizeof(long), sizeof(long long));
    show("Compiled-in tariff:");

    clock_t t0 = clock();
    int n = load_config("tariff.cfg");
    clock_t t1 = clock();
    printf("\nloaded %d value(s) from tariff.cfg in %.6f s\n", n, (double)(t1 - t0) / CLOCKS_PER_SEC);
    show("Tariff after reading tariff.cfg (RATE2 revised to 230, no recompile):");

    printf("\nLimitations: slabs and the 16-bit register are still fixed in code; one tariff for all categories.\n");
    printf("District scale: worst total about 4e11 paise fits long long (9.2e18) with huge margin.\n");
    return 0;
}
