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
    Log::d("Construct", "and", "log");
    Log::i{}("Construct", "then", "log");
    int i = 10, j = 11;
    //Log::d(i); // unnecessary parentheses in declaration of ‘i’
    //In the line above ambiguous syntax is treated as declaration Log::d i;
    Log::d(i, "variable"); // This works
    Log::d{}(i); // This works too
    Log::d(i, j);
    return 0;
}
