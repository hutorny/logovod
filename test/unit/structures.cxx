/*
 * Copyright (C) 2024 Eugene Hutorny <eugene@hutorny.in.ua>
 *
 * This file is a part of logovod library
 *
 * Licensed under MIT License, see full text in LICENSE
 * or visit page https://opensource.org/license/mit/
 */

#include "tests.h"

#include <chrono>
#include <deque>
#include <list>
#include <forward_list>
#include <optional>
#include <variant>
 
using namespace logovod::tests;
using namespace logovod;

TEST_F(Containers, Arrays) {
    signed char sc[] { 1, -2, 3, -4, 127, -128 };
    const signed char csc[] { 10, -20, 30, -40 };
    unsigned char uc[] { 4, 3, 2, 1, 0, 255 };
    const unsigned char cuc[] { 4, 3, 1, 2 };
    signed short ss[] { 1000, -1001 };
    unsigned short us[] { 9988, 65533 };
    short ps[] { 31000, -31000 };
    int i[] { -99, -98, 97, -96 };
    char c8[8] = "char[8]";
    const char c14[] = "const char[]";

    auto& rsc { sc };
    const auto& crsc { sc };
    auto& rcsc { csc };

    auto& ruc { uc };
    const auto& cruc { uc };
    auto& rcuc { cuc };

    L::i{}(sc);      EXPECT_EQ(message, "{1,-2,3,-4,127,-128}");
    L::i{}(csc);     EXPECT_EQ(message, "{10,-20,30,-40}");
    L::i{}(rsc);     EXPECT_EQ(message, "{1,-2,3,-4,127,-128}");
    L::i{}(uc);      EXPECT_EQ(message, "{4,3,2,1,0,255}");
    L::i{}(crsc);    EXPECT_EQ(message, "{1,-2,3,-4,127,-128}");
    L::i{}(rcsc);    EXPECT_EQ(message, "{10,-20,30,-40}");
    L::i{}(ruc);     EXPECT_EQ(message, "{4,3,2,1,0,255}");
    L::i{}(cuc);     EXPECT_EQ(message, "{4,3,1,2}");
    L::i{}(ss);      EXPECT_EQ(message, "{1000,-1001}");
    L::i{}(us);      EXPECT_EQ(message, "{9988,65533}");
    L::i{}(ps);      EXPECT_EQ(message, "{31000,-31000}");
    L::i{}(i);       EXPECT_EQ(message, "{-99,-98,97,-96}");
    L::i{}(cruc);    EXPECT_EQ(message, "{4,3,2,1,0,255}");
    L::i{}(rcuc);    EXPECT_EQ(message, "{4,3,1,2}");
    L::i{}(c8);      EXPECT_EQ(message, "char[8]");
    L::i{}(c14);     EXPECT_EQ(message, "const char[]");
    L::i{} << sc;    EXPECT_EQ(message, "{1,-2,3,-4,127,-128}");
    EXPECT_EQ(write_count, 17);
}


TEST_F(Containers, StdArray) {
    std::array<signed char, 6> sc { 1, -2, 3, -4, 127, -128 };
    std::array<const signed char, 4> csc { 10, -20, 30, -40 };
    constexpr std::array<unsigned char, 6> uc { 4, 3, 2, 1, 0, 255 };
    std::array<const unsigned char, 4> cuc { 4, 3, 1, 2 };
    std::array<signed short, 2> ss { 1000, -1001 };
    std::array<unsigned short, 2> us { 9988, 65533 };
    const std::array<short, 2> ps { 31000, -31000 };
    std::array<int, 4> i { -99, -98, 97, -96 };
    std::array<char, 7> c8 { 'c', 'h', 'a', 'r', '[', '8', ']' };

    auto& rsc { sc };
    const auto& crsc { sc };
    auto& rcsc { csc };

    auto& ruc { uc };
    const auto& cruc { uc };
    auto& rcuc { cuc };

    L::i{}(sc);      EXPECT_EQ(message, "{1,-2,3,-4,127,-128}");
    L::i{}(csc);     EXPECT_EQ(message, "{10,-20,30,-40}");
    L::i{}(rsc);     EXPECT_EQ(message, "{1,-2,3,-4,127,-128}");
    L::i{}(uc);      EXPECT_EQ(message, "{4,3,2,1,0,255}");
    L::i{}(crsc);    EXPECT_EQ(message, "{1,-2,3,-4,127,-128}");
    L::i{}(rcsc);    EXPECT_EQ(message, "{10,-20,30,-40}");
    L::i{}(ruc);     EXPECT_EQ(message, "{4,3,2,1,0,255}");
    L::i{}(cuc);     EXPECT_EQ(message, "{4,3,1,2}");
    L::i{}(ss);      EXPECT_EQ(message, "{1000,-1001}");
    L::i{}(us);      EXPECT_EQ(message, "{9988,65533}");
    L::i{}(ps);      EXPECT_EQ(message, "{31000,-31000}");
    L::i{}(i);       EXPECT_EQ(message, "{-99,-98,97,-96}");
    L::i{}(cruc);    EXPECT_EQ(message, "{4,3,2,1,0,255}");
    L::i{}(rcuc);    EXPECT_EQ(message, "{4,3,1,2}");
    L::i{}(c8);      EXPECT_EQ(message, "{c,h,a,r,[,8,]}");
    L::i{} << sc;    EXPECT_EQ(message, "{1,-2,3,-4,127,-128}");
    EXPECT_EQ(write_count, 16);
}

TEST_F(Containers, Vector) {
    const std::vector<signed char> sc { 1, -2, 3, -4, 127, -128 };
    std::vector<signed char> csc { 10, -20, 30, -40 };
    std::vector<unsigned char> uc { 4, 3, 2, 1, 0, 255 };
    std::vector<unsigned char> cuc { 4, 3, 1, 2 };
    std::vector<signed short> ss { 1000, -1001 };
    std::vector<unsigned short> us { 9988, 65533 };
    std::vector<short> ps { 31000, -31000 };
    const std::vector<int> i { -99, -98, 97, -96 };

    auto& rsc { sc };
    const auto& crsc { sc };
    auto& rcsc { csc };

    auto& ruc { uc };
    const auto& cruc { uc };
    auto& rcuc { cuc };

    L::i{}(sc);      EXPECT_EQ(message, "{1,-2,3,-4,127,-128}");
    L::i{}(csc);     EXPECT_EQ(message, "{10,-20,30,-40}");
    L::i{}(rsc);     EXPECT_EQ(message, "{1,-2,3,-4,127,-128}");
    L::i{}(uc);      EXPECT_EQ(message, "{4,3,2,1,0,255}");
    L::i{}(crsc);    EXPECT_EQ(message, "{1,-2,3,-4,127,-128}");
    L::i{}(rcsc);    EXPECT_EQ(message, "{10,-20,30,-40}");
    L::i{}(ruc);     EXPECT_EQ(message, "{4,3,2,1,0,255}");
    L::i{}(cuc);     EXPECT_EQ(message, "{4,3,1,2}");
    L::i{}(ss);      EXPECT_EQ(message, "{1000,-1001}");
    L::i{}(us);      EXPECT_EQ(message, "{9988,65533}");
    L::i{}(ps);      EXPECT_EQ(message, "{31000,-31000}");
    L::i{}(i);       EXPECT_EQ(message, "{-99,-98,97,-96}");
    L::i{}(cruc);    EXPECT_EQ(message, "{4,3,2,1,0,255}");
    L::i{}(rcuc);    EXPECT_EQ(message, "{4,3,1,2}");
    L::i{} << sc;    EXPECT_EQ(message, "{1,-2,3,-4,127,-128}");
    EXPECT_EQ(write_count, 15);
}

TEST_F(Containers, List) {
    std::list<signed char> sc { 1, -2, 3, -4, 127, -128 };
    std::list<signed char> csc { 10, -20, 30, -40 };
    std::list<unsigned char> uc { 4, 3, 2, 1, 0, 255 };
    const std::list<unsigned char> cuc { 4, 3, 1, 2 };
    std::list<signed short> ss { 1000, -1001 };
    std::list<unsigned short> us { 9988, 65533 };
    std::list<short> ps { 31000, -31000 };
    std::list<int> i { -99, -98, 97, -96 };

    auto& rsc { sc };
    const auto& crsc { sc };
    auto& rcsc { csc };

    auto& ruc { uc };
    const auto& cruc { uc };
    auto& rcuc { cuc };

    L::i{}(sc);      EXPECT_EQ(message, "{1,-2,3,-4,127,-128}");
    L::i{}(csc);     EXPECT_EQ(message, "{10,-20,30,-40}");
    L::i{}(rsc);     EXPECT_EQ(message, "{1,-2,3,-4,127,-128}");
    L::i{}(uc);      EXPECT_EQ(message, "{4,3,2,1,0,255}");
    L::i{}(crsc);    EXPECT_EQ(message, "{1,-2,3,-4,127,-128}");
    L::i{}(rcsc);    EXPECT_EQ(message, "{10,-20,30,-40}");
    L::i{}(ruc);     EXPECT_EQ(message, "{4,3,2,1,0,255}");
    L::i{}(cuc);     EXPECT_EQ(message, "{4,3,1,2}");
    L::i{}(ss);      EXPECT_EQ(message, "{1000,-1001}");
    L::i{}(us);      EXPECT_EQ(message, "{9988,65533}");
    L::i{}(ps);      EXPECT_EQ(message, "{31000,-31000}");
    L::i{}(i);       EXPECT_EQ(message, "{-99,-98,97,-96}");
    L::i{}(cruc);    EXPECT_EQ(message, "{4,3,2,1,0,255}");
    L::i{}(rcuc);    EXPECT_EQ(message, "{4,3,1,2}");
    L::i{} << sc;    EXPECT_EQ(message, "{1,-2,3,-4,127,-128}");
    EXPECT_EQ(write_count, 15);
}

TEST_F(Containers, ForwardList) {
    const std::forward_list<signed char> sc { 1, -2, 3, -4, 127, -128 };
    std::forward_list<signed char> csc { 10, -20, 30, -40 };
    std::forward_list<unsigned char> uc { 4, 3, 2, 1, 0, 255 };
    std::forward_list<unsigned char> cuc { 4, 3, 1, 2 };
    std::forward_list<signed short> ss { 1000, -1001 };
    std::forward_list<unsigned short> us { 9988, 65533 };
    const std::forward_list<short> ps { 31000, -31000 };
    std::forward_list<int> i { -99, -98, 97, -96 };

    auto& rsc { sc };
    const auto& crsc { sc };
    auto& rcsc { csc };

    auto& ruc { uc };
    const auto& cruc { uc };
    auto& rcuc { cuc };

    L::i{}(sc);      EXPECT_EQ(message, "{1,-2,3,-4,127,-128}");
    L::i{}(csc);     EXPECT_EQ(message, "{10,-20,30,-40}");
    L::i{}(rsc);     EXPECT_EQ(message, "{1,-2,3,-4,127,-128}");
    L::i{}(uc);      EXPECT_EQ(message, "{4,3,2,1,0,255}");
    L::i{}(crsc);    EXPECT_EQ(message, "{1,-2,3,-4,127,-128}");
    L::i{}(rcsc);    EXPECT_EQ(message, "{10,-20,30,-40}");
    L::i{}(ruc);     EXPECT_EQ(message, "{4,3,2,1,0,255}");
    L::i{}(cuc);     EXPECT_EQ(message, "{4,3,1,2}");
    L::i{}(ss);      EXPECT_EQ(message, "{1000,-1001}");
    L::i{}(us);      EXPECT_EQ(message, "{9988,65533}");
    L::i{}(ps);      EXPECT_EQ(message, "{31000,-31000}");
    L::i{}(i);       EXPECT_EQ(message, "{-99,-98,97,-96}");
    L::i{}(cruc);    EXPECT_EQ(message, "{4,3,2,1,0,255}");
    L::i{}(rcuc);    EXPECT_EQ(message, "{4,3,1,2}");
    L::i{} << sc;    EXPECT_EQ(message, "{1,-2,3,-4,127,-128}");
    EXPECT_EQ(write_count, 15);
}

TEST_F(Containers, Deque) {
    std::deque<signed char> sc { 1, -2, 3, -4, 127, -128 };
    std::deque<signed char> csc { 10, -20, 30, -40 };
    std::deque<unsigned char> uc { 4, 3, 2, 1, 0, 255 };
    const std::deque<unsigned char> cuc { 4, 3, 1, 2 };
    const std::deque<signed short> ss { 1000, -1001 };
    std::deque<unsigned short> us { 9988, 65533 };
    std::deque<short> ps { 31000, -31000 };
    std::deque<int> i { -99, -98, 97, -96 };

    auto& rsc { sc };
    const auto& crsc { sc };
    auto& rcsc { csc };

    auto& ruc { uc };
    const auto& cruc { uc };
    auto& rcuc { cuc };

    L::i{}(sc);      EXPECT_EQ(message, "{1,-2,3,-4,127,-128}");
    L::i{}(csc);     EXPECT_EQ(message, "{10,-20,30,-40}");
    L::i{}(rsc);     EXPECT_EQ(message, "{1,-2,3,-4,127,-128}");
    L::i{}(uc);      EXPECT_EQ(message, "{4,3,2,1,0,255}");
    L::i{}(crsc);    EXPECT_EQ(message, "{1,-2,3,-4,127,-128}");
    L::i{}(rcsc);    EXPECT_EQ(message, "{10,-20,30,-40}");
    L::i{}(ruc);     EXPECT_EQ(message, "{4,3,2,1,0,255}");
    L::i{}(cuc);     EXPECT_EQ(message, "{4,3,1,2}");
    L::i{}(ss);      EXPECT_EQ(message, "{1000,-1001}");
    L::i{}(us);      EXPECT_EQ(message, "{9988,65533}");
    L::i{}(ps);      EXPECT_EQ(message, "{31000,-31000}");
    L::i{}(i);       EXPECT_EQ(message, "{-99,-98,97,-96}");
    L::i{}(cruc);    EXPECT_EQ(message, "{4,3,2,1,0,255}");
    L::i{}(rcuc);    EXPECT_EQ(message, "{4,3,1,2}");
    L::i{} <<sc;     EXPECT_EQ(message, "{1,-2,3,-4,127,-128}");
    EXPECT_EQ(write_count, 15);
}

TEST_F(Containers, Multidimensional) {
    std::array<int[3], 3> aai {{{1,2,3}, {4,5,6}, {7,8,9}}};
    std::deque<std::vector<std::array<short,2>>> qvas {{{1,2},{3,4}},{{5,6},{7,8}}};

    L::i{}(aai);     EXPECT_EQ(message, "{{1,2,3},{4,5,6},{7,8,9}}");
    L::i{}(qvas);    EXPECT_EQ(message, "{{{1,2},{3,4}},{{5,6},{7,8}}}");
    L::i{} << aai;   EXPECT_EQ(message, "{{1,2,3},{4,5,6},{7,8,9}}"); 
    L::i{} << qvas;  EXPECT_EQ(message, "{{{1,2},{3,4}},{{5,6},{7,8}}}");
    EXPECT_EQ(write_count, 4);
}

TEST_F(Containers, RangeLimit) {
    std::vector<int> v20(20, 1);
    L::i{}(v20);     EXPECT_EQ(message, "{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,...}");
    R::i{}(v20);     EXPECT_EQ(message, "{1,1,1,1,..}");
    EXPECT_EQ(write_count, 2);
}

TEST_F(Structures, Tuple) {
    std::tuple<int, const char*> icc { 10, "char*" };
    const std::tuple<char, std::string> cs { 'A', "Str" };
    const std::tuple<std::string_view, signed char> svsc { "view", 33 };
    char chr[] = "char[]";
    const std::tuple<long, unsigned char, char*> luccp { 123456678L, 255, chr };
    std::pair<unsigned, bool> ub { 98765432, true };
    std::pair<std::string, std::pair<int, const char*>> sic { "string", {1111111, "const char*"} };

    L::i{}(icc);     EXPECT_EQ(message, "{10,char*}");
    L::i{}(cs);      EXPECT_EQ(message, "{A,Str}");
    L::i{}(svsc);    EXPECT_EQ(message, "{view,33}");
    L::i{}(luccp);   EXPECT_EQ(message, "{123456678,255,char[]}");
    L::i{}(ub);      EXPECT_EQ(message, "{98765432,1}");
    L::i{}(sic);     EXPECT_EQ(message, "{string,{1111111,const char*}}");
    L::i{} << icc;   EXPECT_EQ(message, "{10,char*}");
    EXPECT_EQ(write_count, 7);
}

TEST_F(Structures, Optional) {
    std::optional<std::string> sn {};
    std::optional<std::string> ss {"string"};
    const std::optional<std::string> cos {"cos"};
    std::optional<int> in {};
    std::optional<int> ii {1};
    std::optional<std::pair<int,std::string>> op {{1, "op"}};

    L::i{}(sn);      EXPECT_EQ(message, "{}");
    L::i{}(ss);      EXPECT_EQ(message, "string");
    L::i{}(cos);     EXPECT_EQ(message, "cos");
    L::i{}(in);      EXPECT_EQ(message, "{}");
    L::i{}(ii);      EXPECT_EQ(message, "1");
    L::i{}(op);      EXPECT_EQ(message, "{1,op}");
    L::i{} << ss;    EXPECT_EQ(message, "string");
    EXPECT_EQ(write_count, 7);
}

TEST_F(Structures, Variant) {
    std::variant<bool, std::string, int, double> vn {};
    std::variant<std::string, int, double> vs {"string"};
    std::variant<std::string, int, double> vi {10};
    std::variant<std::string, int, double> vd {0.25};
    L::i{}(vn);      EXPECT_EQ(message, "0");
    L::i{}(vs);      EXPECT_EQ(message, "string");
    L::i{}(vi);      EXPECT_EQ(message, "10");
    L::i{}(vd);      EXPECT_EQ(message, "0.25");
    L::i{} << vs;    EXPECT_EQ(message, "string");
    EXPECT_EQ(write_count, 5);
}

TEST_F(Structures, Timepoint) {
    using namespace std::literals;
    std::chrono::system_clock::time_point tps {1709880066s};
    std::chrono::system_clock::time_point tpm {1709880066977ms};
    std::chrono::system_clock::time_point tpu {1709880066977543us};
    setenv("TZ","UTC", true); // logovod prints local time, setting UTC to ensure results match everywhere
    if constexpr(detail::have_lshift_operator_v<std::ostream, std::chrono::system_clock::time_point>) {
        L::i{}(tpu);      EXPECT_EQ(message, "2024-03-08 06:41:06.977543000");
        L::i{}(tps);      EXPECT_EQ(message, "2024-03-08 06:41:06.000000000");
        L::i{}(tpm);      EXPECT_EQ(message, "2024-03-08 06:41:06.977000000");
        L::i{} << tps;    EXPECT_EQ(message, "2024-03-08 06:41:06.000000000");
    } else {
        L::i{}(tpu);      EXPECT_EQ(message, "2024-03-08T06:41:06.977");
        L::i{}(tps);      EXPECT_EQ(message, "2024-03-08T06:41:06.000");
        L::i{}(tpm);      EXPECT_EQ(message, "2024-03-08T06:41:06.977");
        L::i{} << tps;    EXPECT_EQ(message, "2024-03-08T06:41:06.000");
    }
    unsetenv("TZ");
    EXPECT_EQ(write_count, 4);
}

TEST_F(Structures, Duration) {
    using namespace std::literals;
    const auto ds = 80066s;
    const auto dm = 66ms;
    const auto du = 177us;
    const auto dn = 7711ns;
    const auto di = 61min;
    const auto dh = 25h;
    L::i{}(ds);     EXPECT_EQ(message, "80066s");
    L::i{}(dm);     EXPECT_EQ(message, "66ms");
    L::i{}(du);     EXPECT_TRUE(message == "177us" || message == "177µs"); // Some gcc versions print "µs"
    L::i{}(dn);     EXPECT_EQ(message, "7711ns");
    L::i{}(di);     EXPECT_EQ(message, "61min");
    L::i{}(dh);     EXPECT_EQ(message, "25h");
    L::i{} << ds;   EXPECT_EQ(message, "80066s");
}
#if defined(__clang__) && defined(__cpp_lib_source_location)
// clang, as of v.15, seems not properly supporting std::source_location yet - current() is taken from the header
TEST_F(SourceLocation, DISABLED_WrittenAsExpected) {
#else
TEST_F(SourceLocation, WrittenAsExpected) {
#endif
    using namespace std::literals;
    L::i{}("Message");    EXPECT_EQ(message, std::string{basename(__FILE__)}+":" + std::to_string(__LINE__)+":Message");
    L::i("Message");      EXPECT_EQ(message, "--:0:Message");
    L::i{} << "Test";     EXPECT_EQ(message, std::string{basename(__FILE__)}+":" + std::to_string(__LINE__)+":Test");
    L::i{{}}("no sloc");  EXPECT_EQ(message, "--:0:no sloc");
}

#if defined(__clang__) && defined(__cpp_lib_source_location)
TEST_F(SourceLocation, DISABLED_Printed) {
#else
TEST_F(SourceLocation, Printed) {
#endif
    L::i l{};             source_location sl {source_location::current()};
    l << "not here" << std::flush; 
    EXPECT_EQ(message, std::string{basename(sl.file_name())} + ':' + std::to_string(sl.line()) + ":not here");
    L::i{sl}("there");
    EXPECT_EQ(message, std::string{basename(sl.file_name())} + ':' + std::to_string(sl.line()) + ":there");
    L::i{} << sl;         source_location s2 {source_location::current()};
    EXPECT_EQ(message, std::string{basename(s2.file_name())} + ':' + std::to_string(s2.line()) + ":" + sl.file_name() + ":" + std::to_string(sl.line()));
}
#if defined(__clang__) && defined(__cpp_lib_source_location)
TEST_F(SourceLocation, DISABLED_Redefined) {
#else
TEST_F(SourceLocation, Redefined) {
#endif
    L::i l{};
    source_location sl {source_location::current()};
    l(sl);
    EXPECT_EQ(message, "");
    l << sl << "-1" << std::flush;
    EXPECT_EQ(message, std::string{basename(sl.file_name())} + ':' + std::to_string(sl.line()) + ":" + sl.file_name() + ":" + std::to_string(sl.line()) + "-1");
}

