# 
# Copyright (C) 2024 Eugene Hutorny <eugene@hutorny.in.ua>
# 
# This file is a part of logovod library
# 
# Licensed under MIT License, see full text in LICENSE
# or visit page https://opensource.org/license/mit/
# 
SYSNAME := $(shell uname |  tr '[:upper:]' '[:lower:]')
PLATFORM = $(or $(ANDROID:%=android-%), $(SYSNAME))
ARCH     = $(shell arch)
ifdef ANDROID
CXX      = clang++
STD     ?= c++20
else
STD     ?= c++17
endif

BUILDDIR = build
BDIR     := $(BUILDDIR)/$(PLATFORM)/$(ARCH)/$(CXX)/$(STD)/
CFLAGS   = -O2 -ffunction-sections -fdata-sections $(if $(ANDROID),-static)
LDFLAGS  = -Wl,--gc-sections
LIBS     = $(if $(ANDROID),,gtest)
ABI      = $(if $(filter $(ARCH),armv7a),eabi)$(ANDROID)
COMPILE  = $(or $(ANDROID:%=$(ARCH)-$(SYSNAME)-android$(ABI)-$(CXX)),$(CXX))
COMPILER_PATH := $(shell which $(COMPILE))

SOURCES  = $(shell ls -1 *.cxx)
GTESTSRC = gtest-all.cc
OBJECTS  = $(SOURCES:%.cxx=$(BDIR)%.o) $(if $(ANDROID),$(GTESTSRC:%.cc=$(BDIR)%.o)) 
WFALGS   = -pedantic -Wall -Wextra -Weffc++ -Wconversion -Woverloaded-virtual -Wcast-align -Wcast-qual \
           -Wstrict-null-sentinel -Wswitch-default -Wold-style-cast
ANDROID_INCLUDE = $(realpath $(dir $(COMPILER_PATH))/../sysroot/usr/include/)
ANDROID_GTEST = $(realpath $(dir $(shell which i686-linux-android34-clang++))/../../../../../sources/third_party/googletest)
ANDROID_LIBDIR = $(realpath $(dir $(shell which $(COMPILE)))/../sysroot/usr/lib/$(ARCH:armv7a=arm)-$(SYSNAME)-android)

INCLUDES += $(realpath ../../include) $(if $(ANDROID),$(ANDROID_INCLUDE) $(ANDROID_GTEST) $(ANDROID_GTEST:%=%/include))
LIBDIRS +=  $(if $(ANDROID), $(ANDROID_LIBDIR) $(ANDROID_LIBDIR)/$(ANDROID)) 

$(BDIR)logovod-unit-tests: $(OBJECTS) | $(BDIR)
	$(COMPILE) $(CFLAGS) $(CXXFLAGS) $(LDFLAGS) -MMD -MP -MF$(@:.o=.d) -MT$@ -o $@ $^ $(LIBS:%=-l%) $(LIBDIRS:%=-L%)

$(BDIR)gtest-all.o: $(ANDROID_GTEST:%=%/src/gtest-all.cc) 
	$(if $(COMPILER_PATH),,$(error $(COMPILE) not found in the path))
	$(COMPILE) -std=$(STD) $(CFLAGS) $(CXXFLAGS) $(WFLAGS) $(INCLUDES:%=-I%) -MMD -MP -MF$(@:.o=.d) -MT$@ -o $@ -c $<

$(BDIR)%.o: %.cxx | $(BDIR)
	$(if $(COMPILER_PATH),,$(error $(COMPILE) not found in the path))
	$(COMPILE) -std=$(STD) $(CFLAGS) $(ANDROID_CXXFLAGS) $(CXXFLAGS) $(WFLAGS) $(INCLUDES:%=-I%) -MMD -MP -MF$(@:.o=.d) -MT$@ -o $@ -c $<

$(BDIR):
	@mkdir -p $@

.PHONY: run-tests clean gtest help

run-tests: $(BDIR)logovod-unit-tests
ifdef ANDROID
	adb push $(BDIR)logovod-unit-tests /data/local/tmp/
	adb shell /data/local/tmp/logovod-unit-tests
else
	$(BDIR)logovod-unit-tests
endif

clean:
	rm -rf $(BDIR)

clean-all:
	rm -rf $(BUILDDIR)/*

help:
	$(info This makefile builds and runs tests for a given c++ standard with a given CXX compiler:)
	$(info make CXX=clang++-15 STD=c++17) 
	$(info will build tests with clang++-15 and -std=c++17)
	$(info To build tests for Android, ensure you have the toolchain path in the PATH and run )
	$(info make -j=$$(nproc) ARCH=<arch> ANDROID=<ver>)
	$(info e.g. make -j=$$(nproc) ARCH=aarch64 ANDROID=23)
	$(info gtest on Android is supported staring from android-23)
	$(info on earlier version lld fials with error: undefined symbol: pthread_atfork)
	$(info To run tests on  Android)
	$(info 1. ensure you have just one device connected)
	$(info 2. ensure adb is in the PATH and run command)
	$(info 3. run command)
	$(info make run-tests ARCH=<arch> ANDROID=<ver>)
	@true

-include $(shell find  $(BDIR) -name '*.d')