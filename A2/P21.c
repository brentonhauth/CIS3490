#define _NO_DUPLICATE_POINTS    /* so no duplicate points end up in the hull */
#include "P2.h"


Point* convexhull(Point* points, const int size, int* hsize) {
    int h = 0;
    Point* hull = (Point*)malloc(sizeof(Point) * (POINT_COUNT >> 2));
    for (int i = 0; i < (size - 1); ++i) {
        for (int j = (i + 1); j < size; ++j) {
            int l = 0, r = 0;
            // a, b, c used for checking positions of points
            double a = points[j].y - points[i].y;
            double b = points[i].x - points[j].x;
            double c = (points[i].x * points[j].y) - (points[j].x * points[i].y);
            for (int k = 0; k < size; ++k) {
                if (k == i || k == j) {
                    // Stop if k is point on the line
                    continue;
                }
                double p = POS(a, b, c, points + k);
                if (p < 0) {
                    ++l;
                } else if (p > 0) {
                    ++r;
                }
                // else {
                //     // Middle
                // }
                if (l > 0 && r > 0) {
                    // Stops if there are points on both sides
                    break;
                }
            }
            if ((l == 0) != (r == 0)) { // xor check for booleans
                // Checks if there are points on one side or the
                //  other (not both) and adds the points to the hull.
                hull_append(&hull, points + i, &h);
                hull_append(&hull, points + j, &h);
            }
        }
    }
    *hsize = h;
    return hull;
}


int main() {
    Point* data = read_data(TEST_FILE, POINT_COUNT);
    int hsize = 0;

    timestamp_t ts;
    time_start(&ts);
    Point* hull = convexhull(data, POINT_COUNT, &hsize);
    time_elapsed(&ts);

    Point s1, s2;

    read_point("s1", &s1);
    read_point("s2", &s2);

    // Point s1 = { 30.8f, 3064.0f };
    // Point s2 = { 6416.4f, 3607.9f };

    // Point s1 = { 2238.7, 6426.4 };
    // Point s2 = { 3943.7, 32.0 };
    int psize;
    Point* path = NULL;
    double dis = shortest_path(hull, hsize, &s1, &s2, &path, &psize);

    if (psize < 0) {
        printf("** Could not find shortest path.\n");
        free(hull);
        free(data);
        return -1; // exit
    }
    printf("Shortest path from (%.1f, %.1f) to (%.1f, %.1f):\n", s1.x, s1.y, s2.x, s2.y);
    print_points(path, psize);
    printf("\n[%d points]\n", psize);
    printf("Distance:  %.2f units\n", dis);

    free(path);
    free(hull);
    free(data);
    return 0;
}
