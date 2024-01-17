#include "poly.h"
Poly::Poly(std::initializer_list<std::pair<size_t, int64_t>> m) {
    for (std::pair k : m) {
        if (k.second != 0) {
            map_[k.first] = k.second;
        }
    }
}
Poly::Poly(std::initializer_list<int64_t> v) {
    size_t i = 0;
    for (int64_t it : v) {
        if (0 != it) {
            map_[i] = it;
        }
        ++i;
    }
}
bool Poly::operator==(const Poly& pol) const {
    if (pol.map_.size() != map_.size()) {
        return false;
    }
    for (const auto& [key, value] : map_) {
        if (pol.map_.count(key) == 0) {
            return false;
        } else if (pol.map_.at(key) != value) {
            return false;
        }
    }
    return true;
}
bool Poly::operator!=(const Poly& pol) const {
    return !(*this == pol);
}
Poly Poly::operator+(const Poly& pol) {
    Poly anna = *this;
    for (auto [key, value] : pol.map_) {
        if (map_.count(key) == 0) {
            anna.map_[key] = pol.map_.at(key);
        } else {
            anna.map_[key] += pol.map_.at(key);
        }
    }
    for (const auto& [key, value] : anna.map_) {
        if (anna.map_[key] == 0) {
            anna.map_.erase(key);
        }
    }
    return anna;
}
Poly Poly::operator-(const Poly& pol) {
    Poly anna = *this;
    for (const auto& [key, value] : pol.map_) {
        if (map_.count(key) == 0) {
            anna.map_[key] = pol.map_.at(key);
        } else {
            anna.map_[key] -= pol.map_.at(key);
        }
    }
    for (const auto& [key, value] : anna.map_) {
        if (anna.map_[key] == 0) {
            anna.map_.erase(key);
        }
    }
    return anna;
}
Poly Poly::operator*(const Poly& pol) {
    Poly anna;
    for (const auto& [key, value] : map_) {
        for (const auto& [k, v] : pol.map_) {
            if (anna.map_.count(k + key) == 0) {
                anna.map_[k + key] = v * value;
            } else {
                anna.map_[k + key] += v * value;
            }
        }
    }
    for (const auto& [key, value] : anna.map_) {
        if (anna.map_[key] == 0) {
            anna.map_.erase(key);
        }
    }
    return anna;
}
Poly Poly::operator-() const {
    Poly anna = *this;
    for (auto [key, value] : anna.map_) {
        anna.map_[key] = value * (-1);
    }
    return anna;
}
Poly& Poly::operator-=(const Poly& pol) {
    *this = *this - pol;
    return *this;
}
Poly& Poly::operator+=(const Poly& pol) {
    *this = *this + pol;
    return *this;
}
Poly& Poly::operator*=(const Poly& pol) {
    *this = *this * pol;
    return *this;
}
int64_t Poly::operator()(int64_t x) const {
    int64_t res = 0;
    size_t i = 0;
    int64_t xi = 1;
    for (const auto& [key, value] : map_) {
        while (i != key) {
            xi *= x;
            ++i;
        }
        res += xi * static_cast<int64_t>(value);
    }
    return res;
}
std::ostream& operator<<(std::ostream& str, const Poly pol) {
    str << "y = ";
    if (pol.map_.empty()) {
        str << "0";
    } else {
        for (auto it = pol.map_.rbegin(); it != pol.map_.rend(); ++it) {
            if (it != pol.map_.rbegin()) {
                if (it->second < 0) {
                    str << " - " << (-1) * it->second;
                    if (it->first != 0) {
                        str << "x^" << it->first;
                    }
                } else {
                    str << " + " << it->second;
                    if (it->first != 0) {
                        str << "x^" << it->first;
                    }
                }
            } else {
                str << it->second;
                if (it->first != 0) {
                    str << "x^" << it->first;
                }
            }
        }
    }
    return str;
}

