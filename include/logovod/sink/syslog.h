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
#include <syslog.h>

namespace logovod::sink {
class syslog {
public:
    static void writer(std::string_view, std::string_view payload, attributes attrs) noexcept {
        if (attrs.location.line == 0)
            attrs.location.file_name = "-";
        ::syslog(static_cast<int>(attrs.level), format_string,
                static_cast<int>(attrs.tag.size()), attrs.tag.data(),
                static_cast<int>(attrs.location.file_name.size()), attrs.location.file_name.data(),
                attrs.location.line,
                static_cast<int>(payload.size()), payload.data());
    }
    static void prolog(std::ostream&, attributes) noexcept {
        // attributes are written by writer
    }
    static std::size_t epilog(attributes, std::ostream& out) noexcept {
        if(out.fail()) { // write trimmed indication
            out.clear();
            out.seekp(3, std::ios_base::end);
            out << "..."; // "..." accounted as payload
        }
        return 0;
    }
    static void  open(const char *ident, int option=LOG_PID, int facility=LOG_USER) {
        ::openlog(ident, option, facility);
    }
    void operator()(std::string_view message, std::string_view payload, attributes attrs) noexcept {
        writer(message, payload, attrs);
    }
    void operator()(std::ostream& out, attributes attrs) noexcept { prolog(out, attrs); }
    void operator()(attributes attrs, std::ostream& out) noexcept { epilog(attrs, out); }
    static inline const char* format_string = "%.*s:%.*s:%d:%.*s";
    constexpr operator bundle() noexcept { return { prolog, writer, epilog }; }
};
} // namespace logovod::sink {

