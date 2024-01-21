#ifndef CTEST_UTIL_H
#define CTEST_UTIL_H

#include "ctest_types.h"
	
void ctest_before_run_message(const char *tname);

void ctest_control_eval(
	ctest_result tresult,
	ctest_testreport *treport,
	int *keep_running
);

void ctest_after_run_message(
	const char *tname,
	ctest_result tresult,
	unsigned long elapsed
);

void ctest_all_tests_finished_message(
	ctest_testreport report,
	unsigned long elapsed
);
	
#endif	/*	CTEST_UTIL_H	*/
