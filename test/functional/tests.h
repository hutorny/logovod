/*
 * Copyright (C) 2024 Eugene Hutorny <eugene@hutorny.in.ua>
 *
 * This file is a part of logovod library
 *
 * Licensed under MIT License, see full text in LICENSE
 * or visit page https://opensource.org/license/mit/
 */

#include <logovod/logovod.h>
#include <random>

#pragma once

namespace logovod::tests {

struct Evaluate : logovod::category {
    static constexpr auto writer() noexcept { return logovod::sink::fd<1>; }
};
struct Report : logovod::category {
    static constexpr auto writer() noexcept { return logovod::sink::fd<2>; }
};
using Eval = logovod::logger<Evaluate>;
using Rep = logovod::logger<Report>;

inline int pass(std::string_view name) {
    Rep::i(name, "PASSED");
    Eval::i("true");
    return 0;
}

inline int fail(std::string_view name) {
    Rep::e(name, "FAILED");
    Eval::i("false");
    return 1;
}

inline int result(bool success, std::string_view name) {
    name.remove_prefix(name.rfind('/') + 1);
    return success ? pass(name) : fail(name);
}

inline uint64_t rng() {
    static std::random_device rnd{};
    static std::mt19937 gen(rnd());
    static std::uniform_int_distribution<uint64_t> dev{};
    return dev(gen);
}

} // namespace
