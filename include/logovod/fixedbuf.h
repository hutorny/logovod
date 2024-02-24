/*
 * Copyright (C) 2024 Eugene Hutorny <eugene@hutorny.in.ua>
 *
 * This file is a part of logovod library
 *
 * Licensed under MIT License, see full text in LICENSE
 * or visit page https://opensource.org/license/mit/
 */

#pragma once 

#include <iostream>
#include <string_view>
#include <limits>

namespace logovod::detail {

template<std::size_t Size, class CharT, class Traits = std::char_traits<CharT>>
class basic_fixedbuf : public std::basic_streambuf<CharT, Traits> {
public:
  using base = std::basic_streambuf<CharT, Traits>;
  using char_type = typename base::char_type;
  using traits_type = typename base::traits_type;
  using pos_type = typename base::pos_type;
  using off_type = typename base::off_type;
  using int_type = typename base::int_type;
  using streamsize = std::streamsize;
  basic_fixedbuf() : base{} {
    base::setp(std::begin(data_), std::end(data_));
  }
  basic_fixedbuf(const basic_fixedbuf&) = delete;
  basic_fixedbuf(basic_fixedbuf&&) = delete;
  basic_fixedbuf& operator=(const basic_fixedbuf&) = delete;
  basic_fixedbuf& operator=(basic_fixedbuf&&) = delete;
  ~basic_fixedbuf() { }
  std::basic_string_view<char_type, traits_type> view() const noexcept {
    return { base::pbase(), static_cast<std::size_t>(ppos()) };
  }
  inline static const pos_type npos = -1;
  streamsize available() const noexcept {
      return static_cast<streamsize>(Size) - ppos();
  }
protected:
  pos_type seekpos(pos_type pos, std::ios_base::openmode mode) override {
    if ((std::ios_base::out & mode) == 0) return npos;
    if(static_cast<std::size_t>(pos) > Size) return npos;
    base::setp(std::begin(data_), std::end(data_));
    base::pbump(static_cast<int>(pos));
    return ppos();
  }
  pos_type seekoff(off_type off, std::ios_base::seekdir dir, std::ios_base::openmode mode) override {
    if ((std::ios_base::out & mode) == 0) return npos;
    if (dir == std::ios_base::cur) {
      if (off == 0)
        return ppos();
      else {
        if(base::pptr() + off >= base::epptr())
          return npos;
        base::pbump(static_cast<int>(off));
        return ppos();
      }
    } else if (dir == std::ios_base::beg) {
      return seekpos(off, mode);
    } else if (dir == std::ios_base::end) {
      if(static_cast<std::size_t>(off) > Size) return npos;
      return seekpos(static_cast<pos_type>(Size)-off, mode);
    }
    return npos;
  }
private:
  pos_type ppos() const noexcept {
    return base::pptr() - base::pbase();
  }
  char_type data_[Size];
};

template<std::size_t Size>
using fixedbuf = basic_fixedbuf<Size, char>;

} // logovod::detail

