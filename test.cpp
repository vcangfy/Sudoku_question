#include <iostream>
#include <chrono>
#include "Sudoku.h"
#include "try_all_state.h"
#include "data.h"
using namespace std;

int main(){
    auto chess = data::board_highdiffi;
    const int n = chess[0].size();

    Sudoku<n> game = Sudoku<n>(chess);
    
    Sudoku_try_all<n> violate_game = Sudoku_try_all<n>(chess);

    auto start = chrono::high_resolution_clock::now();
    game.fillChessboard();
    //violate_game.solveSudoku();
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    game.showChess();
    cout << "It tooks: " << (double)duration.count()/1000.0 << " s" << endl;
}