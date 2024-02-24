## User Guide
### Basic Concepts
1. Category - a class that defines logger properties and controls it behavior
2. Logger - a template, parametrized by a category that provides log emitters of different priorities
3. Emitter - template, parametrized by priority level that implements core logging functionality
4. Named emitter - an emitter template, instantiated with particular level, such as debug, warning or error
5. Sink - is a combination of three functions – writer, prologer and epiloger
6. Writer - a function that writes log messages (or payloads) to its destination, such as a file, or logging backend
7. Prologer - a function that prepends message with some attributive data, such as timestamp, tag, source location, etc.
8. Epiloger - a function that finalizes the message by appending some content, such as end-of-line.

### Categories
#### Defining a custom category
To define a custom category, declare a class, derived from  logovod::category or its descendants with member constants 
assigned your values, and/or functions returning your values. For example:

```C++
struct MyCategory : category {
    static constexpr sink_types::writer_type writer() noexcept { return sink::cout; }
    static constexpr priority level() noexcept { return priority::informational; }
};
```

and instantiate a logger template with this category

```C++
using Log = logger<MyCategory>;
```

In this example, `Log` will print to `std::cout` messages of `informational` priority or higher.
As the example suggests, writer and level are fixed at compile time.

#### Run-time configurable loggers
By design, configuration of a logger is done via its category. For the compile time configuration functions simply return constants.
To enable run time configuration, a category should return same variables instead:

```C++
struct MyCategory : category {
    static constexpr sink_types::writer_type writer() noexcept { return sink_writer; }
    static constexpr priority level() noexcept { return priority_level; }
    static priority priority_level { priority::informational };
    static sink_types::writer_type sink_writer { sink::cerr };
};
```
    
This can be also done with `logovod` templates that declare static variables for run-time configuration

```C++
// Compile time fixed category to be uses as the base
struct MyFixedCategory : category {
    static constexpr priority level() noexcept { return priority::informational; }
    static constexpr auto writer() noexcept { return sink::fd<2>; }
private:
    static inline sink::syslog syslog_{};
};

// Run-time log category 
struct MyRuntimeCategory : runtime_level<MyRuntimeCategory, runtime_sink<MyRuntimeCategory, MyFixedCategory>> {
    static constexpr std::string_view tag = "RUNTIME";
};

using Log = logger<MyRuntimeCategory>;
```

#### Hierarchical categories
Templates in the previous do not introduce hierarchical categories. For that we need optional sink with fallback to 
the base category.  The code snippet below declares a hierarchy of 5 categories

```
     Root
     /  \
    A    C
    |    |
    B    D
```

```C++
// Root log category with run-time configurable optional sink and level
struct MyRootCategory : optional_all<MyRootCategory, MyFixedCategory> {};

// Hierarchy of four categories
struct MyCategoryA : optional_all<MyCategoryA, MyRootCategory> {};
struct MyCategoryB : optional_all<MyCategoryB, MyCategoryA> {};
struct MyCategoryC : optional_all<MyCategoryC, MyRootCategory> {};
struct MyCategoryD : optional_all<MyCategoryD, MyCategoryC> {};

using LogA = logger<MyCategoryA>;
using LogB = logger<MyCategoryB>;
using LogC = logger<MyCategoryC>;
using LogD = logger<MyCategoryD>;

If a category is configured at run-time for a specific level or writer, that specific settings are used. Otherwise the base category settings are used.
```

### Logging
Although the logger depends on a category, this dependency is loose, and one can start using logger with a simple or a default category, deferring detailed category outline to some later time. 
Logovod supports three styles of use: function calls, stream left shift operators and format string (if std::format is available)

#### Invoke style
Two possible options:

```C++
    Log::d("Construct", "and", "log");
    Log::i{}("Construct", "then", "log");
```

With this syntax logger inserts spaces between values and immediately emits a log message.
Notice: Construct and log variant is missing source location

There is one caveat related to the first variant: 

```C++
    Log::d(i); // unnecessary parentheses in declaration of ‘i’
```

Ambiguous syntax is treated as declaration of variable i: `Log::d i;`

### Left shift style

```C++
    Log::d{} << "Construct" << ' ' << "then" << ' ' << "log";
```

In this line an anonymous emitter variable is constructed, used for left shift inserts. 
On the emitter destruction its buffer is flushed to the sink writer.

#### Format style
This option requires availability of `std::format`

```C++
    Log::i::format("std::{} {}", "format", "style");
```

#### Emitter reuse and deferred use
Invoke and form style assume messages are closed and emitted before return. The left shift style makes possible deferred message emitting 

```C++
    Log::d d{}; // emitter is constructed
    d << "One";
    for (auto& i : {"two", "three"}) {
        d << ' ' << i;
    }
    d << " done" << std::flush;
    d << "Reused and flushed on destruction";
```
This kind of use may require explicit flushing.

#### Containers
Logovod facilitates automatic, length-limited container printing 

```C++
    std::vector<int> v { 1, 2, 3, 4 };
    std::deque<int> q { 7, 8, 9, 10, 11 };
    Log::i{}("vector", v); // prints vector {1,2,3,4}
    Log::d{} << "deque " << q; // prints deque {7,8,9,10,11}
```

Characters `{`, `,`, `}` are provided by the category via `sep()` function

#### Delimiters and Separators
While default delimiter character and container mark-up characters are provided by the category, they can be ad hoc overridden:

```C++
    std::vector<int> v { 1, 2, 3, 4 };
    Log::i{}("Delimiter", "changed", "to", delimiter{','}, "a", "comma"); // prints Delimiter changed to,a,comma
    Log::i{}("vector", separators{'<', '|', '>'}, v); // prints vector<1|2|3|4>
```
    
### Source location
By default, logovod use source location at emitter construction place (see also clang notice).
This source location is then passed to sink functions. This behavior can be altered with  source location override.
To illustrate this with an example we first define a category enabling prolog

```C++
struct MyCategory : category {
    static constexpr auto prolog() noexcept {
        return [](std::ostream& out, attributes attrs) noexcept {
            out << sink::prolog::prolog_traits::basename(attrs.location.file_name) << ':' << attrs.location.line << ':';
        };
    }
};
```

Lines below show different ways of overriding source location

```C++
    Log::i{}("Default location"); // 11-source-location.cxx:27:Default location
    Log::i("No location");        // :0:No location
    Log::i{{}}("Empty location"); // :0:No location
    Log::i i{};
    i << "Location from construction" << std::flush; // 11-source-location.cxx:30:Location from construction
    i(source_location::current(), "Overridden");     // 11-source-location.cxx:32:Overridden
```

### Prologer and epiloger
By default, logovod prints no prolog and end-of-line as epilog. An application may define its own prolog, as show in the previous example, or use a common prolog provided by logovod, that prints timestamp, tag, priority level and source location:

```C++
    Log::i{}("With prolog");      // <curren date/time>|MYTAG   |INFO     |12-prolog.cxx   :32  |With prolog
    Log::w("No source location"); // <curren date/time>|MYTAG   |WARNING  |----------------:----|No source location
```


