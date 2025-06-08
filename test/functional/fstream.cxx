/*
 * Copyright (C) 2024 Eugene Hutorny <eugene@hutorny.in.ua>
 *
 * This file is a part of logovod library
 *
 * Licensed under MIT License, see full text in LICENSE
 * or visit page https://opensource.org/license/mit/
 */

#include "tests.h"
#include <logovod/sink/fstream.h>
#include <filesystem>
#include <random>


struct FileCategory : logovod::category {
    static constexpr std::string_view tag = "FstreamTest";
    static constexpr auto writer() noexcept { return logovod::sink::fstream<0>::writer; }
};

using Log = logovod::logger<FileCategory>;

int main() {
    using namespace logovod::tests;
    using namespace logovod;
    std::filesystem::remove_all("/tmp/loggertest");
    std::filesystem::create_directories("/tmp/loggertest");
    sink::fstream<0>::open("/tmp/loggertest/fstream.log");
    auto num = rng();
    Log::i("Info", num);
    Eval::i("grep",  num, "/tmp/loggertest/fstream.log");
    return 0;
}
