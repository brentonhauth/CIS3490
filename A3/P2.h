#ifndef _P2_H_
/**
 * @brief CIS*3490 A3 - General code used in Q2.1, Q2.2 and Q2.3.
 * @author Brenton Hauth (1229237)
 * @date February 15, 2023
 */
#define _P2_H_

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define TEST_FILE       "data_A3_Q2.txt"
#define BUFSIZE         (80)
#define PATTERN_MAX     (128)
#define LINE_COUNT      (44049)
#define IS_UPPER(c)     ((c) >= 'A' && (c) <= 'Z')
#define IS_LOWER(c)     ((c) >= 'a' && (c) <= 'z')
#define TABLE_SIZE      (26+26+1)    /* 2*26 in alphabet (upper+lower) + 1 for others */
#define PREV_CHAR(s, i) ((i) > 0 ? (s)[(i) - 1] : '\0') /* get previous char */
#define MAX(a, b)       ((a) > (b) ? (a) : (b))    /* Return max of 2 numbers */

/* to have booleans in in the code, sometimes they're defined */
#ifndef TRUE
#define TRUE    (1)
#endif

#ifndef FALSE
#define FALSE   (0)
#endif

typedef char bool_t; /* for using boolean values */
typedef clock_t timestamp_t;

/**
 * Start a timer
 * @param ts where to write the start time
 */
void time_start(timestamp_t* ts) {
    printf("Starting timer...\n");
    *ts = clock();
}

/**
 * Check how much time has elapsed from the timestamp given
 * NOTE: Am rounding the time to a tenth of a milisecond for more precise measurements
 * @param ts the time stamp to check the elapsed time
 */
double time_elapsed(timestamp_t* ts) {
    timestamp_t elapsed = clock() - (*ts);
    double ms = (elapsed / (double)CLOCKS_PER_SEC) * 1000.0;
    printf("\nExecution time = %.1f ms\n", ms);
    return ms;
}


/**
 * Reads the data from a file to be used in the text search
 * @param data where to write the data pointer
 */
int read_data(char** data) {
    FILE* file = fopen(TEST_FILE, "r"); // open file
    char line[BUFSIZE];
    char* a = NULL;

    // Create space in memory
    char* _data = (char*)malloc(sizeof(char) * LINE_COUNT * BUFSIZE);

    int i = 0;
    while ((a = fgets(line, BUFSIZE, file)) != NULL) {
        size_t len = strlen(line);
        memcpy(_data + i, line, len); // copy over data
        i += len; // add on the length
    }
    fclose(file);

    if (i < (LINE_COUNT * BUFSIZE)) {
        // Resize the data to proper size
        _data = (char*)realloc(_data, sizeof(char) * i);
    }
    // Return values out
    *data = _data;
    return i;
}

/**
 * Prompts the user to enter a pattern to be used in the search
 * @param pattern where to write the pattern
 */
int read_pattern(char pattern[PATTERN_MAX]) {
    printf("Enter a pattern: ");
    if (scanf("%s", pattern) < 0) {
        pattern[0] = '\0';
        return -1;
    }
    int psize = strlen(pattern);
    for (int i = 0; i < psize; ++i) {
        if (!IS_LOWER(pattern[i]) && !IS_UPPER(pattern[i])) {
            // If it contains a non lowercase letter
            // or a non uppercase letter, then it's invalid
            printf("Invalid pattern.\n");
            pattern[0] = '\0';
            return -1;
        }
    }
    return psize;
}

/**
 * Converts a character to an index.
 * maps: a-z to 0-25,  A-Z to 26-51,  anything else to 52
 * @param c the char to convert to an index
 * @returns the index in a table
 */
int char_to_index(const char c) {
    if (IS_LOWER(c)) {
        return c - 'a';
    } else if (IS_UPPER(c)) {
        return (c - 'A') + 26;
    } else {
        return TABLE_SIZE - 1; // last index in table
    }
}


/**
 * Useful for debugging bad symbol table.
 * @param table the table to print out.
 */
void print_table(int table[TABLE_SIZE]) {
    for (int l = 'a'; l <= 'z'; ++l) {
        printf(" %c ", l);
    }
    for (int l = 'A'; l <= 'Z'; ++l) {
        printf(" %c ", l);
    }
    printf(" ?\n");
    for (int i = 0; i < TABLE_SIZE; ++i) {
        printf(" %d ", table[i]);
    }
    printf("\n");
}


/**
 * Creates a bad symbol table for the algorithm
 * @param pattern the pattern to convert into a table
 * @param table the place to write the table
 */
void create_shift_table(const char* pattern, int table[TABLE_SIZE]) {
    int psize = strlen(pattern);
    for (int i = 0; i < TABLE_SIZE; ++i) {
        // set default value to size of the pattern
        table[i] = psize;
    }
    for (int i = 0; i < (psize - 1); ++i) {
        int x = char_to_index(pattern[i]);
        table[x] = (psize - i - 1);
    }
}

/**
 * Useful for debugging a good suffix table
 * @param table the good suffix table
 * @param tsize the size of the table
 */
void print_good_suffix(const int* table, const int tsize) {
    for (int i = 0; i < tsize; ++i) {
        printf("k = %d  |  %d\n", i + 1, table[i]);
    }
}

/**
 * Creates a good suffix table used in the Boyer-Moore algorithm.
 * @param pattern the search pattern
 * @param table where to write the good suffix table
 * @returns the size of the good suffix table
 */
int create_good_suffix_table(const char* pattern, int** table) {
    int psize = strlen(pattern);
    if (psize <= 1) {
        // Safety incase the pattern is 1 character or less
        *table = NULL;
        return 0;
    }

    // set the size of the table since it's based on the size
    // of prefix which is 1 less than the table
    int tsize = (psize - 1);

    int* _table = (int*)malloc(sizeof(int) * tsize); // create space in memory

    for (int i = 0; i < tsize; ++i) {
        // set default value for table
        _table[i] = tsize;
    }

    const char* suf = NULL;
    for (int k = 1; k < psize; ++k) {
        suf = &pattern[psize - k]; // slice suffix
        char sufprev = PREV_CHAR(pattern, psize - k); // get character before suffix
        bool_t did_set = FALSE;
        for (int l = psize - k - 1; l >= 0; --l) {
            if (!strncmp(suf, &pattern[l], k)) {
                // Found occurence of suffix
                char occprev = PREV_CHAR(pattern, l); // get char before occurence
                if (sufprev != occprev) {
                    // If the previous characters didn't match then we're golden
                    _table[k - 1] = (psize - k) - l;
                    did_set = TRUE;
                    break; // Done, found a suffix
                }
            }
        }
        if (!did_set) {
            // We need a prefix
            did_set = FALSE;
            for (int l = 1; l < k; ++l) {
                // char* lpre = &pattern[0]; // grab the prefix
                const char* lsuf = &pattern[psize - l]; // grab the suffix of size `l`

                if (!strncmp(pattern, lsuf, l)) {
                    // matched new prefix to suffix
                    // "k - 1" because index starts at 0
                    _table[k - 1] = psize - l;
                    did_set = TRUE; // mark set as true
                    break; // found a valid prefix so we're done
                }
            }

            if (!did_set) {
                _table[k - 1] = tsize; // safety
            }
        }
    }

    // Return values out
    *table = _table;
    return tsize;
}

#endif
