#ifndef SUDOKU_H
#define SUDOKU_H

#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <iostream>
#include <functional>
#include <cmath>
#include <cstdint>//要在模板中用uint_32, 记得引入该库函数

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

//只要用了模板，哪怕是stl的容器，原型和定义就不能分开
typedef array<uint32_t, 2> position;
template <typename T, int N>
using chess = array<array<T, N>, N>;
template<int N>
class Sudoku {
    chess<int, N> Chessboard;

    chess<unordered_set<int>, N> lattices_front;//每个格子可能为
    chess<unordered_set<int>, N> lattices_back;//每个格子必不可能为

    vector<position> sure;
    unordered_set<position> not_sure;
    uint32_t remember_sure;
    int sqrt_n;

    //array<array<unordered_set<uint32_t>, 9>, 3> structrue;

    void init_state() {
        remember_sure = 0;
        unordered_set<int> temp;
        for(int i = 0; i < N; ++i) {
            temp.insert(i+1);
        }
        for (uint32_t i = 0; i < N; ++i) {
            for (uint32_t j = 0; j < N; ++j) {
                if (Chessboard[i][j] == 0) {
                    lattices_front[i][j] = temp;
                    lattices_back[i][j] = {};
                    not_sure.insert({i,j});
                }
                else {
                    lattices_front[i][j] = {(Chessboard[i][j])};
                    lattices_back[i][j] = temp;
                    lattices_back[i][j].erase(Chessboard[i][j]);
                    sure.push_back({i,j});
                }
            }
        }
    };
    
    // void start_state_to_chess() {
    //     structrue[0].fill({1, 2, 3, 4, 5, 6, 7, 8, 9});
    //     structrue[1].fill({1, 2, 3, 4, 5, 6, 7, 8, 9});
    //     structrue[2].fill({1, 2, 3, 4, 5, 6, 7, 8, 9});
    // };

    void eliminate_ambiguity(
        uint32_t i, uint32_t j, int val, 
        function<void(uint32_t, uint32_t)> action
    ) {
            // 同一行其他未填格子减少可能
            for (uint32_t col = 0; col < N; ++col) {
                if (col != j && Chessboard[i][col] == 0 
                    && lattices_front[i][col].find(val) != lattices_front[i][col].end()
                ) {
                    lattices_front[i][col].erase(val);
                    lattices_back[i][col].insert(val);
                    if(lattices_front[i][col].size() == 1) {
                        sure.push_back({i,col});
                        not_sure.erase({i,col});
                        Chessboard[i][col] = *(lattices_front[i][col].begin());
                        action(i,col);
                    }
                }
            }
            // 收集同一列其他未填格子减少可能
            for (uint32_t row = 0; row < N; ++row) {
                if (row != i && Chessboard[row][j] == 0 
                    && lattices_front[row][j].find(val) != lattices_front[row][j].end()
                ) {
                    lattices_front[row][j].erase(val);
                    lattices_back[row][j].insert(val);
                    if(lattices_front[row][j].size() == 1) {
                        sure.push_back({row,j});
                        not_sure.erase({row,j});
                        Chessboard[row][j] = *(lattices_front[row][j].begin());
                        action(row,j);
                    }
                }
            }
            // 收集同一宫其他未填格子减少可能
            uint32_t startRow = i / sqrt_n;
            uint32_t startCol = j / sqrt_n;
            for (uint32_t x = 0; x < sqrt_n; ++x) {
                for (uint32_t y = 0; y < sqrt_n; ++y) {
                    uint32_t currentRow = startRow * sqrt_n + x;
                    uint32_t currentCol = startCol * sqrt_n + y;
                    if(currentRow == i && currentCol == j) continue;
                    if (Chessboard[currentRow][currentCol] == 0 && 
                        lattices_front[currentRow][currentCol].find(val) != lattices_front[currentRow][currentCol].end()
                    ) {
                        lattices_front[currentRow][currentCol].erase(val);
                        lattices_back[currentRow][currentCol].insert(val);
                        if(lattices_front[currentRow][currentCol].size() == 1) {
                            sure.push_back({currentRow,currentCol});
                            not_sure.erase({currentRow,currentCol});
                            Chessboard[currentRow][currentCol] = *(lattices_front[currentRow][currentCol].begin());
                            action(currentRow,currentCol);
                        }
                    }
                }
            }
    }

public:
    Sudoku<N>(chess<int, N> _Chessboard) : Chessboard(_Chessboard) {
        sqrt_n = (int)sqrt(N);
        init_state();
    };

    void method_1_WeiYu() {
        queue<position> qu;

        for (int i = remember_sure; i < sure.size(); ++i) {
            qu.push(sure[i]);
        }

        while(qu.size()) {
            auto [i,j] = qu.front();
            int val = Chessboard[i][j];
            qu.pop();

            eliminate_ambiguity(
                i, j, val, 
                [&qu](uint32_t x,uint32_t y){ 
                    qu.push({x,y}); 
                }
            );
        }
        remember_sure = sure.size();
    };
    
    void method_2_PaiChu() {
        for (auto [i, j]: not_sure) {
            if (Chessboard[i][j] != 0) {
                continue; // 跳过已填的格子
            }

            // 检查每个可能的数字
            for (int num : lattices_front[i][j]) {
                bool canFill = false;

                // 检查行：同一行其他格子的排除集是否都包含 num
                bool rowValid = true;
                for (int col = 0; col < N; ++col) {
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
                    for (int row = 0; row < N; ++row) {
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
                    int blockRow = (i / sqrt_n) * sqrt_n;
                    int blockCol = (j / sqrt_n) * sqrt_n;
                    for (int x = blockRow; x < blockRow + sqrt_n; ++x) {
                        for (int y = blockCol; y < blockCol + sqrt_n; ++y) {
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

                    eliminate_ambiguity(
                        i, j, num, 
                        [](uint32_t x, uint32_t y) {;}
                    );

                    break;
                }
            }
        }
    };

    void method_3_QuKuai() {

    };

    void fillChessboard() {
        while(remember_sure != sure.size()) {
            method_1_WeiYu();
            //showChess();
            method_2_PaiChu();
            //showChess();
        }
    };

    void showChess() {
        cout << endl;
        for (uint32_t i = 0; i < N; i++) {
            if(i && i%sqrt_n == 0)
                cout << endl;
            for (uint32_t j = 0; j < N; j++) {
                if(j && j%sqrt_n == 0)
                    cout << "  ";
                if(Chessboard[i][j] > 9) {;}
                else cout << " ";
                if(!Chessboard[i][j]) {
                    cout << "\x1b[38;5;255m" << Chessboard[i][j] << "\x1b[0m" << " ";
                }
                else {
                    cout << "\x1b[38;5;0m" << Chessboard[i][j] << "\x1b[0m" << " ";
                }
            }
            cout << endl;
        }
        cout << "------ ------ ------ ------";
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