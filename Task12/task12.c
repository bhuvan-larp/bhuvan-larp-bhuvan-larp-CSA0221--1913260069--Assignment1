/* Task 12 - Smart meters: 15-minute readings in Wh, time-of-day tariff, tariff read from a file.
 * Decision: MODIFY the original design (keep integer paise, widen types, tariff from file).
 *   ./task12 [tariff_file]        (default: tariff.txt)
 *
 * Peak rule (assumption): energy used 18:00-22:00 pays PEAK_EXTRA_PERCENT more.
 *   energy_charge = slab charge on the month's total Wh (rounded once, half up)
 *   premium       = energy_charge * peak_Wh * extra% / total_Wh   (rounded half up)
 *   total         = fixed + energy_charge + premium
 * Readings: simulated 30 days x 96 intervals; cumulative register is a 32-bit Wh
 * counter started near its maximum so a roll-over occurs during the month. */
#include <stdio.h>
#include <string.h>

typedef unsigned int wh_t;            /* 32-bit cumulative register, Wh */
typedef unsigned long long whsum_t;   /* sums of Wh over months/years/district */
typedef long long paise_t;

struct tariff {
    long fixed, rate1, rate2, rate3, peak_pct;
    long slab1_kwh, slab2_kwh;
    int peak_start, peak_end;
};

static int load_tariff(const char *path, struct tariff *t)
{
    FILE *f = fopen(path, "r");
    if (!f) return -1;
    char key[48]; long v; int found = 0;
    while (fscanf(f, "%47s %ld", key, &v) == 2) {
        if (!strcmp(key, "FIXED_PAISE")) { t->fixed = v; found++; }
        else if (!strcmp(key, "RATE1_PAISE")) { t->rate1 = v; found++; }
        else if (!strcmp(key, "RATE2_PAISE")) { t->rate2 = v; found++; }
        else if (!strcmp(key, "RATE3_PAISE")) { t->rate3 = v; found++; }
        else if (!strcmp(key, "SLAB1_KWH")) { t->slab1_kwh = v; found++; }
        else if (!strcmp(key, "SLAB2_KWH")) { t->slab2_kwh = v; found++; }
        else if (!strcmp(key, "PEAK_EXTRA_PERCENT")) { t->peak_pct = v; found++; }
        else if (!strcmp(key, "PEAK_START_HOUR")) { t->peak_start = (int)v; found++; }
        else if (!strcmp(key, "PEAK_END_HOUR")) { t->peak_end = (int)v; found++; }
    }
    fclose(f);
    return found == 9 ? 0 : -2;
}

/* rate is paise per kWh, so Wh * rate / 1000 = paise; round once at the end */
static paise_t energy_charge(whsum_t wh, const struct tariff *t)
{
    whsum_t s1 = (whsum_t)t->slab1_kwh * 1000ULL, s2 = (whsum_t)t->slab2_kwh * 1000ULL;
    whsum_t a = wh < s1 ? wh : s1;
    whsum_t b = wh > s1 ? (wh < s2 ? wh : s2) - s1 : 0;
    whsum_t c = wh > s2 ? wh - s2 : 0;
    whsum_t x = a * (whsum_t)t->rate1 + b * (whsum_t)t->rate2 + c * (whsum_t)t->rate3;
    return (paise_t)((x + 500ULL) / 1000ULL);
}

static paise_t peak_premium(paise_t energy, whsum_t peak_wh, whsum_t total_wh, long pct)
{
    if (total_wh == 0) return 0;
    unsigned long long den = total_wh * 100ULL;
    unsigned long long num = (unsigned long long)energy * peak_wh * (unsigned long long)pct;
    return (paise_t)((num + den / 2) / den);
}

static void print_money(const char *label, paise_t p)
{
    printf("%-26s Rs %lld.%02lld\n", label, p / 100, p % 100);
}

int main(int argc, char **argv)
{
    const char *path = argc > 1 ? argv[1] : "tariff.txt";
    struct tariff t;
    int rc = load_tariff(path, &t);
    if (rc != 0) {
        printf("cannot use tariff file '%s' (code %d): stopping, no bill produced\n", path, rc);
        return 1;
    }
    printf("tariff loaded from %s: fixed=%ld r1=%ld r2=%ld r3=%ld paise, peak +%ld%% %02d:00-%02d:00\n\n",
           path, t.fixed, t.rate1, t.rate2, t.rate3, t.peak_pct, t.peak_start, t.peak_end);

    /* Simulate one month of 15-minute readings with a 32-bit register near roll-over. */
    wh_t reg = 4294966000U, prev = reg;
    whsum_t total_wh = 0, peak_wh = 0;
    int rolled = 0;
    for (int day = 0; day < 30; day++) {
        for (int k = 0; k < 96; k++) {
            int hour = k / 4;
            wh_t use = (hour >= 18 && hour < 22) ? 400U : 150U;
            reg += use;                          /* wraps modulo 2^32 */
            if (reg < prev) rolled = 1;
            wh_t delta = (wh_t)(reg - prev);     /* correct across the roll-over */
            prev = reg;
            total_wh += delta;
            if (hour >= t.peak_start && hour < t.peak_end) peak_wh += delta;
        }
    }
    printf("register rolled over during the month: %s\n", rolled ? "yes" : "no");
    printf("month total: %llu Wh (%llu kWh), peak: %llu Wh\n\n", total_wh, total_wh / 1000, peak_wh);

    paise_t energy = energy_charge(total_wh, &t);
    paise_t premium = peak_premium(energy, peak_wh, total_wh, t.peak_pct);
    print_money("fixed charge:", t.fixed);
    print_money("energy charge (slabs):", energy);
    print_money("peak premium:", premium);
    print_money("TOTAL:", t.fixed + energy + premium);

    printf("\n--- sizes and largest values ---\n");
    printf("sizeof(wh_t)=%zu sizeof(whsum_t)=%zu sizeof(paise_t)=%zu\n", sizeof(wh_t), sizeof(whsum_t), sizeof(paise_t));
    printf("old 16-bit register max: 65535 Wh = 65 kWh (a house exceeds this in days)\n");
    unsigned long long house_year = 10ULL * 1000ULL * 8760ULL;      /* 10 kW for 8760 h */
    unsigned long long district_year = house_year * 800000ULL;
    printf("largest house per year : %llu Wh (fits unsigned 32-bit max 4294967295: %s)\n", house_year, house_year <= 4294967295ULL ? "yes" : "no");
    printf("largest district / yr  : %llu Wh (fits unsigned 32-bit: %s, unsigned long long: yes)\n", district_year, district_year <= 4294967295ULL ? "yes" : "no");
    printf("readings per house per year: %d\n", 4 * 8760);
    return 0;
}
