INCLUDES = -I$(top_builddir)

noinst_PROGRAMS = tests
tests_SOURCES = test_main.cpp

check-syntax:
	$(CXXCOMPILE) -Wall -Wextra -Weffc++ -pedantic -fsyntax-only $(CHK_SOURCES)
.PHONY: check-syntax
