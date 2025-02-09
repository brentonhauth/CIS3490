#ifndef _P1_H_
#define _P1_H_
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TEST_FILE       "data_A2_Q1_t.txt"
#define INTS_PER_LINE   (5)
#define INITIAL_SIZE    (10000 * INTS_PER_LINE)
#define BUFSIZE         (256)

typedef unsigned int uint_t; // so I dont't have to keep writing `unsigned int`

/* was having trouble on windows with timespec_get */
typedef clock_t timestamp_t;
void time_start(timestamp_t* start) {
    printf("Starting inversions count...\n");
    *start = clock();
}

void time_elapsed(timestamp_t* start) {
    timestamp_t elapsed = clock() - (*start);
    double ms = (elapsed / (double)CLOCKS_PER_SEC) * 1000.0;
    printf("Time elapsed: %lfms\n", ms);
}

/**
 * Reads the data from the file. File must be in the form
 * of 5 integers per line for 10,000 lines.
 * @param size where to write the size of the points
 * @returns an array of the points, allocated in memory
 */
int read_file(uint_t** data) {
    char filename[BUFSIZE];
    printf("> Enter filename: ");
    scanf("%s", filename);

    int i = 0, s = INITIAL_SIZE;
    FILE* file = fopen(filename, "r");
    if (file == NULL) { // Safety incase file is not found
        printf("** Could not open file \"%s\"\n", filename);
        *data = NULL;
        return -1;
    } else { // Read was valid
        printf("Reading data from \"%s\" ...\n", filename);
    }

    uint_t* _data = (uint_t*)malloc(sizeof(uint_t) * s);
    uint_t line[INTS_PER_LINE];
    while (fscanf(file, "%u %u %u %u %u", line, line+1, line+2, line+3, line+4) != EOF) {
        if ((i + INTS_PER_LINE) > s) {
            s *= 2;
            _data = (uint_t*)realloc(data, s);
        }
        memcpy(_data + i, line, sizeof(uint_t) * INTS_PER_LINE);
        i += INTS_PER_LINE;
    }
    fclose(file);
    if (i != s) {
        // If there's less data
        _data = (uint_t*)realloc(_data, sizeof(uint_t) * i);
    }
    printf("Read %d numbers from \"%s\"\n", i, filename);
    *data = _data;
    return i;
}

#endif // !_P1_H_
