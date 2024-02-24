/*
 * Copyright (C) 2024 Eugene Hutorny <eugene@hutorny.in.ua>
 *
 * This file is a part of logovod library
 *
 * Licensed under MIT License, see full text in LICENSE
 * or visit page https://opensource.org/license/mit/
 */

#pragma once
#include <cstdint>
#include <iterator>
#include <iomanip>
#include <tuple>
#include <type_traits>
#if (__cplusplus < 201703L)
#error "At least C++17 is required"
#endif

#if __has_include(<source_location>)
#include <source_location>
#ifdef __cpp_lib_source_location
// Available since msvc-19.29 (VS16.11)
using source_location = std::source_location;
#elif __has_include(<experimental/source_location>)
// Available since gcc-8 clang-9 icc-19 nvc++22.7
#include <experimental/source_location>
using source_location = std::experimental::source_location;
#else
#  error "source_location or experimental/source_location is required"
#endif
#endif

#if __has_include(<format>)
#include <format>
#endif

namespace logovod {
namespace detail {

template<typename T>
struct has_view {
    template<typename C>
    static auto check(const C&& list, int) -> decltype(std::declval<C>().view());
    template<typename C>
    static void check(const C&&, long);
    static constexpr bool value =
            not std::is_same_v<decltype(check(std::declval<T>(), 0)), void>;
};

template<class Class>
inline constexpr auto has_view_v = has_view<Class>::value;

template<typename T>
struct is_range_iterable {
    template<typename C>
    static auto check(const C&& list, int) -> decltype(*std::cbegin(list));
    template<typename C>
    static void check(const C&&, long);
    static constexpr bool value =
            not std::is_same_v<decltype(check(std::declval<T>(), 0)), void>;
};
template<typename T>
inline constexpr bool is_range_iterable_v = is_range_iterable<T>::value;

template<typename Stream, typename Value>
struct have_lshift_operator {
    template<typename S, typename T>
    static auto check(S&& stream, const T&& list, int) -> decltype(stream << list);
    template<typename S, typename T>
    static void check(S&& stream, const T&& list, long);
    static constexpr bool value = !std::is_same_v<decltype(check(std::declval<Stream>(), std::declval<Value>(), 0)), void>;
};

template<typename Stream, typename Value>
inline constexpr bool have_lshift_operator_v = have_lshift_operator<Stream, Value>::value;

template<typename T, typename UT = std::underlying_type_t<T>>
constexpr auto operator+(T val) { return static_cast<UT>(val); }

template<typename T>
constexpr auto is_intchar_v = std::is_same_v<signed char, std::remove_cv_t<std::remove_reference_t<T>>> || 
                              std::is_same_v<unsigned char, std::remove_cv_t<std::remove_reference_t<T>>>;
} // namespace detail

template<typename Type>
struct formatter {
    template<typename Printer, typename Stream>
    void operator()(Printer&, Stream& stream, const Type& value) {
        static_assert(std::is_enum_v<Type>, "Unsupported format conversion");
        using namespace detail;
        if constexpr(std::is_enum_v<Type>) {
            stream << +value;
        }
    }
};

template<typename CharT>
struct basic_delimiter {
  using char_type = CharT;
  char_type value;
  constexpr basic_delimiter(char_type val) : value {val } {}
  constexpr basic_delimiter(const basic_delimiter&) = default;
  constexpr basic_delimiter(basic_delimiter&&) = default;
  basic_delimiter& operator=(const basic_delimiter&) = default;
  basic_delimiter& operator=(basic_delimiter&&) = default;
  basic_delimiter& operator=(char_type val) noexcept { value = val; return *this; }
  constexpr bool operator==(const basic_delimiter& that) const noexcept { return that.value = value; }
};

using delimiter = basic_delimiter<char>;
using wdelimiter = basic_delimiter<wchar_t>;

template<typename CharT>
struct basic_separators {
  using char_type = CharT;
  char_type start;
  char_type delim;
  char_type finish;
};

using separators = basic_separators<char>;
using wseparators = basic_separators<wchar_t>;

namespace consts {
inline constexpr delimiter default_delimiter { ' ' };
inline constexpr delimiter comma { ',' };
inline constexpr delimiter colon { ':' };
inline constexpr delimiter semi { ';' };
inline constexpr delimiter space { ' ' } ;
inline constexpr delimiter none { '\0' };
inline constexpr separators default_separators { '{', ',', '}' };
}

namespace detail {

template<typename Stream, typename Type>
inline constexpr bool use_default_lshift = have_lshift_operator_v<Stream, Type> && !(std::is_array_v<Type>
  && !std::is_same_v<std::remove_extent_t<Type>, char> && !std::is_same_v<std::remove_extent_t<Type>, wchar_t>);

template<typename CharT, class Traits = std::char_traits<CharT>>
class basic_printer {
public:
  using char_type = CharT;
  using char_traits = Traits;
  using ostream = std::basic_ostream<char_type, char_traits>;
  using string_view = std::basic_string_view<char_type, char_traits>;
  using delimiter_type = basic_delimiter<char_type>;
  using separators_type = basic_separators<char_type>;

  basic_printer(std::size_t range_limit, string_view ellipsis, delimiter_type dlm, separators_type sep)
    :  range_limit_{range_limit}, ellipsis_ {ellipsis}, sep_{sep}, dlm_{dlm} { }
  basic_printer(const basic_printer&) = default;
  basic_printer(basic_printer&&) = default;
  basic_printer& operator=(const basic_printer&) = default;
  basic_printer& operator=(basic_printer&&) = default;
  template<typename Type, typename ... List>
  void operator()(ostream& out, const Type& value, const List& ... values) {
      if constexpr(std::is_same_v<Type, delimiter_type> || std::is_same_v<Type, separators_type>) {
          operator()(value);
      } else {
          if constexpr(use_default_lshift<ostream, Type>) {
              if constexpr(is_intchar_v<Type>) { 
                out << +value;
              } else {
                out << value;
              }
          } else if constexpr(detail::is_range_iterable_v<Type>) {
              char_type dlm {};
              start(out);
              auto limit { range_limit_ };
              for(const auto& i : value) {
                  if (dlm != '\0') out << dlm;
                  if( limit-- == 0 ) {
                    out << ellipsis_;
                    break;
                  }
                  operator()(out, i);
                  dlm = sep_.delim;
              }
              finish(out);
          } else {
              formatter<Type>{}(*this, out, value);
          }
    }

    if ((!no_dlm_after<Type>() || depth_ != 0) && !no_dlm_before<List...>()) {
        delimiter(out);
    }
    if constexpr(sizeof...(List) != 0) {
        operator()(out, values...);
    }
  }
  void operator()(separators_type sep) noexcept { sep_ = sep; }
  void operator()(delimiter_type dlm) noexcept { dlm_ = dlm; }
  void reset(delimiter_type dlm, separators_type sep) noexcept { dlm_ = dlm; sep_ = sep; }
  void start(ostream& out) { if (sep_.start != '\0') out << sep_.start; depth_++; }
  void finish(ostream& out) { if (sep_.finish != '\0') out << sep_.finish; depth_--; }
  void delimiter(ostream& out) { if (dlm_.value != '\0') out << dlm_.value; }
  auto delimiter() const noexcept { return dlm_; }
  void delimiter(char_type dlm) noexcept { dlm_.value = dlm; }
  void delimiter(delimiter_type dlm) noexcept { dlm_ = dlm; }
  auto separator() const noexcept { return sep_.delim; }
private:
  template<typename>
  friend class formatter;

  template<typename T>
  static constexpr bool is_manip() noexcept {
      using namespace std;
      return is_same_v<T, ostream& (*)(ostream&)> || is_same_v<T, ostream& (&)(ostream&)> || is_same_v<T, ostream&(ostream&)>
          || is_same_v<T, decltype(std::setw(0))>
          || is_same_v<T, decltype(std::setprecision(0))>
          || is_same_v<T, ios_base&(&)(ios_base&)> || is_same_v<T, ios_base&(*)(ios_base&)> || is_same_v<T, ios_base&(ios_base&)>;
  }
  template<typename T>
  static constexpr bool no_dlm_after() noexcept {
      using namespace std;
      return is_same_v<T, char_type> || is_same_v<T, separators_type> || is_manip<T>();
  }
  template<typename T, typename ... List>
  static constexpr bool no_dlm_before_() noexcept {
      using namespace std;
      return is_same_v<T, char_type> || is_same_v<T, delimiter_type> || is_same_v<T, separators_type> || is_manip<T>();
  }
  template<typename ... List>
  static constexpr bool no_dlm_before() noexcept {
      if constexpr(sizeof...(List) == 0) {
          return true;
      } else {
          return no_dlm_before_<List...>();
      }
  }
  std::size_t range_limit_;
  string_view ellipsis_;
  separators_type sep_;
  delimiter_type dlm_;
  uint8_t depth_ {};
};
} // namespace detail


enum class radix { bin, oct, dec, hex };

template<radix Radix, typename ... T>
struct representation {
    static inline constexpr auto base = Radix;
    static_assert(((std::is_integral_v<T> || std::is_enum_v<T>) && ...));
    constexpr representation(T ... val) : values { val ... } { }
    std::tuple<T ...> values;
};

template<unsigned Width, unsigned Precision, typename ... T>
struct fixed {
    static inline constexpr auto width = Width;
    static inline constexpr auto precision = Precision;
    static_assert(((std::is_floating_point_v<T>) && ...));
    constexpr fixed(T ... val) : values { val ... } { }
    std::tuple<T ...> values;
};

template<radix Radix, typename ... T>
struct formatter<representation<Radix, T...>> {
    template<typename Printer, typename Stream>
    void operator()(Printer& printer, Stream& stream, const representation<Radix, T...>& value) {
        using namespace detail;
        static_assert(Radix != radix::bin, "binary representation requires std::bitset and implemented in <bitset.h>");
        const auto flags { stream.flags() };
        stream.flags(flag());
        std::apply([&printer,&stream](auto&&... args) {printer(stream, +args ...);}, value.values);
        stream.flags(flags);
    }

    static auto flag() noexcept {
        switch(Radix) {
        case radix::hex: return std::ios_base::hex;
        case radix::dec: return std::ios_base::dec;
        case radix::oct: return std::ios_base::oct;
        default:;
        }
        return std::ios_base::dec;
    }
};

template<unsigned Width, unsigned Precision, typename ... T>
struct formatter<fixed<Width, Precision, T...>> {
    template<typename Printer, typename Stream>
    void operator()(Printer& printer, Stream& stream, const fixed<Width, Precision, T...>& value) {
        using namespace detail;
        const auto flags { stream.flags() };
        const auto width { stream.width() };
        const auto precision { stream.precision() };
        stream.flags(std::ios_base::fixed);
        stream.width(Width);
        stream.precision(Precision);
        std::apply([&printer,&stream](auto&&... args) {printer(stream, args ...);}, value.values);
        stream.width(width);
        stream.precision(precision);
        stream.flags(flags);
    }
};

template<typename ... T>
struct formatter<std::tuple<T...>> {
    template<typename Printer, typename Stream>
    void operator()(Printer& printer, Stream& stream, const std::tuple<T...>& value) {
        auto dlm { printer.delimiter() };
        printer.start(stream);
        printer.delimiter(printer.separator());
        std::apply([&printer,&stream](auto&&... args) { printer(stream, args ...);}, value);
        printer.finish(stream);
        printer.delimiter(dlm);
    }
};
template<typename F, typename S>
struct formatter<std::pair<F, S>> {
    template<typename Printer, typename Stream>
    void operator()(Printer& printer, Stream& stream, const std::pair<F, S>& value) {
        auto dlm { printer.delimiter() };
        printer.start(stream);
        printer.delimiter(printer.separator());
        std::apply([&printer,&stream](auto&&... args) { printer(stream, args ...);}, value);
        printer.finish(stream);
        printer.delimiter(dlm);
    }
};

template<>
struct formatter<source_location> {
    template<typename Printer, typename Stream>
    void operator()(Printer&, Stream& stream, const source_location& value) {
        stream << value.file_name() << ':' << value.line();
    }
};

} // namespace logovod

