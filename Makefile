CC = cc
AR = ar
CFLAGS = -Wall -Werror
INCLUDES = -Isrc/testrunner.h

.PHONY: all clean test

all: target/lib/ctest.a target/bin/test

target/lib/ctest.a: src/testrunner.o
	@mkdir -p target/lib
	@$(AR) rcs $@ $^

src/testrunner.o: src/testrunner.c src/testrunner.h
	@$(CC) $(CFLAGS) -c -o $@ $<

target/bin/test: test/test.o target/lib/ctest.a
	@mkdir -p target/bin
	@$(CC) -o $@ $^

test/test.o: test/test.c src/testrunner.h
	@$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@rm -rfv src/*.o test/*.o target

test: target/bin/test
	@$<
	

