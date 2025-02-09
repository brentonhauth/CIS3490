#ifndef _P1_H_
/**
 * @brief CIS*3490 A3 - General code used in both Q1.1 & Q1.2.
 * @author Brenton Hauth (1229237)
 * @date February 15, 2023
 */
#define _P1_H_
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>

#define POINT_COUNT         30000
#define TEST_FILE           "data_A3_Q1_1.txt"
#define IN_INTERVAL(I, p)   ((p) > (I)->a && (p) < (I)->b)  /* Checks if a point is in interval */
#define MIN(a, b)           ((a) < (b) ? (a) : (b))
#define MAX(a, b)           ((a) > (b) ? (a) : (b))
#define SIZE(I)             ((I)->b - (I)->a - 1)           /* Returns the size of the interval */
// More accurate size, buf slower ((I)->a < (I)->b ? (I)->b - (I)->a - 1 : 0)

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
 * @param ts the time stamp to check the elapsed time
 */
void time_elapsed(timestamp_t* ts) {
    timestamp_t elapsed = clock() - (*ts);
    double ms = (elapsed / (double)CLOCKS_PER_SEC) * 1000.0;
    printf("\nExecution time = %.1lf ms\n", ms);
}


/**
 * Represents an exclusive interval (a, b).
 */
typedef struct Interval {
    int a;
    int b;
} Interval;

/**
 * reads the intervals from a file and writes them into memory
 * @param data where to write the intervals
 * @returns the number of elements in data
 */
int read_data(Interval** data) {
    char filename[FILENAME_MAX];
    printf("> Please enter a filename (enter \".\" for test file): ");
    scanf("%s", filename);
    
    if (!strcmp(filename, ".")) { // set default, easy for testing
        strcpy(filename, TEST_FILE);
    }

    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        // Return if could not open the file
        printf("** Could not open file!\n");
        *data = NULL;
        return -1;
    }

    Interval* _data = (Interval*)malloc(sizeof(Interval) * POINT_COUNT);
    int a, b, i = 0;
    while (fscanf(file, "%d %d", &a, &b) != EOF) {
        // Add each value to the thing
        _data[i].a = a;
        _data[i++].b = b;
    }
    fclose(file);
    *data = _data;
    return i;
}

/**
 * Compares 2 intervals based on their left value.
 * @param i the first interval to compare
 * @param j the second interval to compare
 */
int intr_comp_a(const void* i, const void* j) {
    // Cast to Interval pointer and compare the a values
    return ((Interval*)i)->a - ((Interval*)j)->a;
}

#endif
