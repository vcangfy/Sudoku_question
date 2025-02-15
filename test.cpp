#include <vector>
#include <iostream>
#include <array>
#include "Sudoku.h"
using namespace std;

// namespace std {
//     template<> struct hash<std::array<int, 2>> {
//         size_t operator()(const std::array<int, 2>& arr) const {
//             size_t hash = 0;
//             hash = arr[0] << 32 + arr[1];
//             return hash;
//         }
//     };
// }

int main(){

    array<array<int, 9>, 9> board = {
        array<int,9>{5, 0, 0,    0, 0, 0,   0, 6, 8},
        array<int,9>{3, 0, 0,    0, 5, 0,   4, 0, 0},
        array<int,9>{8, 0, 0,    6, 2, 9,   0, 0, 0},

        array<int,9>{0, 0, 4,    0, 6, 1,   0, 0, 7},
        array<int,9>{0, 0, 3,    0, 0, 0,   0, 8, 1},
        array<int,9>{0, 1, 0,    7, 0, 5,   0, 0, 0},

        array<int,9>{0, 3, 5,    2, 0, 7,   0, 0, 0},
        array<int,9>{0, 6, 0,    0, 0, 3,   2, 4, 0},
        array<int,9>{0, 0, 0,    9, 1, 0,   5, 0, 3}
    };

    Sudoku game = Sudoku(board);
    game.fillChessboeard();
    game.showChess();

}