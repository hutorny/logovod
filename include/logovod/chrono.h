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
#include <iomanip>

namespace logovod {
namespace detail {

template<typename Clock>
struct clock_printer {
    template<typename Stream, typename Duration>
    static Stream& print(Stream& out, std::chrono::time_point<Clock, Duration> tp);
};

template<>
struct clock_printer<std::chrono::system_clock> {
    template<typename Stream, typename Duration>
    static Stream& print(Stream& out, std::chrono::time_point<std::chrono::system_clock, Duration> tp) {
        std::time_t t = std::chrono::system_clock::to_time_t(tp);
        out << std::put_time(std::localtime(&t), "%FT%T.");
        if constexpr(std::is_floating_point_v<typename Duration::rep> || Duration::period::den >= 1000) {
            auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count() % 1000;
            auto flags { out.flags() };
            auto width { out.width() };
            auto fill { out.fill() };
            out << std::dec << std::setw(3) << std::setfill('0') << millis;
            out.fill(fill);
            out.width(width);
            out.flags(flags);
        }
        return out;
    }
};

template<>
struct clock_printer<std::chrono::steady_clock> {
    template<typename Stream, typename Duration>
    static Stream& print(Stream& out, std::chrono::time_point<std::chrono::steady_clock, Duration> tp) {
        return out << tp.time_since_epoch().count();
    }
};

template<typename Clock>
template<typename Stream, typename Duration>
Stream& clock_printer<Clock>::print(Stream& out, std::chrono::time_point<Clock, Duration> tp) {
    auto sys_tp = Clock::to_sys(tp);
    return clock_printer<std::chrono::system_clock>::print(out, sys_tp);
}

template<typename Stream, typename Clock, typename Duration>
Stream& put_time_point(Stream& out, std::chrono::time_point<Clock, Duration> tp) {
    return clock_printer<Clock>::print(out, tp);
}

template<typename Stream, typename Rep, typename Period>
Stream& put_duration(Stream& out, std::chrono::duration<Rep, Period> value) {
    using namespace std::chrono;
    out << value.count();
    switch(Period::den) {
    case nanoseconds::period::den: return out << "ns";
    case microseconds::period::den: return out << "us";
    case milliseconds::period::den: return out << "ms";
    case seconds::period::den:
        switch(Period::num) {
        case seconds::period::num: return out << "s";
        case minutes::period::num: return out << "min";
        case hours::period::num: return out << "h";
        default:;
        }
    default:;
    }
    return out;
}
} // namespace detail

template<typename Clock, typename Duration>
struct formatter<std::chrono::time_point<Clock, Duration>> {
    template<typename Printer, typename Stream>
    void operator()(Printer&, Stream& stream, std::chrono::time_point<Clock, Duration> value) {
        detail::clock_printer<Clock>::print(stream, value);
    }
};

template<typename Rep, typename Period>
struct formatter<std::chrono::duration<Rep, Period>> {
    template<typename Printer, typename Stream>
    void operator()(Printer&, Stream& stream, std::chrono::duration<Rep, Period> value) {
        detail::put_duration(stream, value);
    }
};


} // namespace logovod
