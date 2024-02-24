/*
 * This file is on public domain. Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this file, either in source code form or as a compiled binary, for any purpose, commercial
 * or non-commercial, and by any means.
 *
 * Visit https://unlicense.org/ for details
 */

#include <logovod/logovod.h>

int main() {
    // Instantiate a logger type
    using Log = logovod::logger<logovod::category>;
    // Emit log messages with a function-call-style
    Log::debug("Hello Logovod");
    // or with I/O stream shifts
    Log::info{} << "Hello Logovod I/O stream style";
#if defined(__cpp_lib_format)
    // or with std::format
    Log::notice{}.format("Hello {} format style", "Logovod");
#endif
    return 0;
}
