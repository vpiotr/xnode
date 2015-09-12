//----------------------------------------------------------------------------------
// Name:        cunit.h
// Purpose:     Unit testing utility functions
// Author:      Piotr Likus
// Created:     01/09/2015
// Last change: 12/09/2015
// License:     BSD
//----------------------------------------------------------------------------------

#ifndef __CUNIT_H__
#define __CUNIT_H__

#include <string>
#include <stdexcept>

/// \file cunit.h
/// Unit testing utility functions

inline void Assert(bool assertion, const char *message = NULL)
{
	if (!assertion) {
		if (message != NULL)
			throw std::runtime_error(std::string("assertion = [") + std::string(message) + "]");
		else
			throw std::runtime_error("assertion failed");
	}
}

inline void Assert(bool assertion, const std::string &message)
{
	Assert(assertion, message.c_str());
}

inline void AssertFalse(bool assertion, const char *message = NULL) {
    Assert(!assertion, message);
}

inline void AssertFalse(bool assertion, const std::string &message) {
    Assert(!assertion, message);
}

inline void AssertTrue(bool assertion, const char *message = NULL) {
    Assert(assertion, message);
}

inline void AssertTrue(bool assertion, const std::string &message) {
    Assert(assertion, message);
}

template <typename Func>
inline void AssertThrows(Func assertion, const char *message = NULL)
{
	bool throwFound = false;
	try {
		assertion();
	}
	catch (...) {
		throwFound = true;
	}

	if (!throwFound) {
		if (message != NULL)
			throw std::runtime_error(std::string("assertion.throws = [") + std::string(message) + "]");
		else
			throw std::runtime_error("assertion.throws failed");
	}
}

template <typename Func>
inline void AssertNoThrow(Func assertion, const char *message = NULL)
{
	bool throwFound = false;
	try {
		assertion();
	}
	catch (...) {
		throwFound = true;
	}

	if (throwFound) {
		if (message != NULL)
			throw std::runtime_error(std::string("assertion.throws = [") + std::string(message) + "]");
		else
			throw std::runtime_error("assertion.throws failed");
	}
}

template <typename ObjectType, typename Func>
void AssertThrowsMethod(ObjectType obj, Func func, const char *message = NULL)
{
	bool throwFound = false;
	try {
		(obj.*func)();
	}
	catch (...) {
		throwFound = true;
	}

	if (!throwFound) {
		if (message != NULL)
			throw std::runtime_error(std::string("assertion.throws = [") + std::string(message) + "]");
		else
			throw std::runtime_error("assertion.throws failed");
	}
}

template<typename Func>
void testFunc(const char *name, Func f, bool &failed)
{
	try {
		f();
		std::cout << "OK: Test [" << std::string(name) << "] " << "succeeded\n";
	}
	catch (std::exception &e) {
		std::cout << "Error: Test [" << std::string(name) << "] failed!, error: " << e.what() << "\n";
		failed = true;
	}
}

#define TEST_FUNC(a) testFunc(#a, Test##a, errorFound)

#define TEST_PROLOG() bool errorFound = false

#define TEST_EPILOG() do { if (errorFound) { \
cout << "Failures!\n"; \
return EXIT_FAILURE; \
} \
else { \
	cout << "Success!\n"; \
	return EXIT_SUCCESS; \
} } while(0)

#endif
