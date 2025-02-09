#ifndef _P1_H_
/**
 * @brief CIS*3490 - A5 - Code used in both P1.1 and P1.2
 * @author Brenton Hauth (1229237)
 * @date March 29, 2023
 */
#define _P1_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


#define BUFSIZE     (256)
#define NUMCOUNT    (25)
#define TEST_FILE   "data_A5_Q1_1.txt"

typedef clock_t timestamp_t;
typedef unsigned int uint_t;

/**
 * Start a timer (imported from previous assignment)
 * @param ts where to write the start time
 */
void time_start(timestamp_t* ts) {
    printf("Starting timer...\n");
    *ts = clock();
}

/**
 * Check how much time has elapsed from the timestamp given
 * (imported from previous assignment)
 * @param ts the time stamp to check the elapsed time
 * @returns the number of miliseconds since the start time
 */
double time_elapsed(timestamp_t* ts) {
    timestamp_t elapsed = clock() - (*ts);
    return (elapsed / (double)CLOCKS_PER_SEC) * 1000.0;
}

/**
 * Compares 2 unsigned ints. Used in qsort.
 * @param a pointer to first number
 * @param b pointer to second number
 */
int uintcmp(const void* a, const void* b) {
    uint_t x = *((uint_t*)a);
    uint_t y = *((uint_t*)b);
    return x - y;
}


/**
 * Read the data from the file
 * @param n the number of elements to get from the file
 * @param d where to write the number to check the input against
 * @returns array of numbers of size `n`
 */
uint_t* read_file(const int n, uint_t* d) {
    char input[BUFSIZE];
    printf("Enter the file name and subset sum: ");
    scanf("%s %u", input, d); // grab both inputs

    if (!strcmp(input, ".1")) { // Makes testing much quicker
        strcpy(input, TEST_FILE);
    }

    FILE* file = fopen(input, "r");
    if (file == NULL) { // failsafe
        printf("** Could not open file.\n");
        exit(1);
    }
    uint_t* data = (uint_t*)malloc(sizeof(uint_t) * n);
    uint_t x, i = 0;
    while (fscanf(file, "%u", &x) != EOF && i < n) {
        // Go through each element and add it to the array
        data[i++] = x;
    }
    fclose(file); // done with file
    return data;
}







#endif
