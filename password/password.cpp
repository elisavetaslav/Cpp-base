#include "password.h"
#include <cctype>

const int MIN_LEN = 8;
const int MAX_LEN = 14;
const int MIN_VALUE_ASCII = 33;
const int MAX_VALUE_ASCII = 126;
const int COUNT_CLASS = 3;
bool ValidatePassword(const std::string& password) {
    int was_digit = 0;
    int was_lower = 0;
    int was_upper = 0;
    int was_other = 0;
    for (auto i : password) {
        if (static_cast<int>(i) > MAX_VALUE_ASCII || static_cast<int>(i) < MIN_VALUE_ASCII) {
            return false;
        } else if (islower(i)) {
            was_lower = 1;
        } else if (isdigit(i)) {
            was_digit = 1;
        } else if (isupper(i)) {
            was_upper = 1;
        } else {
            was_other = 1;
        }
    }
    if (password.length() > MAX_LEN || password.length() < MIN_LEN) {
        return false;
    } else if (was_upper + was_lower + was_digit + was_other < COUNT_CLASS) {
        return false;
    }
    return true;
}
