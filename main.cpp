#include <iostream>
#include <tuple>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>

struct IP
{
    int n1, n2, n3, n4;

    IP(const std::string& ipString) {
        std::stringstream ss(ipString);
        char dot;
        ss >> n1 >> dot >> n2 >> dot >> n3 >> dot >> n4;
    }

    std::string toString() const {  
        return std::to_string(n1) + "." + std::to_string(n2) + "." + std::to_string(n3) + "." + std::to_string(n4);
    }
};

auto compareIP = [](const IP& lhs, const IP& rhs) {
    return std::tie(lhs.n1, lhs.n2, lhs.n3, lhs.n4) > std::tie(rhs.n1, rhs.n2, rhs.n3, rhs.n4);
};


int main(int, char *[]) {
    std::vector<IP> adress; 

    std::ifstream file("C:/Users/k0sha/Desktop/lab1adaw/ip_filter.tsv");
     if (!file.is_open()) {
        std::cerr << "Ошибка открытия файла" << std::endl;
        return 1;
    }

    std::string line;
    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string ipString;
        getline(ss, ipString, '\t');
        adress.push_back(ipString);
    }

    std::sort(adress.begin(), adress.end(), compareIP);

    std::cout<< "Вывод всех IP-адресов"<< std::endl;
    for (const auto& ip : adress) {
        std::cout << ip.toString() << std::endl;
    }

    std::cout<< "Вывод всех IP-адресов первй байт которых равен 1"<< std::endl;
    for (const auto& ip : adress) {
        if (ip.n1 == 1){
            std::cout << ip.toString() << std::endl;
        }
    }

    std::cout<< "Вывод всех IP-адресов первый байт 46, второй 70"<< std::endl;
    for (const auto& ip : adress) {
        if (ip.n1 == 46 && ip.n2 == 70){
            std::cout << ip.toString() << std::endl;
        }
    }

    std::cout<< "Вывод всех IP-адресов есть байт 46"<< std::endl;
    for (const auto& ip : adress) {
        if (ip.n1 == 46 || ip.n2 == 46 || ip.n3 == 46 || ip.n4 == 46){
            std::cout << ip.toString() << std::endl;
        }
    }

    return 0;
}
