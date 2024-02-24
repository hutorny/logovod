/*
 * This file is on public domain. Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this file, either in source code form or as a compiled binary, for any purpose, commercial
 * or non-commercial, and by any means.
 *
 * Visit https://unlicense.org/ for details
 */

#include <logovod/logovod.h>
#include <vector>
#include <deque>

using namespace logovod;

using Log = logger<category>;

int main() {
    std::vector<int> v { 1, 2, 3, 4 };
    std::deque<int> q { 7, 8, 9, 10, 11 };
    Log::i{}("vector", v);
    Log::d{} << "deque " << q;
    return 0;
}
