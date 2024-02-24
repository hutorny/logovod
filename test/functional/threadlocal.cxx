/*
 * Copyright (C) 2024 Eugene Hutorny <eugene@hutorny.in.ua>
 *
 * This file is a part of logovod library
 *
 * Licensed under MIT License, see full text in LICENSE
 * or visit page https://opensource.org/license/mit/
 */

#include "tests.h"

#include <logovod/logovod.h>
#include <logovod/sink/attributer.h>
#include <logovod/sink/threadlocal.h>
#include <chrono>
#include <filesystem>
#include <thread>

using namespace logovod;
using namespace logovod::tests;
using namespace std::literals;

struct Category : category {
    static constexpr std::string_view tag = "TESTTAG";
    static constexpr auto writer() noexcept { return sink::threadlocal::writer; }
    static constexpr auto prolog() noexcept { return sink::threadlocal::prolog; }
};
using Log = logger<Category>;
std::thread::id ids[2]{};
static void run(int num) {
    ids[num] = std::this_thread::get_id();
    Log::i("Info");
}

int main() {
    std::filesystem::remove_all("/tmp/loggertest");
    std::filesystem::create_directories("/tmp/loggertest");
    sink::threadlocal::setpath("/tmp/loggertest/log.");
    std::thread t1{run, 0};
    std::thread t2{run, 1};
    t2.join();
    t1.join();
    Eval::i("grep TESTTAG /tmp/loggertest/log", '.', sink::threadlocal::hashid(ids[0]));
    return 0;
}




