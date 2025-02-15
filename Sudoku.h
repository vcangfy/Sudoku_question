#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <iostream>
#include <functional>

#ifndef SUDOKU_H
#define SUDOKU_H
using namespace std;

namespace std {
    template<> struct hash<std::array<uint32_t, 2>> {
        size_t operator()(const std::array<uint32_t, 2>& arr) const {
            size_t hash = 0;
            hash = (size_t)arr[0] << 32 + (size_t)arr[1];
            return hash;
        }
    };
}

typedef array<uint32_t, 2> position;
typedef array<array<int, 9>, 9> chess;
class Sudoku
{
    chess Chessboard;

    unordered_map<position, unordered_set<int>> lattices_front;
    unordered_map<position, unordered_set<int>> lattices_back;

    //array<array<unordered_set<uint32_t>, 9>, 3> structrue;

    void start_state_to_lattices_front() {
        for (uint32_t i = 0; i < 9; ++i) {
            for (uint32_t j = 0; j < 9; ++j) {
                if (Chessboard[i][j] == 0)
                    lattices_front[{i,j}] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
                else
                lattices_front[{i,j}] = {(Chessboard[i][j])};
            }
        }
    };

    void start_state_to_lattices_back() {
        for (uint32_t i = 0; i < 9; ++i) {
            for (uint32_t j = 0; j < 9; ++j) {
                if (Chessboard[i][j] == 0)
                    lattices_back[{i,j}] = {};
                else {
                    lattices_back[{i,j}] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
                    lattices_back[{i,j}].erase(Chessboard[i][j]);
                }
            }
        }
    };
    
    // void start_state_to_chess() {
    //     structrue[0].fill({1, 2, 3, 4, 5, 6, 7, 8, 9});
    //     structrue[1].fill({1, 2, 3, 4, 5, 6, 7, 8, 9});
    //     structrue[2].fill({1, 2, 3, 4, 5, 6, 7, 8, 9});
    // };

    void eliminate_ambiguity(uint32_t x, uint32_t y, int val, function<void(uint32_t, uint32_t)> action) {
        lattices_front[{x,y}].erase(val);
        lattices_back[{x,y}].insert(val);
        if(lattices_front[{x,y}].size() == 1) {
            Chessboard[x][y] = *(lattices_front[{x,y}].begin());
            action(x, y);
            //structrue[0][i].erase(val);
        }
    }

public:
    Sudoku(chess _Chessboard) : Chessboard(_Chessboard) {
        start_state_to_lattices_front();
        //start_state_to_chess();
    };

    void method_1_WeiYu() {
        queue<position> qu;

        for (uint32_t i = 0; i < 9; i++) {
            for (uint32_t j = 0; j < 9; j++) {
                if (lattices_front[{i,j}].size() == 1) {
                    qu.push({i,j});
                }
            }
        }

        while(qu.size()) {
            auto [i,j] = qu.front();
            int val = Chessboard[i][j];
            qu.pop();
            // 同一行其他未填格子减少可能
            for (uint32_t col = 0; col < 9; ++col) {
                if (col != j && Chessboard[i][col] == 0 
                    && lattices_front[{i,col}].find(val) != lattices_front[{i,col}].end()
                ) {
                    eliminate_ambiguity(i, col, val, [&qu](uint32_t x, uint32_t y) { qu.push({x, y});});
                }
            }
            // 收集同一列其他未填格子减少可能
            for (uint32_t row = 0; row < 9; ++row) {
                if (row != i && Chessboard[row][j] == 0 
                    && lattices_front[{row,j}].find(val) != lattices_front[{row,j}].end()
                ) {
                    eliminate_ambiguity(row, j, val, [&qu](uint32_t x, uint32_t y) { qu.push({x, y});});
                }
            }
            // 收集同一宫其他未填格子减少可能
            uint32_t startRow = i / 3;
            uint32_t startCol = j / 3;
            for (uint32_t x = 0; x < 3; ++x) {
                for (uint32_t y = 0; y < 3; ++y) {
                    uint32_t currentRow = startRow * 3 + x;
                    uint32_t currentCol = startCol * 3 + y;
                    if(currentRow == i && currentCol == j) continue;
                    if (Chessboard[currentRow][currentCol] == 0 && 
                        lattices_front[{currentRow,currentCol}].find(val) != lattices_front[{currentRow,currentCol}].end()
                    ) {
                        eliminate_ambiguity(currentRow, currentCol, val, [&qu](uint32_t x, uint32_t y) { qu.push({x, y});});
                    }
                }
            }
        }
    }
    
    void method_2_PaiChu() {
        for (uint32_t i = 0; i < 9; i++) {
            for (uint32_t j = 0; j < 9; j++) {
                //if (exception_from_bro({i,j})) {;}
            }
        }

    }

    void showChess() {
        for (uint32_t i = 0; i < 9; i++) {
            if(i && i%3 == 0)
                cout << endl;
            for (uint32_t j = 0; j < 9; j++) {
                if(j && j%3 == 0)
                    cout << "   ";
                cout << Chessboard[i][j] << " ";
            }
            cout << endl;
        }
    };

    void showAmbiguity() {
        cout << endl;
        for(const auto& ele: lattices_front) {
            if(ele.second.size() > 1) {            
                cout << "{" << ele.first[0] << ", " 
                << ele.first[1] << "}: ";
                for(int val: ele.second) {
                    cout << val << ", ";
                }
                cout << endl;
            }
        }
    }
};

#endif