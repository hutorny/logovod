/*
 * Copyright (C) 2024 Eugene Hutorny <eugene@hutorny.in.ua>
 *
 * logovod library, primary header
 *
 * logovod is a macro-free allocation-free logger front-end for C++17 and newer
 *
 * Licensed under MIT License, see full text in LICENSE
 * or visit page https://opensource.org/license/mit/
 */

#pragma once 
#include <logovod/core.h>
#if __has_include(<bitset>)
#include <logovod/bitset.h>
#endif
#if __has_include(<chrono>)
#include <logovod/chrono.h>
#endif
#if __has_include(<optional>)
#include <logovod/optional.h>
#endif
#if __has_include(<unistd.h>)
#include <logovod/sink/unistd.h>
#endif
#include <logovod/variant.h>
#if __has_include(<android/log.h>)
#include <logovod/sink/android.h>
#endif
#if __has_include(<expected>)
#include <expected>
#ifdef __cpp_lib_expected
#   include <logovod/expected.h>
#endif
#endif
