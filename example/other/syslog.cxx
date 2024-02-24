/*
 * This file is on public domain. Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this file, either in source code form or as a compiled binary, for any purpose, commercial
 * or non-commercial, and by any means.
 *
 * Visit https://unlicense.org/ for details
 */
#include <logovod/logovod.h>
#include <logovod/sink/syslog.h>
#include <filesystem>

using namespace logovod;

// Build-time fixed log category with syslog as a functor
struct MyCategory : category {
    static constexpr priority level() noexcept { return priority::informational; }
    static constexpr auto writer() noexcept { return syslog_; }
    static constexpr auto prolog() noexcept { return syslog_; }
    static constexpr auto epilog() noexcept { return syslog_; }
    static constexpr std::string_view tag = "FIXED";
private:
    static inline sink::syslog syslog_{};
};

// Category with syslog and run-time level
struct RuntimeCategory : runtime_level<RuntimeCategory, MyCategory> {
    static constexpr std::string_view tag = "RUNTIME";
};

using Log = logger<MyCategory>;
using LogR = logger<RuntimeCategory>;

int main(int argc, char* argv[]) {
    sink::syslog::open(nullptr);
    Log::i("Emitting fixed logger info");
    Log::i{}("syslog test fixed logger info, argc", argc, "argv[0] ", '\'', argv[0], '\'');
    Log::i("Emitting run-time logger warning");
    LogR::w{}("syslog test run-time logger warning");
    return 0;
}


