/**
 * @brief CIS*3490 A3 - Code for Q2.2 - "string matching using the Horspool algorithm"
 * @author Brenton Hauth (1229237)
 * @date February 20, 2023
 */
#include "P2.h"


/**
 * Uses the Horspool algorithm to determine the number of matches a pattern has
 * in a data set. Also writes the number of shifts, and the number of comparisons
 * that occured while searching.
 * @param data the data to search through
 * @param size the size of the data
 * @param pattern the search pattern
 * @param _shifts where to write the number of shifts
 * @param _comps where to write the number of comparisons
 */
int string_matches(const char* data, const int size, const char* pattern, int* _shifts, int* _comps) {
    int psize = strlen(pattern);
    if (psize <= 0) {
        // edge case where the pattern is ""
        *_shifts = 0;
        *_comps = 0;
        return 0;
    }

    // Create a shift table for bad characters
    int table[TABLE_SIZE];
    create_shift_table(pattern, table);

    int matches = 0, shifts = 0, comps = 0, i = 0;
    bool_t is_match;
    while ((i + psize) <= size) { // makes sure not to go over the size limit
        is_match = TRUE;
        for (int j = psize - 1; j >= 0; --j) { // Counts from right position to left
            // Check if the pattern doesn't match at j
            ++comps; // count a comparison that occurs
            if (pattern[j] != data[i + j]) {
                is_match = FALSE;
                break; // don't bother continuing if it's a mismatch
            }
        }
        if (is_match) { // is match
            ++matches; // increase matches
        }
        // iterate to next
        i += table[char_to_index(data[i + (psize - 1)])];
        ++shifts; // count a shift
    }
    *_shifts = shifts;
    *_comps = comps;
    return matches;
}

int main() {
    char* data;// = "babarber jim saw me in a barbershop";
    timestamp_t start;
    char pattern[PATTERN_MAX];
    
    int shifts, comps, size = read_data(&data);
    printf("read_data size: %d\n", size);
    
    int plen = read_pattern(pattern);
    if (plen < 0) {
        printf("** Cannot continue with invalid pattern.\n");
        return -1;
    }

    printf("Begining Horspool search with pattern: %s\n", pattern);
    
    time_start(&start);
    int matches = string_matches(data, size, pattern, &shifts, &comps);
    time_elapsed(&start);

    printf("\nFound %d matches in total.\n", matches);
    printf(" > Number of shifts: %d\n", shifts);
    printf(" > Number of comparisons: %d\n", comps);
    // printf(" > ratio shifts/time = %.1f\n", (double)shifts / ms);

    free(data);
    printf("done.\n");
    return 0;
}
