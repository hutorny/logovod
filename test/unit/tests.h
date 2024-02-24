/*
 * Copyright (C) 2024 Eugene Hutorny <eugene@hutorny.in.ua>
 *
 * This file is a part of logovod library
 *
 * Licensed under MIT License, see full text in LICENSE
 * or visit page https://opensource.org/license/mit/
 */

#include <gtest/gtest.h>
#include <logovod/logovod.h>
#include <logovod/sink/attributer.h>

#include <filesystem>
 
namespace logovod::tests {

struct LoggerTest : testing::Test {
    void SetUp() override {
        message.clear();
        payload.clear();
        attrs = attributes{};
        write_count = 0;
    }
    inline static std::string message {};
    inline static std::string payload {};
    inline static attributes attrs {};
    inline static unsigned write_count {};
    struct TestCategory : category {
        static constexpr sink_types::writer_type writer() noexcept { return LoggerTest::testsink; }
        static constexpr sink_types::epiloger epilog() noexcept { return sink::epilog::null; }
    };
    using L = logger<TestCategory>;
    static void testsink(std::string_view msg, std::string_view load, attributes a) noexcept {
       message = msg;
       payload = load;
       attrs = a;
       write_count++;
   }
    static void reset() {
        message = "";
        payload = "";
    }
};

struct ScalarTypes : LoggerTest {};

struct LoggerRadix : LoggerTest {};

struct Strings : LoggerTest {};

struct Containers : LoggerTest {
    struct RangeLimited : TestCategory {
        static constexpr std::size_t range_limit() noexcept { return 4; }
        static constexpr std::string_view trimmed() noexcept { return ".."; }
    };
    using R = logger<RangeLimited>;
};

struct Structures : LoggerTest {};

struct SourceLocation : LoggerTest {
    static constexpr auto basename(std::string_view fn) {
        constexpr auto ps = std::filesystem::path::preferred_separator;
        auto pos = fn.rfind(ps);
        return fn.substr(pos == fn.npos ? 0 : pos+1);
    }
    static void prologsl(std::ostream& o, attributes a) noexcept {
        if (a.location.line != 0) {
            o << basename(a.location.file_name) << ':' << a.location.line << ':';
        } else {
            o << "--:" << a.location.line << ':';
        }
    }
    struct Category : TestCategory {
        static constexpr auto prolog() noexcept { return prologsl; }     
    };
    using L = logger<Category>;
};

struct Delimiters : LoggerTest {
    struct Custom : TestCategory {
        static constexpr delimiter dlm() noexcept { return delimiter{'_'}; }
        static constexpr separators sep() noexcept { return separators {'[',';',']'}; }
    };
    using L = logger<TestCategory>;
    using C = logger<Custom>;
};

struct Levels : LoggerTest {
    struct Warnings : TestCategory {
        static constexpr priority level() noexcept { return priority::warning; }
    };
    using L = logger<Warnings>;
    
    struct RuntimeLevel : runtime_level<RuntimeLevel, TestCategory> {};
    using R = logger<RuntimeLevel>;
    void SetUp() override {
        LoggerTest::SetUp();
        RuntimeLevel::level(priority::error);
    }
};

struct Attributes : LoggerTest {
    struct CategoryA : TestCategory {
        static constexpr std::string_view tag = "A";
    };
    struct CategoryB : TestCategory {
        static constexpr std::string_view tag = "B";
    };
    using A = logger<CategoryA>;
    using B = logger<CategoryB>;
};

struct Buffer : LoggerTest {
    struct Limited : TestCategory {
        static constexpr std::size_t length_limit = 24;
    };
    struct Terminated : Limited {
        static constexpr sink_types::epiloger epilog() noexcept { return sink::epilog::eol; }
    };
    struct Trimmed : Limited {
        static constexpr sink_types::epiloger epilog() noexcept { return sink::epilog::trimeol<Trimmed>; }
    };
    struct Unlimited : TestCategory {
        static constexpr std::size_t length_limit = unlimited;
    };
    using L = logger<Limited>;
    using T = logger<Terminated>;
    using U = logger<Unlimited>;
    using R = logger<Trimmed>;
};

struct FunctorSink {
    void operator()(std::string_view msg, std::string_view load, attributes attr) {
        message = msg;
        payload = load;
        attrs[1] = attr;
    }
    void operator()(std::ostream& out, attributes attr) {
        out << attr.tag << ' ';
        attrs[0] = attr;
    }
    void operator()(attributes attr, std::ostream& out) {
        out << ';';
        attrs[2] = attr;
    }
    std::string payload {};
    std::string message {};
    attributes attrs[3] {};
};

struct Sinks : LoggerTest {
    struct RuntimeCategory1 : runtime_sink<RuntimeCategory1, TestCategory> {};
    struct RuntimeCategory2 : runtime_sink<RuntimeCategory2, TestCategory> {};
    struct FunctorCategory : TestCategory {
        static FunctorSink& writer() noexcept { return sink; }
        static FunctorSink& prolog() noexcept { return sink; }
        static FunctorSink& epilog() noexcept { return sink; }
        static inline FunctorSink sink{};
        static constexpr std::string_view tag = "F";
    };
    using L = logger<RuntimeCategory1>;
    using R = logger<RuntimeCategory2>;
    using F = logger<FunctorCategory>;
    void SetUp() override {
        LoggerTest::SetUp();
        FunctorCategory::sink = FunctorSink{};
        L::category_type::sink(TestCategory::prolog(), TestCategory::writer(), TestCategory::epilog());
        R::category_type::sink(TestCategory::prolog(), TestCategory::writer(), TestCategory::epilog());
    }
};

struct Format : Buffer {
    struct RuntimeCategory : runtime_sink<RuntimeCategory, TestCategory> {};
    using F = logger<TestCategory>;
    struct Warnings : TestCategory {
        static constexpr priority level() noexcept { return priority::warning; }
    };
    using W = logger<Warnings>;
    using R = logger<RuntimeCategory>;
    void SetUp() override {
        LoggerTest::SetUp();
        R::category_type::sink(TestCategory::prolog(), TestCategory::writer(), TestCategory::epilog());
    }
};

struct Expected : LoggerTest {};

struct Optional : LoggerTest {
    struct BaseCategory : TestCategory {
        static constexpr priority level() noexcept { return priority::error; }
    };
    struct SubCategoryA : optional_all<SubCategoryA, BaseCategory> {};
    struct SubCategoryB : optional_all<SubCategoryB, SubCategoryA> {};
    struct SubCategoryC : optional_all<SubCategoryC, SubCategoryA> {};
    struct SubCategoryD : optional_all<SubCategoryD, SubCategoryC> {};
    using A = logger<SubCategoryA>;
    using B = logger<SubCategoryB>;
    using C = logger<SubCategoryC>;
    using D = logger<SubCategoryD>;
    void TearDown() override {
        SubCategoryA::nosink();
        SubCategoryB::nosink();
        SubCategoryC::nosink();
        SubCategoryD::nosink();
        SubCategoryA::nolevel();
        SubCategoryB::nolevel();
        SubCategoryC::nolevel();
        SubCategoryD::nolevel();
    }
    static inline std::string local {};
    static void reset() {
        LoggerTest::reset();
        local = "";
    }
};

struct Wchar : testing::Test {
    void SetUp() override {
        message.clear();
        payload.clear();
        attrs = attributes{};
        write_count = 0;
    }
    inline static std::wstring message {};
    inline static std::wstring payload {};
    inline static attributes attrs {};
    inline static unsigned write_count {};
    static void testsink(std::wstring_view msg, std::wstring_view load, attributes a) noexcept {
       message = msg;
       payload = load;
       attrs = a;
       write_count++;
   }
    static void reset() {
        message = L"";
        payload = L"";
    }
    struct Category : wcategory {
        static constexpr sink_types::writer_type writer() noexcept { return testsink; }
        static constexpr sink_types::epiloger epilog() noexcept { return sink::epilog::wnull; }
    };
    struct Limited : Category {
        static constexpr std::size_t length_limit = 24;
    };
    struct Terminated : Limited {
        static constexpr sink_types::epiloger epilog() noexcept { return sink::epilog::weol; }
    };
    struct Unlimited : Category {
        static constexpr std::size_t length_limit = unlimited;
    };
    using C = logger<Category>;
    using L = logger<Limited>;
    using T = logger<Terminated>;
    using U = logger<Unlimited>;

};

} // namespace logovod::tests
