#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "testrunner.h"

static const char *erralloc = "testrunner: alloc failure";

const char *terrdetails = 0;

test test_new(const char *tname, testfunc fn) {
	test t = {tname, fn};
	return t;
}

testrunner testrunner_new() {
	testrunner t = {0,0,0};
	return t;
}

// returns <0 on error, 0 on success
static int alloc_chk(testrunner *tr) {
	if (tr->alloc_size == 0) {
		// start allocating space for only one test
		unsigned long alloc = sizeof(test);
		test *tests = malloc(alloc);
		if (!tests) {
			perror(erralloc);
			return -1;
		}
		tr->alloc_size = alloc;
		tr->tests = tests;
	}
	else if (tr->alloc_size <= (tr->ntests * sizeof(test))) {
		// double allocation size on each call ro realloc
		unsigned long alloc = tr->alloc_size * 2;
		test *tests = realloc(tr->tests, alloc);
		if (!tests) {
			perror(erralloc);
			return -1;
		}
		tr->alloc_size = alloc;
		tr->tests = tests;
	}
	return 0;
}

int testrunner_add(testrunner *trunner, test t) {
	int err = alloc_chk(trunner);
	if (err) return err;
	trunner->ntests += 1;
	trunner->tests[trunner->ntests-1] = t;
	return 0;
}

void testrunner_drop(testrunner *trunner) {
	free(trunner->tests);
	trunner->ntests = 0;
	trunner->alloc_size = 0;
	trunner->tests = 0;
}

testreport testrunner_run(testrunner *trunner) {
	testreport r = {0,0};
	unsigned long start = time(0);
	for (
		int i = 0, status = tstatus_PASS;
		i < trunner->ntests && status != tstatus_FATAL;
		++i
	) {
		unsigned long tstart = time(0);
		test *rtest = &trunner->tests[i];
		printf("RUNNING: %s", rtest->tname);
		fflush(stdout);
		tstatus s = rtest->fn();
		unsigned long tend = time(0);
		if (s != tstatus_PASS) {
			status = s;
			++r.nfailures;
		} else ++r.npasses;
		printf(
				"\r\33[2K%s: %s: %lus\n",
				rtest->tname, tstatus_tostr(s), tend - tstart
		);
		if (s && terrdetails)
			printf("\t%s failure details: %s\n", rtest->tname, terrdetails);
		terrdetails = 0;
	}
	printf (
			"TEST REPORT: [ PASS: %d, FAIL: %d ]; %lus\n",
			r.npasses, r.nfailures, time(0)-start
	);
	return r;
}

const char *tstatus_tostr(tstatus s) {
	// no default case since the compiler should
	// warn when the enum gets modified
	switch (s) {
	case tstatus_PASS: return "PASS"; break;
	case tstatus_FAIL: return "FAIL"; break;
	case tstatus_FATAL: return "FATAL"; break;
	}
}
