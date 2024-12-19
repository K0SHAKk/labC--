#include <gtest/gtest.h>
#include "defangIPaddr.h"
#include "isPalindrome.h"
#include "maxScoreWords.h"

using namespace std;
TEST(Test, 1) {
    ASSERT_EQ(defangIPaddr("192.168.0.1"), "192[.]168[.]0[.]1");
}

TEST(Test, 2) {
    ASSERT_TRUE(isPalindrome("never odd or even"));
}

TEST(Test, 3) {
    std::vector<std::string> words = {"dog", "cat", "dad", "good"};
    std::vector<char> letters = {'a', 'a', 'c', 'd', 'd', 'g', 'o', 'o'};
    std::vector<int> score = {1, 0, 9, 5, 1, 0, 6, 1, 0, 1, 1, 1, 3, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    ASSERT_EQ(maxScoreWords(words, letters, score), 13);
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  
  return RUN_ALL_TESTS();
}