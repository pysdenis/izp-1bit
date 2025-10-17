// keyfilter.c
// Project 1 – Text processing (virtual keyboard)
// Build: gcc -std=c11 -Wall -Wextra -Werror keyfilter.c -o keyfilter
//
// Program reads a list of addresses from stdin (one per line).
// It prints which keys are enabled for a given prefix (argv[1]).

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_BUFFER_SIZE 2048

#define TRUE 1
#define FALSE 0
typedef int bool;

/** Result while reading stdin. */
typedef struct {
  unsigned long matchCount;           /* number of matching addresses   */
  bool exactMatch;                    /* exact == prefix exists         */
  char nextChars[256];                /* set of next allowed characters */
  char singleMatch[LINE_BUFFER_SIZE]; /* the only match (if 1)     */
} Result;

/**
 * Initialize Result structure with zero values.
 * @param result
 */
void initResult(Result* result) {
  result->matchCount = 0;
  result->exactMatch = FALSE;
  for (int i = 0; i < 256; i++) {
    result->nextChars[i] = 0;
  };
  result->singleMatch[0] = '\0';
}

/**
 * Print error to stderr and exit with error code.
 * @param message
 * @param code
 */
void printErrorAndExit(const char* message, int code) {
  fprintf(stderr, "Error: %s\n", message);
  exit(code);
}

/**
 * Converts all characters in the string to upper case.
 * @param string
 */
void toUpperASCII(char* string) {
  while (*string) {
    if (*string >= 'a' && *string <= 'z') {
      *string = *string - 32;
    }
    string++;
  }
}

/**
 * Check if string starts with the prefix.
 * @param text
 * @param prefix
 * @return TRUE or FALSE
 */
bool startsWith(const char* text, const char* prefix) {
  size_t i = 0;
  while (prefix[i] != '\0') {
    if (text[i] == '\0' || text[i] != prefix[i]) return FALSE;
    i++;
  }
  return TRUE;
}

/**
 * Update result for one matching line.
 * @param result
 * @param upperLine
 * @param lineLen
 * @param prefixLen
 */
void updateResult(Result* result, const char* upperLine, size_t lineLen,
                  size_t prefixLen) {
  result->matchCount++;
  if (lineLen == prefixLen) {
    result->exactMatch = TRUE;
  } else if (lineLen > prefixLen) {
    result->nextChars[(unsigned char)upperLine[prefixLen]] = 1;
  }
  if (result->matchCount == 1) {
    strcpy(result->singleMatch, upperLine);
  }
}

/**
 * Build enabled characters string from charTable.
 * @param charTable
 * @param output Output string to store enabled characters.
 */
void buildEnableChars(const char charTable[256], char output[257]) {
  size_t k = 0;
  for (int i = 0; i < 256; i++)
    if (charTable[i]) output[k++] = (char)i;
  output[k] = '\0';
}

/**
 * Read one line from stdin into buffer.
 * @param buffer
 * @param maxLen Maximum length of buffer.
 * @return TRUE if a line was read, FALSE on EOF before any characters.
 */
bool readLine(char* buffer, size_t maxLen) {
  int ch;
  size_t i = 0;

  while ((ch = getchar()) != EOF) {
    if (ch == '\r') continue;
    if (ch == '\n') break;

    if (i + 1 < maxLen) {
      buffer[i++] = (char)ch;
    } else {
      printErrorAndExit("input line too long", 2);
    }
  }

  buffer[i] = '\0';

  if (ch == EOF && i == 0) return FALSE;

  return TRUE;
}

/**
 * Processes input lines.
 * @param prefixUpper
 * @param prefixLen
 * @param res
 */
void processInput(const char* prefixUpper, size_t prefixLen, Result* res) {
  char line[LINE_BUFFER_SIZE];
  char up[LINE_BUFFER_SIZE];

  while (readLine(line, LINE_BUFFER_SIZE)) {
    if (line[0] == '\0') continue;  // skip empty lines

    strcpy(up, line);
    toUpperASCII(up);

    if (!startsWith(up, prefixUpper)) continue;

    updateResult(res, up, strlen(up), prefixLen);
  }
}

/**
 * Print the final from the result.
 * @param r
 * @param prefixUpper
 * @return 0 on success.
 */
int showResult(const Result* r, const char* prefixUpper) {
  if (r->matchCount == 0) {
    printf("Not found\n");
    return 0;
  }
  if (r->matchCount == 1) {
    printf("Found: %s\n", r->singleMatch);
    return 0;
  }
  if (r->exactMatch) {
    printf("Found: %s\n", prefixUpper);
  }
  char enabled[257];
  buildEnableChars(r->nextChars, enabled);
  printf("Enable: %s\n", enabled);
  return 0;
}

int main(int argc, char* argv[]) {
  if (argc > 2) {
    printErrorAndExit("too many arguments. Usage: ./keyfilter [PREFIX]",
                      EXIT_FAILURE);
  }

  char prefixUpper[LINE_BUFFER_SIZE] = "";
  if (argc == 2 && argv[1] != NULL) {
    strcpy(prefixUpper, argv[1]);
  }

  toUpperASCII(prefixUpper);
  const size_t prefixLen = strlen(prefixUpper);

  Result res;
  initResult(&res);

  processInput(prefixUpper, prefixLen, &res);

  showResult(&res, prefixUpper);

  return EXIT_SUCCESS;
}
