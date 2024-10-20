#ifndef SPHAMOD_H
#define SPHAMOD_H

#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <iomanip>

class spha {
private:
    int x, y, z;
    std::vector<long double> data;

    std::vector<long double> concatenate(const std::vector<long double>& a, const std::vector<long double>& b) const;
    std::vector<long double> slice(const std::vector<long double>& v, int start, int end) const;
    int ps(int index) const;

public:
    spha(int x, int y, int z);
    spha(const std::vector<long double>& initialData);

    std::vector<long double> md(const std::vector<long double>& args) const;
    std::vector<long double> md1(const std::vector<long double>& a, const std::vector<long double>& b) const;
    std::vector<long double> md2(const std::vector<long double>& a, const std::vector<long double>& b) const;
    std::vector<long double> mp(long double a, const std::vector<long double>& b) const;

    spha& operator=(const spha& other);

    friend std::vector<long double> operator+(const std::vector<long double>& a, const std::vector<long double>& b);
    friend std::vector<long double> operator-(const std::vector<long double>& a, const std::vector<long double>& b);
    friend std::vector<long double> operator*(const std::vector<long double>& a, long double scalar);
    friend std::vector<long double> operator*(long double scalar, const std::vector<long double>& a);
    friend std::vector<long double>& operator+=(std::vector<long double>& a, const std::vector<long double>& b);
    friend std::vector<long double>& operator-=(std::vector<long double>& a, const std::vector<long double>& b);
    friend std::vector<long double>& operator*=(std::vector<long double>& a, long double scalar);
};

std::vector<long double> operator+(const std::vector<long double>& a, const std::vector<long double>& b);
std::vector<long double> operator-(const std::vector<long double>& a, const std::vector<long double>& b);
std::vector<long double> operator*(const std::vector<long double>& a, long double scalar);
std::vector<long double> operator*(long double scalar, const std::vector<long double>& a);
std::vector<long double>& operator+=(std::vector<long double>& a, const std::vector<long double>& b);
std::vector<long double>& operator-=(std::vector<long double>& a, const std::vector<long double>& b);
std::vector<long double>& operator*=(std::vector<long double>& a, long double scalar);
std::ostream& operator<<(std::ostream& out, const std::vector<long double>& a);

#endif // SPHA_H
