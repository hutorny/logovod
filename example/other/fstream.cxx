/*
 * This file is on public domain. Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this file, either in source code form or as a compiled binary, for any purpose, commercial
 * or non-commercial, and by any means.
 *
 * Visit https://unlicense.org/ for details
 */

#include <logovod/logovod.h>
#include <logovod/sink/attributer.h>
#include <logovod/sink/fstream.h>
#include <filesystem>

using namespace logovod;

struct Category : runtime_level<Category, runtime_sink<Category, category>> {
    static constexpr std::string_view tag = "TESTTAG";
};
using Log = logger<Category>;

int main() {
    std::filesystem::create_directories("/tmp/logger");
    sink::fstream<0>::open("/tmp/logger/test.log");
    Log::category_type::prolog(sink::prolog::taglvl);
    Log::e{}("Test Error");
    Log::category_type::prolog(sink::prolog::common<>);
    Log::w{}("Test Warning");
    Log::category_type::writer(sink::fstream<0>::writer);
    Log::e{}("Test Error");
    return 0;
}


