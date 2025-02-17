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
#include <assert.h>

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
    uint32_t remember_sure;
    bool is_changed;
    
    unordered_set<int> temp;
    int sqrt_n;

    void init_state();

    void eliminate_ambiguity(
        uint32_t, uint32_t, int, function<void(uint32_t, uint32_t)>
    ); 

    // 辅助函数：处理单个单元（行/列/宫）中的数对
    void processUnitNakedPair_explicit(const vector<position>& unit);

    void processUnitNakedPair_implicit(const vector<position>& unit);

public:
    Sudoku<N>(chess<int, N> _Chessboard) : Chessboard(_Chessboard) {
        sqrt_n = (int)sqrt(N);
        init_state();
    };

    void method_1_WeiYu();
    
    void method_2_PaiChu();

    void method_3_QuKuai();

    void method_4_ShuDui();

    void highlevel_method_();

    void fillChessboard_debug() {
        int temp_sure_count = sure.size();
        while(sure.size() != N * N && is_changed) {
            is_changed = false;

            method_1_WeiYu();
            if(sure.size() > temp_sure_count) 
            {cout << endl << "method 1: " << endl; showChess(); temp_sure_count = sure.size();}

            method_3_QuKuai();
            if(sure.size() > temp_sure_count) 
            {cout << endl << "method 3: " << endl; showChess(); temp_sure_count = sure.size();}

            method_4_ShuDui();
            if(sure.size() > temp_sure_count) 
            {cout << endl << "method 4: " << endl; showChess(); temp_sure_count = sure.size();}

            method_2_PaiChu();
            if(sure.size() > temp_sure_count) 
            {cout << endl << "method 2: " << endl; showChess(); temp_sure_count = sure.size();}
        }
    };

    void fillChessboard() {
        while(sure.size() != N * N && is_changed) {
            is_changed = false;

            method_1_WeiYu();
            method_3_QuKuai();
            method_4_ShuDui();
            method_2_PaiChu();
        }
    }

    void showChess();
};

template<int N>
void Sudoku<N>::init_state() {
    remember_sure = 0;
    for(int i = 0; i < N; ++i) {
        temp.insert(i+1);
    }
    for (uint32_t i = 0; i < N; ++i) {
        for (uint32_t j = 0; j < N; ++j) {
            if (Chessboard[i][j] == 0) {
                lattices_front[i][j] = temp;
                lattices_back[i][j] = {};
            }
            else {
                lattices_front[i][j] = {Chessboard[i][j]};
                lattices_back[i][j] = temp;
                lattices_back[i][j].erase(Chessboard[i][j]);
                sure.push_back({i,j});
            }
        }
    }
    is_changed = true;
}

template<int N>
void Sudoku<N>::eliminate_ambiguity(
    uint32_t i, uint32_t j, int val, 
    function<void(uint32_t, uint32_t)> action
) {
    // 同一行其他未填格子减少可能
    for (uint32_t col = 0; col < N; ++col) {
        if (col != j && Chessboard[i][col] == 0 ) {
            lattices_front[i][col].erase(val);
            lattices_back[i][col].insert(val);
            if(lattices_front[i][col].size() == 1) {
                sure.push_back({i,col});
                Chessboard[i][col] = *(lattices_front[i][col].begin());
                action(i,col);
            }
        }
    }
    // 收集同一列其他未填格子减少可能
    for (uint32_t row = 0; row < N; ++row) {
        if (row != i && Chessboard[row][j] == 0) {
            lattices_front[row][j].erase(val);
            lattices_back[row][j].insert(val);
            if(lattices_front[row][j].size() == 1) {
                sure.push_back({row,j});
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
            if (Chessboard[currentRow][currentCol] == 0) {
                lattices_front[currentRow][currentCol].erase(val);
                lattices_back[currentRow][currentCol].insert(val);
                if(lattices_front[currentRow][currentCol].size() == 1) {
                    sure.push_back({currentRow,currentCol});
                    Chessboard[currentRow][currentCol] = *(lattices_front[currentRow][currentCol].begin());
                    action(currentRow,currentCol);
                }
            }
        }
    }
}

template<int N>
void Sudoku<N>::method_1_WeiYu() {
    queue<position> qu;

    for (int i = remember_sure; i < sure.size(); ++i) {
        qu.push(sure[i]);
    }

    while(qu.size()) {
        auto [i,j] = qu.front();
        int others_not_be = Chessboard[i][j];
        qu.pop();

        eliminate_ambiguity(
            i, j, others_not_be, 
            [&qu](uint32_t x,uint32_t y){ 
                qu.push({x,y}); 
            }
        );
        is_changed = true;
    }
    remember_sure = sure.size();
}

template<int N>
void Sudoku<N>::method_2_PaiChu() {
    for (uint32_t i = 0; i < N; ++i) {
        for(uint32_t j = 0; j < N; ++j) {
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
                                goto blockCheck2; // 使用 goto 跳出多重循环
                            }
                        }
                    }
                    blockCheck2:
                    if (blockValid) {
                        canFill = true;
                    }
                }
    
                // 若满足条件，填入数字并更新排除集
                if (canFill) {
                    is_changed = true;

                    Chessboard[i][j] = num;
                    lattices_front[i][j] = {num};
                    lattices_back[i][j] = temp;
                    lattices_back[i][j].erase(num);
                    sure.push_back({i,j});
    
                    eliminate_ambiguity(
                        i, j, num, 
                        [](uint32_t x, uint32_t y) {;}
                    );
    
                    break;
                }
            }
        }
    }
}

template<int N>
void Sudoku<N>::method_3_QuKuai() {
    for (uint32_t i = 0; i < N; ++i) {
        const uint32_t block_row = (i / sqrt_n) * sqrt_n;
        const uint32_t block_col = (i % sqrt_n) * sqrt_n;
    
        // 检查宫内行
        for (uint32_t r = 0; r < sqrt_n; ++r) {
            const uint32_t x = block_row + r;
            int empty_count = 0;
            vector<int> could_be;
            for (uint32_t y = block_col; y < block_col + sqrt_n; ++y) {
                if (Chessboard[x][y] == 0) ++empty_count;
            }
            
            if (empty_count >= 2) {
                for (int num = 1; num <= N; ++num) {
                    bool blockValid = true;
                    // 检查同宫其他行
                    for (uint32_t orow = block_row; orow < block_row + sqrt_n; ++orow) {
                        if (orow == x) continue;
                        for (uint32_t y = block_col; y < block_col + sqrt_n; ++y) {
                            if (lattices_back[orow][y].find(num) == lattices_back[orow][y].end()) {
                                blockValid = false;
                                break;
                            }
                        }
                        if (!blockValid) break;
                    }
                    
                    if (blockValid) {
                        // 排除同行其他单元格
                        could_be.push_back(num);
                        for (uint32_t y_global = 0; y_global < N; ++y_global) {
                            if ((y_global < block_col || y_global >= block_col + sqrt_n) && Chessboard[x][y_global] == 0 ) {
                                //is_changed = true;

                                lattices_front[x][y_global].erase(num);
                                lattices_back[x][y_global].insert(num);
                                if(lattices_front[x][y_global].size() == 1) {
                                    sure.push_back({x,y_global});
                                    Chessboard[x][y_global] = *(lattices_front[x][y_global].begin());
                                }
                            }
                        }
                    }
                }

                // if(could_be.size() == empty_count) {
                // //简单数对
                //     bool can_ShuDui = true;
                //     for (uint32_t y = block_col; y < block_col + sqrt_n; ++y) {
                //         if(Chessboard[x][y] == 0) {
                //             for(int num: could_be) {
                //                 if(lattices_front[x][y].find(num) == lattices_front[x][y].end()) {
                //                     can_ShuDui = false;
                //                     break;
                //                 }
                //             }
                //         }
                //         if(!can_ShuDui) break;
                //     }    
                //     if(can_ShuDui) {
                //         for (uint32_t y = block_col; y < block_col + sqrt_n; ++y) {
                //             if(Chessboard[x][y] == 0) {
                //                 lattices_back[x][y] = temp;
                //                 lattices_front[x][y] = {};
                //                 for(int num: could_be) {
                //                     lattices_back[x][y].erase(num);
                //                     lattices_front[x][y].insert(num);
                //                 }
                //             }
                //         }
                //     } 
                // }
            }
        }
    
        // 新增宫内列判断
        for (uint32_t c = 0; c < sqrt_n; ++c) {
            const uint32_t y = block_col + c;
            int empty_count = 0;
            vector<int> could_be;
            for (uint32_t x = block_row; x < block_row + sqrt_n; ++x) {
                if (Chessboard[x][y] == 0) ++empty_count;
            }
            
            if (empty_count >= 2) {
                for (int num = 1; num <= N; ++num) {
                    bool blockValid = true;
                    // 检查同宫其他列
                    for (uint32_t ocol = block_col; ocol < block_col + sqrt_n; ++ocol) {
                        if (ocol == y) continue;
                        for (uint32_t x = block_row; x < block_row + sqrt_n; ++x) {
                            if (lattices_back[x][ocol].find(num) == lattices_back[x][ocol].end()) {
                                blockValid = false;
                                break;
                            }
                        }
                        if (!blockValid) break;
                    }
                    
                    if (blockValid) {
                        // 排除同列其他单元格
                        could_be.push_back(num);
                        for (uint32_t x_global = 0; x_global < N; ++x_global) {
                            if ((x_global < block_row || x_global >= block_row + sqrt_n) && Chessboard[x_global][y] == 0) {
                                //is_changed = true;
                                
                                lattices_front[x_global][y].erase(num);
                                lattices_back[x_global][y].insert(num);
                                if(lattices_front[x_global][y].size() == 1) {
                                    sure.push_back({x_global,y});
                                    Chessboard[x_global][y] = *(lattices_front[x_global][y].begin());
                                }
                            }
                        }
                    }
                }

                // if(could_be.size() == empty_count) {
                //     //数对
                //     bool can_ShuDui = true;
                //     for (uint32_t x = block_row; x < block_row + sqrt_n; ++x) {
                //         if(Chessboard[x][y] == 0) {
                //             for(int num: could_be) {
                //                 if(lattices_front[x][y].find(num) == lattices_front[x][y].end()) {
                //                     can_ShuDui = false;
                //                     break;
                //                 }
                //             }
                //         }
                //         if(!can_ShuDui) break;
                //     }    
                //     if(can_ShuDui) {
                //         for (uint32_t x = block_row; x < block_row + sqrt_n; ++x) {
                //             if(Chessboard[x][y] == 0) {
                //                 lattices_back[x][y] = temp;
                //                 lattices_front[x][y] = {};
                //                 for(int num: could_be) {
                //                     lattices_back[x][y].erase(num);
                //                     lattices_front[x][y].insert(num);
                //                 }
                //             }
                //         }
                //     }                
                // }
            }
        }
    }
}

template<int N>
void Sudoku<N>::method_4_ShuDui() {
    // 处理所有行
    for (uint32_t row = 0; row < N; ++row) {
        vector<position> unit;
        for (uint32_t col = 0; col < N; ++col) {
            unit.push_back({row, col});
        }
        processUnitNakedPair_explicit(unit);
        processUnitNakedPair_implicit(unit);
    }

    // 处理所有列
    for (uint32_t col = 0; col < N; ++col) {
        vector<position> unit;
        for (uint32_t row = 0; row < N; ++row) {
            unit.push_back({row, col});
        }
        processUnitNakedPair_explicit(unit);
        processUnitNakedPair_implicit(unit);
    }

    // 处理所有宫（3x3）
    for (uint32_t block_row = 0; block_row < sqrt_n; ++block_row) {
        for (uint32_t block_col = 0; block_col < sqrt_n; ++block_col) {
            vector<position> unit;
            for (uint32_t i = 0; i < sqrt_n; ++i) {
                for (uint32_t j = 0; j < sqrt_n; ++j) {
                    uint32_t r = block_row * sqrt_n + i;
                    uint32_t c = block_col * sqrt_n + j;
                    unit.push_back({r, c});
                }
            }
            processUnitNakedPair_explicit(unit);
            processUnitNakedPair_implicit(unit);
        }
    }
}

template<int N>
void Sudoku<N>::processUnitNakedPair_explicit(
    const vector<position>& unit
){
    // 收集候选数数量为2的格子坐标
    vector<position> possible_pairs;
    for (const auto& cell : unit) {
        uint32_t r = cell[0], c = cell[1];
        if (Chessboard[r][c] == 0 && lattices_front[r][c].size() == 2) {
            possible_pairs.push_back({r, c});
        }
    }

    // 寻找候选数完全相同的格子对
    for (size_t i = 0; i < possible_pairs.size(); ++i) {
        for (size_t j = i + 1; j < possible_pairs.size(); ++j) {
            auto& [r1, c1] = possible_pairs[i];
            auto& [r2, c2] = possible_pairs[j];
            // 判断候选数是否严格相同
            if (lattices_front[r1][c1] == lattices_front[r2][c2]) {
                // 获取数对的值
                const auto& pair_values = lattices_front[r1][c1];
                // 处理单元内所有格子
                for (const auto& cell : unit) {
                    uint32_t r = cell[0], c = cell[1];
                    if (Chessboard[r][c] != 0) continue;
                        
                    if ((r == r1 && c == c1) || (r == r2 && c == c2)) {;}
                    else {
                        // 处理其他格子：移除数对值
                        for (int val : pair_values) {
                            if (lattices_front[r][c].count(val)) {
                                is_changed = true;

                                lattices_front[r][c].erase(val);
                                lattices_back[r][c].insert(val);
                                if(lattices_front[r][c].size() == 1) {
                                    Chessboard[r][c] = val;
                                    sure.push_back({r,c});
                                }
                            }
                        }
                    }
                }
                //return; // 每次只处理一个数对
            }
        }
    }
}

template<int N>
void Sudoku<N>::processUnitNakedPair_implicit(
    const vector<position>& unit
) {
    // 遍历所有两两组合
    for (int i = 0; i < unit.size(); ++i) {
        for (int j = i + 1; j < unit.size(); ++j) {
            auto& [x1, y1] = unit[i];
            auto& [x2, y2] = unit[j];
            
            // 跳过已填数字的格子
            if (Chessboard[x1][y1] != 0 || Chessboard[x2][y2] != 0) continue;
            
            unordered_set<int> common;
            for (int num : lattices_front[x1][y1]) {
                if (lattices_front[x2][y2].count(num)) {
                    common.insert(num);
                }
            }
            
            // 公共候选数小于等于2个则跳过
            if (common.size() <= 2) continue;  
            
            // 生成所有可能的数对组合
            vector<int> candidates(common.begin(), common.end());
            for (int a = 0; a < candidates.size(); ++a) {
                for (int b = a + 1; b < candidates.size(); ++b) {
                    int num1 = candidates[a];
                    int num2 = candidates[b];
                    
                    // 检查单元其他格子是否都不含这两个数
                    bool valid = true;
                    for (auto& pos : unit) {
                        auto& [x, y] = pos;
                        if ((x == x1 && y == y1) || (x == x2 && y == y2)) continue;
                        
                        if (lattices_front[x][y].count(num1) || 
                            lattices_front[x][y].count(num2)) {
                            valid = false;
                            break;
                        }
                    }
                    
                    if (valid) {
                        is_changed = true;

                        // 处理第一个格子
                        lattices_front[x1][y1] = {num1, num2};
                        lattices_back[x1][y1] = temp;
                        lattices_back[x1][y1].erase(num1);
                        lattices_back[x1][y1].erase(num2);
                        
                        // 处理第二个格子
                        lattices_front[x2][y2] = {num1, num2};
                        lattices_back[x2][y2] = temp;
                        lattices_back[x2][y2].erase(num1);
                        lattices_back[x2][y2].erase(num2);
                    }
                }
            }
        }
    }
}

template<int N>
void Sudoku<N>::highlevel_method_() {
    ;
}

template<int N>
void Sudoku<N>::showChess() {
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
    cout << "------ ------ ------ ------ ------";
    if(sure.size() == N*N) cout << endl << "Finish. ";
};

#endif