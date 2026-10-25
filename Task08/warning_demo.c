/* Warnings shown by gcc -Wall -Wextra, and how each is fixed (see comments). */
#include <stdio.h>
int main(void)
{
    int unused;            /* -Wunused-variable: fix by deleting it */
    long p = 16250L;
    printf("%d\n", p);     /* -Wformat: %d with a long. Fix: use %ld */
    return 0;
}
