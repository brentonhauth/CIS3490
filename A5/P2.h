#ifndef _P2_H_
/**
 * @brief CIS*3490 - A5 - Code used in both P2.1 and P2.2
 * @author Brenton Hauth (1229237)
 * @date March 29, 2023
 */
#define _P2_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>

#define BUFSIZE         (256)
#define MATSIZE         (12)
#define TEST_FILE       "data_A5_Q2_1.txt"
#define IS_SPACE(c)     ((c) == '\x20' || (c) == '\n' || (c) == '\0')

#ifndef TRUE
#define TRUE (1)
#endif
#ifndef FALSE
#define FALSE (0)
#endif


typedef unsigned char bool_t;
typedef unsigned int uint_t;
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
 * Read data from a file and save to a matrix
 * NOTE: matrix is as a 1D array but can access data[i,j] with data[i * n + j]
 * @param n the size of the matrix to return
 * @returns matrix of size n
 */
uint_t* read_data(const int n) {
    char buf[BUFSIZE];
    printf("Enter the file name: ");
    scanf("%s", buf);

    if (!strcmp(buf, ".1")) { // Easier for testing
        strcpy(buf, TEST_FILE);
    }

    FILE* file = fopen(buf, "r");
    if (file == NULL) {
        // safety, in case you can't open the file, or it doesn't exist
        printf("** Could not open file. Goodbye.\n");
        exit(1);
    }

    uint_t* data = (uint_t*)malloc(sizeof(uint_t) * n * n); // creates n x n matrix

    for (int i = 0; i < n; ++i) { // iter through rows
        fgets(buf, BUFSIZE, file); // get current row of numbers
        int plen = strlen(buf);

        int j = 0, start = 0;
        while (start < plen) {
            if (IS_SPACE(buf[start])) {
                ++start; // continue to next char in line
                continue;
            }
            int end = start + 1; // have end point of row
            for (; end <= plen; ++end) {
                // either the end has reached the length or
                // they have reached a "space"
                if (end == plen || IS_SPACE(buf[end])) {
                    buf[end] = '\0'; // set terminator to prevent overflow

                    // convert string to int and set to
                    data[i * n + (j++)] = atoi(&buf[start]);
                    break;
                }
            }
            start = end + 1; //
        }
    }
    fclose(file); // done with file
    return data; // give back matrix
}




#endif
