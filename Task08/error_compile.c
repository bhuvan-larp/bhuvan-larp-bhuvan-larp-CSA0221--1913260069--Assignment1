/* COMPILE-TIME error: missing semicolon and an undeclared variable */
#include <stdio.h>
int main(void)
{
    int x = 5
    printf("%d\n", y);
    return 0;
}
