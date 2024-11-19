#include <gtest/gtest.h>
#include "main.h"
using namespace std;
TEST(Test, 1) {
    IPAddress ip("192.168.1.1");
    ASSERT_EQ(ip.n1, 192);
    ASSERT_EQ(ip.n2, 168);
    ASSERT_EQ(ip.n3, 1);
    ASSERT_EQ(ip.n4, 1);
}

TEST(Test, 2) {
    IPAddress ip("192.168.1.1");
    ASSERT_EQ(ip.toString(), "192.168.1.1");
}

TEST(Test, 3) {
    IPAddress ip1("192.168.1.1");
    IPAddress ip2("192.168.1.2");
    ASSERT_TRUE(compareIP(ip1, ip2));
    ASSERT_FALSE(compareIP(ip2, ip1));
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  
  return RUN_ALL_TESTS();
}