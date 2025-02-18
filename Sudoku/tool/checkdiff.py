import json

def compare_sudoku_solutions():
    try:
        with open('ans.json', 'r') as f:
            data = json.load(f)
        
        solutions = data['solution']
        if not solutions:
            raise ValueError("解决方案列表为空")
        
        # 生成差异矩阵
        diff = []
        for i in range(9):
            row = []
            for j in range(9):
                base = solutions[0][i][j]
                all_match = all(sol[i][j] == base for sol in solutions)
                row.append(base if all_match else 0)
            diff.append(row)
        
        # 手动构建格式化输出
        diff_str = '[\n    ' + ',\n    '.join(
            '[' + ', '.join(map(str, row)) + ']' for row in diff
        ) + '\n  ]'
        
        final_json = '{\n  "diff": ' + diff_str + '\n}'
        
        with open('diff.json', 'w') as f:
            f.write(final_json)
        
        print("差异文件已生成：diff.json")

    except Exception as e:
        print(f"处理出错: {str(e)}")

if __name__ == "__main__":
    compare_sudoku_solutions()