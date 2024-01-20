#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ctest.h"

static const char *erralloc = "testrunner: alloc failure";

const char *ctest_errdetails = 0;

ctest_test ctest_test_new(const char *tname, testfunc fn) {
	ctest_test t = {tname, fn};
	return t;
}

ctest_testrunner ctest_testrunner_new() {
	ctest_testrunner t = {0,0,0};
	return t;
}

// returns <0 on error, 0 on success
static int alloc_chk(ctest_testrunner *tr) {
	if (tr->alloc_size == 0) {
		// start allocating space for only one test
		unsigned long alloc = sizeof(ctest_test);
		ctest_test *tests = malloc(alloc);
		if (!tests) {
			perror(erralloc);
			return -1;
		}
		tr->alloc_size = alloc;
		tr->tests = tests;
	}
	else if (tr->alloc_size <= (tr->ntests * sizeof(ctest_test))) {
		// double allocation size on each call ro realloc
		unsigned long alloc = tr->alloc_size * 2;
		ctest_test *tests = realloc(tr->tests, alloc);
		if (!tests) {
			perror(erralloc);
			return -1;
		}
		tr->alloc_size = alloc;
		tr->tests = tests;
	}
	return 0;
}

int ctest_testrunner_addtest(ctest_testrunner *trunner, ctest_test t) {
	int err = alloc_chk(trunner);
	if (err) return err;
	trunner->ntests += 1;
	trunner->tests[trunner->ntests-1] = t;
	return 0;
}

void ctest_testrunner_drop(ctest_testrunner *trunner) {
	free(trunner->tests);
	trunner->ntests = 0;
	trunner->alloc_size = 0;
	trunner->tests = 0;
}

ctest_testreport ctest_testrunner_run(ctest_testrunner *trunner) {
	ctest_testreport r = {0,0};
	unsigned long start = time(0);
	for (
		int i = 0, status = CTEST_STATUS_PASS;
		i < trunner->ntests && status != CTEST_STATUS_FATAL;
		++i
	) {
		unsigned long tstart = time(0);
		ctest_test *rtest = &trunner->tests[i];
		ctest_status s = rtest->fn();
		unsigned long tend = time(0);
		if (s != CTEST_STATUS_PASS) {
			status = s;
			++r.nfailures;
		} else ++r.npasses;
		printf(
				"%s: %s: %lus\n",
				rtest->tname, ctest_status_tostr(s), tend - tstart
		);
		if (s && ctest_errdetails)
			printf("\t%s failure details: %s\n", rtest->tname, ctest_errdetails);
		ctest_errdetails = 0;
	}
	printf (
			"TEST REPORT: [ PASS: %d, FAIL: %d ]; %lus\n",
			r.npasses, r.nfailures, time(0)-start
	);
	return r;
}

const char *ctest_status_tostr(ctest_status s) {
	// no default case since the compiler should
	// warn when the enum gets modified
	switch (s) {
	case CTEST_STATUS_PASS: return "PASS"; break;
	case CTEST_STATUS_FAIL: return "FAIL"; break;
	case CTEST_STATUS_FATAL: return "FATAL"; break;
	}
}
