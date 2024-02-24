/*
 * 12-prolog.cxx
 *
 *  Created on: 24 бер. 2024 р.
 *      Author: eugene
 */


/*
 * This file is on public domain. Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this file, either in source code form or as a compiled binary, for any purpose, commercial
 * or non-commercial, and by any means.
 *
 * Visit https://unlicense.org/ for details
 */

#include <logovod/logovod.h>
#include <logovod/sink/attributer.h>

using namespace logovod;

struct MyCategory : category {
    static constexpr std::string_view tag = "MYTAG";
    static constexpr auto prolog() noexcept {
        return sink::prolog::common;
    }
};

using Log = logger<MyCategory>;

int main() {
    Log::i{}("With prolog");      // <curren date/time>|MYTAG   |INFO     |12-prolog.cxx   :32  |With prolog
    Log::w("No source location"); // <curren date/time>|MYTAG   |WARNING  |----------------:----|No source location
    return 0;
}

