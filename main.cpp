#include <iostream>
#include "defangIPaddr.h"
#include "isPalindrome.h"
#include "maxScoreWords.h"


int main() {
    std::string ip = "192.168.0.1";
    std::cout << defangIPaddr(ip) << std::endl;
    
    std::string s = "never odd or even";
    std::cout << (isPalindrome(s) ? "True" : "False") << std::endl;
    
    std::vector<std::string> words = {"dog", "cat", "dad", "good"};
    std::vector<char> letters = {'a', 'a', 'c', 'd', 'd', 'g', 'o', 'o'};
    std::vector<int> score = {1, 0, 9, 5, 1, 0, 6, 1, 0, 1, 1, 1, 3, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    std::cout << maxScoreWords(words, letters, score) << std::endl;
    return 0;
}
