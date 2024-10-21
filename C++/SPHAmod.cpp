#include "sphamod.h"
#include <bitset>

using namespace std;

spha::spha(int x, int y, int z) : x(x), y(y), z(z) {}

spha::spha(const vector<long double>& initialData) : data(initialData) {}

vector<long double> spha::concatenate(const vector<long double>& a, const vector<long double>& b) const {
    vector<long double> result = a;
    result.insert(result.end(), b.begin(), b.end());
    return result;
}

vector<long double> spha::slice(const vector<long double>& v, int start, int end) const {
    return vector<long double>(v.begin() + start, v.begin() + end);
}

int spha::ps(int index) const {
    if (index >= (1 << (x+y))) return 0;
    string binary = bitset<32>(index).to_string().substr(32 - (x+y+z));
    int ones_count = count(binary.begin() + y + z, binary.end(), '1');
    return (ones_count % 2 == 0) ? 1 : -1;
}

vector<long double> spha::md(const vector<long double>& args) const {
    return vector<long double>(args.begin(), args.begin() + pow(2, x + y + z));
}

vector<long double> spha::md1(const vector<long double>& a, const vector<long double>& b) const {
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

vector<long double> spha::md2(const vector<long double>& a, const vector<long double>& b) const {
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

vector<long double> spha::mp(long double a, const vector<long double>& b) const {
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

spha& spha::operator=(const spha& other) {
    if (this != &other) {
        if (data.size() != other.data.size()) {
            throw invalid_argument("Vectors must be of the same size");
        }
        for (size_t i = 0; i < data.size(); ++i) {
            data[i] = other.data[i];
        }
    }
    return *this;
}

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
