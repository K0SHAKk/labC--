#include <string>
#include <algorithm>

bool isPalindrome(const std::string& s) {
    std::string filtered, reversed;
    for (char c : s) {
        if (std::isalpha(c)) {
            filtered += std::tolower(c);
        }
    }
    reversed = filtered;
    std::reverse(reversed.begin(), reversed.end());
    return filtered == reversed;
}