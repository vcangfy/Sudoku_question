//from deepseek
#ifndef JSS_H
#define JSS_H

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>

using namespace std;
//用到了模板的话，就必须要要把函数定义与头文件写在一起。

template <typename T>
struct Problem{
    const T id;
    int val;
    const vector<T> prerequisites;

    Problem<T>(T _id, vector<T> _prerequisites): 
        id(_id), prerequisites(_prerequisites) {
            val = -1;
        };
};

template <typename T>
pair<bool, bool> checkSolvability(vector<Problem<T>>& problems) {
    unordered_set<T> explicit_ids;//显式问题：有前置问题的问题
    for (const Problem<T>& p : problems) {
        explicit_ids.insert(p.id);
    }

    unordered_set<T> implicit_ids;//隐式问题：初始无前置问题的问题
    for (const Problem<T>& p : problems) {
        for (T u : p.prerequisites) {
            if (explicit_ids.find(u) == explicit_ids.end()) {
                //T需要有==符号函数
                implicit_ids.insert(u);
            }
        }
    }

    unordered_set<T> all_ids = explicit_ids;//所有问题
    all_ids.insert(implicit_ids.begin(), implicit_ids.end());

    unordered_map<T, vector<T>> all_prerequisites;//将问题族用字典结构存储
    for (const Problem<T>& p : problems) {
        unordered_set<T> seen;
        vector<T> unique_prereq;
        for (T u : p.prerequisites) {
            if (seen.insert(u).second) {
                unique_prereq.push_back(u);
            }
        }
        all_prerequisites[p.id] = unique_prereq;
    }
    for (T u : implicit_ids) {
        all_prerequisites[u] = {};
    }//完善问题族：
    //出题的时候问题族中有些问题提出来了但是没有前置问题
    //但是还需要把问题族中没有提出的问题(默认为已被解决的问题)显式的加入到问题族中
    //all_prerequisites字典的结构为：“id”:"{该id所需的前置问题s的id}"

    unordered_map<T, vector<T>> reverse_adj;
    unordered_map<T, int> in_degree;

    for (T id : all_ids) {
        const vector<T>& pre = all_prerequisites[id];
        //pre：“id”为id的前置问题们
        in_degree[id] = pre.size();
        //in_degree：该id的前置问题个数
        for (T u : pre) {
            reverse_adj[u].push_back(id);
            //reverse_adj结构为：“id”：“{该id能解决的ids}”
        }
    }//

    queue<T> q;
    unordered_map<T, bool> solvable;
    //solvable字典结构为：“id”:“该id的可解性”

    for (T id : all_ids) {
        if (in_degree[id] == 0) {
            solvable[id] = true;
            q.push(id);
            //将前置问题为0的问题标记为可解并压入栈
        }
        //else solvable[id] = false;
        // 非为0的id标记为不可解，扩充确认solvable存有有全部id
    }

    while (!q.empty()) {
        T u = q.front();
        q.pop();
        //获取问题族可解问题的id为u
        for (T v : reverse_adj[u]) {
            //读取解决u后的可解问题id v
            if (--in_degree[v] == 0) {
                solvable[v] = true;
                q.push(v);
                //v的前置问题数量减1，若为0则标记为可解并压入栈
            }
        }
    }

    int total = all_ids.size();
    int solvable_count = 0;
    for (const auto& kv : solvable) {
        if (kv.second) solvable_count++;
    }

    bool is_completely_unsolvable = (solvable_count == 0);
    bool is_partially_unsolvable = (solvable_count > 0 && solvable_count < total);

    return {is_completely_unsolvable, is_partially_unsolvable};
}

#endif