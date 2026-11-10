#ifndef _TESTS_H_
#define _TESTS_H_

#include <list.h>
#include <log.h>

int     StartTestSystem();
lst_err CapacityErrorVerifier();
lst_err SizeErrorVerifier();
lst_err IncorrectArgumentVerifier();
lst_err ListOverflowVerifier();
lst_err NullListPtrVerifier();
lst_err EmptyListVerifier();
lst_err ListOOMVerifier();
lst_err LinkErrorVerifier();
lst_err HashCorruptionError();
lst_err ListDumpTest();

#endif
