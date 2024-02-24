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
    static constexpr auto prolog() noexcept {
        return [](std::ostream& out, attributes attrs) noexcept {
            out << sink::prolog::prolog_traits::basename(attrs.location.file_name) << ':' << attrs.location.line << ':';
        };
    }
};

using Log = logger<MyCategory>;

int main() {
    Log::i{}("Default location"); // 11-source-location.cxx:27:Default location
    Log::i("No location");        // :0:No location
    Log::i{{}}("Empty location"); // :0:No location
    Log::i i{};
    i << "Location from construction" << std::flush; // 11-source-location.cxx:30:Location from construction
    i(source_location::current(), "Overridden");     // 11-source-location.cxx:32:Overridden
    return 0;
}
