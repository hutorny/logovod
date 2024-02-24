/*
 * Copyright (C) 2024 Eugene Hutorny <eugene@hutorny.in.ua>
 *
 * This file is a part of logovod library
 *
 * Licensed under MIT License, see full text in LICENSE
 * or visit page https://opensource.org/license/mit/
 */

#include "tests.h"
#include <logovod/sink/syslog.h>

struct SyslogCategory : logovod::category {
    static constexpr std::string_view tag = "SyslogTest";
    static constexpr auto writer() noexcept { return logovod::sink::syslog::writer; }
    static constexpr auto prolog() noexcept { return logovod::sink::syslog::prolog; }
};

using Log = logovod::logger<SyslogCategory>;


int main(int, char *argv[]) {
    using namespace logovod::tests;
    using namespace logovod;
    logovod::sink::syslog::open(nullptr);
    uint64_t num = rng();
    Log::i{}("vodolog syslog test", num);
    Eval::i("journalctl -S today --user -g", SyslogCategory::tag, '.','*', num);
    return 0;
}
