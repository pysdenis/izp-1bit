#include <stdio.h>

/*int isChar(const char c) {
	return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

int arrayIsMadeOutOfChars(const char *array) {
	for (int i = 0; array[i] != '\0'; i++) {
		char c = array[i];
		if (!isChar(c)) {
			printf("Jmeno obsahuje neplatny znak:");
			return 1;
		}
	}
	return 0;
}*/

int my_max(int a, int b) {
	return a > b ? a : b;
}

int my_crazy_min(int a, int b) {
	return a < b ? a : b;
}

unsigned int my_strlen(const char *array) {
	unsigned int len = 0;
	while (array[len]) {
		len++;
	}
	return len;
}

int my_strchr(const char str[], const char ch) {
	for (int i = 0; str[i] != '\0'; i++) {
		char c = str[i];
		if (c == ch) {
			return 0;
		}
	}
	return -1;
}

int my_strrchr(const char str[], const char ch) {
	unsigned int length = my_strlen(str);
	for (int i = length; i != 0; i--) {
		char c = str[i];
		if (c == ch) {
			return 0;
		}
	}
	return -1;
}

char *hodiny2str(int hodin) {
	if (hodin % 10 == 1 && hodin % 100 != 11) {
		return "hodina";
	} else if (hodin % 10 >= 2 && hodin % 10 <= 4 && (hodin % 100 < 10 || hodin % 100 >= 20)) {
		return "hodiny";
	} else {
		return "hodin";
	}
}

char *minuta2str(int minut) {
	if (minut % 10 == 1 && minut % 100 != 11) {
		return "minuta";
	} else if (minut % 10 >= 2 && minut % 10 <= 4 && (minut % 100 < 10 || minut % 100 >= 20)) {
		return "minuty";
	} else {
		return "minut";
	}
}

int findCommonDigits(int arr1[], int arr2[], int size) {
	int common[10] = {0};
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (arr1[i] == arr2[j]) {
				common[arr1[i]] = 1;
			}
		}
	}
	int count = 0;
	for (int i = 0; i < 10; i++) {
		if (common[i]) {
			count++;
		}
	}
	return count;
}

int main() {
	/*char firstName[50];
	char lastName[50];

	printf("Zadejte jmeno: ");
	if (scanf("%49s", firstName) != 1) {
		return 1;
	}

	printf("Zadejte prijmeni: ");
	if (scanf("%49s", lastName) != 1) {
		return 1;
	}

	if (arrayIsMadeOutOfChars(firstName) || arrayIsMadeOutOfChars(lastName)) {
		return 1;
	}

	printf("Jmeno a prijmeni jsou platne.\n");*/

	printf("Max z 5 a 10 je: %d\n", my_max(5, 10));
	printf("Min z 5 a 10 je: %d\n", my_crazy_min(5, 10));
	printf("Delka retezce 'Ahoj' je: %u\n", my_strlen("Ahoj"));
	if (my_strchr("Ahoj", 'o') == -1) {
		printf("Znak nenalezen\n");
	}
	if (my_strrchr("Ahoj", 'o') == -1) {
		printf("Znak nenalezen\n");
	}
	printf("1 %s, 2 %s, 5 %s\n", hodiny2str(1), hodiny2str(2), hodiny2str(5));
	printf("1 %s, 2 %s, 5 %s\n", minuta2str(1), minuta2str(2), minuta2str(5));

	int arr1[5];
	int arr2[5];
	printf("Zadejte prvni pole (5 cislic): ");
	for (int i = 0; i < 5; i++) {
		if (scanf("%1d", &arr1[i]) != 1) {
			return 1;
		}
	}
	printf("Zadejte druhe pole (5 cislic): ");
	for (int i = 0; i < 5; i++) {
		if (scanf("%1d", &arr2[i]) != 1) {
			return 1;
		}
	}
	int count = findCommonDigits(arr1, arr2, 5);
	printf("Pocet spolecnych cislic: %d\n", count);
	

	return 0;
}

