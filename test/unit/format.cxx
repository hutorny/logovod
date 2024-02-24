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
#if defined(__cpp_lib_format)

TEST_F(Format, Printed) {
    int i = 11;
    constexpr long l = 111L;
    int& r { i };
    std::string s { "string" };
    std::string_view v { "string view" };
    const char* cs = "const char*";
    char c[] = "char[]";

    F::i{}.format("FMT {} {} {}", 10, "test", i);   EXPECT_EQ(message, "FMT 10 test 11");
    F::i{}.format("long {} ref {}", l, r);          EXPECT_EQ(message, "long 111 ref 11");
    F::i{{}}.format("{} {}", s, v);                 EXPECT_EQ(message, "string string view");
    F::i().format("{} {}", cs, c);                  EXPECT_EQ(message, "const char* char[]");
}

TEST_F(Format, Appended) {
    F::i f{};
    f << "<<";
    f.format("{}>>", "!");                          EXPECT_EQ(message, "<<!>>");
    f.format("New {}", "string");                   EXPECT_EQ(message, "New string");
}

TEST_F(Format, LevelRespected) {
    W::i{}.format("Ignored");                       EXPECT_EQ(message, "");
    W::d{}.format("Ignored");                       EXPECT_EQ(message, "");
    W::n{}.format("Not written");                   EXPECT_EQ(message, "");
    W::w{}.format("Written");                       EXPECT_EQ(message, "Written");
    W::e{}.format("Emitted");                       EXPECT_EQ(message, "Emitted");
    W::a{}.format("Alerted");                       EXPECT_EQ(message, "Alerted");
    W::c{}.format("Critical");                      EXPECT_EQ(message, "Critical");
    W::f{}.format("Filed");                         EXPECT_EQ(message, "Filed");
}

TEST_F(Format, Limited) {
    std::string s(32, '-');
    L::i{}.format("{}", s);                         EXPECT_EQ(message,s.substr(0, 24));
    L::i i{{}};
    i << "123456789";
    i.format("{}", s);                              EXPECT_EQ(message,"123456789" + s.substr(0, 15));
    i.format("--------------------{}", 12345);      EXPECT_EQ(message,s.substr(0, 20) + "1234");
}

TEST_F(Format, Unimited) {
    std::string s(0x10000, '-');
    U::i{}.format("{}",s);                          EXPECT_EQ(message, s);
    EXPECT_EQ(write_count, 1);
}

TEST_F(Format, RuntimeProlog) {
    R::category_type::prolog([](std::ostream& out, attributes) noexcept { out << "R:"; });
    R::i{}.format("Test{}", 1);                     EXPECT_EQ(message, "R:Test1");
    R::i{}.format("{}2","Test");                    EXPECT_EQ(message, "R:Test2");
    EXPECT_EQ(write_count, 2);
}

TEST_F(Format, RuntimeEpilog) {
    R::category_type::epilog([](attributes, std::ostream& out) noexcept -> std::size_t { out << ":R"; return 0; });
    R::i{}.format("Test{}", 1);                     EXPECT_EQ(message, "Test1:R");
    R::i{}.format("{}2","Test");                    EXPECT_EQ(message, "Test2:R");
    EXPECT_EQ(write_count, 2);
}

TEST_F(Format, Payload) {
    R::category_type::prolog([](std::ostream& out, attributes) noexcept { out << "R:"; });
    R::category_type::epilog([](attributes, std::ostream& out) noexcept -> std::size_t { out << ":R"; return 0; });

    static std::string local_message {};
    R::category_type::writer([](std::string_view, std::string_view pload, attributes) noexcept {
        local_message = pload;
    });
    R::i{}.format("Test{}", 1);                     EXPECT_EQ(local_message, "Test1");
    R::i{}.format("{}2","Test");                    EXPECT_EQ(local_message, "Test2");
    EXPECT_EQ(write_count, 0);
}

#endif
