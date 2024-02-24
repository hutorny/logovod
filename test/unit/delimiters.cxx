/*
 * Copyright (C) 2024 Eugene Hutorny <eugene@hutorny.in.ua>
 *
 * This file is a part of logovod library
 *
 * Licensed under MIT License, see full text in LICENSE
 * or visit page https://opensource.org/license/mit/
 */

#include "tests.h"

using namespace logovod::tests;
using namespace logovod;

TEST_F(Delimiters, Default) {
    using namespace std;
    L::i(1, 2, 3);              EXPECT_EQ(message,"1 2 3");
    L::i(1, '+', 2, 3);         EXPECT_EQ(message,"1+2 3");
    L::i(1, '+', 2, 3);         EXPECT_EQ(message,"1+2 3");
    L::i("x=", setw(6), setprecision(4), 1.2); EXPECT_EQ(message,"x=   1.2");
}

TEST_F(Delimiters, Custom) {
    const int i[] { 5, 6, 7};
    C::i(1, 2, 3);              EXPECT_EQ(message,"1_2_3");
    C::i{}(i);                  EXPECT_EQ(message,"[5;6;7]");
}

TEST_F(Delimiters, Override) {
    static constexpr auto c = consts::comma;
    const int i[] { 5, 6, 7};
    const char* s[] { "a", "B", "c" };
    L::i(1, c, 2, 3);                   EXPECT_EQ(message,"1,2,3");
    C::i(1, 2, c, 3, 4);                EXPECT_EQ(message,"1_2,3,4");
    L::i(separators{'<','/','>'}, i);   EXPECT_EQ(message,"<5/6/7>");
    C::i(separators{'<','/','>'}, s);   EXPECT_EQ(message,"<a/B/c>");
}

