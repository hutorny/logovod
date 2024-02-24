/*
 * Copyright (C) 2024 Eugene Hutorny <eugene@hutorny.in.ua>
 *
 * This file is a part of logovod library
 *
 * Licensed under MIT License, see full text in LICENSE
 * or visit page https://opensource.org/license/mit/
 */

#pragma once 
#include <logovod/detail.h>
#include <logovod/fixedbuf.h>
#include <cstdint>
#include <string_view>
#include <iostream>
#include <limits>

namespace logovod {

// Priorities (log levels) are defined syslog severities https://datatracker.ietf.org/doc/html/rfc5424#section-6.2.1
enum class priority {
    emergency,
    alert,
    critical,
    error,
    warning,
    notice,
    informational,
    debug
};

struct attributes {
    struct source_location {
        std::string_view file_name;
        std::uint_least32_t line;
    };
    priority level;
    std::string_view tag;
    source_location location;
};

template<typename CharT, class Traits = std::char_traits<CharT>>
struct basic_sink_types {
    // writer - writes message or its payload part to one or more sinks
    using writer_type = void (*)(std::basic_string_view<CharT, Traits> message, std::basic_string_view<CharT, Traits> payload, attributes) noexcept;
    // prologer - writes prolog to the output buffer
    using prologer = void(*)(std::basic_ostream<CharT, Traits>&, attributes) noexcept;
    // epiloger - writes epilog to the output buffer, normally returns 0. Non-zero result indicates number of characters
    // to be deducted from the payload due to buffer overflow
    using epiloger = std::size_t(*)(attributes, std::basic_ostream<CharT, Traits>&) noexcept;
    prologer prolog;
    writer_type writer;
    epiloger epliog;
};

namespace sink {
using bundle = basic_sink_types<char>;

// write to a predefined file descriptor, such as 1 or 2
template<int FD>
void fd(std::string_view message, std::string_view payload, attributes) noexcept;

// write to std::clog
inline void clog(std::string_view message, std::string_view, attributes) noexcept {
    std::clog.write(message.data(), static_cast<std::streamsize>(message.size()));
}
// write to std::cout
inline void cout(std::string_view message, std::string_view, attributes) noexcept {
    std::cout.write(message.data(), static_cast<std::streamsize>(message.size()));
}
// write to std::cerr
inline void cerr(std::string_view message, std::string_view, attributes) noexcept {
    std::cerr.write(message.data(), static_cast<std::streamsize>(message.size()));
}

// write to std::clog
inline void wclog(std::wstring_view message, std::wstring_view, attributes) noexcept {
    std::wclog.write(message.data(), static_cast<std::streamsize>(message.size()));
}
// write to std::cout
inline void wcout(std::wstring_view message, std::wstring_view, attributes) noexcept {
    std::wcout.write(message.data(), static_cast<std::streamsize>(message.size()));
}
// write to std::cerr
inline void wcerr(std::wstring_view message, std::wstring_view, attributes) noexcept {
    std::wcerr.write(message.data(), static_cast<std::streamsize>(message.size()));
}

namespace prolog {
// Null prolog
inline void null(std::ostream&, attributes) noexcept {}
inline void wnull(std::wostream&, attributes) noexcept {}
} // namespace prolog

namespace epilog {
// Null epilog
inline std::size_t null(attributes, std::ostream&) noexcept { return 0; }
inline std::size_t wnull(attributes, std::wostream&) noexcept { return 0; }
// End-of-line epilog
inline std::size_t eol(attributes, std::ostream &out) noexcept {
    if(out.put('\n').fail()) { // ensure epilog is always written
        out.clear();
        out.seekp(1, std::ios_base::end);
        out.put('\n');
        return 1;
    }
    return 0;
}
inline std::size_t weol(attributes, std::wostream &out) noexcept {
    if(out.put(L'\n').fail()) { // ensure epilog is always written
        out.clear();
        out.seekp(1, std::ios_base::end);
        out.put(L'\n');
        return 1;
    }
    return 0;
}
} // namespace epilog
} // namespace sink

inline constexpr auto unlimited = std::numeric_limits<std::size_t>::max();

struct category {
    using char_type = char;
    using char_traits = std::char_traits<char>;
    using sink_types = basic_sink_types<char_type, char_traits>;
    // Always constexpr properties
    // Category tag, passed to prolog and epilog via attributes
    static constexpr std::string_view tag = "";
    // Message length limit (buffer size)
    static constexpr std::size_t length_limit = 1024; // set to unlimited for no limit
    // Properties that can be changed from constexpr or dynamic/run-time
    static constexpr priority level() noexcept { return priority::debug; }
    static constexpr sink_types::writer_type writer() noexcept { return sink::clog; }
    static constexpr sink_types::prologer prolog() noexcept { return sink::prolog::null; }
    static constexpr sink_types::epiloger epilog() noexcept { return sink::epilog::eol; }
    // Delimiter to be inserted between values, strings in logger::operator(...)
    static constexpr delimiter dlm() noexcept { return consts::default_delimiter; }
    // Element separators for printing containter's content in logger::operator(...)
    static constexpr separators sep() noexcept { return consts::default_separators; }
    // Limits number of printed container elements
    static constexpr std::size_t range_limit() noexcept { return 16; }
    // Indication of trimmed container printing imposed by range_limit()
    static constexpr std::string_view trimmed() noexcept { return "..."; }
};

struct wcategory {
    using char_type = wchar_t;
    using char_traits = std::char_traits<wchar_t>;
    using sink_types = basic_sink_types<char_type, char_traits>;
    // Always constexpr properties
    // Category tag, passed to prolog and epilog via attributes. Not wstring_view by intent
    static constexpr std::string_view tag = "";
    // Message length limit (buffer size)
    static constexpr std::size_t length_limit = 1024; // set to unlimited for no limit
    // Properties that can be changed from constexpr or dynamic/run-time
    static constexpr priority level() noexcept { return priority::debug; }
    static constexpr sink_types::writer_type writer() noexcept { return sink::wclog; }
    static constexpr sink_types::prologer prolog() noexcept { return sink::prolog::wnull; }
    static constexpr sink_types::epiloger epilog() noexcept { return sink::epilog::weol; }
    // Delimiter to be inserted between values, strings in logger::operator(...)
    static constexpr wdelimiter dlm() noexcept { return wdelimiter{L' '}; }
    // Element separators for printing containter's content in logger::operator(...)
    static constexpr wseparators sep() noexcept { return wseparators {L'{', L',', L'}'}; }
    // Limits number of printed container elements
    static constexpr std::size_t range_limit() noexcept { return 16; }
    // Indication of trimmed container printing imposed by range_limit()
    static constexpr std::wstring_view trimmed() noexcept { return L"..."; }
};


template<class Category>
class logger {
public:
    using char_type = typename Category::char_type;
    using char_traits = typename Category::char_traits;
    using printer = detail::basic_printer<char_type, char_traits>;
    using category_type = Category;
    using ostream = std::basic_ostream<char_type, char_traits>;

    class basic_emitter {
    protected:
        constexpr basic_emitter(source_location&& sl) noexcept : location_ { std::move(sl) } {}
        constexpr basic_emitter(const source_location& sl) noexcept : location_ { sl } {}
        basic_emitter(const basic_emitter&) = delete;
        basic_emitter(basic_emitter&&) = delete;
        basic_emitter& operator=(const basic_emitter&) = delete;
        basic_emitter& operator=(basic_emitter&&) = delete;
        ~basic_emitter() = default;
        template<typename ... T>
        void operator()(const T &... val) {
            printer_(stream_, val ...);
        }
        template<typename T>
        basic_emitter& operator<<(const T &value) {
            if constexpr (detail::use_default_lshift<ostream, T>) {
              if constexpr(detail::is_intchar_v<std::remove_reference_t<T>>) {
                stream_ << +value;
              } else {
                stream_ << value;
              }
            } else {
                printer_(stream_, value);
            }
            return *this;
        }
        auto tellp() noexcept {
            return buffer_.pubseekoff(0, std::ios_base::cur, std::ios_base::out);
        }
#if defined(__cpp_lib_format)
        template<typename ... T>
        void format(std::basic_format_string<char_type, std::type_identity_t<T>...> fmt, T&&... args) {
            if constexpr(category_type::length_limit == unlimited) {
                std::format_to(std::ostreambuf_iterator<char_type, char_traits>{&buffer_}, fmt, args...);
            } else {
                std::format_to_n(std::ostreambuf_iterator<char_type, char_traits>{&buffer_}, buffer_.available(), fmt, std::forward<T>(args)...);
            }
        }
#endif
        void prolog(attributes attrs) {
            if (!prolog_done_) {
                if (epilog_done_) reset();
                category_type::prolog()(stream_, attrs);
                payload_begin_ = tellp();
                prolog_done_ = true;
            }
        }
        void epilog(attributes attrs) {
            if (!epilog_done_) {
                payload_end_ = tellp();
                // functors are allowed to use epilog that returns void
                if constexpr(std::is_void_v<decltype(category_type::epilog()(attrs, stream_))>) {
                    category_type::epilog()(attrs, stream_);
                } else {
                    payload_end_ -= static_cast<std::streamoff>(category_type::epilog()(attrs, stream_));
                }
                epilog_done_ = true;
            }
        }
        attributes make_attrs(priority p) const noexcept {
            return {p, category_type::tag, {location_.file_name(), location_.line()}};
        }
        void flush(attributes attrs) {
            epilog(attrs);
            if constexpr (detail::has_view_v<buffer_type>) {
                const auto message = buffer_.view();
                const auto payload = message.substr(static_cast<std::size_t>(payload_begin_), static_cast<std::size_t>(payload_end_ - payload_begin_));
                category_type::writer()(message, payload, attrs);
            } else {
                const auto str = buffer_.str();
                const auto message { str };
                const auto payload = message.substr(static_cast<std::size_t>(payload_begin_), static_cast<std::size_t>(payload_end_ - payload_begin_));
                category_type::writer()(message, payload, attrs);
            }
            prolog_done_ = false;
        }
        void reset() {
            buffer_.pubseekpos(0);
            prolog_done_ = false;
            epilog_done_ = false;
            location_ = source_location { };
            printer_.reset(category_type::dlm(), category_type::sep());
            stream_.clear();
            stream_.flags(fmtstate_.flags_);
            stream_.width(fmtstate_.width_); // TODO consider removing
            stream_.precision(fmtstate_.precision_); // TODO consider removing
        }
        using buffer_type = typename std::conditional<category_type::length_limit == unlimited,
        std::basic_stringbuf<char_type, char_traits>, detail::basic_fixedbuf<category_type::length_limit, char_type, char_traits>>::type;
        using pos_type = typename buffer_type::pos_type;
        buffer_type buffer_ { };
        std::basic_ostream<char_type, char_traits> stream_ { &buffer_ };
        struct fmtstate {
            std::streamsize width_;
            std::streamsize precision_;
            std::ios_base::fmtflags flags_;
        } fmtstate_ { stream_.width(), stream_.precision(), stream_.flags() };
        printer printer_ { category_type::range_limit(), category_type::trimmed(), category_type::dlm(), category_type::sep() };
        source_location location_ { };
        pos_type payload_begin_ { };
        pos_type payload_end_ { };
        bool prolog_done_ { };
        bool epilog_done_ { };
    };

    template<priority Priority>
    class emitter : basic_emitter {
    public:
        using basic_emitter::basic_emitter;
        constexpr emitter(source_location&& sl = source_location::current()) noexcept : basic_emitter { std::move(sl) } {}
        constexpr emitter(const source_location& sl) noexcept : basic_emitter { sl } {}
        template<typename T, typename ... Args>
        constexpr emitter(const T &val, const Args &... vals) noexcept : basic_emitter {{}} {
            operator()(val, vals...);
        }
        emitter(const emitter&) = delete;
        emitter(emitter&&) = delete;
        emitter& operator=(const emitter&) = delete;
        emitter& operator=(emitter&&) = delete;
        ~emitter() { if (basic_emitter::prolog_done_) flush(); }
        constexpr operator bool() const noexcept { return Priority <= category_type::level(); }

        template<typename ... T>
        void operator()(const T &... val) {
            if (Priority <= category_type::level()) {
                const auto attrs { make_attrs(Priority) };
                prolog(attrs);
                basic_emitter::operator()(val...);
                flush(attrs);
            }
        }
        template<typename ... T>
        void operator()(source_location &&sl, const T &... val) {
            if (Priority <= category_type::level()) {
                basic_emitter::location_ = std::move(sl);
                if constexpr(sizeof...(T) != 0) {
                    const auto attrs { make_attrs(Priority) };
                    prolog(attrs);
                    basic_emitter::operator()(val...);
                    flush(attrs);
                }
            }
        }
        template<typename ... T>
        void operator()(const source_location &sl, const T &... val) {
            if (Priority <= category_type::level()) {
                basic_emitter::location_ = sl;
                if constexpr (sizeof...(T) != 0) {
                    const auto attrs { make_attrs(Priority) };
                    prolog(attrs);
                    basic_emitter::operator()(val...);
                    flush(attrs);
                }
            }
        }
        template<typename T>
        emitter& operator<<(const T &value) {
            if (Priority <= category_type::level()) {
                prolog(make_attrs(Priority));
                basic_emitter::operator<<(value);
            }
            return *this;
        }
        emitter& operator<<(ostream& (*f)(ostream&)) {
            if (Priority <= category_type::level()) {
                const auto attrs { make_attrs(Priority) };
                prolog(attrs);
                static constexpr ostream& (*el)(ostream&) = &std::endl;
                static constexpr ostream& (*fl)(ostream&) = &std::flush;
                if (f == el || f == fl) flush(attrs);
            }
            return *this;
        }
        emitter& flush() {
            if (Priority <= category_type::level()) {
                flush(make_attrs(Priority));
            }
            return *this;
        }
#if defined(__cpp_lib_format)
        template<typename ... T>
        void format(std::basic_format_string<char_type, std::type_identity_t<T>...> fmt, T&&... args) {
            if (Priority <= category_type::level()) {
                const auto attrs { make_attrs(Priority) };
                prolog(attrs);
                basic_emitter::format(fmt, std::forward<T>(args)...);
                flush(attrs);
            }
        }
#endif
    private:
        using basic_emitter::flush;
        using basic_emitter::prolog;
        using basic_emitter::make_attrs;
    };
    class log : basic_emitter {
    public:
        using basic_emitter::basic_emitter;
        constexpr log(priority p, source_location&& sl = source_location::current()) noexcept
        : basic_emitter { std::move(sl) }, priority_{p} {}
        constexpr log(priority p, const source_location& sl) noexcept : basic_emitter { sl }, priority_{p} {}
        template<typename T, typename ... Args>
        constexpr log(priority p, const T &val, const Args &... vals) noexcept : basic_emitter {{}}, priority_{p} {
            operator()(val, vals...);
        }
        log(const log&) = delete;
        log(log&&) = delete;
        log& operator=(const log&) = delete;
        log& operator=(log&&) = delete;
        ~log() { if (basic_emitter::prolog_done_) flush(); }
        void operator()(priority p) noexcept {
            priority_ = p;
        }
        operator bool() const noexcept { return priority_ <= category_type::level(); }
        template<typename ... T>
        void operator()(const T &... val) {
            if (priority_ <= category_type::level()) {
                const auto attrs { make_attrs(priority_) };
                prolog(attrs);
                basic_emitter::operator()(val...);
                flush(attrs);
            }
        }
        template<typename ... T>
        void operator()(source_location &&sl, const T &... val) {
            if (priority_ <= category_type::level()) {
                basic_emitter::location_ = std::move(sl);
                if constexpr (sizeof...(T) != 0) {
                    const auto attrs { make_attrs(priority_) };
                    prolog(attrs);
                    basic_emitter::operator()(val...);
                    flush(attrs);
                }
            }
        }
        template<typename ... T>
        void operator()(const source_location &sl, const T &... val) {
            if (priority_ <= category_type::level()) {
                basic_emitter::location_ = sl;
                if constexpr (sizeof...(T) != 0) {
                    const auto attrs { make_attrs(priority_) };
                    prolog(attrs);
                    basic_emitter::operator()(val...);
                    flush(attrs);
                }
            }
        }
        template<typename T>
        log& operator<<(const T &value) {
            if (priority_ <= category_type::level()) {
                prolog(make_attrs(priority_));
                basic_emitter::operator<<(value);
            }
            return *this;
        }
        log& operator<<(ostream& (*f)(ostream&)) {
            if (priority_ <= category_type::level()) {
                const auto attrs { make_attrs(priority_) };
                prolog(attrs);
                static constexpr ostream& (*el)(ostream&) = &std::endl;
                static constexpr ostream& (*fl)(ostream&) = &std::flush;
                if (f == el || f == fl) flush(attrs);
            }
            return *this;
        }
        log& flush() {
            if (priority_ <= category_type::level()) {
                flush(make_attrs(priority_));
            }
            return *this;
        }
#if defined(__cpp_lib_format)
        template<typename ... T>
        void format(std::basic_format_string<char_type, std::type_identity_t<T>...> fmt, T&&... args) {
            if (priority_ <= category_type::level()) {
                const auto attrs { make_attrs(priority_) };
                prolog(attrs);
                basic_emitter::format(fmt, std::forward<T>(args)...);
                flush(attrs);
            }
        }
#endif
    private:
        using basic_emitter::flush;
        using basic_emitter::prolog;
        using basic_emitter::make_attrs;
        priority priority_;
    };
    // Representations changers
    template<typename ... T>
    static constexpr auto x(T ... values) noexcept {
        return representation<radix::hex, T...> { values... };
    }
    template<typename ... T>
    static constexpr auto o(T ... values) noexcept {
        return representation<radix::oct, T...> { values... };
    }
    template<typename ... T>
    static constexpr auto b(T ... values) noexcept {
        return representation<radix::bin, T...> { values... };
    }
    template<typename ... T>
    static constexpr auto t(T ... values) noexcept {
        return representation<radix::dec, T...> { values... };
    }
    template<unsigned Width, unsigned Precision, typename ... T>
    static constexpr auto fix(T ... values) noexcept {
        return fixed<Width, Precision, T...> { values... };
    }
// Shortcodes
    using d = emitter<priority::debug>;
    using i = emitter<priority::informational>;
    using n = emitter<priority::notice>;
    using w = emitter<priority::warning>;
    using e = emitter<priority::error>;
    using c = emitter<priority::critical>;
    using a = emitter<priority::alert>;
    using f = emitter<priority::emergency>;
    // Midcodes
    using info  = emitter<priority::informational>;
    using warn  = emitter<priority::warning>;
    using crit  = emitter<priority::critical>;
    using emerg = emitter<priority::emergency>;
    // Full names
    using debug         = emitter<priority::debug>;
    using notice        = emitter<priority::notice>;
    using informational = emitter<priority::informational>;
    using warning       = emitter<priority::warning>;
    using error         = emitter<priority::error>;
    using critical      = emitter<priority::critical>;
    using alert         = emitter<priority::alert>;
    using emergency     = emitter<priority::emergency>;
};

template<class Category, class Base>
struct runtime_level : Base {
  static priority level() noexcept { return level_; }
  static void level(priority value) noexcept { level_ = value; }
private:  
  static inline priority level_ { Base::level() };
};

template<class Category, class Base>
struct runtime_sink : Base {
  using writer_type = typename Base::sink_types::writer_type;
  using prologer = typename Base::sink_types::prologer;
  using epiloger = typename Base::sink_types::epiloger;
  static auto writer() noexcept { return writer_; }
  static void writer(writer_type value) noexcept { writer_ = value; }
  static auto prolog() noexcept { return prolog_; }
  static void prolog(prologer value) noexcept { prolog_ = value; }
  static auto epilog() noexcept { return epilog_; }
  static void epilog(epiloger value) noexcept { epilog_ = value; }
  static void sink(sink::bundle bundle) noexcept {
      prolog_ = bundle.prolog;
      writer_ = bundle.writer;
      epilog_ = bundle.epliog;
  }
  static void sink(prologer p, writer_type w, epiloger e) noexcept {
      prolog_ = p;
      writer_ = w;
      epilog_ = e;
  }
private:
  static inline auto writer_ { Base::writer() };
  static inline auto prolog_ { Base::prolog() };
  static inline auto epilog_ { Base::epilog() };
};

} // namespace logovod

