#include <limits.h>
#include <stdio.h>

void print_helloworld()
{
    // TODO 1. Print 'Hello World' to stdout.
    printf("Hello world");
    return;
}

#define A -4
#define B 2
#define C 3
void print_discriminant()
{
    // TODO 1. Calculate and print discriminant with values defined above to stdout.
    int discriminant = (B * B) - (4 * A * C);
    printf("%d", discriminant);
    return;
}

void read_print_is_even()
{
    // TODO 1. Read a number from stdin and output to stdout whether the read number is even or not.
    int temp;

    printf("Zadej cislo: ");
    scanf("%d", &temp);

    temp % 2 == 0 ? printf("\nCislo je sude") : printf("\nCislo je liche");
    return;
}

void read_print_is_in_interval()
{
    // TODO 1. Read 2 numbers from stdin. These two numbers represent a interval.
    int min, max, temp;

    printf("Zadej min: ");
    scanf("%d", &min);
    printf("\nZadej max: ");
    scanf("%d", &max);
    printf("\nZadej cislo: ");
    scanf("%d", &temp);

    if ((temp >= min) && (temp <= max)) {
        printf("Zadane cislo je v intervalu");
    } else {
        printf("Zadane cislo neni v intervalu");
    }

    // TODO 2. Read third number from stdin. Output to stdout whether the third number is contained in the previously
    // read interval.
    return;
}

void read_print_is_leap_year()
{
    // TODO 1. Read a number representing a year from stdin.
    int year;

    printf("Zadej rok: ");
    scanf("%d", &year);

    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        printf("Rok je prestupny");
    } else {
        printf("Rok neni prestupny");
    }
    return;
}

void read_print_maximum()
{
    // TODO 1. Read 3 numbers from stdin.
    int pole[3], temp = INT_MIN;

    for (int i = 0; i < 3; i++) {
        printf("Zadej %d cislo", pole[i]);
        scanf("%d", (pole + i));
        if (*(pole + i) > temp) {
            temp = *(pole + i);
        }
    }

    printf("%d", temp);

    // TODO 2. Output the largest of read numbers to stdout.
    return;
}

#ifndef TEST_BUILD

int main()
{
    printf("\n--print_helloworld--\n");
    print_helloworld();

    printf("\n--print_discriminant--\n");
    print_discriminant();

    printf("\n--read_print_is_even--\n");
    read_print_is_even();

    printf("\n--read_print_is_in_interval--\n");
    read_print_is_in_interval();

    printf("\n--read_print_is_leap_year--\n");
    read_print_is_leap_year();

    printf("\n--read_print_maximum--\n");
    read_print_maximum();

    return 0;
}

#endif
