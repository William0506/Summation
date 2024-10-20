#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <fstream>
using namespace std;

int calculateBernoulliNumbers(int n) {
    ofstream ofs;
    vector<long double> B(n + 1);
    vector<long double> A(n + 1, 0);
    ofs.open("B.txt");
    for (int m = 0; m <= n; m++) {
        A[m] = 1.0 / (m + 1);
        for (int j = m; j > 0; j--) {
            A[j - 1] = j * (A[j - 1] - A[j]);
        }
        B[m] = A[0];

        // 設置奇數索引（除了B[1]）的伯努利數為0
        if (m > 1 && m % 2 == 1) {
            B[m] = 0;
        }
        if (abs(B[m]) < 1e-10) {
            ofs << "0.0" << "\n";
        } else {
            ofs << fixed << setprecision(15) << B[m] << "\n";
        }
    }
    ofs.close();
    return 0;
}

int main() {
    int n;
    cin >> n;
    calculateBernoulliNumbers(n);
    return 0;
}
