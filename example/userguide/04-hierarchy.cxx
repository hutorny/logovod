/*
 * This file is on public domain. Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this file, either in source code form or as a compiled binary, for any purpose, commercial
 * or non-commercial, and by any means.
 *
 * Visit https://unlicense.org/ for details
 */

#include <logovod/logovod.h>

using namespace logovod;

// Compile time fixed category to be uses as the base
struct MyFixedCategory : category {
    static constexpr priority level() noexcept { return priority::error; }
    static constexpr auto writer() noexcept { return sink::cerr; }
};

// Root log category with run-time configurable optional sink and level
struct MyRootCategory : optional_all<MyRootCategory, MyFixedCategory> {};

// Hierarchy of four categories
struct MyCategoryA : optional_all<MyCategoryA, MyRootCategory> {};
struct MyCategoryB : optional_all<MyCategoryB, MyCategoryA> {};
struct MyCategoryC : optional_all<MyCategoryC, MyRootCategory> {};
struct MyCategoryD : optional_all<MyCategoryD, MyCategoryC> {};

using LogA = logger<MyCategoryA>;
using LogB = logger<MyCategoryB>;
using LogC = logger<MyCategoryC>;
using LogD = logger<MyCategoryD>;

int main() {
    LogA::d("Not printed");
    LogA::category_type::level(priority::debug);
    LogA::d("Printed to stderr");
    LogB::d("Also printed to stderr");
    LogD::category_type::writer(sink::fd<1>);
    LogD::d("Printed to stdout");
    return 0;
}
