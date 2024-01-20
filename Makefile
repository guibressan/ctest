#!/usr/bin/env make
####################

.PHONY: all check clean

all: build
	@cmake --build build

build:
	@cmake -S . -B build

clean:
	@rm -rf build

check:
	@cmake --build build
	@./build/test/test
