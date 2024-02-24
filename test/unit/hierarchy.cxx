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

TEST_F(Optional, DefaultLevels) {
    A::w("A ignored");                          EXPECT_EQ(message,"");
    A::e("A error");                            EXPECT_EQ(message,"A error");   reset();
    B::w("B ignored");                          EXPECT_EQ(message,"");
    B::e("B error");                            EXPECT_EQ(message,"B error");   reset();
    C::w("C ignored");                          EXPECT_EQ(message,"");
    C::e("C error");                            EXPECT_EQ(message,"C error");   reset();
    D::w("D ignored");                          EXPECT_EQ(message,"");
    D::e("D error");                            EXPECT_EQ(message,"D error");   reset();
}

TEST_F(Optional, SubLevelsOverriden) {
    B::category_type::level(priority::notice);
    D::category_type::level(priority::warning);
    A::w("A ignored");                          EXPECT_EQ(message,"");
    A::e("A error");                            EXPECT_EQ(message,"A error");   reset();
    B::i("B ignored");                          EXPECT_EQ(message,"");
    B::n("B notice");                           EXPECT_EQ(message,"B notice");
    B::w("B warning");                          EXPECT_EQ(message,"B warning");
    B::e("B error");                            EXPECT_EQ(message,"B error");   reset();
    C::w("C ignored");                          EXPECT_EQ(message,"");
    C::e("C error");                            EXPECT_EQ(message,"C error");   reset();
    D::n("D ignored");                          EXPECT_EQ(message,"");
    D::w("D warning");                          EXPECT_EQ(message,"D warning");
    D::e("D error");                            EXPECT_EQ(message,"D error");   reset();
}

TEST_F(Optional, MidLevelsOverriden) {
    C::category_type::level(priority::warning);
    A::w("A ignored");                          EXPECT_EQ(message,"");
    A::e("A error");                            EXPECT_EQ(message,"A error");   reset();
    B::w("B ignored");                          EXPECT_EQ(message,"");
    B::e("B error");                            EXPECT_EQ(message,"B error");   reset();
    C::n("C ignored");                          EXPECT_EQ(message,"");
    C::w("C warning");                          EXPECT_EQ(message,"C warning");
    C::e("C error");                            EXPECT_EQ(message,"C error");   reset();
    D::n("D ignored");                          EXPECT_EQ(message,"");
    D::w("D warning");                          EXPECT_EQ(message,"D warning");
    D::e("D error");                            EXPECT_EQ(message,"D error");   reset();
}

TEST_F(Optional, RootLevelsOverriden) {
    A::category_type::level(priority::warning);
    A::n("A ignored");                          EXPECT_EQ(message,"");
    A::w("A warning");                          EXPECT_EQ(message,"A warning");
    A::e("A error");                            EXPECT_EQ(message,"A error");   reset();
    B::n("B ignored");                          EXPECT_EQ(message,"");
    B::w("B warning");                          EXPECT_EQ(message,"B warning");
    B::e("B error");                            EXPECT_EQ(message,"B error");   reset();
    C::n("C ignored");                          EXPECT_EQ(message,"");
    C::w("C warning");                          EXPECT_EQ(message,"C warning");
    C::e("C error");                            EXPECT_EQ(message,"C error");   reset();
    D::n("D ignored");                          EXPECT_EQ(message,"");
    D::w("D warning");                          EXPECT_EQ(message,"D warning");
    D::e("D error");                            EXPECT_EQ(message,"D error");   reset();
}

TEST_F(Optional, LevelsReset) {
    A::category_type::level(priority::critical);
    B::category_type::level(priority::warning);
    C::category_type::level(priority::informational);
    D::category_type::level(priority::notice);

    A::e("A ignored");                          EXPECT_EQ(message,"");
    A::c("A critical");                         EXPECT_EQ(message,"A critical"); reset();
    B::n("B ignored");                          EXPECT_EQ(message,"");
    B::w("B warning");                          EXPECT_EQ(message,"B warning");  reset();
    C::d("C ignored");                          EXPECT_EQ(message,"");
    C::i("C info");                             EXPECT_EQ(message,"C info");     reset();
    D::d("D ignored");                          EXPECT_EQ(message,"");
    D::n("D notice");                           EXPECT_EQ(message,"D notice");   reset();
    D::category_type::nolevel();
    A::e("A ignored");                          EXPECT_EQ(message,"");
    A::c("A critical");                         EXPECT_EQ(message,"A critical"); reset();
    B::n("B ignored");                          EXPECT_EQ(message,"");
    B::w("B warning");                          EXPECT_EQ(message,"B warning");  reset();
    C::d("C ignored");                          EXPECT_EQ(message,"");
    C::i("C info");                             EXPECT_EQ(message,"C info");     reset();
    D::d("D ignored");                          EXPECT_EQ(message,"");
    D::i("D info");                             EXPECT_EQ(message,"D info");     reset();
    C::category_type::nolevel();
    A::e("A ignored");                          EXPECT_EQ(message,"");
    A::c("A critical");                         EXPECT_EQ(message,"A critical"); reset();
    B::n("B ignored");                          EXPECT_EQ(message,"");
    B::w("B warning");                          EXPECT_EQ(message,"B warning");  reset();
    C::e("C ignored");                          EXPECT_EQ(message,"");
    C::c("C critical");                         EXPECT_EQ(message,"C critical"); reset();
    D::e("D ignored");                          EXPECT_EQ(message,"");
    D::c("D critical");                         EXPECT_EQ(message,"D critical"); reset();
    B::category_type::nolevel();
    A::e("A ignored");                          EXPECT_EQ(message,"");
    A::c("A critical");                         EXPECT_EQ(message,"A critical"); reset();
    B::e("B ignored");                          EXPECT_EQ(message,"");
    B::c("B critical");                         EXPECT_EQ(message,"B critical"); reset();
    C::e("C ignored");                          EXPECT_EQ(message,"");
    C::c("C critical");                         EXPECT_EQ(message,"C critical"); reset();
    D::e("D ignored");                          EXPECT_EQ(message,"");
    D::c("D critical");                         EXPECT_EQ(message,"D critical"); reset();
    A::category_type::nolevel();
    A::w("A ignored");                          EXPECT_EQ(message,"");
    A::e("A error");                            EXPECT_EQ(message,"A error");   reset();
    B::w("B ignored");                          EXPECT_EQ(message,"");
    B::e("B error");                            EXPECT_EQ(message,"B error");   reset();
    C::w("C ignored");                          EXPECT_EQ(message,"");
    C::e("C error");                            EXPECT_EQ(message,"C error");   reset();
    D::w("D ignored");                          EXPECT_EQ(message,"");
    D::e("D error");                            EXPECT_EQ(message,"D error");   reset();
}

TEST_F(Optional, SinksOverriden) {
    static constexpr sink::bundle test_sink {
        [](std::ostream& out, attributes) noexcept { out << "P:"; },
        [](std::string_view msg, std::string_view, attributes) noexcept { local = msg; },
        [](attributes, std::ostream& out) noexcept -> std::size_t { out << ":E"; return 0; },
    };

    B::category_type::sink(test_sink);
    D::category_type::sink(test_sink);
    A::w("A ignored");                          EXPECT_EQ(message,"");
    A::e("A error");                            EXPECT_EQ(message,"A error");   reset();
    B::w("B ignored");                          EXPECT_EQ(message,"");
    B::e("B error");                            EXPECT_EQ(local,"P:B error:E"); EXPECT_EQ(message,"");  reset();
    C::w("C ignored");                          EXPECT_EQ(message,"");
    C::e("C error");                            EXPECT_EQ(message,"C error");   reset();
    D::w("D ignored");                          EXPECT_EQ(message,"");
    D::e("D error");                            EXPECT_EQ(local,"P:D error:E"); EXPECT_EQ(message,"");   reset();
    A::category_type::prolog([](std::ostream& out, attributes) noexcept { out << "p:"; });
    C::category_type::epilog([](attributes, std::ostream& out) noexcept -> std::size_t { out << ":e"; return 0; });
    A::w("A ignored");                          EXPECT_EQ(message,"");
    A::e("A error");                            EXPECT_EQ(message,"p:A error");   reset();
    B::w("B ignored");                          EXPECT_EQ(message,"");
    B::e("B error");                            EXPECT_EQ(local,"P:B error:E"); EXPECT_EQ(message,"");  reset();
    C::w("C ignored");                          EXPECT_EQ(message,"");
    C::e("C error");                            EXPECT_EQ(message,"p:C error:e");   reset();
    D::w("D ignored");                          EXPECT_EQ(message,"");
    D::e("D error");                            EXPECT_EQ(local,"P:D error:E"); EXPECT_EQ(message,"");   reset();
    B::category_type::noprolog();
    D::category_type::noepilog();
    A::w("A ignored");                          EXPECT_EQ(message,"");
    A::e("A error");                            EXPECT_EQ(message,"p:A error");   reset();
    B::w("B ignored");                          EXPECT_EQ(message,"");
    B::e("B error");                            EXPECT_EQ(local,"p:B error:E"); EXPECT_EQ(message,"");  reset();
    C::w("C ignored");                          EXPECT_EQ(message,"");
    C::e("C error");                            EXPECT_EQ(message,"p:C error:e");   reset();
    D::w("D ignored");                          EXPECT_EQ(message,"");
    D::e("D error");                            EXPECT_EQ(local,"P:D error:e"); EXPECT_EQ(message,"");   reset();
}


