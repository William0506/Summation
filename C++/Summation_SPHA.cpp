#include "sphamod.h"
#include <algorithm>
#include <bitset>
#include <cmath>
#include <complex>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

int factorial(int n) {
    if (n == 0 || n == 1) return 1;
    return n * factorial(n - 1);
}

vector<long double> B = {};

bool containsInfOrNaN(const vector<long double>& vec) {
    return std::any_of(vec.begin(), vec.end(), [](long double x) { return std::isinf(x) || std::isnan(x); });
}

long double sumAbsValues(const vector<long double>& vec) {
    return std::accumulate(vec.begin(), vec.end(), 0.0L,
        [](long double sum, long double val) { return sum + std::abs(val); });
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    ifstream ifs("B.txt", ios::in);
    string s;
    while (getline(ifs, s)) {
        B.push_back(strtold(s.c_str(), nullptr));
    }
    ifs.close();
    long double n;
    int cx, dx, ex, l;
    vector<long double> k;
    while (true) {
        string input;
        cout << "upper bound of summation(enter 'exit' or 'cls' to exit)?" << flush;
        getline(cin, input);
        try {
            n = stod(input);
            break;
        } catch (...) {
            cout << "upper bound of summation must be a real number." << endl;
        }
    }
    while (true) {
        string input;
        cout << "field num (three non-negative integers:cx,dx,ex) (enter 'exit' or 'cls' to exit)?" << flush;
        getline(cin, input);
        stringstream ss(input);
        string token;
        vector<int> values;
        while (getline(ss, token, ',')) {
            try {
                int value = stoi(token);
                if (value < 0) throw invalid_argument("Negative value");
                values.push_back(value);
            } catch (...) {
                values.clear();
                break;
            }
        }
        if (values.size() == 3) {
            cx = values[0];
            dx = values[1];
            ex = values[2];
            break;
        }
        cout << "field num must be three non-negative integers separated by a comma." << endl;
    }
    if (n < 0 && cx == 0 && dx == 0 && ex == 0) {
        cx = 1;
    }
    spha spha_obj(cx, dx, ex);
    int g = cx + dx + ex;
    vector<long double> r2(1 << g, 0);
    long double d0=numeric_limits<long double>::max();
    while (true) {
        string input;
        cout << "power(Using ',' to separate " << (1 << g) << " parts.)(enter 'exit' or 'cls' to exit)?" << flush;
        getline(cin, input);
        stringstream ss(input);
        string token;
        vector<long double> values;
        while (getline(ss, token, ',')) {
            try {
                values.push_back(stod(token));
            } catch (...) {
                values.clear();
                break;
            }
        }
        if (values.size() == (1 << g)) {
            k = spha_obj.md(values);
            break;
        }
        cout << "power must be " << (1 << g) << " numbers in the field you set." << endl;
    }
    while (true) {
        string input;
        cout << "precision(must be a natural number)(enter 'exit' or 'cls' to exit)?"<< flush;
        getline(cin, input);
        try {
            l = stoi(input);
            if (l < 0) throw invalid_argument("Negative value");
            if (l > 1000) l = 1000;
            if (fmod(k[0], 1) == 0 && l > k[0] && k[0] > 0 && all_of(k.begin() + 1, k.end(), [](long double x) { return x == 0; })) {
                l = static_cast<int>(k[0]) + 1;
            }
            break;
        } catch (...) {
            cout << "precision must be a natural number." << endl;
        }
    }
    vector<long double> r1;
    if (all_of(k.begin() + 1, k.end(), [](long double x) { return x == 0; }) && k[0] != -1) {
        r1 = spha_obj.md({riemann_zetal(-k[0])});
    } else if (k[0] == -1) {
        r1 = spha_obj.md({0.577215664901532860606512090082402431042159335});
    } else {
        r1 = vector<long double>(1 << g, 0);
    }
    for (int a = 0; a <= l; ++a) {
        if (a % 2 == 1 && a != 1) {
            continue;
        }
        long double d_temp = B[a] / factorial(a);
        vector<long double> d_temp_vec = {d_temp};
        d_temp_vec.resize(1 << g, 0);
        vector<long double> k_temp = k;
        k_temp[0] -= a - 1;
        d_temp_vec = spha_obj.md1(d_temp_vec, spha_obj.mp(n, k_temp));
        if (a == 0 && k[0] == -1 && all_of(k.begin() + 1, k.end(), [](long double x) { return x == 0; })) {
            if (n > 0) {
            d_temp_vec = spha_obj.md({log(n)});
            } else {
                d_temp_vec = spha_obj.md({numeric_limits<long double>::infinity()});
            }
            r1+=d_temp_vec;
            continue;
        } else if (a > 0) {
            for (int b = 0; b < a - 1; ++b) {
                vector<long double> temp = {a + fmod(k[0], 1) - (fmod(k[0], 1) != 0 ? 1 : 0) - b};
                temp.resize(1 << g, 0);
                d_temp_vec = spha_obj.md1(d_temp_vec, temp);
                if (k[0] != b) {
                    vector<long double> temp2 = k;
                    for (size_t i = 1; i < temp2.size(); ++i) {
                        temp2[i] /= (k[0] - b);
                    }
                    temp2[0] = 1;
                    d_temp_vec = spha_obj.md1(d_temp_vec, temp2);
                } else {
                    if (k[0] != -1) {
                        vector<long double> temp = {fmod(k[0], 1) - (fmod(k[0], 1) != 0 ? 1 : 0) + 1};
                        temp.resize(1 << g, 0);
                        d_temp_vec = spha_obj.md2(d_temp_vec, temp);
                        vector<long double> temp2 = k;
                        for (size_t i = 1; i < temp2.size(); ++i) {
                            temp2[i] /= (k[0] + 1);
                        }
                        temp2[0] = 1;
                        d_temp_vec = spha_obj.md2(d_temp_vec, temp2);
                    } else {
                        vector<long double> temp2 = k;
                        temp2[0] = 0;
                        d_temp_vec = spha_obj.md2(d_temp_vec, temp2);
                    }
                }
            }
        } else {
            vector<long double> temp = {fmod(k[0], 1) - (fmod(k[0], 1) != 0 ? 1 : 0) + 1};
            temp.resize(1 << g, 0);
            d_temp_vec = spha_obj.md2(d_temp_vec, temp);
            vector<long double> temp2 = k;
            for (size_t i = 1; i < temp2.size(); ++i) {
                temp2[i] /= (k[0] + 1);
            }
            temp2[0] = 1;
            d_temp_vec = spha_obj.md2(d_temp_vec, temp2);
        }
        if (a == 1) {
        } else if (a == 0 && k[0] == -1 && all_of(k.begin() + 1, k.end(), [](long double x) { return x == 0; })) {
            vector<long double> temp = {log(n)};
            temp.resize(1 << g, 0);
            d_temp_vec = spha_obj.md1(d_temp_vec, temp);
        } else if (ceil(k[0]) - a > -1) {
            for (int c = 1; c <= ceil(k[0]) - a ; ++c) {
                vector<long double> temp = {(c + fmod(k[0], 1) - (fmod(k[0], 1) == 0 ? 0 : 1) + a) / (c + fmod(k[0], 1) - (fmod(k[0], 1) == 0 ? 0 : 1) + 1)};
                temp.resize(1 << g, 0);
                d_temp_vec = spha_obj.md1(d_temp_vec, temp);
            }
        } else {
            for (int c = ceil(k[0]) - a + 1; c < 1; ++c) {
                if (c == -a && fmod(k[0], 1) == 0) {
                    vector<long double> temp = {1.0 - a};
                    temp.resize(1 << g, 0);
                    d_temp_vec = spha_obj.md2(d_temp_vec, temp);
                } else if (c == -1 && fmod(k[0], 1) == 0) {
                    vector<long double> temp = {static_cast<long double>(a - 1)};
                    temp.resize(1 << g, 0);
                    d_temp_vec = spha_obj.md1(d_temp_vec, temp);
                } else {
                    vector<long double> temp = {(c + fmod(k[0], 1) - (fmod(k[0], 1) != 0 ? 1 : 0) + a) / (c + fmod(k[0], 1) - (fmod(k[0], 1) != 0 ? 1 : 0) + 1)};
                    temp.resize(1 << g, 0);
                    d_temp_vec = spha_obj.md2(d_temp_vec, temp);
                }
            }
        }
        if (containsInfOrNaN(d_temp_vec) or sumAbsValues(d_temp_vec)>d0) {
                break;
        }
        r1+=d_temp_vec;
        d0=sumAbsValues(d_temp_vec);
    }
    cout << "Approximation: ";
    cout << r1 <<endl;

    if (n > 0 && fmod(n, 1) == 0) {
        for (int i = 1; i <= n; i++) {
            r2 += spha_obj.mp(i,k);
        }
        cout << "Actual: ";
        cout << r2 <<endl;
        cout << "Error: ";
        cout << r1-r2 <<endl;
    } else if (fmod(n, 1) == 0) {
        for (int i = n + 1; i < 1; i++) {
            r2 -= spha_obj.mp(i,k);
        }
        cout << "Actual: ";
        cout << r2 <<endl;
        cout << "Error: ";
        cout << r1-r2 <<endl;
    } else {
        string r3 = "undefined";
        cout << r3 << endl;
    }
    cin.get();
    return 0;
}
