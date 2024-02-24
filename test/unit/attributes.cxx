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

TEST_F(Attributes, Tag) {
    using namespace std;
    A::i{}("test");             EXPECT_EQ(attrs.tag, "A");
    B::i{}(1);                  EXPECT_EQ(attrs.tag, "B");
    A::i{}(4);                  EXPECT_EQ(attrs.tag, "A");
}
#if defined(__clang__) && defined(__cpp_lib_source_location)
TEST_F(Attributes, DISABLED_Location) {
#else
TEST_F(Attributes, Location) {
#endif
    using namespace std;
    A::i{}("test");  source_location l1 {source_location::current()};
    EXPECT_EQ(attrs.location.file_name, l1.file_name());
    EXPECT_EQ(attrs.location.line, l1.line());
    B::i{}("test");  source_location l2 {source_location::current()};
    EXPECT_EQ(attrs.location.file_name, l2.file_name());
    EXPECT_EQ(attrs.location.line, l2.line());
}

TEST_F(Attributes, Level) {
    A::d{}("test");             EXPECT_EQ(attrs.level, priority::debug);
    A::i{}("test");             EXPECT_EQ(attrs.level, priority::informational);
    A::n{}("test");             EXPECT_EQ(attrs.level, priority::notice);
    A::w{}("test");             EXPECT_EQ(attrs.level, priority::warning);
    A::e{}("test");             EXPECT_EQ(attrs.level, priority::error);
    A::c{}("test");             EXPECT_EQ(attrs.level, priority::critical);
    A::a{}("test");             EXPECT_EQ(attrs.level, priority::alert);
    A::f{}("test");             EXPECT_EQ(attrs.level, priority::emergency);

    A::info{}("test");          EXPECT_EQ(attrs.level, priority::informational);
    A::warn{}("test");          EXPECT_EQ(attrs.level, priority::warning);
    A::crit{}("test");          EXPECT_EQ(attrs.level, priority::critical);
    A::emerg{}("test");         EXPECT_EQ(attrs.level, priority::emergency);

    A::debug{}("test");         EXPECT_EQ(attrs.level, priority::debug);
    A::informational{}("test"); EXPECT_EQ(attrs.level, priority::informational);
    A::notice{}("test");        EXPECT_EQ(attrs.level, priority::notice);
    A::warning{}("test");       EXPECT_EQ(attrs.level, priority::warning);
    A::error{}("test");         EXPECT_EQ(attrs.level, priority::error);
    A::critical{}("test");      EXPECT_EQ(attrs.level, priority::critical);
    A::alert{}("test");         EXPECT_EQ(attrs.level, priority::alert);
    A::emergency{}("test");     EXPECT_EQ(attrs.level, priority::emergency);
}
