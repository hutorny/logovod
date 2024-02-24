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
#include <unistd.h>

namespace logovod::sink {
template<int FD>
void fd(std::string_view message, std::string_view, attributes) noexcept {
    auto written = ::write(FD, message.data(), message.size());
    while (written > 0 && static_cast<std::size_t>(written) < message.size() && message.size() != 0) {
        message.remove_prefix(written);
        written = ::write(FD, message.data(), message.size());
    }
}
} // namespace logovod::sink

