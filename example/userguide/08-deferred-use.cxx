/*
 * This file is on public domain. Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this file, either in source code form or as a compiled binary, for any purpose, commercial
 * or non-commercial, and by any means.
 *
 * Visit https://unlicense.org/ for details
 */

#include <logovod/logovod.h>

using namespace logovod;

using Log = logger<category>;

int main() {
    Log::d d{}; // emitter is constructed
    d << "One";
    for (auto& i : {"two", "three"}) {
        d << ' ' << i;
    }
    d << " done" << std::flush;
    d << "Reused and flushed on destruction";
    return 0;
}
