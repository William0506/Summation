#include "sphamod.h"
#include <bitset>

spha::spha(int x, int y, int z) : x(x), y(y), z(z) {}

spha::spha(const std::vector<long double>& initialData) : data(initialData) {}

std::vector<long double> spha::concatenate(const std::vector<long double>& a, const std::vector<long double>& b) const {
    std::vector<long double> result = a;
    result.insert(result.end(), b.begin(), b.end());
    return result;
}

std::vector<long double> spha::slice(const std::vector<long double>& v, int start, int end) const {
    return std::vector<long double>(v.begin() + start, v.begin() + end);
}

int spha::ps(int index) const {
    if (index >= (1 << (x+y))) return 0;
    std::string binary = std::bitset<32>(index).to_string().substr(32 - (x+y+z));
    int ones_count = std::count(binary.begin() + y + z, binary.end(), '1');
    return (ones_count % 2 == 0) ? 1 : -1;
}

std::vector<long double> spha::md(const std::vector<long double>& args) const {
    return std::vector<long double>(args.begin(), args.begin() + std::pow(2, x + y + z));
}

std::vector<long double> spha::md1(const std::vector<long double>& a, const std::vector<long double>& b) const {
    int n = x + y + z;
    if (n == 0) return {a[0] * b[0]};
    if (n == 1) {
        if (x == 1) return {a[0]*b[0] - a[1]*b[1], a[0]*b[1] + a[1]*b[0]};
        else if (y == 1) return {a[0]*b[0] + a[1]*b[1], a[0]*b[1] + a[1]*b[0]};
        else return {a[0]*b[0], a[0]*b[1] + a[1]*b[0]};
    } else {
        int half = std::pow(2, n-1);
        std::vector<long double> a1 = slice(a, 0, half), a2 = slice(a, half, a.size());
        std::vector<long double> b1 = slice(b, 0, half), b2 = slice(b, half, b.size());
        std::vector<long double> c1, c2;
        if (z > 0) {
            spha subspha(x, y, z-1);
            c1 = subspha.md1(a1, b1);
            c2 = subspha.md1(a1, b2);
            std::transform(c2.begin(), c2.end(), subspha.md1(a2, b1).begin(), c2.begin(), std::plus<long double>());
        } else if (y > 0) {
            spha subspha(x, y-1, z);
            c1 = subspha.md1(a1, b1);
            std::transform(c1.begin(), c1.end(), subspha.md1(a2, b2).begin(), c1.begin(), std::plus<long double>());
            c2 = subspha.md1(a1, b2);
            std::transform(c2.begin(), c2.end(), subspha.md1(a2, b1).begin(), c2.begin(), std::plus<long double>());
        } else {
            spha subspha(x-1, y, z);
            c1 = subspha.md1(a1, b1);
            std::transform(c1.begin(), c1.end(), subspha.md1(a2, b2).begin(), c1.begin(), std::minus<long double>());
            c2 = subspha.md1(a1, b2);
            std::transform(c2.begin(), c2.end(), subspha.md1(a2, b1).begin(), c2.begin(), std::plus<long double>());
        }
        return concatenate(c1, c2);
    }
}

std::vector<long double> spha::md2(const std::vector<long double>& a, const std::vector<long double>& b) const {
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
        int half = std::pow(2, n-1);
        std::vector<long double> b1 = slice(b, 0, half);
        std::vector<long double> b2 = slice(b, half, b.size());
        std::vector<long double> negB2(b2.size());
        std::transform(b2.begin(), b2.end(), negB2.begin(), std::negate<long double>());
        std::vector<long double> numerator = md1(a, concatenate(b1, negB2));
        std::vector<long double> denominator;
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
            std::transform(denominator.begin(), denominator.end(), subspha.md1(b2, b2).begin(), denominator.begin(), std::minus<long double>());
            return concatenate(
                subspha.md2(slice(numerator, 0, half), denominator),
                subspha.md2(slice(numerator, half, numerator.size()), denominator)
            );
        } else {
            spha subspha(x-1, y, z);
            denominator = subspha.md1(b1, b1);
            std::transform(denominator.begin(), denominator.end(), subspha.md1(b2, b2).begin(), denominator.begin(), std::plus<long double>());
            return concatenate(
                subspha.md2(slice(numerator, 0, half), denominator),
                subspha.md2(slice(numerator, half, numerator.size()), denominator)
            );
        }
    }
}

std::vector<long double> spha::mp(long double a, const std::vector<long double>& b) const {
    int n = x + y + z;
    std::vector<long double> result(std::pow(2, n), 0);
    result[0] = std::pow(a, b[0]);
    for (int i = 1; i < std::pow(2, n); i++) {
        std::vector<long double> term(std::pow(2, n), 0);
        if (ps(i) > 0) {
            term[0] = std::cosh(b[i] * std::log(a));
            term[i] = std::sinh(b[i] * std::log(a));
        } else if (ps(i) < 0) {
            term[0] = std::cos(b[i] * std::log(a));
            term[i] = std::sin(b[i] * std::log(a));
        } else {
            term[0] = 1;
            term[i] = b[i] * std::log(a);
        }
        result = md1(result, term);
    }
    return result;
}

spha& spha::operator=(const spha& other) {
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

std::vector<long double> operator+(const std::vector<long double>& a, const std::vector<long double>& b) {
    if (a.size() != b.size()) {
        throw std::invalid_argument("Vectors must be of the same size");
    }
    std::vector<long double> result(a.size());
    for (size_t i = 0; i < a.size(); ++i) {
        result[i] = a[i] + b[i];
    }
    return result;
}

std::vector<long double> operator-(const std::vector<long double>& a, const std::vector<long double>& b) {
    if (a.size() != b.size()) {
        throw std::invalid_argument("Vectors must be of the same size");
    }
    std::vector<long double> result(a.size());
    for (size_t i = 0; i < a.size(); ++i) {
        result[i] = a[i] - b[i];
    }
    return result;
}

std::vector<long double> operator*(const std::vector<long double>& a, long double scalar) {
    std::vector<long double> result(a.size());
    for (size_t i = 0; i < a.size(); ++i) {
        result[i] = a[i] * scalar;
    }
    return result;
}

std::vector<long double> operator*(long double scalar, const std::vector<long double>& a) {
    return a * scalar;
}

std::vector<long double>& operator+=(std::vector<long double>& a, const std::vector<long double>& b) {
    if (a.size() != b.size()) {
        throw std::invalid_argument("Vectors must be of the same size");
    }
    for (size_t i = 0; i < a.size(); ++i) {
        a[i] += b[i];
    }
    return a;
}

std::vector<long double>& operator-=(std::vector<long double>& a, const std::vector<long double>& b) {
    if (a.size() != b.size()) {
        throw std::invalid_argument("Vectors must be of the same size");
    }
    for (size_t i = 0; i < a.size(); ++i) {
        a[i] -= b[i];
    }
    return a;
}

std::vector<long double>& operator*=(std::vector<long double>& a, long double scalar) {
    for (size_t i = 0; i < a.size(); ++i) {
        a[i] *= scalar;
    }
    return a;
}

std::ostream& operator<<(std::ostream& out, const std::vector<long double>& a) {
    out << std::setprecision(15);
    out << a[0];
    for (size_t i = 1; i < a.size(); ++i) {
        out << ", " << a[i];
    }
    return out;
}
