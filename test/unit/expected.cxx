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

#ifdef __cpp_lib_expected
TEST_F(Expected, Printed) {
    std::expected<std::string, int> sie {"yes"};
    std::expected<std::string, int> siu { std::unexpected{100} };
    std::expected<std::string, int> si { };
    std::expected<long, bool> lbe { 10L };
    std::expected<long, bool> lbu { std::unexpected{false} };
    std::expected<long, bool> lb { };
    L::i{}(sie, siu, '\'', si, '\'');           EXPECT_EQ(message, "yes {100}''");
    L::i{}(lbe, lbu, lb);           EXPECT_EQ(message, "10 {0} 0");
}
#endif
