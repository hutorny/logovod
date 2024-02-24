# 
# Copyright (C) 2024 Eugene Hutorny <eugene@hutorny.in.ua>
# 
# This file is a part of logovod library
# 
# Licensed under MIT License, see full text in LICENSE
# or visit page https://opensource.org/license/mit/
# 
DESTDIR=/usr/include
CXXS = $(CXX)
STDS = c++17
BUILDDIR=build
LOGOVOD_DEB=logovod_1.0-dev
export BUILDDIR

build:
	$(info This library is header only. Use)
	$(info sudo make install DESTDIR=/usr/include )
	echo to install headers or run make help to see other options
	

install: #!   Installs headers at DESTDIR
	install -d $(DESTDIR:%=%/logovod)
	cp -r include/logovod $(DESTDIR:%=%/logovod)

package: #!   Assembles deb package
	@rm -rf $(BUILDDIR)/$(LOGOVOD_DEB)
	@cp -r deb $(BUILDDIR)/
	@mkdir -p $(BUILDDIR)/$(LOGOVOD_DEB)/usr/include
	@cp -r include/logovod $(BUILDDIR)/$(LOGOVOD_DEB)/usr/include
	@cp -r deb/DEBIAN $(BUILDDIR)/$(LOGOVOD_DEB)/
	cd $(BUILDDIR) && dpkg-deb --build --root-owner-group $(LOGOVOD_DEB)

examples: #!  Build examples
	$(MAKE) -C example

tests: #!     Build tests.
	$(foreach c,$(CXXS),$(foreach s,$(STDS),$(MAKE) -C test/unit CXX="$(c)" STD="$(s)";))
#	$(foreach c,$(CXXS),$(foreach s,$(STDS),$(MAKE) -C test/functional CXX="$(c)" STD="$(s)";))

run-tests: tests #! Build and run unit-tests.
	$(foreach c,$(CXXS),$(foreach s,$(STDS),$(MAKE) -C test/unit run-tests CXX="$(c)" STD="$(s)";))
	$(foreach c,$(CXXS),$(foreach s,$(STDS),$(MAKE) -C test/functional run-tests CXX="$(c)" STD="$(s)";))

help:
	@echo The following make targets are available:
	@sed -n 's/\:.*#\!/ /p' Makefile
	@echo Optional variables for the examples, tests and run-tests targets:
	@echo 'CXXS provides list of compilers, e.g.: CXXS="g++-12 g++-13 clang++-15"' 
	@echo 'STDS - list of C++ standards, e.g.:    STDS="c++17 c++-20"'
	@echo For example
	@echo 'make -j$$(nproc) run-tests CXXS="g++-11 g++-12 g++-13 clang++-15" STDS="c++17 c++20"'

.PHONY: help build tests install run-tests examples