/*
 * Copyright (C) 2024 Eugene Hutorny <eugene@hutorny.in.ua>
 *
 * This file is a part of logovod library
 *
 * Licensed under MIT License, see full text in LICENSE
 * or visit page https://opensource.org/license/mit/
 */

#include "tests.h"
#include <logovod/sink/unistd.h>
#include <malloc.h>

struct Category : logovod::category {
    static constexpr auto writer() noexcept { return logovod::sink::fd<2>; }
};
using Log = logovod::logger<Category>;

static bool test(size_t uordblks, const char* progname) {
    Log::i("This is a simple mallinfo2 test. Use valgrind for better precision:");
    Log::i("valgrind --tool=massif --threshold=0.0", progname);
    Log::i("ms_print --threshold=0.0 massif.out", '.', getpid());
    Log::i("Entered malloc test, uordblks :", uordblks);
    auto inflight = mallinfo2();
    Log::i("Delta uordblks in flight      :", inflight.uordblks - uordblks);
    inflight = mallinfo2();
    Log::i("Delta uordblks in flight      :", inflight.uordblks - uordblks);
    return inflight.uordblks == uordblks;
}

int main(int, char *argv[]) {
    const auto onstart = mallinfo2();
    const bool success = test(onstart.uordblks, argv[0]);
    const auto onfinish = mallinfo2();
    {
        Log::i("Delta uordblks on finish      :", onfinish.uordblks - onstart.uordblks);
    }
    return logovod::tests::result(success && (onfinish.uordblks == onstart.uordblks), argv[0]);
}
