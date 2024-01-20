#include <malloc/_malloc.h>
#include <stdio.h>
#include <time.h>
#include "ctest.h"

static const char *erralloc = "testrunner: alloc failure";

ctest_result ctest_pass(void) {
	ctest_result r = {0, CTEST_STATUS_PASS};
	return r;
}

ctest_result ctest_fail(const char *failreason) {
	ctest_result r = {failreason, CTEST_STATUS_FAIL};
	return r;
}

ctest_result ctest_fatal(const char *failreason) {
	ctest_result r = {failreason, CTEST_STATUS_FATAL};
	return r;
}

ctest_testrunner ctest_testrunner_new() {
	ctest_testrunner t = {0,0,0};
	return t;
}

static ctest_test ctest_new(const char *testname, testfunc fn) {
	ctest_test t;
	t.tname = testname;
	t.fn = fn;
	return t;
}

// returns 0 on error, 1 on success
static int alloc_chk(ctest_testrunner *tr) {
	if (tr->alloc_size == 0) {
		// start allocating space for only one test
		unsigned long alloc = sizeof(ctest_test);
		ctest_test *tests = malloc(alloc);
		if (!tests) {
			perror(erralloc);
			return 0;
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
			return 0;
		}
		tr->alloc_size = alloc;
		tr->tests = tests;
	}
	return 1;
}

int ctest_testrunner_addtest(ctest_testrunner *tr, const char *testname, testfunc fn) {
	ctest_test t = ctest_new(testname, fn);
	if (alloc_chk(tr) != 1) return 0;
	tr->ntests += 1;
	tr->tests[tr->ntests-1] = t;
	return 1;
}

void ctest_testrunner_drop(ctest_testrunner *tr) {
	free(tr->tests);
	tr->ntests = 0;
	tr->alloc_size = 0;
	tr->tests = 0;
}

ctest_testreport ctest_testrunner_run(ctest_testrunner *tr) {
	ctest_testreport r = {0,0};
	int keep_running = 1;
	unsigned long start = time(0);
	for (
		int i = 0, status = CTEST_STATUS_PASS;
		i < tr->ntests && keep_running;
		++i
	) {
		unsigned long tstart = time(0);
		ctest_test *rtest = &tr->tests[i];
		printf("=== RUN\t%s\n", rtest->tname);
		ctest_result s = rtest->fn();
		unsigned long tend = time(0);
		if (s.status == CTEST_STATUS_PASS) ++r.npasses;
		else {
			++r.nfailures;
			if (s.status == CTEST_STATUS_FATAL) keep_running = 0;
		}
		printf(
				"--- %s: %s (%lus)\n",
				ctest_status_tostr(s.status), rtest->tname, tend - tstart
		);
		if (s.status != CTEST_STATUS_PASS && s.failreason)
			printf("\t%s failure details: %s\n", rtest->tname, s.failreason);
	}
	printf (
			"TEST REPORT: [ PASS: %d, FAIL: %d ] (%lus)\n",
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
