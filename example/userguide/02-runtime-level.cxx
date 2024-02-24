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
    static sink_types::writer_type writer() noexcept { return sink_writer; }
    static priority level() noexcept { return priority_level; }
    static inline priority priority_level { priority::informational };
    static inline sink_types::writer_type sink_writer { sink::cerr };
};

using Log = logger<MyCategory>;

int main() {
    Log::d("Not printed");
    MyCategory::priority_level = priority::debug;
    Log::d("Printed to cerr");
    MyCategory::sink_writer = sink::cout;
    Log::d("Printed to cout");
    return 0;
}


