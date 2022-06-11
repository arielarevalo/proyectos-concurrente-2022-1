APPNAME=$(shell basename $(shell pwd))
HEADERS=$(wildcard src/include/*.h)
SOURCES=$(wildcard src/*.c)
EXEFILE=bin/$(APPNAME)

LINTFILTERS=$\
	-build/c++11,$\
	-build/header_guard,$\
	-build/include_subdir,$\
	-readability/casting,$\
	-runtime/references

.PHONY: memcheck
memcheck:
	valgrind --leak-check=full $(EXEFILE)

.PHONY: helgrind
helgrind:
	valgrind --tool=helgrind --quiet $(EXEFILE)

.PHONY: lint
lint:
	cpplint --filter=$(LINTFILTERS) $(HEADERS) $(SOURCES)
