/*
 * Copyright (C) 2024 Eugene Hutorny <eugene@hutorny.in.ua>
 *
 * This file is a part of logovod library
 *
 * Licensed under MIT License, see full text in LICENSE
 * or visit page https://opensource.org/license/mit/
 */

#include "tests.h"
using namespace logovod;
using namespace logovod::tests;

TEST_F(Wchar, Wstrings) {
    using namespace std;
    const wstring acs {L"a const wstring"};
    wstring ps { L"plain wstring" };
    static constexpr auto cev = L"view"sv;
    wstring_view psv { ps };
    const wchar_t * cs = L"C wstring";
    const wchar_t ca[] = L"C[] wstring";
    wchar_t rw[] = L"rw";
    auto& acr { acs };
    auto& psr { ps };
    auto& csr { cs };
    auto& car { ca };
    auto& rwr { rw };
    C::i{}(acs, ps, cev, psv, cs, ca);
    EXPECT_EQ(message, L"a const wstring plain wstring view plain wstring C wstring C[] wstring");
    C::i(rw, acr, psr, csr, car, rwr);
    EXPECT_EQ(message, L"rw a const wstring plain wstring C wstring C[] wstring rw");
    EXPECT_EQ(write_count, 2);
}

TEST_F(Wchar, Limited) {
    std::wstring s(32, '-');
    L::i{}(s); EXPECT_EQ(message,s.substr(0, 24));
    L::i i{};
    i << L"123456789";
    i << s << std::flush;
    EXPECT_EQ(message,L"123456789" + s.substr(0, 15));
    L::i(6789, s); EXPECT_EQ(message,L"6789 " + s.substr(0, 19));
    L::i{} << s.substr(0, 20) << 987654321; EXPECT_EQ(message,s.substr(0, 20)+L"9876");
    EXPECT_EQ(write_count, 4);
}

TEST_F(Wchar, Terminated) {
    std::wstring s(32, '-');
    T::i{}(s); EXPECT_EQ(message,s.substr(0, 23) + L'\n');
    T::i i{};
    i << L"123456789";
    i << s << std::flush;
    EXPECT_EQ(message,L"123456789" + s.substr(0, 14) + L'\n');
    T::i(6789, s); EXPECT_EQ(message,L"6789 " + s.substr(0, 18) + L'\n');
    T::i{} << s.substr(0, 20) << 987654321; EXPECT_EQ(message,s.substr(0, 20)+L"987\n");
    EXPECT_EQ(write_count, 4);
}

TEST_F(Wchar, UsableAfterOverflow) {
    std::wstring s(32, '-');
    L::i i{};
    i << s << std::flush;         EXPECT_EQ(message,s.substr(0, 24));
    i << 987654;
    i << s << std::flush;         EXPECT_EQ(message,L"987654" + s.substr(0, 18));
    EXPECT_EQ(write_count, 2);
}

TEST_F(Wchar, Unlimited) {
    std::wstring s(0x10000, '-');
    U::i{}(s); EXPECT_EQ(message, s);
    EXPECT_EQ(write_count, 1);
}

