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
#include <variant>

namespace logovod {
template<>
struct formatter<std::monostate> {
    template<typename Printer, typename Stream>
    void operator()(Printer& printer, Stream& stream, const std::monostate&) {
        printer.start(stream);
        printer.finish(stream);
    }
};

template<typename ... T>
struct formatter<std::variant<T...>> {
    template<typename Printer, typename Stream>
    void operator()(Printer& printer, Stream& stream, const std::variant<T...>& value) {
        std::visit([&printer, &stream](const auto& val){printer(stream, val);}, value);
    }
};

} // namespace logovod
