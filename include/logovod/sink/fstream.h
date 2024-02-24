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
#include <fstream>

namespace logovod::sink {
// sink to a statically allocated fstream. ID designates an identity
template<unsigned ID>
class fstream {
public:
    void operator()(std::string_view m, std::string_view pl, attributes a) const noexcept {
        writer(m, pl, a);
    }
    static void writer(std::string_view message, std::string_view, attributes) noexcept {
        out.write(message.data(), static_cast<std::streamsize>(message.size()));
    }
    template<typename Path>
    static void open(const Path& path, std::ios_base::openmode mode = std::ios_base::app) {
        return out.open(path, mode);
    }
private:
    static inline std::ofstream out {};
};

} // namespace
