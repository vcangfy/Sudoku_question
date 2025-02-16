#include <iostream>
#include <cmath>
using namespace std;

int main() {
    int n = 25;
    int sqrt_n = (int)sqrt(25);
    for(int i = 0; i < n; ++i) {
        if(i && i % sqrt_n == 0)
            cout << endl;
        cout << "\t" << "array<int," << n << ">{";
        for(int j = 0; j < n; ++j) {
            if(j && j % sqrt_n == 0)
                cout << "  ";
            cout << "  0,";
        }
        cout << "}," << endl;
    }
}