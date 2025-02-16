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
        array<int,9>{0, 0, 6,    0, 0, 4,   0, 9, 5},
        array<int,9>{0, 0, 4,    0, 0, 8,   0, 0, 6},
        array<int,9>{0, 5, 0,    9, 0, 0,   0, 0, 3},

        array<int,9>{0, 0, 0,    8, 3, 0,   0, 7, 0},
        array<int,9>{6, 0, 2,    0, 9, 0,   8, 0, 0},
        array<int,9>{8, 3, 0,    0, 0, 0,   1, 0, 0},

        array<int,9>{0, 0, 0,    1, 0, 5,   6, 0, 0},
        array<int,9>{0, 6, 7,    4, 0, 0,   0, 5, 0},
        array<int,9>{5, 0, 0,    0, 2, 9,   0, 0, 0}
    };

    Sudoku game = Sudoku(board);
    game.fillChessboard();
    game.showChess();
    //game.showAmbiguity();

}