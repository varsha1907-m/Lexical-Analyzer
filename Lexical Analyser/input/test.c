
#include <stdio.h>

// Testing different tokens
int main()
{
    int a = 10, b = 20;
    float result = 25.5;

    char ch = 'A';
    char name[] = "Varsha";

    if (a < b && b != 0)
    {
        result += a + b;
        printf("Name: %s\n", name);
    }
    else
    {
        printf("Invalid\n");
    }

    /* Multi-line
       comment */

    for (int i = 0; i < 5; i++)
    {
        printf("%d ", i);
    }

    return 0;
}
