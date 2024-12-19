#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

int maxScoreWords(const std::vector<std::string>& words, const std::vector<char>& letters, const std::vector<int>& score) {
    std::unordered_map<char, int> letterCount;
    for (char c : letters) {
        letterCount[c]++;
    }
    int maxScore = 0;
    for (const std::string& word : words) {
        int currentScore = 0;
        std::unordered_map<char, int> wordCount;
        bool canForm = true;
        for (char c : word) {
            wordCount[c]++;
            if (wordCount[c] > letterCount[c]) {
                canForm = false;
                break;
            }
            currentScore += score[c - 'a'];
        }
        if (canForm) {
            maxScore = std::max(maxScore, currentScore);
        }
    }
    return maxScore;
}