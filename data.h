#ifndef DATA_H
#define DATA_H

#include <array>
using namespace std;

namespace data
{
    array<array<int, 9>, 9> board = {
        array<int,9>{4, 0, 0,    0, 7, 0,   0, 8, 0},
        array<int,9>{0, 0, 0,    2, 0, 9,   0, 3, 0},
        array<int,9>{0, 6, 7,    0, 3, 0,   9, 0, 0},

        array<int,9>{0, 5, 0,    0, 0, 0,   0, 9, 2},
        array<int,9>{0, 0, 0,    6, 9, 0,   0, 1, 0},
        array<int,9>{0, 0, 9,    0, 0, 5,   3, 0, 0},

        array<int,9>{0, 4, 8,    0, 0, 0,   0, 0, 0},
        array<int,9>{0, 0, 0,    4, 0, 0,   1, 0, 0},
        array<int,9>{0, 0, 0,    1, 0, 7,   0, 2, 4}
    };

    array<array<int, 16>, 16> super_board = {
        array<int,16>{  8,  0,  4,  0,    0,  0,  0,  0,    2,  0,  0, 13,    0,  5, 12,  0,},
        array<int,16>{  0,  0,  5,  0,    6, 11,  0,  2,    0,  4,  0, 12,    0,  9,  0,  7,},
        array<int,16>{ 13,  0, 10,  0,    0,  0,  0,  0,   16,  0,  7,  0,    8, 15,  0, 11,},
        array<int,16>{  0, 14,  2,  0,    1,  0, 12,  0,    0,  0,  0,  0,   10,  0, 16,  0,},

        array<int,16>{  0, 13,  0,  0,    7,  1,  0,  0,   12,  5,  0, 15,    3, 10,  9,  0,},
        array<int,16>{  0,  0,  0,  7,    0,  0,  0,  0,    0,  6,  3, 10,    0,  1,  0, 14,},
        array<int,16>{  0,  0,  1,  0,    0,  0,  0,  0,    0,  7,  0,  0,    0,  0,  0,  0,},
        array<int,16>{  0,  0, 11,  3,    2, 10,  0, 13,    0,  0,  9,  8,    5,  0,  0,  0,},

        array<int,16>{  0,  0,  7, 15,    0,  0,  0,  0,    9,  1, 10,  0,    0,  0,  0,  4,},
        array<int,16>{  3,  0,  0,  1,    0,  5, 15,  7,    4,  2, 16,  0,   11,  0,  0,  0,},
        array<int,16>{  0,  4,  0,  0,   13,  0,  0,  0,    0,  3,  0,  0,    0,  0, 15,  9,},
        array<int,16>{ 10,  5,  0,  0,   16,  0,  0,  0,   15,  0,  0,  0,    0,  7,  0,  0,},

        array<int,16>{  0,  0,  0,  8,    3,  0,  0,  0,    7,  0,  6,  0,    0, 14,  0,  0,},
        array<int,16>{  0, 10,  0,  5,   11,  0,  7, 12,    0, 14,  0,  0,   16,  0,  0,  0,},
        array<int,16>{ 14,  0,  6,  4,    5, 15, 16,  0,    0,  0,  0,  0,    0,  0, 13,  3,},
        array<int,16>{  0,  0,  0,  0,   10,  0,  0,  0,    0,  0,  0,  0,    7,  0,  0,  1,},
    };
} // namespace data


#endif