/**
 * @brief CIS*3490 A3 - Code for Q2.1 - "Brute force string matching"
 * @author Brenton Hauth (1229237)
 * @date February 20, 2023
 */
#include "P2.h"

/**
 * Uses a brute force algorithm to determine how many matches occured in a data set.
 * Also informs how many shifts and comparisons occured.
 * @param data the data of text to search
 * @param size the size of the data
 * @param pattern the search pattern to apply to the data
 * @param _shifts where to write the number of shifts that occured
 * @param _comps where to write the number of comparisons that occured
 */
int string_matches(const char* data, const int size, const char* pattern, int* _shifts, int* _comps) {
    int plen = strlen(pattern);
    int psize = strlen(pattern);
    if (psize <= 0) { // Edge case for if pattern is ""
        *_shifts = 0;
        *_comps = 0;
        return 0;
    }

    int matches = 0, shifts = 0, comps = 0, i = 0;
    bool_t is_match = FALSE;
    while ((i + plen) <= size) { // Ensures it doesn't go over the limit
        is_match = TRUE;
        for (int j = 0; j < plen; ++j) {
            ++comps; // increase number of comparisons
            if (data[i + j] != pattern[j]) {
                is_match = FALSE;
                break; // break if there's a mismatch
            }
        }
        if (is_match) {
            ++matches; // count that a match occured
        }
        ++i; // increment
        ++shifts; // increase number of shifts
    }

    // Return out values
    *_shifts = shifts;
    *_comps = comps;
    return matches;
}



int main() {
    char* data;
    int shifts, comps;
    timestamp_t start;
    char pattern[PATTERN_MAX];
    
    int size = read_data(&data);
    int plen = read_pattern(pattern);
    if (plen < 0) {
        printf("** Cannot continue with invalid pattern.\n");
        return -1;
    }

    printf("Begining brute force search with pattern: %s\n", pattern);
    
    time_start(&start);
    int matches = string_matches(data, size, pattern, &shifts, &comps);
    time_elapsed(&start);

    printf("Found %d matches in total.\n", matches);
    printf(" > Number of shifts: %d\n", shifts);
    printf(" > Number of comparisons: %d\n", comps);

    free(data);
    printf("done.\n");
    return 0;
}
