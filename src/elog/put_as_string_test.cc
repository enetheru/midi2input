#include <deque>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <gtest/gtest.h>
#include "put_as_string.h"

namespace LOG {

namespace {

template <typename T>
void VerifyStringOfValue(const T& t, const std::string& string) {
  std::ostringstream stream;
  PutAsString(t, stream);
  EXPECT_EQ(string, stream.str());
}

}  // anonymous namespace

TEST(StringBuilderTest, SingleConstCharPtr) {
  VerifyStringOfValue("abc", "abc");
}

TEST(StringBuilderTest, SingleString) {
  const std::string message("abc");
  VerifyStringOfValue(message, message);
}

TEST(StringBuilderTest, SingleInteger) {
  VerifyStringOfValue(123, "123");
}

TEST(StringBuilderTest, SingleChar) {
  VerifyStringOfValue('A', "A");
}

TEST(StringBuilderTest, SingleSignedChar) {
  const signed char signed_char = -10;
  VerifyStringOfValue(signed_char, "-10");
}

TEST(StringBuilderTest, SingleUnsignedChar) {
  const unsigned char unsigned_char = 234;
  VerifyStringOfValue(unsigned_char, "234");
}

TEST(StringBuilderTest, SinglePair) {
  const std::pair<int, std::string> pair(123, "abc");
  VerifyStringOfValue(pair, "(123,abc)");
}

TEST(StringBuilderTest, SingleVector) {
  std::vector<std::string> vector;
  vector.push_back("ghi");
  vector.push_back("def");
  vector.push_back("abc");
  VerifyStringOfValue(vector, "ghi,def,abc");
}

TEST(StringBuilderTest, SingleDeque) {
  std::deque<std::string> deque;
  deque.push_back("ghi");
  deque.push_back("def");
  deque.push_back("abc");
  VerifyStringOfValue(deque, "ghi,def,abc");
}

TEST(StringBuilderTest, SingleList) {
  std::list<std::string> list;
  list.push_back("ghi");
  list.push_back("def");
  list.push_back("abc");
  VerifyStringOfValue(list, "ghi,def,abc");
}

TEST(StringBuilderTest, SingleMap) {
  std::map<int, std::string> map;
  map.insert(std::make_pair(10, "abc"));
  map.insert(std::make_pair(-10, "def"));
  map.insert(std::make_pair(0, "ghi"));
  VerifyStringOfValue(map, "(-10,def),(0,ghi),(10,abc)");
}

TEST(StringBuilderTest, SingleSet) {
  std::set<std::string> set;
  set.insert("ghi");
  set.insert("def");
  set.insert("abc");
  VerifyStringOfValue(set, "abc,def,ghi");
}

}  // namespace LOG
