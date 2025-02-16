#ifndef TAS_H
#define TAS_H

#include <array>
#include <iostream>
#include <cstdint>
#include <cmath>
using namespace std;

template <int n>
class Sudoku_try_all {

    array<array<int, n>, n> grid;
    
    void solveSudoku_DFS(int x, int y) {
        int row = n - 1, col = n;
        // 寻找第一个空格
        for (int i = x; i < n; ++i) {
            for (int j = y; j < n; ++j) {
                if (grid[i][j] == 0) {
                    row = i;
                    col = j;
                    goto solve;
                }else {
                    
                }
            }
        }
        solve:
        // 如果没有空格，打印解
        if (row * n + col == n * n) {
            cout << endl;
            for (const auto& r : grid) {
                for (int num : r) {
                    cout << num;
                    if(num > 9) cout << " ";
                    else cout << "  ";
                }
                cout << endl;
            }
            cout << "-------------------" << endl;
            return;
        }
        //尝试填入1-n的数字
        for (int num = 1; num <= n; ++num) {
            if (isValid(row, col, num)) {
                grid[row][col] = num;
                ++col;
                solveSudoku_DFS(row+col/n, col%n);  // 递归处理下一个空格
                --col;
            }
        }
        grid[row][col] = 0;
    };

    bool isValid(
        uint32_t row, 
        uint32_t col, 
        int num
    ) {
        // 检查当前行是否存在相同数字
        for (int c = 0; c < n; ++c) {
            if (grid[row][c] == num)
                return false;
        }
        // 检查当前列是否存在相同数字
        for (int r = 0; r < n; ++r) {
            if (grid[r][col] == num)
                return false;
        }
        // 检查当前3x3宫格是否存在相同数字
        int n_sqrt = (int)sqrt(n);
        int startRow = row - row % n_sqrt;
        int startCol = col - col % n_sqrt;
        for (int r = 0; r < n_sqrt; ++r) {
            for (int c = 0; c < n_sqrt; ++c) {
                if (grid[startRow + r][startCol + c] == num)
                    return false;
            }
        }
        return true;
    };

public:

    Sudoku_try_all<n>(array<array<int, n>, n> _grid): grid(_grid) {;};

    void solveSudoku() {
        solveSudoku_DFS(0, 0);
    };   

};

#endif