CC = cc
AR = ar
CFLAGS = -Wall -Werror

.PHONY: all clean test

all: target target/testrunner.o target/test

target:
	@mkdir -p target

target/testrunner.o: src/testrunner.c src/testrunner.h
	@$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@rm -rf target

target/test: target/test.o target/testrunner.o
	@$(CC) -o $@ $^

target/test.o: test/test.c src/testrunner.h
	@$(CC) $(CFLAGS) -c -o $@ $<

test: all
	@./target/test


