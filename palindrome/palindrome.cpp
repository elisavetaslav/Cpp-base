#include "palindrome.h"

bool IsPalindrome(const std::string& str) {
    auto left = str.begin();
    auto right = str.end() - 1;
    while (left < right) {
        if (*left == ' ') {
            left += 1;
        } else if (*right == ' ') {
            right -= 1;
        } else if (*left != *right) {
            return false;
        } else {
            left += 1;
            right -= 1;
        }
    }
    return true;
}
