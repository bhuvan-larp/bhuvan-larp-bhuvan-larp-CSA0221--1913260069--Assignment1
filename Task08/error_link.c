/* LINK-TIME error: function is declared but never defined */
#include <stdio.h>
long compute_bill(int units);
int main(void)
{
    printf("%ld\n", compute_bill(150));
    return 0;
}
