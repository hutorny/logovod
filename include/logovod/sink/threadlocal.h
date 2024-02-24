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
#include <fcntl.h>
#include <filesystem>
#include <thread>

namespace logovod::sink {

class threadlocal {
public:
    static void writer(std::string_view message, std::string_view, attributes) noexcept {
        if(file_ != -1) {
            auto written = ::write(file_, message.data(), message.size());
            while (written > 0 && static_cast<std::size_t>(written) < message.size() && message.size() != 0) {
                message.remove_prefix(written);
                written = ::write(file_, message.data(), message.size());
            }
        }
    }
    static void prolog(std::ostream& out, attributes attrs) noexcept {
        out << attrs.tag << ':' << static_cast<int>(attrs.level) << ':' << std::this_thread::get_id() << ':';
    }
    void operator()(std::string_view m, std::string_view pl, attributes a) const noexcept {
        writer(m, pl, a);
    }
    void operator()(std::ostream& out, attributes attrs) const noexcept {
        prolog(out, attrs);
    }
    static void setpath(std::filesystem::path&& p) { path = std::move(p); }
    static void setpath(const std::filesystem::path& p) { path = p; }
    static auto hashid(std::thread::id id) {
        std::hash<std::thread::id> idhash{};
        return idhash(id);
    }
private:
    static inline std::filesystem::path path{};
    static auto makefn() {
        auto filename { path };
        // thread::id not convertible to string, hash(thread::id) is
        filename += std::to_string(hashid(std::this_thread::get_id()));
        return filename;
    }
    struct filedescriptor {
        static inline constexpr int oflags = O_WRONLY | O_CREAT | O_APPEND;
        static inline constexpr int omode = S_IWUSR | S_IRUSR | S_IRGRP;
        filedescriptor() : fd_{ ::open(makefn().c_str(), oflags, omode) } {}
        ~filedescriptor() { ::close(fd_); }
        operator int() const noexcept { return fd_; }
    private:
        int fd_;
    };
    static inline thread_local filedescriptor file_{};
};
} // namespace logovod::sink

