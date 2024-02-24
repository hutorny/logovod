/*
 * This file is on public domain. Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this file, either in source code form or as a compiled binary, for any purpose, commercial
 * or non-commercial, and by any means.
 *
 * Visit https://unlicense.org/ for details
 */

#include <logovod/logovod.h>

using namespace logovod;

struct MyCategory : category {
    static constexpr sink_types::writer_type writer() noexcept { return sink::cout; }
    static constexpr priority level() noexcept { return priority::informational; }
};

using Log = logger<MyCategory>;

int main() {
    Log::d("Not printed");
    Log::i("Printed");
    return 0;
}
