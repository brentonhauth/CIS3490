/**
 * @brief CIS*3490 A3 - Code for Q2.3 - "string matching using the Boyer-Moore algorithm"
 * @author Brenton Hauth (1229237)
 * @date February 20, 2023
 */
#include "P2.h"

/**
 * Uses the Boyer-Moore algorithm to determine how many matches occured in a data set.
 * Also informs how many shifts and comparisons occured.
 * @param data the data of text to search
 * @param size the size of the data
 * @param pattern the search pattern to apply to the data
 * @param _shifts where to write the number of shifts that occured
 * @param _comps where to write the number of comparisons that occured
 */
int string_matches(const char* data, const int size, const char* pattern, int* _shifts, int* _comps) {
    int psize = strlen(pattern);
    if (psize <= 0) { // Edge case for if pattern is ""
        *_shifts = 0;
        *_comps = 0;
        return 0;
    }

    // Bad symbol table can be on the stack since we know the size
    int bad_symbol[TABLE_SIZE];
    create_shift_table(pattern, bad_symbol);

    // Good suffix table is dependant on the size of the pattern
    int* good_suffix = NULL;
    create_good_suffix_table(pattern, &good_suffix);

    int matches = 0, shifts = 0, comps = 0, i = 0;

    bool_t is_match = FALSE; // used for if there are matches

    while ((i + psize) <= size) { // Ensures it doesn't go over the limit
        is_match = TRUE; // mark as true
        for (int j = (psize - 1); j >= 0; --j) {
            ++comps; // count how many checks occured
            if (data[i + j] != pattern[j]) {
                int k = (psize - 1) - j; // count how many spots

                // Grab the bad symbol and offset by the suffex length
                int d1 = bad_symbol[char_to_index(data[i + j])] - k;
                if (d1 < 1) {
                    // Makes sure it doesn't go into the negatives or 0
                    d1 = 1;
                }

                if (k == 0) {
                    // Shift just like Horspool's algorithm
                    i += d1;
                } else {
                    // minus additional 1 since array starts at 0
                    i += MAX(d1, good_suffix[k - 1]);
                }
                ++shifts; // count a shift
                is_match = FALSE; // flag as a match 
                break;
            }
        }
        if (is_match) { // Match occured
            ++matches;
            // Shift like Horsepool
            i += bad_symbol[char_to_index(data[i + psize - 1])];
            ++shifts; // count a shift
        }
    }
    free(good_suffix); // done with good suffix table
    *_shifts = shifts;
    *_comps = comps;
    return matches;
}


int main() {
    char* data;
    timestamp_t start;
    char pattern[PATTERN_MAX];

    
    int plen = read_pattern(pattern);
    if (plen < 0) {
        printf("** Cannot continue with invalid pattern.\n");
        return -1;
    }

    int* table = NULL;
    create_good_suffix_table(pattern, &table);

    int comps, shifts, size = read_data(&data);
    printf("read_data size: %d\n", size);
    printf("Begining Boyer-Moore search with pattern: %s\n", pattern);
    
    time_start(&start);
    int matches = string_matches(data, size, pattern, &shifts, &comps);
    time_elapsed(&start);

    printf("\nFound %d matches in total.\n", matches);
    printf(" > number of shifts: %d\n", shifts);
    printf(" > number of comparisons: %d\n", comps);
    // printf(" > ratio shifts/time = %.1f\n", (double)shifts / ms);

    free(data);
    free(table);
    printf("done.\n");
    return 0;
}
