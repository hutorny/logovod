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
#include <optional>

namespace logovod {
template<typename T>
struct formatter<std::optional<T>> {
    template<typename Printer, typename Stream>
    void operator()(Printer& printer, Stream& stream, const std::optional<T>& value) {
      if( value.has_value() ) printer(stream, value.value());
      else {
        printer.start(stream);
        printer.finish(stream);
      }
    }
};

template<class Category, class Base>
struct optional_level : Base {
  static priority level() noexcept { return level_.has_value() ? level_.value() : Base::level(); }
  static void level(priority value) noexcept { level_ = value; }
  static void nolevel() noexcept { level_.reset(); }
private:
  static inline std::optional<priority> level_ {};
};

template<class Category, class Base>
struct optional_sink : Base {
  using writer_type = typename Base::sink_types::writer_type;
  using prologer = typename Base::sink_types::prologer;
  using epiloger = typename Base::sink_types::epiloger;
  static writer_type writer() noexcept { return writer_.has_value() ? writer_.value() : Base::writer(); }
  static void writer(writer_type value) noexcept { writer_ = value; }
  static void nowriter() noexcept { writer_.reset(); }
  static prologer prolog() noexcept { return prolog_.has_value() ? prolog_.value() : Base::prolog(); }
  static void prolog(prologer value) noexcept { prolog_ = value; }
  static void noprolog() noexcept { prolog_.reset(); }
  static epiloger epilog() noexcept { return epilog_.has_value() ? epilog_.value() : Base::epilog(); }
  static void epilog(epiloger value) noexcept { epilog_ = value; }
  static void noepilog() noexcept { epilog_.reset(); }
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
  static void nosink() noexcept {
      noprolog();
      nowriter();
      noepilog();
  }
private:
  static inline std::optional<writer_type> writer_ {};
  static inline std::optional<prologer> prolog_ {};
  static inline std::optional<epiloger> epilog_ {};
};

template<class Category, class Base>
struct optional_all : optional_sink<Category, optional_level<Category, Base>> {};

} // namespace logovod
