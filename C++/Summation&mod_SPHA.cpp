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
class spha {
private:
    int x, y, z;
    vector<long double> data;
    vector<long double> concatenate(const vector<long double>& a, const vector<long double>& b) const {
        vector<long double> result = a;
        result.insert(result.end(), b.begin(), b.end());
        return result;
    }
    vector<long double> slice(const vector<long double>& v, int start, int end) const {
        return vector<long double>(v.begin() + start, v.begin() + end);
    }
    int ps(int index) const {
        if (index >= (1 << (x+y))) return 0;
        string binary = bitset<32>(index).to_string().substr(32 - (x+y+z));
        int ones_count = count(binary.begin() + y + z, binary.end(), '1');
        return (ones_count % 2 == 0) ? 1 : -1;
    }
public:
    spha(int x, int y, int z) : x(x), y(y), z(z) {}

    vector<long double> md(const vector<long double>& args) const {
        return vector<long double>(args.begin(), args.begin() + pow(2, x + y + z));
    }
    vector<long double> md1(const vector<long double>& a, const vector<long double>& b) const {
        int n = x + y + z;
        if (n == 0) return {a[0] * b[0]};
        if (n == 1) {
            if (x == 1) return {a[0]*b[0] - a[1]*b[1], a[0]*b[1] + a[1]*b[0]};
            else if (y == 1) return {a[0]*b[0] + a[1]*b[1], a[0]*b[1] + a[1]*b[0]};
            else return {a[0]*b[0], a[0]*b[1] + a[1]*b[0]};
        } else {
            int half = pow(2, n-1);
            vector<long double> a1 = slice(a, 0, half), a2 = slice(a, half, a.size());
            vector<long double> b1 = slice(b, 0, half), b2 = slice(b, half, b.size());
            vector<long double> c1, c2;
            if (z > 0) {
                spha subspha(x, y, z-1);
                c1 = subspha.md1(a1, b1);
                c2 = subspha.md1(a1, b2);
                transform(c2.begin(), c2.end(), subspha.md1(a2, b1).begin(), c2.begin(), plus<long double>());
            } else if (y > 0) {
                spha subspha(x, y-1, z);
                c1 = subspha.md1(a1, b1);
                transform(c1.begin(), c1.end(), subspha.md1(a2, b2).begin(), c1.begin(), plus<long double>());
                c2 = subspha.md1(a1, b2);
                transform(c2.begin(), c2.end(), subspha.md1(a2, b1).begin(), c2.begin(), plus<long double>());
            } else {
                spha subspha(x-1, y, z);
                c1 = subspha.md1(a1, b1);
                transform(c1.begin(), c1.end(), subspha.md1(a2, b2).begin(), c1.begin(), minus<long double>());
                c2 = subspha.md1(a1, b2);
                transform(c2.begin(), c2.end(), subspha.md1(a2, b1).begin(), c2.begin(), plus<long double>());
            }
            return concatenate(c1, c2);
        }
    }
    vector<long double> md2(const vector<long double>& a, const vector<long double>& b) const {
        int n = x + y + z;
        if (n == 0) return {a[0] / b[0]};
        if (n == 1) {
            if (x == 1) {
                long double denominator = b[0]*b[0] + b[1]*b[1];
                return {(a[0]*b[0] + a[1]*b[1]) / denominator, (a[1]*b[0] - a[0]*b[1]) / denominator};
            } else if (y == 1) {
                long double denominator = b[0]*b[0] - b[1]*b[1];
                return {(a[0]*b[0] - a[1]*b[1]) / denominator, (a[1]*b[0] - a[0]*b[1]) / denominator};
            } else {
                return {a[0]*b[0] / (b[0]*b[0]), (a[1]*b[0] - a[0]*b[1]) / (b[0]*b[0])};
            }
        } else {
            int half = pow(2, n-1);
            vector<long double> b1 = slice(b, 0, half);
            vector<long double> b2 = slice(b, half, b.size());
            vector<long double> negB2(b2.size());
            transform(b2.begin(), b2.end(), negB2.begin(), negate<long double>());
            vector<long double> numerator = md1(a, concatenate(b1, negB2));
            vector<long double> denominator;
            if (z > 0) {
                spha subspha(x, y, z-1);
                denominator = subspha.md1(b1, b1);
                return concatenate(
                    subspha.md2(slice(numerator, 0, half), denominator),
                    subspha.md2(slice(numerator, half, numerator.size()), denominator)
                );
            } else if (y > 0) {
                spha subspha(x, y-1, z);
                denominator = subspha.md1(b1, b1);
                transform(denominator.begin(), denominator.end(), subspha.md1(b2, b2).begin(), denominator.begin(), minus<long double>());
                return concatenate(
                    subspha.md2(slice(numerator, 0, half), denominator),
                    subspha.md2(slice(numerator, half, numerator.size()), denominator)
                );
            } else {
                spha subspha(x-1, y, z);
                denominator = subspha.md1(b1, b1);
                transform(denominator.begin(), denominator.end(), subspha.md1(b2, b2).begin(), denominator.begin(), plus<long double>());
                return concatenate(
                    subspha.md2(slice(numerator, 0, half), denominator),
                    subspha.md2(slice(numerator, half, numerator.size()), denominator)
                );
            }
        }
    }
    vector<long double> mp(long double a, const vector<long double>& b) const {
        int n = x + y + z;
        vector<long double> result(pow(2, n), 0);
        if (a != 0) {
            long double abs_a = abs(a);
            long double log_abs_a = log(abs_a);
            if (a > 0) {
                result[0] = pow(a, b[0]);
            } else {
                result[0] = pow(-a, b[0]) * cos(b[0] * M_PI);
                result[pow(2, n-1)] = pow(-a, b[0]) * sin(b[0] * M_PI);
            }
            for (int i = 1; i < pow(2, n); i++) {
                vector<long double> term(pow(2, n), 0);
                int ps_i = ps(i);
                if (ps_i > 0) {
                    term[0] = cosh(b[i] * log_abs_a);
                    term[i] = sinh(b[i] * log_abs_a);
                } else if (ps_i < 0) {
                    term[0] = cos(b[i] * log_abs_a);
                    term[i] = sin(b[i] * log_abs_a);
                } else {
                    term[0] = 1;
                    term[i] = b[i] * log_abs_a;
                }
                result = md1(result, term);
                if (a < 0) {
                    long double rotationFactor = exp(ps_i * M_PI * b[i]);
                    for (auto& v : result) {
                        v *= rotationFactor;
                    }
                }
            }
        } else {
            if (all_of(b.begin(), b.end(), [](long double v) { return v == 0; })) {
                result[0] = 1;
            } else {
                result[0] = 0;
            }
        }
        return result;
    }
    spha(const std::vector<long double>& initialData) : data(initialData) {}
    spha& operator=(const spha& other) {
        if (this != &other) {
            if (data.size() != other.data.size()) {
                throw std::invalid_argument("Vectors must be of the same size");
            }
            for (size_t i = 0; i < data.size(); ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }
    friend std::vector<long double> operator+(const vector<long double>& a, const vector<long double>& b);
    friend std::vector<long double> operator-(const vector<long double>& a, const vector<long double>& b);
    friend std::vector<long double> operator*(const vector<long double>& a, long double scalar);
    friend std::vector<long double> operator*(long double scalar, const std::vector<long double>& a);
    friend std::vector<long double>& operator+=(vector<long double>& a, const vector<long double>& b);
    friend std::vector<long double>& operator-=(vector<long double>& a, const vector<long double>& b);
    friend std::vector<long double>& operator*=(vector<long double>& a, long double scalar);
};
vector<long double> operator+(const vector<long double>& a, const vector<long double>& b) {
    if (a.size() != b.size()) {
        throw invalid_argument("Vectors must be of the same size");
    }
    vector<long double> result(a.size());
    for (size_t i = 0; i < a.size(); ++i) {
        result[i] = a[i] + b[i];
    }
    return result;
}
vector<long double> operator-(const vector<long double>& a, const vector<long double>& b) {
    if (a.size() != b.size()) {
        throw invalid_argument("Vectors must be of the same size");
    }
    vector<long double> result(a.size());
    for (size_t i = 0; i < a.size(); ++i) {
        result[i] = a[i] - b[i];
    }
    return result;
}
vector<long double> operator*(const vector<long double>& a, long double scalar) {
    vector<long double> result(a.size());
    for (size_t i = 0; i < a.size(); ++i) {
        result[i] = a[i] * scalar;
    }
    return result;
}

vector<long double> operator*(long double scalar, const vector<long double>& a) {
    return a * scalar;
}
vector<long double>& operator+=(vector<long double>& a, const vector<long double>& b) {
    if (a.size() != b.size()) {
        throw invalid_argument("Vectors must be of the same size");
    }
    for (size_t i = 0; i < a.size(); ++i) {
        a[i] += b[i];
    }
    return a;
}
vector<long double>& operator-=(vector<long double>& a, const vector<long double>& b) {
    if (a.size() != b.size()) {
        throw invalid_argument("Vectors must be of the same size");
    }
    for (size_t i = 0; i < a.size(); ++i) {
        a[i] -= b[i];
    }
    return a;
}
vector<long double>& operator*=(vector<long double>& a, long double scalar) {
    for (size_t i = 0; i < a.size(); ++i) {
        a[i] *= scalar;
    }
    return a;
}
ostream& operator<<(ostream& out, const vector<long double>& a) {
    out << setprecision(15);
    out << a[0];
    for (size_t i = 1; i < a.size(); ++i) {
        out << ", " << a[i];
    }
    return out;
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
    spha spha(cx, dx, ex);
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
            k = spha.md(values);
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
            if (l <= 0) throw invalid_argument("Non-positive value");
            if (l > 1500) l = 1500;
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
        r1 = spha.md({riemann_zetal(-k[0])});
    } else if (k[0] == -1) {
        r1 = spha.md({0.577215664901532860606512090082402431042159335});
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
        d_temp_vec = spha.md1(d_temp_vec, spha.mp(n, k_temp));
        if (a == 0 && k[0] == -1 && all_of(k.begin() + 1, k.end(), [](long double x) { return x == 0; })) {
            d_temp_vec = spha.md({log(n)});
            r1+=d_temp_vec;
            continue;
        } else if (a > 0) {
            for (int b = 0; b < a - 1; ++b) {
                vector<long double> temp = {a + fmod(k[0], 1) - (fmod(k[0], 1) != 0 ? 1 : 0) - b};
                temp.resize(1 << g, 0);
                d_temp_vec = spha.md1(d_temp_vec, temp);
                if (k[0] != b) {
                    vector<long double> temp2 = k;
                    for (size_t i = 1; i < temp2.size(); ++i) {
                        temp2[i] /= (k[0] - b);
                    }
                    temp2[0] = 1;
                    d_temp_vec = spha.md1(d_temp_vec, temp2);
                } else {
                    if (k[0] != -1) {
                        vector<long double> temp = {fmod(k[0], 1) - (fmod(k[0], 1) != 0 ? 1 : 0) + 1};
                        temp.resize(1 << g, 0);
                        d_temp_vec = spha.md2(d_temp_vec, temp);
                        vector<long double> temp2 = k;
                        for (size_t i = 1; i < temp2.size(); ++i) {
                            temp2[i] /= (k[0] + 1);
                        }
                        temp2[0] = 1;
                        d_temp_vec = spha.md2(d_temp_vec, temp2);
                    } else {
                        vector<long double> temp2 = k;
                        temp2[0] = 0;
                        d_temp_vec = spha.md2(d_temp_vec, temp2);
                    }
                }
            }
        } else {
            vector<long double> temp = {fmod(k[0], 1) - (fmod(k[0], 1) != 0 ? 1 : 0) + 1};
            temp.resize(1 << g, 0);
            d_temp_vec = spha.md2(d_temp_vec, temp);
            vector<long double> temp2 = k;
            for (size_t i = 1; i < temp2.size(); ++i) {
                temp2[i] /= (k[0] + 1);
            }
            temp2[0] = 1;
            d_temp_vec = spha.md2(d_temp_vec, temp2);
        }
        if (a == 1) {
        } else if (a == 0 && k[0] == -1 && all_of(k.begin() + 1, k.end(), [](long double x) { return x == 0; })) {
            vector<long double> temp = {log(n)};
            temp.resize(1 << g, 0);
            d_temp_vec = spha.md1(d_temp_vec, temp);
        } else if (ceil(k[0]) - a > -1) {
            for (int c = 1; c <= ceil(k[0]) - a ; ++c) {
                vector<long double> temp = {(c + fmod(k[0], 1) - (fmod(k[0], 1) == 0 ? 0 : 1) + a) / (c + fmod(k[0], 1) - (fmod(k[0], 1) == 0 ? 0 : 1) + 1)};
                temp.resize(1 << g, 0);
                d_temp_vec = spha.md1(d_temp_vec, temp);
            }
        } else {
            for (int c = ceil(k[0]) - a + 1; c < 1; ++c) {
                if (c == -a && fmod(k[0], 1) == 0) {
                    vector<long double> temp = {1.0 - a};
                    temp.resize(1 << g, 0);
                    d_temp_vec = spha.md2(d_temp_vec, temp);
                } else if (c == -1 && fmod(k[0], 1) == 0) {
                    vector<long double> temp = {static_cast<long double>(a - 1)};
                    temp.resize(1 << g, 0);
                    d_temp_vec = spha.md1(d_temp_vec, temp);
                } else {
                    vector<long double> temp = {(c + fmod(k[0], 1) - (fmod(k[0], 1) != 0 ? 1 : 0) + a) / (c + fmod(k[0], 1) - (fmod(k[0], 1) != 0 ? 1 : 0) + 1)};
                    temp.resize(1 << g, 0);
                    d_temp_vec = spha.md2(d_temp_vec, temp);
                }
            }
        }
        if (containsInfOrNaN(d_temp_vec)or sumAbsValues(d_temp_vec)>d0) {
                break;
        }
        r1+=d_temp_vec;
        d0=sumAbsValues(d_temp_vec);
    }
    cout << "Approximation: ";
    cout << r1 <<endl;

    if (n > 0 && fmod(n, 1) == 0) {
        for (int i = 1; i <= n; i++) {
            r2 += spha.mp(i,k);
        }
        cout << "Actual: ";
        cout << r2 <<endl;
        cout << "Error: ";
        cout << r1-r2 <<endl;
    } else if (fmod(n, 1) == 0) {
        for (int i = n + 1; i < 1; i++) {
            r2 -= spha.mp(i,k);
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
