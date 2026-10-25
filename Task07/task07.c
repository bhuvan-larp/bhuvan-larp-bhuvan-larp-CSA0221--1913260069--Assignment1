/* Task 7 - type conversion and precision. */
#include <stdio.h>

/* Rounding rule used everywhere: round half up to the nearest paise. */
static long round_paise(double rupees) { return (long)(rupees * 100.0 + 0.5); }

int main(void)
{
    printf("--- three unexpected results ---\n");
    printf("1) 7 / 2            = %d     (integer division truncates; 3.5 expected)\n", 7 / 2);
    printf("2) 100 / 3 * 3      = %d    (33 * 3; the 0.33.. is lost)\n", 100 / 3 * 3);
    long big = 5000000000L;
    printf("3) (int)5000000000L = %d (value does not fit in 32 bits)\n", (int)big);
    unsigned int u = 5; int neg = -10;
    /* -Wextra warns here: the int is converted to unsigned, so -10 becomes 4294967286 */
    printf("4) (-10 < 5u)       = %d     (expected 1; -10 is converted to unsigned)\n", (neg < u) ? 1 : 0);
    printf("5) 199 / 100        = %d     (integer division; 1.99 expected)\n\n", 199 / 100);

    printf("--- rounding to paise ---\n");
    printf("round_paise(162.505) = %ld\n", round_paise(162.505));
    printf("round_paise(0.125)   = %ld\n\n", round_paise(0.125));

    printf("--- error growth: float vs integer paise (n bills of Rs 0.10) ---\n");
    int ns[] = {4000, 60000, 800000};
    for (int k = 0; k < 3; k++) {
        float ft = 0.0f; long pt = 0;
        for (int i = 0; i < ns[k]; i++) { ft += 0.10f; pt += 10; }
        double exact = ns[k] * 0.10;
        printf("n=%7d float=%12.2f paise=%12.2f exact=%12.2f float error=%.2f\n",
               ns[k], (double)ft, pt / 100.0, exact, (double)ft - exact);
    }
    return 0;
}
