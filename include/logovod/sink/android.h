/*
 * Copyright (C) 2024 Eugene Hutorny <eugene@hutorny.in.ua>
 *
 * This file is a part of logovod library
 *
 * Licensed under MIT License, see full text in LICENSE
 * or visit page https://opensource.org/license/mit/
 */

#pragma once 
#include <android/log.h>
#include <logovod/core.h>

namespace logovod::sink {
namespace detail {
inline constexpr android_LogPriority from(priority level) {
    const auto ilevel = static_cast<std::size_t>(level);
    constexpr android_LogPriority lt[] = {
        ANDROID_LOG_FATAL, ANDROID_LOG_FATAL, ANDROID_LOG_ERROR, ANDROID_LOG_WARN,
        ANDROID_LOG_INFO, ANDROID_LOG_INFO, ANDROID_LOG_DEBUG };
        return (ilevel < sizeof(lt)) ? lt[ilevel] : ANDROID_LOG_UNKNOWN;
}
} // namespace detail

inline void android(std::string_view message, std::string_view payload, attributes attrs) noexcept {
    ::__android_log_write(detail::from(attrs.level), attrs.tag.data(), payload.data());
}

} // namespace logovod::sink

