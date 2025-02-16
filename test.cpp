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

    Sudoku game = Sudoku(board);
    game.fillChessboard();
    game.showChess();
    //game.showAmbiguity();

}