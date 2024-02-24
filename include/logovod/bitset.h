/*
 * Copyright (C) 2024 Eugene Hutorny <eugene@hutorny.in.ua>
 *
 * This file is a part of logovod library
 *
 * Licensed under MIT License, see full text in LICENSE
 * or visit page https://opensource.org/license/mit/
 */

#pragma once
#include <logovod/core.h>
#include <bitset>

namespace logovod {
template<typename ... T>
struct formatter<representation<radix::bin, T...>> {
    template<typename Printer, typename Stream>
    void operator()(Printer& printer, Stream& stream, const representation<radix::bin, T...>& value) {
        static_assert((std::is_integral_v<T> && ...), "Only integral types are supported");
        std::apply([&printer,&stream](auto&&... args) {printer(stream, std::bitset<sizeof(T)*8>(args) ...);}, value.values);
    }
};

} // namespace logovod
