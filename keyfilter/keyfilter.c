// keyfilter.c
// Project 1 – Text processing (virtual keyboard)
// Build: gcc -std=c11 -Wall -Wextra -Werror keyfilter.c -o keyfilter
//
// Program reads a list of addresses from stdin (one per line).
// It prints which keys are enabled for a given prefix (argv[1]).
// Case-insensitive (ASCII). No dynamic memory, no external files.
// Output (exactly):
//   Found: S
//   Enable: CHARS
//   Not found
//
// Special case: if the prefix is an exact address AND a prefix of another,
// print two lines: "Found: PREFIX" and "Enable: CHARS".

#include <stdio.h>
#include <stdlib.h>

#define LINE_BUFFER_SIZE 2048  // program knows its limit; >= 100 as required
#define TRUE 1
#define FALSE 0
typedef int bool;

/* ----------------------------- Types ------------------------------ */

/** Aggregated result gathered while reading stdin. */
typedef struct {
  unsigned long matchCount;           /* number of matching addresses   */
  bool exactMatch;                    /* exact == prefix exists         */
  char nextChars[256];                /* set of next allowed characters */
  char singleMatch[LINE_BUFFER_SIZE]; /* the only match (if 1)     */
} Result;

/* ----------------------------- Errors ------------------------------ */

/**
 * Print error to stderr and exit with non-zero code.
 * @param message Error message to print.
 * @param code Exit code (non-zero).
 */
void printErrorAndExit(const char *message, int code) {
  fprintf(stderr, "Error: %s\n", message);
  exit(code);
}

/* ------------------------- String helpers ------------------------- */

/**
 * Calculate length of a string (like strlen).
 * @param str Input string.
 * @return Length of the string.
 */
size_t strLen(const char *str) {
  const char *s = str;
  while (*s) s++;
  return s - str;
}

/**
 * Compare two strings (like strcmp), case-insensitive (ASCII only).
 * @param s1 First string.
 * @param s2 Second string.
 * @return <0 if s1<s2, 0 if s1==s2, >0 if s1>s2
 */
int strCmp(const char *s1, const char *s2) {
  while (*s1 && *s2) {
    char c1 = (*s1 >= 'A' && *s1 <= 'Z') ? (*s1 + 32) : *s1;
    char c2 = (*s2 >= 'A' && *s2 <= 'Z') ? (*s2 + 32) : *s2;
    if (c1 != c2) return c1 - c2;
    s1++;
    s2++;
  }
  return *s1 - *s2;
}

/**
 * Copy string from src to dest (like strcpy).
 * @param dest Destination string (must be large enough).
 * @param src Source string.
 */
void strCpy(char *dest, const char *src) {
  while (*src) {
    *dest++ = *src++;
  }
  *dest = '\0';
}

/**
 * Converts all characters in the string to upper case (ASCII only).
 * @param str Input string to convert (modified in place).
 */
void toUpperASCII(char *str) {
  while (*str) {
    if (*str >= 'a' && *str <= 'z') {
      *str = *str - 32;
    }
    str++;
  }
}

/**
 * Trim trailing newline and carriage return characters from the end of the
 * string.
 * @param s Input string to trim (modified in place).
 */
void trimLineEnd(char *s) {
  size_t n = strLen(s);
  while (n > 0 && (s[n - 1] == '\n' || s[n - 1] == '\r')) {
    s[n - 1] = '\0';
    n--;
  }
}

/**
 * Check if text starts with the given prefix.
 * @param text Input text.
 * @param prefix Prefix to check.
 * @return TRUE if text starts with prefix, FALSE otherwise.
 */
bool startsWith(const char *text, const char *prefix) {
  size_t i = 0;
  while (prefix[i] != '\0') {
    if (text[i] == '\0' || text[i] != prefix[i]) return FALSE;
    i++;
  }
  return TRUE;
}

/* --------------------- Input safety / limits ---------------------- */

/**
 * If fgets filled the buffer and there is no '\n', the input line is longer
 * than we support. Consume the rest of the line and fail.
 */
void guardLineLengthOrDie(char *buf) {
  size_t len = strLen(buf);
  int hasNewline = 0;
  for (size_t i = 0; buf[i] != '\0'; i++) {
    if (buf[i] == '\n') {
      hasNewline = 1;
      break;
    }
  }
  if (!hasNewline && len == LINE_BUFFER_SIZE - 1) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) { /* discard */
    }
    printErrorAndExit("input line too long", 2);
  }
}

/* ------------------------- Result helpers ------------------------- */

/** Initialize Result structure with zero values. */
void initResult(Result *r) {
  r->matchCount = 0;
  r->exactMatch = FALSE;
  for (int i = 0; i < 256; i++) r->nextChars[i] = 0;
  r->singleMatch[0] = '\0';
}

/** Note the character that can follow the prefix. */
void markNextChar(Result *r, unsigned char c) { r->nextChars[c] = 1; }

/** Save the only matching line (uppercased). */
void saveSingleMatch(Result *r, const char *upperLine) {
  strCpy(r->singleMatch, upperLine);
}

/** Update aggregated result for one matching line. */
void updateAggregates(Result *r, const char *upperLine, size_t lineLen,
                      size_t prefixLen) {
  r->matchCount++;
  if (lineLen == prefixLen) {
    r->exactMatch = TRUE;
  } else if (lineLen > prefixLen) {
    markNextChar(r, (unsigned char)upperLine[prefixLen]);
  }
  if (r->matchCount == 1) {
    saveSingleMatch(r, upperLine);
  }
}

/** Build enabled characters string from presence table. */
void buildEnableChars(const char present[256], char output[257]) {
  size_t k = 0;
  for (int i = 0; i < 256; i++)
    if (present[i]) output[k++] = (char)i;
  output[k] = '\0';
}

bool readLine(char *buffer, size_t maxLen) {
  int ch;
  size_t i = 0;
  int tooLong = FALSE;

  while ((ch = getchar()) != EOF) {
    if (ch == '\r') continue;  // skip CR in Windows endings
    if (ch == '\n') break;     // end of line

    if (i + 1 < maxLen) {
      buffer[i++] = (char)ch;
    } else {
      tooLong = TRUE;  // mark too long
    }
  }

  buffer[i] = '\0';

  if (tooLong) {
    // consume rest of line until newline/EOF
    while (ch != '\n' && ch != EOF) {
      ch = getchar();
    }
    printErrorAndExit("input line too long", 2);
  }

  // If EOF reached and no characters read -> no more lines
  if (ch == EOF && i == 0) return FALSE;

  return TRUE;
}

/* -------------------------- Core logic ---------------------------- */

/**
 * Process stdin: read lines, normalize to uppercase, check prefix,
 * and accumulate information into Result.
 */
void processInput(const char *prefixUpper, size_t prefixLen, Result *res) {
  char line[LINE_BUFFER_SIZE];
  char up[LINE_BUFFER_SIZE];

  while (readLine(line, LINE_BUFFER_SIZE)) {
    if (line[0] == '\0') continue;  // skip empty lines

    strCpy(up, line);
    toUpperASCII(up);

    if (!startsWith(up, prefixUpper)) continue;

    updateAggregates(res, up, strLen(up), prefixLen);
  }
}

/**
 * Print the final output exactly as required.
 * Returns 0 (program exit code).
 */
int printFinalOutput(const Result *r, const char *prefixUpper) {
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

/* ----------------------------- Main ------------------------------- */

int main(int argc, char *argv[]) {
  /* Check launch syntax: at most one optional PREFIX argument. */
  if (argc > 2) {
    printErrorAndExit("too many arguments. Usage: ./keyfilter [PREFIX]",
                      EXIT_FAILURE);
  }

  char prefixUpper[LINE_BUFFER_SIZE] = "";
  if (argc == 2 && argv[1] != NULL) {
    strCpy(prefixUpper, argv[1]);
  }

  toUpperASCII(prefixUpper);
  const size_t prefixLen = strLen(prefixUpper);

  Result res;
  initResult(&res);

  processInput(prefixUpper, prefixLen, &res);

  printFinalOutput(&res, prefixUpper);

  return EXIT_SUCCESS;
}
