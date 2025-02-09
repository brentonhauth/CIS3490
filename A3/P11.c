/**
 * @brief CIS*3490 A3 - Code for Q1.1 - "Interval counter via brute force"
 * @author Brenton Hauth (1229237)
 * @date February 20, 2023
 */
#include "P1.h"

/**
 * Counts the number of overlapping intervals in a brute force fashion by
 * finding the minimum and maximum range in the intervals and counting all
 * intervals a given point overlaps with.
 * @param data the intervals to iterate over
 * @param size the number of elements in data
 * @param point where to write the common point
 * @returns the number of intervals that have overlapped
 */
int count_intervals_overlap(const Interval* data, const int size, int* point) {
    int min = INT_MAX, max = INT_MIN; // default values to be overwritten
    int count = 0; // counting how many intervals overlap
    
    // Find the minimum & maximum values of the range
    for (int i = 0; i < size; ++i) {
        const Interval* I = data + i;
        if (I->a < min) {
            min = I->a; // update to minimum range
        }
        if (I->b > max) {
            max = I->b; // update to maximum range
        }
    }

    int _point = 0; // Default common point
    
    // iterates from minimum to maximum (excluding min & max values)
    for (int p = (min + 1); p < max; ++p) {
        int n = 0; // temp value for counting intervals.
        for (int i = 0; i < size; ++i) {
            const Interval* I = data + i; // Current interval
            if (IN_INTERVAL(I, p)) { // Checks whether the point is in the interval
                ++n;
            }
        }
        if (n > count) {
            // Updates count and common point if new max was reached
            _point = p;
            count = n;
        }
    }

    // returns common point and interval count
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

    printf("\nFound maximum number of overlapped intervals: %d\n", max);
    printf("Common point: %d\n", point);
    free(data);
    return 0;
}
