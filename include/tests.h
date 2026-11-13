#ifndef _TESTS_H_
#define _TESTS_H_

#include <list.h>
#include <log.h>

int     StartTestSystem();
enum lst_err CapacityErrorVerifier();
enum lst_err SizeErrorVerifier();
enum lst_err IncorrectArgumentVerifier();
enum lst_err ListOverflowVerifier();
enum lst_err NullListPtrVerifier();
enum lst_err EmptyListVerifier();
enum lst_err ListOOMVerifier();
enum lst_err LinkErrorVerifier();
enum lst_err HashCorruptionError();
enum lst_err ListDumpTest();

#endif
