#ifndef _P2_H_
#define _P2_H_
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>

#ifndef POINT_COUNT
#define POINT_COUNT (30000)
#endif

#ifndef CMP_ERR
#define CMP_ERR     (0.01)
#endif

#define TEST_FILE   "data_A2_Q2.txt"


typedef clock_t timestamp_t;
void time_start(timestamp_t* start) {
    printf("Starting to compute convex hull...\n");
    *start = clock();
}

void time_elapsed(timestamp_t* start) {
    timestamp_t elapsed = clock() - (*start);
    double ms = (elapsed / (double)CLOCKS_PER_SEC) * 1000.0;
    printf("Time elapsed: %lfms\n", ms);
}

/**
 * Formula retrieved from page 113 in textbook.
 * where a, b & c are paramaters used to calculate the line
 * @param a (y2 - y1)
 * @param b (x1 - x2)
 * @param c ((x1 * y2) - (x2 * y1))
 * @param p point to check position of
 */
#define POS(a, b, c, p)    (((a) * ((p)->x)) + ((b) * ((p)->y)) - (c))
#define POS_LINE(l, p)    ((((l)->a) * ((p)->x)) + (((l)->b) * ((p)->y)) - ((l)->c))


/**
 * represents an (x, y) coordinate in 2D space
 */
typedef struct Point {
    double x;
    double y;
} Point;

/**
 * Created from 2 points, used in calculations for
 * a position (from page 113 of textbook).
 * @param a (y2 - y1)
 * @param b (x1 - x2)
 * @param c ((x1 * y2) - (x2 * y1))
 */
typedef struct Line {
    double a;
    double b;
    double c;
    const Point* p1;
    const Point* p2;
} Line;

// typedef struct PointList {
//     unsigned int size, max;
//     Point* points;
// } PointList;

/**
 * Sets the line values, calculations from page 113 in textbook.
 * @param line the line to write the data
 * @param p1 the first point to use
 * @param p2 the second point to use
 */
void calculate_line(Line* line, const Point* p1, const Point* p2) {
    line->a = p2->y - p1->y;
    line->b = p1->x - p2->x;
    line->c = (p1->x * p2->y) - (p2->x * p1->y);
    line->p1 = p1;
    line->p2 = p2;
}

/**
 * Reads the points from the file
 * @param filename name of the file
 * @param len the length of the data
 * @returns Array of points allocated on memory
 */
Point* read_data(const char* filename, const int len) {
    FILE* file = fopen(filename, "r");
    Point* data = (Point*)malloc(sizeof(Point) * len);
    int i = 0;
    float x, y; // gets mad if I don't use floats
    while (fscanf(file, "%f %f", &x, &y) != EOF) {
        data[i].x = x;
        data[i++].y = y; // Increments `i` after use
    }
    return data;
}



/**
 * Checks the perpendicular distance between a line (from `a` to `b`) and a point.
 * @param a the start of the line
 * @param b the end of the line
 * @param p the point to check the distance from
 */
double distance(const Point* a, const Point* b, const Point* p) {
    double m = (b->y - a->y) / (b->x - a->x); // slope of the line
    // Through messing around in desmos, I was able
    // to come up with this formula:
    double mI = 1.0 / m; // inverse of m
    // x & y intercepts
    double xInt = ((mI * p->x) + (p->y) + (m*a->x) - (a->y)) / (m + mI);
    double yInt = (m * (xInt - a->x) + a->y);
    xInt = p->x - xInt;
    yInt = p->y - yInt;
    return sqrt((xInt * xInt) + (yInt * yInt));
}

/**
 * Calculates the distance between two points
 * @param p1 the first point to calculate the distance from
 * @param p2 the second point to calculate the distance to
 */
double point_distance(const Point* p1, const Point* p2) {
    double x = p2->x - p1->x;
    double y = p2->y - p1->y;
    return sqrt((x * x) + (y * y));
}

/**
 * Compares two points to see if they have the same data.
 * Allows for a margin of error defined by `CMP_ERR`.
 * @param a first point to compare
 * @param b second point to compare
 * @returns 1 if the points are the same, 0 if not.
 */
int pointcmp(const Point* a, const Point* b) {
    double c = (a->x - b->x) + (a->y - b->y);
    return fabs(c) <= CMP_ERR ? 1 : 0;
}

/**
 * Copies the data from one point to another
 * @param dest the destination of where to copy the points
 * @param src the source of where to copy from
 */
void pointcpy(Point* dest, const Point* src) {
    dest->x = src->x;
    dest->y = src->y;
}

void print_points(const Point* points, const int size) {
    for (int i = 0; i < size; ++i) {
        printf("(%.1f, %.1f)\n", points[i].x, points[i].y);
    }
}

void read_point(const char* title, Point* p) {
    printf("\n> Enter x for %s$ ", title);
    scanf("%lf", &(p->x));
    printf("> Enter y for %s$ ", title);
    scanf("%lf", &(p->y));
}


/**
 * Adds a point to the convex hull. If `_NO_DUPLICATE_POINTS` is
 * defined then it will check if a point with the same values
 * has already been added to the hull.
 * @param hull the convex hull to add points to
 * @param p the point to be added to the hull
 * @param hsize the pointer to length of the hull
 */
void hull_append(Point** hull, const Point* p, int* hsize) {
    Point* _hull = *hull;
#ifdef _NO_DUPLICATE_POINTS
    // Useful in bruteforce algo to not add any duplicate points
    for (int i = 0; i < (*hsize); ++i) {
        if (pointcmp(&(_hull[i]), p)) {
            return;
        }
    }
#endif
    pointcpy(&(_hull[(*hsize)]), p);
    printf("Found extreme point, adding to Hull[%d]:  (%.1f, %.1f)\n", *hsize, p->x, p->y);
    (*hsize)++;
}

void hull_insert(Point** hull, const int index, const Point* p, int* hsize) {
    Point* _hull = *hull;
    if (index < (*hsize)) {
        for (int i = ((*hsize) - 1); i >= index; --i) {
            pointcpy(_hull + i + 1, _hull + i);
        }
        pointcpy(_hull + index, p);
    } else {
        pointcpy(_hull + (*hsize), p);
    }
    ++(*hsize);
}

/**
 * @param points the points in the thing
 * @param i the first index to swap
 * @param j the second index to swap
 */
void swap_points(Point* points, const int i, const int j) {
    Point t;
    pointcpy(&t, points + i);
    pointcpy(points + i, points + j);
    pointcpy(points + j, &t);
}

/**
 * Compares the x coord of the 2 points in ascending order
 * @param p1 the first point to compare
 * @param p2 the second point to compare
 */
int asc_x(const void* p1, const void* p2) {
    double x = ((Point*)p1)->x - ((Point*)p2)->x;
    if (x < 0) { // p1 is left of p2
        return -1;
    } else if (x > 0) { // p1 is right of p2
        return 1;
    } else { // same x coordinate
        return 0;
    }
}

/**
 * Compares the x coord of the 2 points in descending order
 * @param p1 the first point to compare
 * @param p2 the second point to compare
 */
int desc_x(const void* p1, const void* p2) {
    // Negates ascending x to switch it
    return (-asc_x(p1, p2));
}


/**
 * Organizes the hull of points in a counter clockwise fasion
 * @param hull the hull to organize
 * @param hsize the size of the hull
 */
void sorthull(Point* hull, const int hsize) {
    int max = 0, min = 0;
    // Grab max & min x values
    for (int i = 1; i < hsize; ++i) {
        if (hull[i].x > hull[max].x) {
            max = i;
        } else if (hull[i].x < hull[min].x) {
            min = i;
        }
    }

    // Create a line through the middle
    Line line;
    calculate_line(&line, hull + min, hull + max);
    Point* sorted = (Point*)malloc(sizeof(Point) * hsize);
    int l = 0, r = hsize;
    // Adds points in the upper hull from the left
    pointcpy(sorted + (l++), hull + min);
    // Adds points in the lower hull from the right
    pointcpy(sorted + (--r), hull + max);

    for (int i = 0; i < hsize; ++i) {
        if (i == max || i == min) {
            continue;
        }
        double p = POS_LINE(&line, hull + i);
        if (p < 0) { // is upper hull
            pointcpy(sorted + (l++), hull + i);
        } else { // is lower hull
            pointcpy(sorted + (--r), hull + i);
        }
    }
    // sorts upper points in acending & lower in descending
    //   so that the hull is sorted clockwise
    qsort(sorted, l, sizeof(Point), asc_x);
    qsort(sorted + r, hsize - r, sizeof(Point), desc_x);
    memcpy(hull, sorted, sizeof(Point) * hsize);
    free(sorted);
}


/**
 * Calculates the distance of of a path going counterclockwise
 * from the `start` index to the `end` index.
 * @param hull the hull points
 * @param hsize the size of the hull
 * @param start the starting point index
 * @param end the ending point index
 * @param path where to write the path
 * @param psize where to write the size of the path
 * @returns the distance of the path
 */
double calc_path_distance(
    Point* hull, const int hsize,
    const int start, const int end,
    Point** path, int* psize) {
    //
    int i = start, _psize = 0;
    double dis = 0;
    Point* _path = (Point*)malloc(sizeof(Point) * hsize);
    Point *prev = NULL, *p = NULL;

    while (1) {
        i %= hsize; // prevents overflow
        if (i == start) {
            p = hull + start;
        } else {
            p = &(hull[i % hsize]);
            dis += point_distance(p, prev);
        }
        pointcpy(_path + (_psize++), p);
        prev = p;
        if (i++ == end) {
            break;
        }
    }
    *path = _path;
    *psize = _psize;
    return dis;
}

/**
 * Calculates the shortest path around a hull from s1 to s2.
 * @param hull the hull to calculate the shortest path around
 * @param hsize the size of the hull
 * @param s1 the starting point on the hull
 * @param s2 the ending point on the hull
 * @param path where to write the path
 * @param psize where to write the size of the path
 * @returns the distance of the shortest path
 */
double shortest_path(
    Point* hull, const int hsize,
    const Point* s1, const Point* s2,
    Point** path, int* psize) {
    //
    if (pointcmp(s1, s2)) { // same point
        *path = (Point*)malloc(sizeof(Point));
        pointcpy((*path), s1); // copies the first point
        *psize = 1;
        return 0; // Distance when s1 = s2
    }
    sorthull(hull, hsize);

    int i1 = -1, i2 = -1; // index for s1, and index for s2

    for (int i = 0; i < hsize; ++i) {
        if (pointcmp(hull + i, s1)) {
            i1 = i; // set indexe for s1
        } else if (pointcmp(hull + i, s2)) {
            i2 = i; // set index for s2
        }
        if (i1 >= 0 && i2 >= 0) {
            // stops when both indices have been set
            break;
        }
    }

    if (i1 == -1 || i2 == -1) {
        // Points were not in hull
        *path = NULL;
        *psize = -1;
        return -1.0;
    }

    Point *path1, *path2;
    int psize1, psize2;
    double dis1 = calc_path_distance(hull, hsize, i1, i2, &path1, &psize1);
    double dis2 = calc_path_distance(hull, hsize, i2, i1, &path2, &psize2);

    if (dis2 < dis1) {
        free(path1);
        *psize = psize2;
        *path = path2;
        return dis2;
    } else {
        free(path2);
        *psize = psize1;
        *path = path1;
        return dis1;
    }
}



#endif // !_P2_H_
