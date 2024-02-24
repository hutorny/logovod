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
#if defined(__cpp_lib_format)
    Log::i{}.format("std::{} {}", "format", "style");
#else
    Log::e{}("std::format is not available");
#endif

    return 0;
}
