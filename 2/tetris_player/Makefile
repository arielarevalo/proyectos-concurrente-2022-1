APPNAME=$(shell basename $(shell pwd))
ALLFILES=./src/*.* ./src/*/*.*
EXEFILE=bin/$(APPNAME)

LINTFILTERS=$\
	-build/c++17,$\
	-build/header_guard,$\
	-build/include_subdir,$\
	-readability/casting,$\
	-runtime/references,$\
	-whitespace

.PHONY: memcheck
memcheck:
	valgrind --leak-check=full $(EXEFILE)

.PHONY: helgrind
helgrind:
	valgrind --tool=helgrind --quiet $(EXEFILE)

.PHONY: lint
lint:
	cpplint --filter=$(LINTFILTERS) $(ALLFILES)
