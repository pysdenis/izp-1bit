// keyfilter.c
// Program: Emulace povolování kláves na virtuální klávesnici navigace
// Překlad: gcc -std=c11 -Wall -Wextra -Werror keyfilter.c -o keyfilter
// Vstup: seznam adres po řádcích ze stdin
// Výstup: VŽDY na stderr (Found / Enable / Not found)
// Pozn.: case-insensitive porovnávání; dynamické čtení, dlouhé řádky, ošetřené chyby

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#define INITIAL_CAP 64
#define LINE_CHUNK 128

typedef struct {
    char **items;
    size_t size;
    size_t cap;
} StrVec;

static void die(const char *msg) {
    // Chybová hlášení mají být anglicky
    if (msg) fprintf(stderr, "Error: %s\n", msg);
    else     fprintf(stderr, "Error\n");
    exit(2);
}

static void *xrealloc(void *ptr, size_t newsize) {
    void *p = realloc(ptr, newsize);
    if (!p) die("Out of memory");
    return p;
}

static char *xstrdup(const char *s) {
    size_t n = strlen(s) + 1;
    char *p = (char *)malloc(n);
    if (!p) die("Out of memory");
    memcpy(p, s, n);
    return p;
}

static void vec_init(StrVec *v) {
    v->cap = INITIAL_CAP;
    v->size = 0;
    v->items = (char **)malloc(v->cap * sizeof(char *));
    if (!v->items) die("Out of memory");
}

static void vec_push(StrVec *v, const char *s) {
    if (v->size == v->cap) {
        v->cap *= 2;
        v->items = (char **)xrealloc(v->items, v->cap * sizeof(char *));
    }
    v->items[v->size++] = xstrdup(s);
}

static void vec_free(StrVec *v) {
    for (size_t i = 0; i < v->size; ++i) free(v->items[i]);
    free(v->items);
}

// Trim newline (\n, \r\n) at end
static void rstrip_newline(char *s) {
    size_t n = strlen(s);
    while (n > 0 && (s[n-1] == '\n' || s[n-1] == '\r')) {
        s[--n] = '\0';
    }
}

// Case-insensitive startswith
static int ci_starts_with(const char *str, const char *prefix) {
    for (; *prefix; ++prefix, ++str) {
        unsigned char a = (unsigned char)*str;
        unsigned char b = (unsigned char)*prefix;
        if (a == '\0') return 0;
        if (tolower(a) != tolower(b)) return 0;
    }
    return 1;
}

// Case-insensitive equality
static int ci_equals(const char *a, const char *b) {
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) return 0;
        ++a; ++b;
    }
    return *a == '\0' && *b == '\0';
}

// Uppercase copy (ASCII), returns newly allocated
static char *to_upper_copy(const char *s) {
    size_t n = strlen(s);
    char *o = (char *)malloc(n + 1);
    if (!o) die("Out of memory");
    for (size_t i = 0; i < n; ++i) {
        o[i] = (char)toupper((unsigned char)s[i]);
    }
    o[n] = '\0';
    return o;
}

// Read one line of arbitrary length (no POSIX getline dependency)
static char *read_line(FILE *f) {
    char *buf = NULL;
    size_t used = 0;
    size_t cap = 0;

    for (;;) {
        if (used + 1 >= cap) {
            size_t newcap = cap ? cap + LINE_CHUNK : LINE_CHUNK;
            char *nbuf = (char *)realloc(buf, newcap);
            if (!nbuf) {
                free(buf);
                die("Out of memory");
            }
            buf = nbuf;
            cap = newcap;
        }

        int c = fgetc(f);
        if (c == EOF) {
            if (ferror(f)) {
                free(buf);
                die("Input error");
            }
            if (used == 0) { free(buf); return NULL; } // no more lines
            break; // return last line without newline
        }

        buf[used++] = (char)c;
        if (c == '\n') break;
    }
    buf[used] = '\0';
    return buf;
}

// qsort comparator for chars
static int cmp_char(const void *a, const void *b) {
    unsigned char ca = *(const unsigned char *)a;
    unsigned char cb = *(const unsigned char *)b;
    if (ca < cb) return -1;
    if (ca > cb) return 1;
    return 0;
}

int main(int argc, char **argv) {
    // Read prefix argument (may be empty)
    const char *prefix_in = (argc >= 2) ? argv[1] : "";
    char *prefix_upper = to_upper_copy(prefix_in);
    size_t prefix_len = strlen(prefix_in);

    // Read addresses from stdin
    StrVec addresses;
    vec_init(&addresses);

    for (;;) {
        char *line = read_line(stdin);
        if (!line) break;
        rstrip_newline(line);

        // Ignore completely empty lines
        // (Optional: if you want to keep them, comment the following block)
        if (line[0] == '\0') {
            free(line);
            continue;
        }

        vec_push(&addresses, line);
        free(line);
    }

    if (addresses.size == 0) {
        // No data -> treat as unexpected input
        free(prefix_upper);
        vec_free(&addresses);
        die("No addresses on input");
    }

    // Collect matching addresses & track exact matches
    StrVec matches;
    vec_init(&matches);

    int have_exact = 0;
    StrVec exacts;
    vec_init(&exacts);

    for (size_t i = 0; i < addresses.size; ++i) {
        const char *addr = addresses.items[i];
        if (ci_starts_with(addr, prefix_in)) {
            vec_push(&matches, addr);
            if (ci_equals(addr, prefix_in)) {
                have_exact = 1;
                vec_push(&exacts, addr);
            }
        }
    }

    if (matches.size == 0) {
        fprintf(stderr, "Not found\n");
        free(prefix_upper);
        vec_free(&matches);
        vec_free(&exacts);
        vec_free(&addresses);
        return 0;
    }

    // If exactly one match and prefix is not equal to it -> Found only
    if (matches.size == 1 && !ci_equals(matches.items[0], prefix_in)) {
        char *up = to_upper_copy(matches.items[0]);
        fprintf(stderr, "Found: %s\n", up);
        free(up);
        free(prefix_upper);
        vec_free(&matches);
        vec_free(&exacts);
        vec_free(&addresses);
        return 0;
    }

    // If at least one exact match, print Found for the (first) exact match (spec ukazuje jediný řádek)
    if (have_exact) {
        // Vytiskneme Found: pro první exact (případ duplikátů ošetříme jedním výpisem)
        char *up = to_upper_copy(exacts.items[0]);
        fprintf(stderr, "Found: %s\n", up);
        free(up);
    }

    // We need to compute ENABLE chars from all matches that are STRICTLY longer than prefix
    // For each such address, take next char at position prefix_len (uppercased)
    // Create set of unique chars, then sort and print.
    unsigned char present[256] = {0};
    size_t count_chars = 0;

    for (size_t i = 0; i < matches.size; ++i) {
        const char *addr = matches.items[i];
        size_t alen = strlen(addr);
        if (alen > prefix_len) {
            unsigned char ch = (unsigned char)addr[prefix_len];
            ch = (unsigned char)toupper(ch);
            if (!present[ch]) {
                present[ch] = 1;
                ++count_chars;
            }
        }
    }

    if (count_chars == 0) {
        // No way to continue typing beyond prefix (all matches equal to prefix)
        // If we already printed Found (have_exact), nothing else is needed.
        // Otherwise (theoreticky by nastala shoda bez exactu, ale s nulovým next char) – to nenastane.
        free(prefix_upper);
        vec_free(&matches);
        vec_free(&exacts);
        vec_free(&addresses);
        return 0;
    }

    char *chars = (char *)malloc(count_chars);
    if (!chars) die("Out of memory");

    size_t idx = 0;
    for (int c = 0; c < 256; ++c) {
        if (present[c]) {
            chars[idx++] = (char)c;
        }
    }

    qsort(chars, count_chars, sizeof(char), cmp_char);

    // Print as concatenated string (no spaces)
    fprintf(stderr, "Enable: ");
    for (size_t i = 0; i < count_chars; ++i) fputc(chars[i], stderr);
    fputc('\n', stderr);

    free(chars);
    free(prefix_upper);
    vec_free(&matches);
    vec_free(&exacts);
    vec_free(&addresses);
    return 0;
}