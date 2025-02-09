#include "P2.h"

/**
 * @param hull where to put hull points
 * @param h length of the hull
 * @param points the points to pull from
 * @param psize the size of the points array
 * @param p1_p2 a line going from p1 to p2
 */
void upperhull(Point** hull, int* h, Point* points, const int psize, const Line* p1_p2) {
    if (psize == 0) {
        // base case: return if 0
        return;
    } else if (psize == 1) {
        // add lonely point and leave
        hull_append(hull, &(points[0]), h);
        return;
    }

    // Find the point of the greatest distance from the line
    double dis = distance(p1_p2->p1, p1_p2->p2, points + 0);
    for (int i = 1; i < psize; ++i) {
        const Point* pt = points + i;
        double d = distance(p1_p2->p1, p1_p2->p2, pt);
        if (d > dis) {
            swap_points(points, 0, i);
            dis = d;
        }
    }

    int l = 1, r = psize;
    Line p1_m, m_p2;
    calculate_line(&p1_m, p1_p2->p1, points);
    calculate_line(&m_p2, points, p1_p2->p2);

    // Fill in right side first
    int i = 1; // start at 1, because index 0 is the middle point
    while (i < r) {
        if (POS_LINE(&m_p2, points + i) < 0) {
            swap_points(points, i, --r);
        } else {
            ++i;
        }
    }
    // Fill in left side second
    i = r - 1;
    while (i >= l) {
        if (POS_LINE(&p1_m, points + i) < 0) {
            swap_points(points, l++, i);
        } else {
            --i;
        }
    }


    hull_append(hull, points, h); // Append to hull
    upperhull(hull, h, &(points[1]), l - 1, &p1_m);
    upperhull(hull, h, &(points[r]), psize - r, &m_p2);
}

/**
 * @param points the set of points to use
 * @param size the size of the points array
 * @param len where to write the length of the hull
 */
Point* quickhull(Point* points, const int size, int* len) {
    // Find maximum and minimum x coords
    // Place minimum at index [0], maximum at index [size - 1]
    for (int i = 1; i < (size - 1); ++i) {
        if (points[i].x > points[size - 1].x) {
            swap_points(points, i, size - 1);
        } else if (points[i].x < points[0].x) {
            swap_points(points, i, 0);
        }
    }

    Point* p1 = points + 0; // min
    Point* p2 = points + (size - 1); // max

    // Create line from p1 to p2, and vice versa
    Line p1_p2, p2_p1;
    calculate_line(&p1_p2, p1, p2);
    calculate_line(&p2_p1, p2, p1);

    int l = 1, r = size - 1;
    while (l < r) {
        // Keep swapping left & right points until
        // left is full of points in upper hull
        // and right is full of points in lower hull
        Point* pt = points + l;
        double side = POS_LINE(&p1_p2, pt);
        if (side > 0) {
            swap_points(points, l, --r);
        } else { // don't need to check cuz it is
            ++l;
        }
    }

    *len = (size >> 2); // make hull size 1/4 size of points array
    Point* hull = (Point*)malloc(sizeof(Point) * (*len));
    int h = 0;
    // Add minimum & maximum to hull
    hull_append(&hull, &(points[0]), &h);
    hull_append(&hull, &(points[size - 1]), &h);

    // Slice left & right hulls
    upperhull(&hull, &h, &(points[1]), (l - 1), &p1_p2);
    upperhull(&hull, &h, &(points[r]), (size - r - 1), &p2_p1);

    if (h < (*len)) {
        // Resize hull
        hull = (Point*)realloc(hull, h * sizeof(Point));
    }
    *len = h;
    return hull;
}



int main() {
    int hsize = 0;
    
    Point* data = read_data(TEST_FILE, POINT_COUNT);
    timestamp_t ts;
    time_start(&ts);
    Point* hull = quickhull(data, POINT_COUNT, &hsize);
    time_elapsed(&ts);

    // sorthull(hull, hsize);
    // print_points(hull, hsize);

    Point s1, s2;

    read_point("s1", &s1);
    read_point("s2", &s2);
    int psize;
    Point* path = NULL;
    double dis = shortest_path(hull, hsize, &s1, &s2, &path, &psize);


    if (psize < 0) {
        printf("** Invalid points. Could not find shortest path.\n");
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
