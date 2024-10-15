#include <iostream>
#include <tuple>

auto foo(int, int) {
    return std::make_tuple(std::string("surname"), 19, 3.2);
}

int main(int, char *[]) {
    auto r = foo(1, 2);
    std::cout << std::get<0>(r) << std::endl;
    std::cout << std::get<1>(r) << std::endl;
    std::cout << std::get<double>(r) << std::endl;
    return 0;
}
