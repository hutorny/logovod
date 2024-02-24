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
    static constexpr priority level() noexcept { return priority::informational; }
    static constexpr auto writer() noexcept { return sink::fd<2>; }
};

// Log category with run-time configurable sink and level
struct MyRuntimeCategory : runtime_level<MyRuntimeCategory, runtime_sink<MyRuntimeCategory, MyFixedCategory>> {
};

using Log = logger<MyRuntimeCategory>;

int main() {
    Log::d("Not printed");
    MyRuntimeCategory::level(priority::debug);
    Log::d("Printed to stderr");
    MyRuntimeCategory::writer(sink::fd<1>);
    Log::d("Printed to stdout");
    return 0;
}
