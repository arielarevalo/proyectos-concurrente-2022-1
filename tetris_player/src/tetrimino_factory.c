// Copyright 2022 Jose Andres Mena <jose.menaarias@ucr.ac.cr>.

#include <stdlib.h>
#include "include/tetrimino_factory.h"

#define ONE_ROTATION 1
#define TWO_ROTATIONS 2
#define FOUR_ROTATIONS 4
#define INVALID_ROTATIONS -1

tetrimino I[TWO_ROTATIONS] = {{1,  4,  (char *[]){"1111"}},
                       {4,  1,  (char *[]){"1",  "1",  "1",  "1"}}};

tetrimino Z[TWO_ROTATIONS] = {{2,  3,  (char *[]){"220",  "022"}},
                       {3,  2,  (char *[]){"02",  "22",  "20"}}};

tetrimino S[TWO_ROTATIONS] = {{2,  3,  (char *[]){"033",  "330"}},
                       {3,  2,  (char *[]){"30",  "33",  "03"}}};

tetrimino L[FOUR_ROTATIONS] = {
    {2,  3,  (char *[]){"004",  "444"}},
    {3,  2,  (char *[]){"40",  "40",  "44"}},
    {2,  3,  (char *[]){"444",  "400"}},
    {3,  2,  (char *[]){"44",  "04",  "04"}}
};

tetrimino J[FOUR_ROTATIONS] = {
    {2,  3,  (char *[]){"500",  "555"}},
    {3,  2,  (char *[]){"55",  "50",  "50"}},
    {2,  3,  (char *[]){"555",  "005"}},
    {3,  2,  (char *[]){"05",  "05",  "55"}}
};

tetrimino O[ONE_ROTATION] = {{2,  2,  (char *[]){"66",  "66"}}};

tetrimino T[FOUR_ROTATIONS] = {
    {2,  3,  (char *[]){"070",  "777"}},
    {3,  2,  (char *[]){"70",  "77",  "70"}},
    {2,  3,  (char *[]){"777",  "070"}},
    {3,  2,  (char *[]){"07",  "77",  "07"}}
};

int get_tetrimino_num_rotations(char tet) {
    int num_rotations;
    switch (tet) {
        case 'I':
            num_rotations = TWO_ROTATIONS;
            break;
        case 'Z':
            num_rotations = TWO_ROTATIONS;
            break;
        case 'S':
            num_rotations = TWO_ROTATIONS;
            break;
        case 'L':
            num_rotations = FOUR_ROTATIONS;
            break;
        case 'J':
            num_rotations = FOUR_ROTATIONS;
            break;
        case 'O':
            num_rotations = ONE_ROTATION;
            break;
        case 'T':
            num_rotations = FOUR_ROTATIONS;
            break;
        default:
            num_rotations = INVALID_ROTATIONS;
            break;
    }

    return num_rotations;
}

tetrimino* get_tetrimino(char tet, int num_rotations) {
    tetrimino* out;
    int max_num_rotations = get_tetrimino_num_rotations(tet);

    if (num_rotations < 0 || num_rotations >= max_num_rotations) {
        return NULL;
    }

    switch (tet) {
        case 'I':
            out = &I[num_rotations];
            break;
        case 'Z':
            out = &Z[num_rotations];
            break;
        case 'S':
            out = &S[num_rotations];
            break;
        case 'L':
            out = &L[num_rotations];
            break;
        case 'J':
            out = &J[num_rotations];
            break;
        case 'O':
            out = &O[num_rotations];
            break;
        case 'T':
            out = &T[num_rotations];
            break;
        default:
            out = NULL;
            break;
    }

    return out;
}
