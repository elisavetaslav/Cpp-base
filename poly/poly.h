#pragma once

#include <cctype>
#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <string_view>
#include <tuple>

class Poly {
public:
    Poly() = default;
    Poly(std::initializer_list<std::pair<size_t, int64_t>> m);
    Poly(std::initializer_list<int64_t> v);
    bool operator==(const Poly& pol) const;
    bool operator!=(const Poly& pol) const;
    Poly operator+(const Poly& pol);
    Poly operator-(const Poly& pol);
    Poly operator*(const Poly& pol);
    Poly operator-() const;
    Poly& operator-=(const Poly& pol);
    Poly& operator+=(const Poly& pol);
    Poly& operator*=(const Poly& pol);
    int64_t operator()(int64_t x) const;
    friend std::ostream& operator<<(std::ostream& str, const Poly pol);

private:
    std::map<size_t, int64_t> map_;
};

