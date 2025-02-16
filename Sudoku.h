#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <iostream>
#include <functional>
#include <cstdint>//要在模板中用uint_32, 记得引入该库函数

#ifndef SUDOKU_H
#define SUDOKU_H
using namespace std;

//只要用了模板，哪怕是stl的容器，原型和定义就不能分开
typedef array<uint32_t, 2> position;
template <typename T>
using chess = array<array<T, 9>, 9>;
class Sudoku
{
    chess<int> Chessboard;

    chess<unordered_set<int>> lattices_front;
    chess<unordered_set<int>> lattices_back;

    //array<array<unordered_set<uint32_t>, 9>, 3> structrue;

    void start_state_to_lattices_front() {
        for (uint32_t i = 0; i < 9; ++i) {
            for (uint32_t j = 0; j < 9; ++j) {
                if (Chessboard[i][j] == 0)
                    lattices_front[i][j] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
                else
                lattices_front[i][j] = {(Chessboard[i][j])};
            }
        }
    };

    void start_state_to_lattices_back() {
        for (uint32_t i = 0; i < 9; ++i) {
            for (uint32_t j = 0; j < 9; ++j) {
                if (Chessboard[i][j] == 0)
                    lattices_back[i][j] = {};
                else {
                    lattices_back[i][j] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
                    lattices_back[i][j].erase(Chessboard[i][j]);
                }
            }
        }
    };
    
    // void start_state_to_chess() {
    //     structrue[0].fill({1, 2, 3, 4, 5, 6, 7, 8, 9});
    //     structrue[1].fill({1, 2, 3, 4, 5, 6, 7, 8, 9});
    //     structrue[2].fill({1, 2, 3, 4, 5, 6, 7, 8, 9});
    // };

    void eliminate_ambiguity(uint32_t i, uint32_t j, int val, function<void(uint32_t, uint32_t)> action) {
            // 同一行其他未填格子减少可能
            for (uint32_t col = 0; col < 9; ++col) {
                if (col != j && Chessboard[i][col] == 0 
                    && lattices_front[i][col].find(val) != lattices_front[i][col].end()
                ) {
                    lattices_front[i][col].erase(val);
                    lattices_back[i][col].insert(val);
                    if(lattices_front[i][col].size() == 1) {
                        Chessboard[i][col] = *(lattices_front[i][col].begin());
                        action(i,col);
                    }
                }
            }
            // 收集同一列其他未填格子减少可能
            for (uint32_t row = 0; row < 9; ++row) {
                if (row != i && Chessboard[row][j] == 0 
                    && lattices_front[row][j].find(val) != lattices_front[row][j].end()
                ) {
                    lattices_front[row][j].erase(val);
                    lattices_back[row][j].insert(val);
                    if(lattices_front[row][j].size() == 1) {
                        Chessboard[row][j] = *(lattices_front[row][j].begin());
                        action(row,j);
                    }
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
                        lattices_front[currentRow][currentCol].find(val) != lattices_front[currentRow][currentCol].end()
                    ) {
                        lattices_front[currentRow][currentCol].erase(val);
                        lattices_back[currentRow][currentCol].insert(val);
                        if(lattices_front[currentRow][currentCol].size() == 1) {
                            Chessboard[currentRow][currentCol] = *(lattices_front[currentRow][currentCol].begin());
                            action(currentRow,currentCol);
                        }
                    }
                }
            }
    }

public:
    Sudoku(chess<int> _Chessboard) : Chessboard(_Chessboard) {
        start_state_to_lattices_front();
        start_state_to_lattices_back();
        //start_state_to_chess();
    };

    void method_1_WeiYu() {
        queue<position> qu;

        for (uint32_t i = 0; i < 9; i++) {
            for (uint32_t j = 0; j < 9; j++) {
                if (lattices_front[i][j].size() == 1) {
                    qu.push({i,j});
                }
            }
        }

        while(qu.size()) {
            auto [i,j] = qu.front();
            int val = Chessboard[i][j];
            qu.pop();

            eliminate_ambiguity(i, j, val, [&qu](uint32_t x,uint32_t y){ qu.push({x,y});});
        }
    }
    
    void method_2_PaiChu() {
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (Chessboard[i][j] != 0) {
                    continue; // 跳过已填的格子
                }
    
                // 检查每个可能的数字
                for (int num : lattices_front[i][j]) {
                    bool canFill = false;
    
                    // 检查行：同一行其他格子的排除集是否都包含 num
                    bool rowValid = true;
                    for (int col = 0; col < 9; ++col) {
                        if (col == j) continue;
                        if (lattices_back[i][col].find(num) == lattices_back[i][col].end()) {
                            //伙伴格子必不可能的数没有该数
                            rowValid = false;
                            break;
                        }
                    }
                    if (rowValid) {
                        canFill = true;
                    }
    
                    // 检查列：同一列其他格子的排除集是否都包含 num
                    bool colValid = true;
                    if (!canFill) { // 若行已满足，无需再检查
                        for (int row = 0; row < 9; ++row) {
                            if (row == i) continue;
                            if (lattices_back[row][j].find(num) == lattices_back[row][j].end()) {
                                colValid = false;
                                break;
                            }
                        }
                        if (colValid) {
                            canFill = true;
                        }
                    }
    
                    // 检查宫：同一宫其他格子的排除集是否都包含 num
                    bool blockValid = true;
                    if (!canFill) { // 若行或列已满足，无需再检查
                        int blockRow = (i / 3) * 3;
                        int blockCol = (j / 3) * 3;
                        for (int x = blockRow; x < blockRow + 3; ++x) {
                            for (int y = blockCol; y < blockCol + 3; ++y) {
                                if (x == i && y == j) continue;
                                if (lattices_back[x][y].find(num) == lattices_back[x][y].end()) {
                                    blockValid = false;
                                    goto blockCheckEnd; // 使用 goto 跳出多重循环
                                }
                            }
                        }
                        blockCheckEnd:
                        if (blockValid) {
                            canFill = true;
                        }
                    }
    
                    // 若满足条件，填入数字并更新排除集
                    if (canFill) {
                        Chessboard[i][j] = num;
    
                        eliminate_ambiguity(i, j, num, [](uint32_t x, uint32_t y) {;});
    
                        return ;
                    }
                }
            }
        }
    }

    void fillChessboard() {
        method_1_WeiYu();
        method_2_PaiChu();
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
    //     for(const auto& ele: lattices_front) {
    //         if(ele.second.size() > 1) {            
    //             cout << "{" << ele.first[0] << ", " 
    //             << ele.first[1] << "}: ";
    //             for(int val: ele.second) {
    //                 cout << val << ", ";
    //             }
    //             cout << endl;
    //         }
    //     }
    }
};

#endif