/*
 * This file is on public domain. Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this file, either in source code form or as a compiled binary, for any purpose, commercial
 * or non-commercial, and by any means.
 *
 * Visit https://unlicense.org/ for details
 */

#include <logovod/logovod.h>
#include <array>
#include <iostream>
#include <iomanip>
#include <string_view>
#include <initializer_list>
#include <tuple>
#include <chrono>
#include <vector>
#include <variant>

using namespace std;
using namespace std::literals;
using namespace logovod;

struct MyCategory : runtime_level<MyCategory, category> {};
struct Category : runtime_level<Category, runtime_sink<Category, category>> {};

enum class logical { no, yes };

inline std::ostream& put_tp(std::ostream& out, std::chrono::system_clock::time_point tp) {
    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count() % 1000;
    return out << std::put_time(std::localtime(&t), "%FT%T.") << std::dec << std::setw(3) << std::setfill('0') << millis;
}
 
int main() {
  static_assert(is_same_v<decltype(std::hex), ios_base&(ios_base&)>);
  using log = logger<category>;
  char chars[] = "chars";
  static constexpr auto comma = consts::comma;
  log::i(std::tuple<short, std::string_view, int> { 10, "ddd", 1000});
  log::i(std::pair<short, std::string> { 12, "pair"});
  log::i("inf string"s, comma, "const char*", chars, setw(6), 100, true, hex, 100, '-', 200, logical::yes);
  log::d{} << "dbg string"s << ' ' << "const char*" << ' ' << chars << ' ' << 100 << ' ' << true << ' ' << hex << 100 << '-' << 200 << logical::yes << flush;
  log::d d{};
  std::array a { 10, 20, 30 };
  d("Array ", a, log::fix<10,7>(0.1, 0.2, 99.999999), 99.999999);
  d << "dec " << 100 << " hex " << hex << 100 << flush;
  d << "dec " << 100 << " hex " << hex << 100 << flush;
  clog << "dec " << 100 << " hex " << hex << 100 << endl;
  clog << "dec " << 100 << " hex " << hex << 100 << endl;
  log::n(log::x(100, 200, 300, 's', 'F'));
  log::n(log::b(10U, 20U, 30U, 's'));
  log::n(log::o(10, 20, 30, logical::yes));
  log::n(log::t(0x10, 0x20, 0x30));
  cout << "sizeof(ios_base) = " << sizeof(ios_base::fmtflags) << '\n';
  cout << 9E-9 << '\n';
  using Log = logger<MyCategory>;
  using Cog = logger<Category>;
  MyCategory::level(priority::warning);
  Category::level(priority::informational);
  Log::i("Not expected");
  Log::e("Expected");
  Cog::i("Expected");
  Cog::e("Expected");
  detail::put_time_point(clog, std::chrono::system_clock::now()) << '\n';
  detail::put_time_point(clog, std::chrono::steady_clock::now()) << '\n';
  clog << std::dec << sizeof(log::d) << '\n';
  detail::put_duration(cout, std::chrono::seconds(200)) << '\n';
  log::i("Time", std::chrono::system_clock::now(), std::chrono::seconds(200));
  signed char sc = 77;
  unsigned char uc = 88;
  static_assert(detail::is_intchar_v<const signed char&>);
  log::i("signed char", sc, "unsigned char", uc);
  
  std::optional<long> ol {};
  std::optional<long> ll {1000L};
  log::d(ol, ll);
  std::vector<short> ssv { 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60 };
  log::d{}(ssv);
  signed short ssa [] { 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60 };
  log::d{}(ssa);
  signed short ssc [] { 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60 };
  log::d{}(ssc);

  std::variant<std::string, long, double, std::monostate> any { 100L }, none{std::monostate{}};
  log::d{}("variant", any, none);
#if defined(__cpp_lib_expected) && (__cpp_lib_expected >= 202202L)
  std::expected<std::string, int> result {"yes"}, err { 100 };
#endif
  return 0;
}
