/**
 * @brief CIS*3490 - A5 - Subset Sum Problem via Brute Force
 * @author Brenton Hauth (1229237)
 * @date March 29, 2023
 */
#include "P1.h"


/**
 * Implementation of the subset sum problem using a brute force method.
 * Uses bit manimpulation to keep track of subsets (faster than dynamically
 * adding things to a list of elements).
 * @param data the list of values to create subsets from
 * @param n the number of elements in data
 * @param d the value that subsets have to sum to
 * @param total where to write the total number of subsets evaluated
 * @returns the number of subsets that sum to `d`
 */
uint_t subset_sum(uint_t* data, const int n, const uint_t d, uint_t* total) {
    /// NOTE: if n > 32, then we can use other types (ull), or dynamically add
    ///       values to a list, but I really like the approach I came up with

    // Set bit at n so we have 1000...00 (n zeros). Then `-1`, to make 0111...11 (n ones)
    // and by doing this we now have a number that represents the full set.
    uint_t S = (1 << n) - 1;
    uint_t subsets = 0; // keep track of subsets that sum to d

    // go through all subsets (from {} to S), where each bit represents an element in data.
    // If the i'th bit is 1, then the i'th element is included in the subset.
    for (uint_t set = 0; set <= S; ++set) {
        uint_t sum = 0; // sum for current subset
        // go through all elements to check if they are in the subset
        for (int i = 0; i < n; ++i) {
            // check if i'th bit is in current subset
            if (((1 << i) & set) != 0) {
                sum += data[i]; // if so, then add the i'th element to the sum
                if (sum > d) {
                    // if sum is greater than `d`, then no point in continuing
                    break;
                }
            }
        }

        if (sum == d) {
            // when finished with the subset, check if it equals `d`
            ++subsets;
        }
    }

    *total = S + 1; // went through all subsets of S, +1 for the empty set
    return subsets; // return subsets that add to the sum
}

int main() {
    printf("[Brute force program for subset sum problem]\n");
    const int n = NUMCOUNT;
    uint_t d, subsets, total;
    timestamp_t ts;
    double ms;

    uint_t* data = read_file(n, &d);

    time_start(&ts);
    subsets = subset_sum(data, n, d, &total);
    ms = time_elapsed(&ts);

    printf("Total number of subsets evaluated: %u\n", total);
    printf("Number of subsets that sum to %u: %u\n", d, subsets);
    printf("\nExecution time = %d ms\n", (int)ms);
    free(data);
    return 0;
}
