#include <iostream>
#include <chrono>
#include "_Sudoku.h"
#include "_try_all_state.h"
#include "data.h"
using namespace std;

int main(){
    auto chess = data::board_extremediffi;
    const int n = chess[0].size();

    Sudoku<n> game = Sudoku<n>(chess);
    
    Sudoku_try_all<n> violate_game = Sudoku_try_all<n>(chess);

    auto start = chrono::high_resolution_clock::now();
    game.fillChessboard();
    violate_game.solveSudoku();
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "It tooks: " << (double)duration.count()/1000.0 << " s" << endl;
}