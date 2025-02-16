#ifndef TAS_H
#define TAS_H

#include <array>
#include <iostream>
#include <cstdint>
using namespace std;

bool isValid(const array<array<int, 9>, 9>& grid, uint32_t row, uint32_t col, int num) {
    // 检查当前行是否存在相同数字
    for (int c = 0; c < 9; ++c) {
        if (grid[row][c] == num)
            return false;
    }
    // 检查当前列是否存在相同数字
    for (int r = 0; r < 9; ++r) {
        if (grid[r][col] == num)
            return false;
    }
    // 检查当前3x3宫格是否存在相同数字
    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            if (grid[startRow + r][startCol + c] == num)
                return false;
        }
    }
    return true;
}

void solveSudoku_DFS(array<array<int, 9>, 9>& grid) {
    int row = 8, col = 9;
    // 寻找第一个空格
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (grid[i][j] == 0) {
                row = i;
                col = j;
                goto solve;
            }
        }
    }
    solve:
    // 如果没有空格，打印解
    if (row * 9 + col == 81) {
        cout << endl;
        for (const auto& r : grid) {
            for (int num : r) {
                cout << num << " ";
            }
            cout << endl;
        }
        cout << "-------------------" << endl;
        return;
    }
    // 尝试填入1-9的数字
    for (int num = 1; num <= 9; ++num) {
        if (isValid(grid, row, col, num)) {
            grid[row][col] = num;
            solveSudoku_DFS(grid);  // 递归处理下一个空格
        }
    }
    grid[row][col] = 0;
}

void solveSudoku(array<array<int, 9>, 9> grid) {
    solveSudoku_DFS(grid);
}

#endif