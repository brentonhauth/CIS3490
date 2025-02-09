/**
 * @brief CIS*3490 A3 - Code for Q1.2 - "Interval counter with presorting"
 * @author Brenton Hauth (1229237)
 * @date February 20, 2023
 */
#include "P1.h"

/**
 * Counts the number of overlapping intervals by first presorting the intervals
 * in ascending order by their left value ("a"), then working backwards to see how many
 * intervals have an upper range that crosses over that "a" value.
 * @param data the intervals to iterate over
 * @param size the number of elements in data
 * @param point where to write the common point
 * @returns the number of intervals that have overlapped
 */
int count_intervals_overlap(Interval* data, const int size, int* point) {
    qsort(data, size, sizeof(Interval), intr_comp_a); // presort intervals based on a
    int count = 0, _point = INT_MIN; // keep track of interval count and common point
    
    // count backwards from intervals
    for (int i = (size - 1); i >= 0; --i) {
        /// NOTE: have "i >= 0" to handle edge case of all
        /// intervals (except the first) having a size of 0
        const Interval* I = data + i; // current interval
        if (SIZE(I) <= 0) {
            // Don't bother with intervals of size 0  e.g. (5, 6) since the range is exlusive.
            /// NOTE: Have to check `<=` in case the intervals are backwards
            continue;
        }

        int n = 1; // Count overaps for current interval, set to 1 since current is included
        
        for (int j = (i - 1); j >= 0; --j) { // start at 1 previous interval
            
            const Interval* J = data + j; // interval preceeding current interval
            
            // Check that the preceeding interval has a valid size
            // and that it overlaps with the current interval.
            if (SIZE(J) > 0 && (J->b) > (I->a + 1)) {
                ++n; // increase number of overlaps
            }
        }
        if (n > count) {
            // update new count and first common point
            count = n;
            _point = (I->a + 1);
        }
    }

    *point = _point;
    return count;
}



int main() {
    Interval* data;
    timestamp_t start;
    int point, size = read_data(&data);
    if (size < 0) {
        // Error occured, exit
        return -1;
    }

    printf("Read %d intervals.\n", size);
    time_start(&start);
    int max = count_intervals_overlap(data, size, &point);
    time_elapsed(&start);

    printf("\nThe maximum number of intervals: %d\n", max);
    printf("The intervals include point: %d\n", point);
    free(data);
}
