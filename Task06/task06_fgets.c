/* Task 6 alternative: read each line with fgets, then convert with sscanf.
 * A bad line is simply discarded (the whole line is already consumed), so
 * recovery is simple; time per line is O(L) and the buffer is a fixed 128 bytes. */
#include <stdio.h>

int main(void)
{
    char buf[128];
    unsigned int house;
    int prev, curr, n = 0;
    while (fgets(buf, sizeof buf, stdin) != NULL) {
        n++;
        if (sscanf(buf, "%u %d %d", &house, &prev, &curr) != 3) {
            printf("line %d: invalid input, skipped\n", n);
            continue;
        }
        unsigned int units = (unsigned short)(curr - prev);
        printf("house %u: %u units\n", house, units);
    }
    return 0;
}
