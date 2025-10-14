
#include <stdio.h>

int strlen(const char *s) {
	size_t len = 0;
	while (s[len]) {
		len++;
	}
	return len;
}

int strcmp(const char *s1, const char *s2) {
	size_t i = 0;
	while (s1[i] && s2[i]) {
		if (s1[i] != s2[i]) {
			return (unsigned char)s1[i] - (unsigned char)s2[i];
		}
		i++;
	}
	return (unsigned char)s1[i] - (unsigned char)s2[i];
}

int main(int argc, char *argv[]) {
	// 1.
	char s[101];
	if (scanf("%100s", s) != 1) {
		return 1;
	}

	// 2.
	printf("Delka: %zu\n", strlen(s));


	// 3.
	int letters = 0, digits = 0;
	for (size_t i = 0; s[i]; ++i) {
		unsigned char c = (unsigned char)s[i];
		if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
			letters++;
		} else if (c >= '0' && c <= '9') {
			digits++;
		}
	}
	printf("Pismen: %d, Cislic: %d\n", letters, digits);

	// 4.
	for (size_t i = 0; s[i]; ++i) {
		unsigned char c = (unsigned char)s[i];
		if (c >= 'a' && c <= 'z') {
			s[i] = c - ('a' - 'A');
		} else if (c >= 'A' && c <= 'Z') {
			s[i] = c + ('a' - 'A');
		}
	}
	printf("Prehozene: %s\n", s);

	// 5.
	printf("Zadejte znak: ");
	char ch;
	if (scanf(" %c", &ch) != 1) {
		return 1;
	}
	for (size_t i = 0; s[i]; ++i) {
		if (s[i] == ch) {
			s[i] = '-';
		}
	}
	printf("Po nahrade: %s\n", s);

	// 6.
	char t[101];
	if (scanf("%100s", t) != 1) {
		return 1;
	}
	int cmp = strcmp(s, t);
	if (cmp < 0) {
		printf("Retezec '%s' je mensi nez '%s'\n", s, t);
	} else if (cmp > 0) {
		printf("Retezec '%s' je vetsi nez '%s'\n", s, t);
	} else {
		printf("Retezce jsou stejne: '%s' == '%s'\n", s, t);
	}

	// 7.
	printf("Zadejte radek textu: ");
	int count = 0;
	int c;
	getchar();
	while ((c = getchar()) != '\n' && c != EOF) {
		count++;
	}
	printf("Pocet znaku v radku: %d\n", count);

	// 8.
	if (argc > 1) {
		int max_len = strlen(argv[1]);
		char *longest = argv[1];
		for (int i = 2; i < argc; ++i) {
			int len = strlen(argv[i]);
			if (len > max_len) {
				max_len = len;
				longest = argv[i];
			}
		}
		printf("Nejdelsi argument: %s\n", longest);
	} else {
		printf("Zadne argumenty programu.\n");
	}

	return 0;
}