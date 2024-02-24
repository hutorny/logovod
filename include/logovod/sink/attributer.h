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
#include <chrono>
#include <filesystem>

namespace logovod::sink {
namespace prolog {

struct prolog_traits {
    static constexpr char field_delimiter = '|';
    static constexpr unsigned tag_field_width = 8;
    static constexpr unsigned filename_field_width = 16;
    static constexpr unsigned linenum_field_width = 4;
    static constexpr bool print_location = true;
    static constexpr auto level_name(priority level) noexcept {
        constexpr const char* names[] = { "EMERGENCY", "ALERT    ", "CRITICAL ", "ERROR    ",
                                          "WARNING  ", "NOTICE   ", "INFO     ", "DEBUG    "};
        return static_cast<std::size_t>(level) <= static_cast<std::size_t>(priority::debug) ?
                names[static_cast<std::size_t>(level)] : "INVALID  ";
    }
    static constexpr auto basename(std::string_view fn) {
        constexpr auto ps = std::filesystem::path::preferred_separator;
        auto pos = fn.rfind(ps);
        return fn.substr(pos == fn.npos ? 0 : pos+1);
    }
};

template<typename Traits = prolog_traits>
void common(std::ostream& out, attributes attrs) noexcept {
    logovod::detail::put_time_point(out, std::chrono::system_clock::now())
        << Traits::field_delimiter
        << std::setw(Traits::tag_field_width) << std::left
        << attrs.tag
        << Traits::field_delimiter
        << Traits::level_name(attrs.level)
        << Traits::field_delimiter;
    if (Traits::print_location) {
        if (attrs.location.line != 0)
            out << std::setw(Traits::filename_field_width)
                << Traits::basename(attrs.location.file_name) << ':'
                << std::setw(Traits::linenum_field_width)
                << std::left
                << attrs.location.line
                << Traits::field_delimiter;
        else
            out << std::setw(Traits::filename_field_width)
                << std::setfill('-') << '-' << ':'
                << std::setw(Traits::linenum_field_width)
                << std::left
                << std::setfill('-') << '-'
                << Traits::field_delimiter;
    }
}

inline void taglvl(std::ostream& out, attributes attrs) noexcept {
    out << attrs.tag << ':' << static_cast<int>(attrs.level) << ':';
}
} // namespace prolog

namespace epilog {
// epilog with trimmed data indication, specified in the category
template<class Category>
std::size_t trimeol(attributes, std::ostream &out) noexcept {
    if(out.put('\n').fail()) { // ensure epilog is always written
        out.clear();
        const auto pos = 1 + Category::trimmed().size();
        out.seekp(static_cast<std::ostream::off_type>(pos), std::ios_base::end);
        out << Category::trimmed() << '\n'; // trimmed indication accounted as payload
        return 1;
    }
    return 0;
}

}
} // namespace logovod::sink
