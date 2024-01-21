#include "ctest.h"
#include <stdio.h>

void ctest_before_run_message(const char *tname) {
		printf("=== RUN\t%s\n", tname);
}

void ctest_control_eval(
	ctest_result tresult,
	ctest_testreport *treport,
	int *keep_running
){
	switch (tresult.status) {
	case CTEST_STATUS_PASS: ++treport->npasses; break;
	case CTEST_STATUS_FAIL: ++treport->nfailures; break;
	case CTEST_STATUS_FATAL: ++treport->nfailures; *keep_running = 0; break;
	}
}

void ctest_after_run_message(
	const char *tname,
	ctest_result tresult,
	unsigned long elapsed
) {
		printf(
				"--- %s: %s (%lus)\n",
				ctest_status_tostr(tresult.status), tname, elapsed
		);
		if (tresult.status != CTEST_STATUS_PASS && tresult.failreason)
			printf("\t%s failure details: %s\n", tname, tresult.failreason);
}

void ctest_all_tests_finished_message(
	ctest_testreport report,
	unsigned long elapsed
) {
	printf (
			"TEST REPORT: [ PASS: %d, FAIL: %d ] (%lus)\n",
			report.npasses, report.nfailures, elapsed
	);
}
