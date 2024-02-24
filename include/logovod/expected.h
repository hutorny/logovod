/*
 * Copyright (C) 2024 Eugene Hutorny <eugene@hutorny.in.ua>
 *
 * This file is a part of logovod library
 *
 * Licensed under MIT License, see full text in LICENSE
 * or visit page https://opensource.org/license/mit/
 */

#pragma once

#pragma once
#include <logovod/core.h>
#include <expected>

namespace logovod {
template<typename T, typename E>
struct formatter<std::expected<T,E>> {
    template<typename Printer, typename Stream>
    void operator()(Printer& printer, Stream& stream, const std::expected<T,E>& value) {
        if (value.has_value()) printer(stream, value.value());
        else {
            printer.start(stream);
            printer(stream, value.error());
            printer.finish(stream);
        }
    }
};

} // namespace
