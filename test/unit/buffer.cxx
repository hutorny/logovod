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

TEST_F(Buffer, Limited) {
    std::string s(32, '-');
    L::i{}(s); EXPECT_EQ(message,s.substr(0, 24));
    L::i i{};
    i << "123456789";
    i << s << std::flush;
    EXPECT_EQ(message,"123456789" + s.substr(0, 15));
    L::i(6789, s); EXPECT_EQ(message,"6789 " + s.substr(0, 19));
    L::i{} << s.substr(0, 20) << 987654321; EXPECT_EQ(message,s.substr(0, 20)+"9876");
    EXPECT_EQ(write_count, 4);
}

TEST_F(Buffer, Terminated) {
    std::string s(32, '-');
    T::i{}(s);
    EXPECT_EQ(message,s.substr(0, 23) + '\n');
    EXPECT_EQ(payload,s.substr(0, 23));
    T::i i{};
    i << "123456789";
    i << s << std::flush;
    EXPECT_EQ(message,"123456789" + s.substr(0, 14) + '\n');
    T::i(6789, s); EXPECT_EQ(message,"6789 " + s.substr(0, 18) + '\n');
    T::i{} << s.substr(0, 20) << 987654321; EXPECT_EQ(message,s.substr(0, 20)+"987\n");
    EXPECT_EQ(write_count, 4);
}

TEST_F(Buffer, Triimed) {
    std::string s(32, '-');
    R::i{}(s);
    EXPECT_EQ(message,s.substr(0, 20) + "...\n");
    EXPECT_EQ(payload,s.substr(0, 20) + "...");
    R::i i{};
    i << "123456789";
    i << s << std::flush;
    EXPECT_EQ(message,"123456789" + s.substr(0, 11) + "...\n");
    R::i(6789, s); EXPECT_EQ(message,"6789 " + s.substr(0, 15) + "...\n");
    R::i{} << s.substr(0, 20) << 987654321; EXPECT_EQ(message,s.substr(0, 20)+"...\n");
    EXPECT_EQ(write_count, 4);
}

TEST_F(Buffer, UsableAfterOverflow) {
    std::string s(32, '-');
    L::i i{};
    i << s << std::flush;         EXPECT_EQ(message,s.substr(0, 24));
    i << 987654;
    i << s << std::flush;         EXPECT_EQ(message,"987654" + s.substr(0, 18));
    EXPECT_EQ(write_count, 2);
}

TEST_F(Buffer, Unlimited) {
    std::string s(0x10000, '-');
    U::i{}(s); EXPECT_EQ(message, s);
    EXPECT_EQ(write_count, 1);
}

