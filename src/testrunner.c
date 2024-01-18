#include <stdio.h>
#include <stdlib.h>
#include "testrunner.h"

const char *terrdetails = 0;

test test_new(const char *tname, testfunc fn) {
	test t;
	t.tname = tname;
	t.fn = fn;
	return t;
}

testrunner testrunner_new() {
	testrunner t;
	t.ntests = 0;
	t.alloc_size = 0;
	t.tests = 0;
	return t;
}

// returns <0 on error, 0 on success
static int alloc_chk(testrunner *tr) {
	if (tr->alloc_size == 0) {
		unsigned long alloc = sizeof(test);
		test *tests = malloc(alloc);
		if (!tests) {
			//perror("testrunner: alloc failure");
			return -1;
		}
		tr->alloc_size = alloc;
		tr->tests = tests;
	}
	else if (tr->alloc_size <= (tr->ntests * sizeof(test))) {
		unsigned long alloc = tr->alloc_size * 2;
		test *tests = realloc(tr->tests, alloc);
		if (!tests) {
			//perror("testrunner: alloc failure");
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
	for (int i = 0, status = PASS; i < trunner->ntests && status != FATAL; ++i) {
		test *rtest = &trunner->tests[i];
		printf("RUNNING: %s", rtest->tname);
		fflush(stdout);
		tstatus s = rtest->fn();
		if (s != PASS) {
			status = s;
			++r.nfailures;
		} else ++r.npasses;
		printf("\r\33[2K%s: %s\n", rtest->tname, tstatustostr(s));
		if (s && terrdetails)
			printf("\t%s failure details: %s\n", rtest->tname, terrdetails);
		terrdetails = 0;
	}
	printf ("TEST REPORT: [ PASS: %d, FAIL: %d ]\n", r.npasses, r.nfailures);
	return r;
}

const char *tstatustostr(tstatus s) {
	// no default case since the compiler should
	// warn when the enum gets modified
	switch (s) {
	case PASS: return "PASS"; break;
	case FAIL: return "FAIL"; break;
	case FATAL: return "FATAL"; break;
	}
}