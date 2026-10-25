/* Task 10 - compare expectations with execution at village, taluk and district scale.
 * Bill for house i uses units = i % 400 (deterministic, covers all three slabs). */
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
    long ns[] = {4000, 60000, 800000};
    printf("%-8s %-16s %-16s %-16s %-12s %-12s\n", "bills", "paise total (Rs)", "float total (Rs)", "double total(Rs)",
           "float diff", "double diff");
    for (int k = 0; k < 3; k++) {
        float ft = 0.0f; double dt = 0.0; long long pt = 0;
        for (long i = 0; i < ns[k]; i++) {
            long p = compute_bill((unsigned)(i % 400));
            pt += p;
            ft += (float)p / 100.0f;
            dt += (double)p / 100.0;
        }
        double exact = (double)pt / 100.0;
        printf("%-8ld %-16.2f %-16.2f %-16.2f %-12.2f %-12.6f\n", ns[k], exact, (double)ft, dt, (double)ft - exact, dt - exact);
        printf("         fits in int? %s   long (this system)? %s   long long? %s\n",
               pt <= INT_MAX ? "yes" : "NO (overflow)",
               pt <= LONG_MAX ? "yes" : "NO",
               pt <= LLONG_MAX ? "yes" : "NO");
    }

    printf("\nsizeof on this system vs minimum guaranteed by the C standard:\n");
    printf("short     : %zu bytes (standard minimum 2)\n", sizeof(short));
    printf("int       : %zu bytes (standard minimum 2)\n", sizeof(int));
    printf("long      : %zu bytes (standard minimum 4)\n", sizeof(long));
    printf("long long : %zu bytes (standard minimum 8)\n", sizeof(long long));
    printf("INT_MAX = %d, LONG_MAX = %ld, LLONG_MAX = %lld\n", INT_MAX, LONG_MAX, LLONG_MAX);

    printf("\nDouble loses integer exactness above 2^53 = 9007199254740992 paise (about Rs 9e13);\n");
    printf("district worst case is about 4e11 paise, so double is exact in value but adds rounding in fractions.\n");
    return 0;
}
