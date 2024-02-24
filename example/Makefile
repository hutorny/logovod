# 
# Copyright (C) 2024 Eugene Hutorny <eugene@hutorny.in.ua>
# 
# This file is a part of logovod library
# 
# Licensed under MIT License, see full text in LICENSE
# or visit page https://opensource.org/license/mit/
# 
BUILDDIR = build
BDIR     := $(BUILDDIR:%=%/)
CFLAGS   = -O2 -ffunction-sections -fdata-sections
LDFLAGS  = -Wl,--gc-sections
STD      = c++20

SOURCES  = $(shell find -name '*.cxx')
EXAMPLES = $(SOURCES:./%.cxx=$(BDIR)%) 
WFALGS   = -pedantic -Wall -Wextra -Weffc++ -Wconversion -Woverloaded-virtual -Wcast-align -Wcast-qual \
           -Wstrict-null-sentinel -Wswitch-default -Wold-style-cast

INCLUDES += $(realpath ../include) $(if $(ANDROID),$(ANDROID_INCLUDE) $(ANDROID_GTEST) $(ANDROID_GTEST:%=%/include))
LIBDIRS +=  $(if $(ANDROID), $(ANDROID_LIBDIR) $(ANDROID_LIBDIR)/$(ANDROID)) 

all: $(EXAMPLES)

$(BDIR)%: %.cxx
	@mkdir -p $(dir $@)
	$(CXX) -std=$(STD) $(CFLAGS) $(CXXFLAGS) $(WFLAGS) $(INCLUDES:%=-I%) -MMD -MP -MF$(@:.o=.d) -MT$@ -o $@ $<

.PHONY: all clean help

clean:
	rm -rf $(BDIR)

help:
	$(info This makefile builds examples)
	$(info $(SOURCES))
	$(info $(EXAMPLES))
	@true

-include $(shell find  $(BDIR) -name '*.d')