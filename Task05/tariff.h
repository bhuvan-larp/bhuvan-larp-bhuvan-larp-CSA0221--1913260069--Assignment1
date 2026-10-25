#ifndef TARIFF_H
#define TARIFF_H

/* Single place for every tariff value: a revision is made here only. */
#define SLAB1_UNITS 100
#define SLAB2_UNITS 200
#define RATE1_PAISE 0
#define RATE2_PAISE 225
#define RATE3_PAISE 450
#define FIXED_CHARGE_PAISE 5000L

long compute_bill(int units);

#endif /* TARIFF_H */
