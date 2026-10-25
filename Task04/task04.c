/* Task 4 - Using macros safely. Run  gcc -E task04.c  to see the expansion. */
#include <stdio.h>

#define RUPEES_TO_PAISE(r) ((r) * 100L)

/* WRONG versions (kept to demonstrate the pitfalls) */
#define BAD_SQUARE(x) x * x
#define BAD_MAX(a, b) ((a) > (b) ? (a) : (b)) /* parenthesised, but evaluates an argument twice */

/* Correct macro */
#define SQUARE(x) ((x) * (x))

/* Safe alternative: static inline functions evaluate each argument once */
static inline int max_fn(int a, int b) { return a > b ? a : b; }
static inline int square_fn(int x) { return x * x; }

int main(void)
{
    int a = 4;
    printf("BAD_SQUARE(a+1)  = %d  (expected 25; expands to a+1 * a+1 = 4+1*4+1)\n", BAD_SQUARE(a + 1));
    printf("SQUARE(a+1)      = %d  (correct)\n", SQUARE(a + 1));

    int i = 5, j = 3;
    int m = BAD_MAX(i++, j);
    printf("BAD_MAX(i++, j)  = %d, i is now %d  (i++ ran twice, expected i = 6)\n", m, i);
    i = 5;
    m = max_fn(i++, j);
    printf("max_fn(i++, j)   = %d, i is now %d  (argument evaluated once)\n", m, i);

    printf("square_fn(a+1)   = %d\n", square_fn(a + 1));
    printf("RUPEES_TO_PAISE(162) = %ld\n", RUPEES_TO_PAISE(162));
    return 0;
}
