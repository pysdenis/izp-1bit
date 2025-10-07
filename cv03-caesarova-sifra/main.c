#include <stdio.h>

#define POSUN 3

void zasifruj(char* text, int posun) {
	while (*text) {
		if (*text >= 'a' && *text <= 'z') {
			*text = ((*text - 'a' + posun) % 26) + 'a';
		}
		else if (*text >= 'A' && *text <= 'Z') {
			*text = ((*text - 'A' + posun) % 26) + 'A';
		}
		text++;
	}
}

void desifruj(char* text, int posun) {
	while (*text) {
		if (*text >= 'a' && *text <= 'z') {
			*text = ((*text - 'a' - posun + 26) % 26) + 'a';
		}
		else if (*text >= 'A' && *text <= 'Z') {
			*text = ((*text - 'A' - posun + 26) % 26) + 'A';
		}
		text++;
	}
}

int main(void) {
	char text[101];
	printf("Zadejte text (max 100 znaku): ");
	scanf("%100[^\n]", text);

	printf("Puvodni text: %s\n", text);
	zasifruj(text, POSUN);
	printf("Zasifrovany text: %s\n", text);
	desifruj(text, POSUN);
	printf("Desifrovany text: %s\n", text);
	return 0;
}
