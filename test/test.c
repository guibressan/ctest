#include <stdio.h>
#include <unistd.h>
#include "../src/testrunner.h"

tstatus test1(void) {
	sleep(1);
	return tstatus_PASS;
}
tstatus test2(void) {
	terrdetails = "test2 err description";
	return tstatus_FAIL;
}
tstatus test3(void) {
	terrdetails = "test3 err description";
	return tstatus_FATAL;
}
tstatus test4(void) {
	terrdetails = "test4 err description";
	return tstatus_PASS;
}

int testing_1(void){
	testrunner t = testrunner_new();
	testrunner_add(&t, test_new("test 1", test1));
	testrunner_add(&t, test_new("test 2", test2));
	testrunner_add(&t, test_new("test 3", test3));
	testrunner_add(&t, test_new("test 4", test4));
	testreport r = testrunner_run(&t);
	testrunner_drop(&t);
	if (r.nfailures != 2 && r.npasses != 1) {
		fprintf(
				stderr,
				"testing_1 failed. nfailures: %d, npasses: %d",
				r.nfailures,
				r.npasses
		);
		return -1;
	}
	return 0;
}

int testing_2(void){
	testrunner t = testrunner_new();
	testrunner_add(&t, test_new("test 1", test1));
	testrunner_add(&t, test_new("test 2", test2));
	testrunner_add(&t, test_new("test 4", test4));
	testreport r = testrunner_run(&t);
	testrunner_drop(&t);
	if (r.nfailures != 1 && r.npasses != 2) {
		fprintf(
				stderr, "testing_2 failed. nfailures: %d, npasses: %d",
				r.nfailures,
				r.npasses
		);
		return -1;
	}
	return 0;
}

int main(void) {
	if (testing_1() || testing_2()) return 1;
	return 0;
}
