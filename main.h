#ifndef SOMETHING_H
#define SOMETHING_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <tuple>
#include <cstdint>

using namespace std;
struct IP {
    int n1, n2, n3, n4;

    // Конструктор для получения IP-адреса из строки
    IP(const string& ipString) {
        stringstream ss(ipString);
        char dot;
        ss >> n1 >> dot >> n2 >> dot >> n3 >> dot >> n4;
    }

    // Метод для получения строкового представления IP-адреса
    string toString() const {
        return to_string(n1) + "." + to_string(n2) + "." + to_string(n3) + "." + to_string(n4);
    }
};

// Сравнение IP-адресов в обратном лексикографическом порядке
auto compareIP = [](const IP& lhs, const IP& rhs) {
    return tie(lhs.n1, lhs.n2, lhs.n3, lhs.n4) > tie(rhs.n1, rhs.n2, rhs.n3, rhs.n4);
};

#endif