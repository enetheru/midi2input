// Copyright (c) 2011 Seiya Tokui <beam.web@gmail.com>. All Rights Reserved.
// This source code is distributed under MIT License in LICENSE file.

#include <typeinfo>
#include <gtest/gtest.h>
#include "type_info.h"

namespace LOG {

namespace {

class TypeInfoTest : public ::testing::Test {
 protected:
  template <typename T>
  static void CheckEquivalenceOfTypeInfosWithBuiltinTypes() {
    CheckEquivalenceOfTypeInfos<T, bool>();
    CheckEquivalenceOfTypeInfos<T, char>();
    CheckEquivalenceOfTypeInfos<T, unsigned char>();
    CheckEquivalenceOfTypeInfos<T, short>();
    CheckEquivalenceOfTypeInfos<T, unsigned short>();
    CheckEquivalenceOfTypeInfos<T, int>();
    CheckEquivalenceOfTypeInfos<T, unsigned int>();
    CheckEquivalenceOfTypeInfos<T, long>();
    CheckEquivalenceOfTypeInfos<T, unsigned long>();
    CheckEquivalenceOfTypeInfos<T, long long>();
    CheckEquivalenceOfTypeInfos<T, unsigned long long>();
    CheckEquivalenceOfTypeInfos<T, float>();
    CheckEquivalenceOfTypeInfos<T, double>();
    CheckEquivalenceOfTypeInfos<T, long double>();
  }

  template <typename T, typename S>
  static void CheckEquivalenceOfTypeInfosInVariations() {
    CheckEquivalenceOfTypeInfos<T, S>();
    CheckEquivalenceOfTypeInfos<T, S*>();
    CheckEquivalenceOfTypeInfos<T, const S>();
    CheckEquivalenceOfTypeInfos<T, const S*>();
    CheckEquivalenceOfTypeInfos<T, volatile S>();
    CheckEquivalenceOfTypeInfos<T, volatile S*>();
    CheckEquivalenceOfTypeInfos<T, const volatile S>();
    CheckEquivalenceOfTypeInfos<T, const volatile S*>();
    CheckEquivalenceOfTypeInfos<T, S ()>();
    CheckEquivalenceOfTypeInfos<T, void (S)>();
    CheckEquivalenceOfTypeInfos<T, void (S, S)>();
    CheckEquivalenceOfTypeInfos<T, void (const S&)>();
    CheckEquivalenceOfTypeInfos<T, void (const S&, const S&)>();
    CheckEquivalenceOfTypeInfos<T, S (S)>();
    CheckEquivalenceOfTypeInfos<T, S (S, S)>();
    CheckEquivalenceOfTypeInfos<T, S (const S&)>();
    CheckEquivalenceOfTypeInfos<T, S (const S&, const S&)>();
  }

  template <typename T, typename S>
  static void CheckEquivalenceOfTypeInfos() {
    if (typeid(T) == typeid(S)) {
      EXPECT_TRUE(TypeInfo(Type<T>()) == TypeInfo(Type<S>()));
    } else {
      EXPECT_FALSE(TypeInfo(Type<T>()) == TypeInfo(Type<S>()));
    }
  }
};

class UserDefinedTypeA {};
class UserDefinedTypeB {};

}  // anonymous namespace

TEST_F(TypeInfoTest, CheckEquivalenceOfTypeInfosOfBuiltinTypes) {
  CheckEquivalenceOfTypeInfosWithBuiltinTypes<void>();
  CheckEquivalenceOfTypeInfosWithBuiltinTypes<bool>();
  CheckEquivalenceOfTypeInfosWithBuiltinTypes<char>();
  CheckEquivalenceOfTypeInfosWithBuiltinTypes<unsigned char>();
  CheckEquivalenceOfTypeInfosWithBuiltinTypes<short>();
  CheckEquivalenceOfTypeInfosWithBuiltinTypes<unsigned short>();
  CheckEquivalenceOfTypeInfosWithBuiltinTypes<int>();
  CheckEquivalenceOfTypeInfosWithBuiltinTypes<unsigned int>();
  CheckEquivalenceOfTypeInfosWithBuiltinTypes<long>();
  CheckEquivalenceOfTypeInfosWithBuiltinTypes<unsigned long>();
  CheckEquivalenceOfTypeInfosWithBuiltinTypes<long long>();
  CheckEquivalenceOfTypeInfosWithBuiltinTypes<unsigned long long>();
  CheckEquivalenceOfTypeInfosWithBuiltinTypes<float>();
  CheckEquivalenceOfTypeInfosWithBuiltinTypes<double>();
  CheckEquivalenceOfTypeInfosWithBuiltinTypes<long double>();
}

TEST_F(TypeInfoTest, CheckEquivalenceOfIntInVariations) {
  CheckEquivalenceOfTypeInfosInVariations<void, int>();
  CheckEquivalenceOfTypeInfosInVariations<char, int>();
  CheckEquivalenceOfTypeInfosInVariations<int, int>();
}

TEST_F(TypeInfoTest, CheckEquivalenceOfUserDefinedTypes) {
  CheckEquivalenceOfTypeInfosInVariations<UserDefinedTypeA, UserDefinedTypeA>();
  CheckEquivalenceOfTypeInfosInVariations<UserDefinedTypeA, UserDefinedTypeB>();
}

}  // namespace LOG
