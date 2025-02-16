#include <iostream>
using namespace std;

int main() {
    int n = 16;
    int sqrt_n = 4;
    for(int i = 0; i < n; ++i) {
        if(i && i % sqrt_n == 0)
            cout << endl;
        cout << "\t" << "array<int,16>{";
        for(int j = 0; j < n; ++j) {
            if(j && j % sqrt_n == 0)
                cout << "  ";
            cout << "  0,";
        }
        cout << "}," << endl;
    }
}