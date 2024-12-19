#include <iostream>
#include <string>

std::string defangIPaddr(const std::string& address) {
    std::string defanged;
    for (char c : address) {
        if (c == '.') {
            defanged += "[.]";
        } else {
            defanged += c;
        }
    }
    return defanged;
}