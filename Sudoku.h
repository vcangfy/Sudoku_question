#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <iostream>

#ifndef SUDOKU_H
#define SUDOKU_H
using namespace std;

namespace std {
    template<> struct hash<std::array<int, 2>> {
        size_t operator()(const std::array<int, 2>& arr) const {
            size_t hash = 0;
            hash = arr[0] << 32 + arr[1];
            return hash;
        }
    };
}

class Sudoku
{
    array<array<int, 9>, 9> Chessboard;
    unordered_map<array<int, 2>, unordered_set<int>> lattices;
    void state_to_lattices() {
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (Chessboard[i][j] == 0)
                    lattices[{i,j}] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
                else
                lattices[{i,j}] = {(Chessboard[i][j])};
            }
        }
    };

public:
    Sudoku(array<array<int, 9>, 9> _Chessboard) : Chessboard(_Chessboard) {
        state_to_lattices();
    };

    void fillChessboeard() {
        queue<array<int, 2>> qu;

        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (lattices[{i,j}].size() == 1) {
                    qu.push({i,j});
                }
            }
        }

        while(qu.size()) {
            auto [i,j] = qu.front();
            int val = Chessboard[i][j];
            qu.pop();
            // 同一行其他未填格子减少可能
            for (int col = 0; col < 9; ++col) {
                if (col != j && Chessboard[i][col] == 0 
                    && lattices[{i,col}].find(val) != lattices[{i,col}].end()
                ) {
                    lattices[{i,col}].erase(val);
                    if(lattices[{i,col}].size() == 1) {
                        Chessboard[i][col] = *(lattices[{i,col}].begin());
                        qu.push({i,col});
                    }
                }
            }
            // 收集同一列其他未填格子减少可能
            for (int row = 0; row < 9; ++row) {
                if (row != i && Chessboard[row][j] == 0 
                    && lattices[{row,j}].find(val) != lattices[{row,j}].end()
                ) {
                    lattices[{row,j}].erase(val);
                    if(lattices[{row,j}].size() == 1) {
                        Chessboard[row][j] = *(lattices[{row,j}].begin());
                        qu.push({row,j});
                    }
                }
            }
            // 收集同一宫其他未填格子减少可能
            int startRow = (i / 3) * 3;
            int startCol = (j / 3) * 3;
            for (int x = 0; x < 3; ++x) {
                for (int y = 0; y < 3; ++y) {
                    int currentRow = startRow + x;
                    int currentCol = startCol + y;
                    if(currentRow == i && currentCol == j) continue;
                    if (Chessboard[currentRow][currentCol] == 0 && 
                        lattices[{currentRow,currentCol}].find(val) != lattices[{currentRow,currentCol}].end()
                    ) {
                        lattices[{currentRow,currentCol}].erase(val);
                        if(lattices[{currentRow,currentCol}].size() == 1){
                                Chessboard[currentRow][currentCol] = *(lattices[{currentRow,currentCol}].begin());
                                qu.push({currentRow,currentCol});
                            }
                    }
                }
            }
        }
    }

    void showChess() {
        for (int i = 0; i < 9; i++) {
            if(i && i%3 == 0)
                cout << endl;
            for (int j = 0; j < 9; j++) {
                if(j && j%3 == 0)
                    cout << "   ";
                cout << Chessboard[i][j] << " ";
            }
            cout << endl;
        }
    };

};

#endif