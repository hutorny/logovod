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
namespace {
constexpr bool operator==(const attributes::source_location& a, const attributes::source_location& b) noexcept {
    return a.file_name == b.file_name && a.line == b.line;
}

constexpr bool operator==(attributes a, attributes b) noexcept {
    return a.level == b.level && a.tag == b.tag && a.location == b.location;
}
}

TEST_F(Sinks, FunctorOperational) {
    using namespace testing;
    const auto& S { FunctorCategory::sink };
    F::i{}("Functor"); source_location sl{source_location::current()};
    EXPECT_TRUE(message.empty());
    EXPECT_EQ(S.message, "F Functor;");
    EXPECT_EQ(S.payload, "Functor");
#if (defined(__clang__) && defined(__cpp_lib_source_location))
    const bool skip_location = !GTEST_FLAG(also_run_disabled_tests);
#else
    constexpr bool skip_location = false;
#endif
    if(!skip_location) {
        EXPECT_EQ(S.attrs[0].location.file_name, sl.file_name());
        EXPECT_EQ(S.attrs[0].location.line, sl.line());
    }
    EXPECT_EQ(S.attrs[0].tag, "F");
    EXPECT_EQ(S.attrs[0].level, priority::informational);
    EXPECT_TRUE(S.attrs[0] == S.attrs[1]);
    EXPECT_TRUE(S.attrs[0] == S.attrs[2]);
    F::i("Test"); EXPECT_EQ(S.message, "F Test;");
    EXPECT_TRUE(message.empty());
    F::i i{};
    i << "Flush";
    i << " test" << std::flush;
    EXPECT_EQ(S.message, "F Flush test;");
    EXPECT_EQ(write_count, 0);
}

TEST_F(Sinks, RuntimeDefaultOperational) {
    L::i("Test1"); EXPECT_EQ(message, "Test1");
    R::i("Test2"); EXPECT_EQ(message, "Test2");
    EXPECT_EQ(write_count, 2);
}

TEST_F(Sinks, RuntimeRedirectable) {
    static std::string local_message {};
    L::category_type::writer([](std::string_view msg, std::string_view, attributes) noexcept {
        local_message = msg;
    });
    L::i("Test1");
    EXPECT_EQ(message, "");
    EXPECT_EQ(local_message, "Test1");
    EXPECT_EQ(write_count, 0);
}

TEST_F(Sinks, RuntimeDistinctRedirectable) {
    static std::string local_message1 {};
    L::category_type::writer([](std::string_view msg, std::string_view, attributes) noexcept {
        local_message1 = msg;
    });
    static std::string local_message2 {};
    R::category_type::writer([](std::string_view msg, std::string_view, attributes) noexcept {
        local_message2 = msg;
    });
    L::i("Test1");
    R::i("Test2");
    EXPECT_EQ(message, "");
    EXPECT_EQ(local_message1, "Test1");
    EXPECT_EQ(local_message2, "Test2");
    EXPECT_EQ(write_count, 0);
}

TEST_F(Sinks, RuntimeProlog) {
    L::category_type::prolog([](std::ostream& out, attributes) noexcept { out << "L:"; });
    R::category_type::prolog([](std::ostream& out, attributes) noexcept { out << "R:"; });
    L::i("Test1");
    EXPECT_EQ(message, "L:Test1");
    R::i("Test2");
    EXPECT_EQ(message, "R:Test2");
    EXPECT_EQ(write_count, 2);
}

TEST_F(Sinks, RuntimeEpilog) {
    L::category_type::epilog([](attributes, std::ostream& out) noexcept -> std::size_t { out << ":L"; return 0; });
    R::category_type::epilog([](attributes, std::ostream& out) noexcept -> std::size_t { out << ":R"; return 0; });
    L::i("Test1");
    EXPECT_EQ(message, "Test1:L");
    R::i("Test2");
    EXPECT_EQ(message, "Test2:R");
    EXPECT_EQ(write_count, 2);
}

TEST_F(Sinks, Payload) {
    L::category_type::prolog([](std::ostream& out, attributes) noexcept { out << "L:"; });
    R::category_type::prolog([](std::ostream& out, attributes) noexcept { out << "R:"; });
    L::category_type::epilog([](attributes, std::ostream& out) noexcept -> std::size_t { out << ":L"; return 0; });
    R::category_type::epilog([](attributes, std::ostream& out) noexcept -> std::size_t { out << ":R"; return 0; });

    static std::string local_message1 {};
    L::category_type::writer([](std::string_view, std::string_view pload, attributes) noexcept {
        local_message1 = pload;
    });
    static std::string local_message2 {};
    R::category_type::writer([](std::string_view, std::string_view pload, attributes) noexcept {
        local_message2 = pload;
    });
    L::i("Test1");
    EXPECT_EQ(local_message1, "Test1");
    R::i("Test2");
    EXPECT_EQ(local_message2, "Test2");
    EXPECT_EQ(write_count, 0);
}
