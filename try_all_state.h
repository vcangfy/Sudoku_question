#ifndef TAS_H
#define TAS_H

#include <array>
#include <iostream>
#include <cstdint>
#include <cmath>
#include <unordered_set>

#include <fstream>
#include <iomanip>
using namespace std;

template <int n>
class Sudoku_try_all {

    array<array<int, n>, n> grid;
    array<array<array<int, n>, n>, 3> sturcture;//行列宫还没有用的数，1表示还没用

    ofstream * file;

    int sqrt_n;
    int ans_count;

    void quick_solveSudoku_DFS(int x, int y);

    void slow_solveSudoku_DFS(int x, int y);

    vector<int> can_choose(int x, int y);

    bool isValid(
        uint32_t row, uint32_t col, int num
    );

public:
    Sudoku_try_all<n>(array<array<int, n>, n> _grid): grid(_grid) {
        sqrt_n = (int)sqrt(n);
        ans_count = 0;
        // array<int, n> temp; temp.fill(1);
        // array<array<int, n>, n> temps; temps.fill(temp);
        // sturcture.fill(temps);

        // for(int x = 0; x < n; x++) {
        //     for(int y = 0; y < n; y++) {
        //         int val = grid[x][y];
        //         if(val) {
        //             --sturcture[0][x][val-1];
        //             --sturcture[1][y][val-1];
        //             --sturcture[2][x/sqrt_n*sqrt_n+y/sqrt_n][val-1];
        //         }
        //     }
        // }
    };

    void printGrid() {
        cout << endl;
        for (uint32_t i = 0; i < n; i++) {
            if(i && i%sqrt_n == 0)
                cout << endl;
            for (uint32_t j = 0; j < n; j++) {
                if(j && j%sqrt_n == 0)
                    cout << "  ";
                if(grid[i][j] > 9) {
                    cout << "  ";
                }
                else cout << " ";
                cout << grid[i][j];
            }
            cout << endl;
        }
        cout << "-------------------" << endl;
    }

    void printGridToJson() {
        // 构建 JSON 结构
        *file << "\n  \"solution_" << ans_count++ << "\": [\n";
        for (uint32_t i = 0; i < n; ++i) {
            *file << "    [";
            for (uint32_t j = 0; j < n; ++j) {
                *file << grid[i][j];
                if (j < n - 1) *file << ", ";  // 自动处理逗号分隔
            }
            *file << "]";
            if (i < n - 1) *file << ",";      // 最后一行的特殊处理
            *file << "\n";
        }
        *file << "  ],";
    }

    void solveSudoku() {
        ofstream outFile("ans.json");
        file = &outFile;
        if (!(*file)) {
            std::cerr << "无法创建 ans.json 文件" << std::endl;
            return;
        }

        *file << "{";

        //quick_solveSudoku_DFS(0, 0);
        slow_solveSudoku_DFS(0, 0);//反而这个更快

        *file << "\n  \"count\": " << ans_count << "\n}";
    }   

};


template <int n>
void Sudoku_try_all<n>::quick_solveSudoku_DFS(int x, int y) {
    int row = n - 1, col = n;
    // 寻找第一个空格

    for (int i = x * n + y; i < n*n; ++i) {
        if(grid[i/n][i%n] == 0) {
            row = i/n;
            col = i%n;
            break;
        }
    }
    // 如果没有空格，打印解
    if (row * n + col == n * n) {
        printGrid();
        //printGridToJson();
        return;
    }

    vector<int> nums = can_choose(row, col);
    for (int num: nums) {
        grid[row][col] = num;
        //printGrid();
        --sturcture[0][row][num-1];
        --sturcture[1][col][num-1];
        --sturcture[2][row/sqrt_n*sqrt_n+col/sqrt_n][num-1];
        quick_solveSudoku_DFS(row, col+1);  // 递归处理下一个空格
        ++sturcture[0][row][num-1];
        ++sturcture[1][col][num-1];
        ++sturcture[2][row/sqrt_n*sqrt_n+col/sqrt_n][num-1];

    }
    grid[row][col] = 0;
}

template <int n>
vector<int> Sudoku_try_all<n>::can_choose(int x, int y) {
    vector<int> optional;
    for(int i = 1; i <= n; ++i) {
        if (sturcture[0][x][i-1] != 0 &&
            sturcture[1][y][i-1] != 0 && 
            sturcture[2][x/sqrt_n*sqrt_n+y/sqrt_n][i-1] != 0
        ){
            optional.push_back(i);
        }
    }
    return optional;
}

template <int n>
void Sudoku_try_all<n>::slow_solveSudoku_DFS(int x, int y) {
    int row = n - 1, col = n;
    // 寻找第一个空格
    for (int i = x * n + y; i < n*n; ++i) {
        if(grid[i/n][i%n] == 0) {
            row = i/n;
            col = i%n;
            break;
        }
    }
    // 如果没有空格，打印解
    if (row * n + col == n * n) {
        //printGrid();
        printGridToJson();
    }
    //尝试填入1-n的数字
    for (int num = 1; num <= n; ++num) {
        if (isValid(row, col, num)) {
            grid[row][col] = num;
            ++col;
            slow_solveSudoku_DFS(row+col/n, col%n);  // 递归处理下一个空格
            --col;
        }
    }
    grid[row][col] = 0;
}

template <int n>
bool Sudoku_try_all<n>::isValid(
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
}
#endif