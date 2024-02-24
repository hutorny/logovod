/*
 * Copyright (C) 2024 Eugene Hutorny <eugene@hutorny.in.ua>
 *
 * This file is a part of logovod library
 *
 * Licensed under MIT License, see full text in LICENSE
 * or visit page https://opensource.org/license/mit/
 */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wsign-promo"

#include "tests.h"
 
using namespace logovod::tests;
using namespace logovod;

TEST_F(ScalarTypes, Integers) {
    L::i{}(100);               EXPECT_EQ(message,"100");
    L::i{}(1000UL);            EXPECT_EQ(message,"1000");
    L::i{}(-10000LL);          EXPECT_EQ(message,"-10000");
    short s { -999 };
    L::i{}(s);                 EXPECT_EQ(message,"-999");
    L::i{}(s++);               EXPECT_EQ(message,"-999");
    [&s]() { L::i{}(s); }();   EXPECT_EQ(message,"-998");
    constexpr signed char c = 33;
    L::i{}(c);                 EXPECT_EQ(message,"33");
    unsigned char u = 44;
    L::i{}(u);                 EXPECT_EQ(message,"44");
    unsigned char& ur { ++u };
    L::i{}(ur);                EXPECT_EQ(message,"45");
    auto sp = &c;
    L::i{}(*sp);               EXPECT_EQ(message,"33");
    L::i{} << s << u << *sp;   EXPECT_EQ(message,"-9984533");
    EXPECT_EQ(write_count, 11);
}

TEST_F(ScalarTypes, Enums) {
    enum A { a, b, c };
    enum class B { e, f, g };
    A a1 { c };
    A& ar { a1 };
    A* ap { &a1 };
    const A& ac { a1 };
    B b1 { B::f };
    B& br { b1 };
    B* bp { &b1 };
    const B& bc { b1 };

    L::i{}(a1);               EXPECT_EQ(message,"2");
    L::i{}(b1);               EXPECT_EQ(message,"1");
    L::i{}(ar);               EXPECT_EQ(message,"2");
    L::i{}(br);               EXPECT_EQ(message,"1");
    L::i{}(ac);               EXPECT_EQ(message,"2");
    L::i{}(bc);               EXPECT_EQ(message,"1");
    L::i{}(*ap);              EXPECT_EQ(message,"2");
    L::i{}(*bp);              EXPECT_EQ(message,"1");
    L::i{} << a1 << b1 << ar << br; EXPECT_EQ(message,"2121");
    EXPECT_EQ(write_count, 9);
}

TEST_F(ScalarTypes, Floats) {
    float f = 2.3f;
    double d = 7.03e-8;
    auto& r { f };
    const auto& c { d };
    L::i{}(1.1);                     EXPECT_EQ(message,"1.1");
    L::i{}(f);                       EXPECT_EQ(message,"2.3");
    L::i{}(d);                       EXPECT_EQ(message,"7.03e-08");
    L::i{}(r);                       EXPECT_EQ(message,"2.3");
    L::i{}(c);                       EXPECT_EQ(message,"7.03e-08");
    EXPECT_EQ(write_count, 5);
}

TEST_F(LoggerRadix, Hex) {
    using namespace std; 
    L::i{}(hex, 255);                     EXPECT_EQ(message,"ff");
    L::i{} << hex << 254;                 EXPECT_EQ(message,"fe");
    L::i{}(L::x(253));                    EXPECT_EQ(message,"fd");
    L::i{}(L::x(252, 0x123456789LL));     EXPECT_EQ(message,"fc 123456789");
    constexpr signed char c = 0x33;
    L::i{}(hex, c);                       EXPECT_EQ(message,"33");
    unsigned char u = 0x44;
    L::i{}(L::x(c, u, 'f'));              EXPECT_EQ(message,"33 44 66");
    const auto& r {u};
    auto p {&u};
    L::i{}(L::x(r), L::x(*p));            EXPECT_EQ(message,"44 44");
    L::i{} << L::x(0x33);                 EXPECT_EQ(message,"33");

    EXPECT_EQ(write_count, 8);
}

TEST_F(LoggerRadix, Oct) {
    using namespace std; 
    L::i{}(oct, 0255);                     EXPECT_EQ(message,"255");
    L::i{} << oct << 0245;                 EXPECT_EQ(message,"245");
    L::i{}(L::o(0253));                    EXPECT_EQ(message,"253");
    L::i{}(L::o(0252, 0123456701LL));      EXPECT_EQ(message,"252 123456701");
    constexpr signed char c = 033;
    L::i{}(oct, c);                        EXPECT_EQ(message,"33");
    unsigned char u = 044;
    L::i{}(L::o(c, u, 'F'));               EXPECT_EQ(message,"33 44 106");
    const auto& r {u};
    auto p {&u};
    L::i{}(L::o(r), L::o(*p));             EXPECT_EQ(message,"44 44");
    L::i{} << L::o(033);                   EXPECT_EQ(message,"33");

    EXPECT_EQ(write_count, 8);
}

TEST_F(LoggerRadix, Bin) {
    using namespace std; 
    L::i{}(L::b(0b1010111));                EXPECT_EQ(message,"00000000000000000000000001010111");
    L::i{}(L::b(0b1111110000LL));           EXPECT_EQ(message,"0000000000000000000000000000000000000000000000000000001111110000");
    constexpr unsigned char c = 0b11110000;
    L::i{}(L::b(c));                        EXPECT_EQ(message,"11110000");
    unsigned char u = 0b10101010;
    L::i{}(L::b('U'));                      EXPECT_EQ(message,"01010101");
    const auto& r {u};
    auto p {&c};
    L::i{}(L::b(*p));                       EXPECT_EQ(message,"11110000");
    L::i{}(L::b(r));                        EXPECT_EQ(message,"10101010");
    L::i() << L::b('U');                    EXPECT_EQ(message,"01010101");

    EXPECT_EQ(write_count, 7);
}
#pragma GCC diagnostic pop

TEST_F(LoggerRadix, Mix) {
    using namespace std; 
    L::i{}(L::x(0x10), L::t(10), L::o(010)); EXPECT_EQ(message,"10 10 10");
    L::i{}(hex, 0x20,  L::t(20), 0x20);      EXPECT_EQ(message,"20 20 20");
    L::i{} << hex << 0x30 << L::o(021);      EXPECT_EQ(message,"3021"); 
    EXPECT_EQ(write_count, 3);
}

TEST_F(LoggerRadix, Fixed) {
    using namespace std;
    float f = 2.3f;
    double d = 7.03e-4;
    auto& r { f };
    const auto& c { d };
    L::i{}(L::fix<8,4>(1e-3));            EXPECT_EQ(message,"  0.0010");
    L::i{}(L::fix<8,2>(-12345.678));      EXPECT_EQ(message,"-12345.68");
    L::i{} << L::fix<8,2>(135.7931);      EXPECT_EQ(message,"  135.79");
    L::i{}(L::fix<6,4>(r));               EXPECT_EQ(message,"2.3000");
    L::i{}(L::fix<6,3>(d));               EXPECT_EQ(message," 0.001");
    L::i{}(setw(6), setprecision(4), 1.1234, L::fix<8,2>(f));             EXPECT_EQ(message," 1.123     2.30");
    L::i{} << setw(10) << setprecision(9) << 12.345678 << L::fix<9,4>(c); EXPECT_EQ(message," 12.345678   0.0007");
    EXPECT_EQ(write_count, 7);
}

TEST_F(LoggerRadix, FormatBeingReset) {
    using namespace std;
    L::i l{};
    l << setw(12) << setfill('_') << setprecision(4) << 1.001 << endl;
    l << 1.0 << " RIGHT " << '.' << endl; EXPECT_EQ(message,"1 RIGHT .");
    l << 1.0 << " RIGHT " << '.' << endl;
    l << hex << 0x1000 << endl;
    l << 1000 << endl;                    EXPECT_EQ(message,"1000");
    l << oct << 02000 << endl;
    l << 2000 << endl;                    EXPECT_EQ(message,"2000");
    EXPECT_EQ(write_count, 7);
}

TEST_F(LoggerRadix, UsedAsIostream) {
    using namespace std;
    L::i l{};
    for(int i = 1; i <= 4; i++)
        l << setw(2) << i;
    l << "." << endl;
    EXPECT_EQ(message," 1 2 3 4.");
    EXPECT_EQ(write_count, 1);
}

TEST_F(LoggerRadix, ConstructAndWrite) {
    const int i = 888;
    L::i("Test", i);                      EXPECT_EQ(message,"Test 888");
    EXPECT_EQ(write_count, 1);
}

TEST_F(Strings, Strings) {
    using namespace std;
    const string acs {"a const string"};
    string ps { "plain string" };
    static constexpr auto cev = "view"sv;
    string_view psv { ps };
    const char * cs = "C string";
    const char ca[] = "C[] string";
    char rw[] = "rw";
    auto& acr { acs };
    auto& psr { ps };
    auto& csr { cs };
    auto& car { ca };
    auto& rwr { rw };

    L::i{}(acs, ps, cev, psv, cs, ca);
    EXPECT_EQ(message, "a const string plain string view plain string C string C[] string");
    L::i(rw, acr, psr, csr, car, rwr);
    EXPECT_EQ(message, "rw a const string plain string C string C[] string rw");
    EXPECT_EQ(write_count, 2);
}
