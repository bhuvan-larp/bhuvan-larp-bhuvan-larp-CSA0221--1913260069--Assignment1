/* RUN-TIME error: integer division by zero (units / households with households = 0) */
#include <stdio.h>
int main(int argc, char **argv)
{
    (void)argv;
    int households = argc - 1; /* 0 when run without arguments */
    printf("average = %d\n", 1000 / households);
    return 0;
}
