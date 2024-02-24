/*
 * This file is on public domain. Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this file, either in source code form or as a compiled binary, for any purpose, commercial
 * or non-commercial, and by any means.
 *
 * Visit https://unlicense.org/ for details
 */

#include <logovod/logovod.h>
#include <logovod/sink/attributer.h>
#include <logovod/sink/threadlocal.h>
#include <chrono>
#include <filesystem>
#include <thread>

using namespace logovod;
using namespace std::literals;

struct Category : runtime_level<Category, runtime_sink<Category, category>> {
    static constexpr std::string_view tag = "TESTTAG";
};
using Log = logger<Category>;

static void run() {
    Log::i("Info");
    std::this_thread::sleep_for(10ms);
    Log::w("Warning");
    std::this_thread::sleep_for(10ms);
    Log::e("Error");
}

int main() {
    std::cout << "Thread local test\n";
    std::filesystem::create_directories("/tmp/logger");
    sink::threadlocal::setpath("/tmp/logger/log.");
    Log::category_type::level(priority::informational);
    Log::category_type::writer(sink::threadlocal::writer);
    Log::category_type::prolog(sink::threadlocal::prolog);
    Log::category_type::epilog(sink::epilog::trimeol<Log::category_type>);

    std::thread t1{run};
    std::thread t2{run};
    t2.join();
    t1.join();
    return 0;
}




