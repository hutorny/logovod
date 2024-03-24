# Logovod

logovod is a macro-free allocation-free user-configurable logger front-end for C++17 and newer.

## License

Licensed under MIT

## Features
logovod features
1. Three styles of use — with functions, ostream lefst hifts, format string  
2. Automatic space insertion of functional style
3. Automatic printing of standard containers, tuples, variants, optional
4. Compile or run-time priority levels
5. Compile or run-time sinks
6. Constrained message length
7. Support for wide-char streams
8. Priority levels according to [RFC-5424 section-6.2.1 ](https://datatracker.ietf.org/doc/html/rfc5424#section-6.2.1)

## Quick Start Guide

```C++
    // Instantiate a logger type
    using Log = logovod::logger<logovod::category>;
    // Emit log messages with a function-call style
    Log::debug("Hello Logovod");
    // or with I/O stream shifts
    Log::info{} << "Hello Logovod I/O stream style";
    // or with std::format
    Log::notice{}.format("Hello {} format style", "Logovod");
```

## Design Documentation

1. Concerns [doc/Concerns.md](doc/Concerns.md)
2. Design Decisions [doc/Design.md](doc/Design.md)

## User Guide

1. User Guide [doc/UserGuide.md](doc/UserGuide.md)

## Tested with

### Linux

|  | c++17  | c++20  | c++23  |
|---|---|---|---|
| g++-11  | PASS  | PASS  | N/A  |
| g++-12  | PASS  | PASS  | N/A  |
| g++-13  | PASS  | PASS  | PASS |
| clang++-15  | **NOTE 1**   | **NOTE 1**  | N/A  |

**NOTE 1** clang++-15 does not fully support source location. A workaround exists.

### Android

* NDK 26.1.10909125
* clang++-17
* c++20
* android23 - android34



