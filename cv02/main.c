#include <stdio.h>
#include <math.h>

void quadraticEquation(double a, double b, double c) {
	double d = b * b - 4 * a * c;

	if (d > 0) {
		double x1 = (-b + sqrt(d)) / (2 * a);
		double x2 = (-b - sqrt(d)) / (2 * a);
		printf("Rovnice ma dve reseni:\n");
		printf("x1 = %f\n", x1);
		printf("x2 = %f\n", x2);
	} else if (d == 0) {
		double x = -b / (2 * a);
		printf("Rovnice ma jedno reseni:\n");
		printf("x = %f\n", x);
	} else {
		double real = -b / (2 * a);
		double imag = sqrt(-d) / (2 * a);
		printf("Rovnice ma komplexni reseni:\n");
		printf("x1 = %f + %fi\n", real, imag);
		printf("x2 = %f - %fi\n", real, imag);
	}
}

void isChar(char input) {
	if ((input >= 'A' && input <= 'Z') || (input >= 'a' && input <= 'z'))
	{
		printf("Input je pismeno - %c", input);
	} else {
		printf("Input není pismeno");
	}
}

void isEven() {
	int l;
	printf("Kolik chcete nacist cisel: ");
	scanf("%d", &l);

	for (int i = 0; i < l; i++) {
		int number;
		printf("\n Zadej cislo: ");
		scanf("%d", &number);
		number % 2 == 0 ? printf("Cislo je sude") : printf("Cislo je liche");
	}
}

int getFact(int n) {
	if (n == 0) {
		return 1;
	} else {
		return n * getFact(n - 1);
	}
}

void printReverseArray(int arr[], int size) {
	for (int i = size - 1; i >= 0; i--) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}

void findMaxInArray(int arr[], int size) {
	int max = arr[0];
	for (int i = 1; i < size; i++) {
		if (arr[i] > max) {
			max = arr[i];
		}
	}
	printf("Nejvetsi prvek pole je: %d\n", max);
}

void sumAndAverage(int arr[], int size) {
	int sum = 0;
	for (int i = 0; i < size; i++) {
		sum += arr[i];
	}
	double average = (double)sum / size;
	printf("Soucet prvku pole je: %d\n", sum);
	printf("Prumer prvku pole je: %.2f\n", average);
}

int main(void) {
	/* double a, b, c;
	printf("Zadej koeficienty kvadraticke rovnice (a, b, c): ");
	scanf("%lf %lf %lf", &a, &b, &c);

	quadraticEquation(a, b, c);*/

	/*char input;
	printf("Zadej znak: ");
	scanf(" %c", &input);
	isChar(input);
	isEven();
	int fact;
	printf("Zadej cislo pro vypocet faktorialu: ");
	scanf("%d", &fact);
	getFact(fact);
	*/
	int arr[5];
	for (int i = 0; i < 5; i++) {
		printf("Zadej %d. prvek pole: ", i + 1);
		scanf("%d", &arr[i]);
	}
	printReverseArray(arr, 5);
	findMaxInArray(arr, 5);
	sumAndAverage(arr, 5);

	return 0;
}