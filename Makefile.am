noinst_PROGRAMS = shiritori_server
shiritori_server_SOURCES = \
	server_main.cpp

noinst_HEADERS = \
	server.hpp \
	request_handler.hpp \
	request_spec.hpp \
	configuration.hpp \
	game.hpp \
	player.hpp

EXTRA_DIST = README.asciidoc

SUBDIRS = test

check-syntax:
	$(CXXCOMPILE) -Wall -Wextra -Weffc++ -pedantic -fsyntax-only $(CHK_SOURCES)
.PHONY: check-syntax
