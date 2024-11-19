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


int main(int, char *[]) {
    vector<IP> adress; 

    ifstream file("C:/Users/k0sha/Desktop/lab1adaw/ip_filter.tsv");
     if (!file.is_open()) {
        cerr << "Ошибка открытия файла" << endl;
        return 1;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string ipString;
        getline(ss, ipString, '\t');
        adress.push_back(ipString);
    }

    sort(adress.begin(), adress.end(), compareIP);

    cout<< "Вывод всех IP-адресов"<< endl;
    for (const auto& ip : adress) {
        cout << ip.toString() << endl;
    }

    cout<< "Вывод всех IP-адресов первй байт которых равен 1"<< endl;
    for (const auto& ip : adress) {
        if (ip.n1 == 1){
            cout << ip.toString() << endl;
        }
    }

    cout<< "Вывод всех IP-адресов первый байт 46, второй 70"<< endl;
    for (const auto& ip : adress) {
        if (ip.n1 == 46 && ip.n2 == 70){
            cout << ip.toString() << endl;
        }
    }

    cout<< "Вывод всех IP-адресов есть байт 46"<< endl;
    for (const auto& ip : adress) {
        if (ip.n1 == 46 || ip.n2 == 46 || ip.n3 == 46 || ip.n4 == 46){
            cout << ip.toString() << endl;
        }
    }

    return 0;
}
