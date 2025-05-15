//----------------------------------------------------------------------------------
// Name:        xnode_test.cpp
// Purpose:     Unit tests for xnode library
// Author:      Piotr Likus
// Created:     01/09/2015
// Last change: 12/09/2015
// License:     BSD
//----------------------------------------------------------------------------------

#include "xnode.h"
#include <iostream>

// for std::accumulate
#include <numeric>

// for std::sort
#include <algorithm>

#include "cunit.h"
#include "xnode_long_double.h"

using namespace std;

void TestDefCntr() {
	xnode value;
	Assert(value.is_null());
}


void TestCopyConstr() {
	xnode val1;
	const int testv = 123;
	val1.set_as(testv);
	xnode val2(val1);
	xnode val3 = val1;

	Assert(val1.get_as<int>() == testv);
	Assert(val2.get_as<int>() == testv);
	Assert(val3.get_as<int>() == testv);
}

void TestAssignOper() {
	xnode val1;
	const int testv = 123;
	val1.set_as(testv);
	xnode val2;

	val2 = val1;

	Assert(val1.get_as<int>() == testv);
	Assert(val2.get_as<int>() == testv);
}

void TestDestructor() {
	struct TestStr {
		int &counterCnt_;

		TestStr(const TestStr &src) : counterCnt_(src.counterCnt_) {}
		TestStr(int &counterCnt) :counterCnt_(counterCnt) { }
		TestStr &operator=(const TestStr &src) { counterCnt_ = src.counterCnt_;  return *this; }

		~TestStr() {
			counterCnt_--;
		}
	};

	int a = 0;
	TestStr b(a);

	xnode svalue;
	svalue.set_as(b);
	Assert(!svalue.is_null());

	a = 3;
	svalue.reset();
	Assert(svalue.is_null());
	Assert(a == 2);
}

void TestSetAsInt() {
	xnode ivalue;
	ivalue.set_as(5);
	Assert(ivalue.type() == typeid(int));
}

void TestGetAsInt() {
	xnode ivalue;
	ivalue.set_as(5);
	Assert(ivalue.get_as<int>() == 5);
}

void TestTypeChangeOnSet() {
	xnode value;
	value.set_as(5);
	Assert(value.type() == typeid(int));
	value.set_as(false);
	Assert(value.type() == typeid(bool));
}

void TestReset() {
	xnode ivalue;
	ivalue.set_as(5);
	Assert(ivalue.type() == typeid(int));
	ivalue.reset();
	Assert(ivalue.is_null());
}

void TestSetAsString() {
	xnode value;
	std::string s("test");
	value.set_as(s);
	Assert(value.get_as<std::string>() == s);
}

void TestConvIntToLong() {
	int a = 5;
	xnode ivalue;
	ivalue.set_as(a);
	Assert(ivalue.type() == typeid(int));
	Assert(ivalue.get_as<long>() == a);
}

void TestConvIntToString() {
	int a = 5;
	xnode ivalue;
	ivalue.set_as(a);
	Assert(ivalue.type() == typeid(int));
	Assert(ivalue.get_as<std::string>() == to_string(a));
}

void TestConvStringToInt() {
	xnode ivalue;
	std::string s("123");
	ivalue.set_as(s);
	Assert(ivalue.type() == typeid(std::string));
	Assert(ivalue.get_as<int>() == 123);
}

void TestSetValueInt() {
	xnode ivalue = xnode::value_of(12);
	Assert(ivalue.type() == typeid(int));
	Assert(ivalue.is<int>());

	long b = 123;
	ivalue.set_value(b);
	Assert(ivalue.is<int>());
	Assert(ivalue.get_as<int>() == 123);
}

void TestSetValueStr() {
	std::string s("test");
	xnode value = xnode::value_of(s);
	Assert(value.type() == typeid(std::string));
	Assert(value.is<std::string>());

	long b = 123;
	value.set_value(b);
	Assert(value.is<std::string>());
	Assert(value.get_as<int>() == 123);
	Assert(value.get_as<std::string>() == "123");
}

void TestCharPtr() {
	std::string s("test");

	xnode value;

	const char *cptr = s.c_str();
	value.set_as(cptr);
	Assert(value.type() == typeid(const char *), "typeid");
	Assert(value.get_as<const char *>() == cptr, "text pointer");
}

void TestCharLiteral() {
	xnode value = xnode::value_of("test");

	Assert(value.is<std::string>(), "typeid");
	Assert(value.get_as<std::string>() == "test", "get as string");
}

void TestStringLiteralSetAs() {
    // Test setting string literals directly with set_as
    xnode value;
    
    // Test with direct char array (C-style string literal)
    value.set_as("Direct String Literal");
    Assert(value.is<std::string>(), "Direct string literal should be stored as std::string");
    Assert(value.get_as<std::string>() == "Direct String Literal", "Direct string literal should match expected value");
    
    // Test empty char array
    value.set_as("");
    Assert(value.is<std::string>(), "Empty direct string literal should be stored as std::string");
    Assert(value.get_as<std::string>().empty(), "Empty direct string literal should result in empty string");
    
    // Test with special characters in char array
    value.set_as("Special\tChars\nTest");
    Assert(value.is<std::string>(), "Direct string literal with special chars should be stored as std::string");
    Assert(value.get_as<std::string>() == "Special\tChars\nTest", "Special chars should be preserved");
    
    // Test with numeric char array
    value.set_as("98765");
    Assert(value.is<std::string>(), "Numeric char array should be stored as std::string");
    Assert(value.get_as<int>() == 98765, "Numeric char array should convert to correct integer");
}

void TestStringLiteralValueOf() {
    // Test creation with value_of for string literals
    
    // Basic string literal
    xnode value1 = xnode::value_of("Test String");
    Assert(value1.is<std::string>(), "String literal should be stored as std::string");
    Assert(value1.get_as<std::string>() == "Test String", "String should match the literal");
    
    // Longer string literal with special characters
    xnode value2 = xnode::value_of("Line 1\nLine 2\tTabbed");
    Assert(value2.is<std::string>(), "String with escape chars should be stored as std::string");
    Assert(value2.get_as<std::string>() == "Line 1\nLine 2\tTabbed", "String should preserve escape chars");
    
    // Unicode characters in string literal
    xnode value3 = xnode::value_of("Unicode: \u00A9 \u2022 \u00AE");
    Assert(value3.is<std::string>(), "Unicode string should be stored as std::string");
}

void TestStringLiteralConversions() {
    // Test conversions between string literals and other types
    
    // String literal to numeric types
    xnode value1 = xnode::value_of("42");
    Assert(value1.is<std::string>(), "Numeric string should be stored as std::string");
    Assert(value1.is_convertable_to<int>(), "Numeric string should be convertible to int");
    Assert(value1.get_as<int>() == 42, "String should convert to correct integer");
    Assert(value1.get_as<double>() == 42.0, "String should convert to correct double");
    
    // String literal with floating point
    xnode value2 = xnode::value_of("3.14159");
    Assert(value2.is<std::string>(), "Float string should be stored as std::string");
    Assert(value2.is_convertable_to<double>(), "Float string should be convertible to double");
    Assert(value2.is_convertable_to<float>(), "Float string should be convertible to float");
    Assert(abs(value2.get_as<float>() - 3.14159f) < 0.0001f, "String should convert to correct float");
    
    // String literal to boolean
    xnode value3 = xnode::value_of("true");
    Assert(value3.is<std::string>(), "Boolean string should be stored as std::string");
    Assert(value3.get_as<bool>(), "String 'true' should convert to boolean true");
    
    xnode value4 = xnode::value_of("1");
    Assert(value4.is<std::string>(), "Numeric string should be stored as std::string");
    Assert(value4.get_as<bool>(), "String '1' should convert to boolean true");
    
    // Non-convertible string literals
    xnode value5 = xnode::value_of("not a number");
    Assert(value5.is<std::string>(), "Text string should be stored as std::string");
    AssertFalse(value5.is_convertable_to<int>(), "Text string should not be convertible to int");
    
    // Test exception throwing with a lambda
    try {
        value5.get_as<int>();
        Assert(false, "Should have thrown exception for non-convertible string");
    } catch (...) {
        // Expected exception
    }
}

void TestStringLiteralComparisons() {
    // Test comparison operations with string literals
    
    // Equality comparisons
    xnode value1 = xnode::value_of("test");
    xnode value2 = xnode::value_of("test");
    xnode value3 = xnode::value_of("different");
    
    Assert(value1 == value2, "Identical string literals should be equal");
    Assert(value1 != value3, "Different string literals should not be equal");
    
    // Direct comparison with string literals
    Assert(value1.get_as<std::string>() == "test", "String should equal literal");
    AssertFalse(value1.get_as<std::string>() == "different", "String should not equal different literal");
    
    // Case sensitivity
    xnode value4 = xnode::value_of("TEST");
    AssertFalse(value1 == value4, "String comparison should be case sensitive");
    
    // Length comparison
    xnode value5 = xnode::value_of("test_longer");
    Assert(value1 != value5, "Different length strings should not be equal");
}

void TestStringLiteralWithRawPointers() {
    // Test interaction between string literals and char pointers
    
    // Create from literal vs pointer
    const char* cstr = "test string";
    xnode value1 = xnode::value_of(std::string("test string"));
    xnode value2 = xnode::value_of(std::string(cstr));
    
    Assert(value1.get_as<std::string>() == value2.get_as<std::string>(), 
           "String from literal should equal string from pointer");
    Assert(value1.is<std::string>(), "String from literal should be std::string");
    Assert(value2.is<std::string>(), "String from pointer should be std::string");
    
    // Raw char array
    char array[] = "hello world";
    xnode value3 = xnode::value_of(std::string(array));
    Assert(value3.is<std::string>(), "String from array should be std::string");
    Assert(value3.get_as<std::string>() == "hello world", "String should equal original array");
    
    // Modifying the original should not affect xnode value
    array[0] = 'H';
    Assert(value3.get_as<std::string>() == "hello world", "Modifying source array should not affect xnode");
}

void TestAnyScalar() {
	// assuming there is no matching caster & xnode_type_code for this type
	typedef signed char schar;

	xnode svalue;

	schar a, b;
	a = 2;
	b = 3;

	svalue.set_as(a);
    Assert(svalue.get_type_code() == xnode_cast_policy_type_codes<xnode_def_value_policy>::def_code, "type code = default");
	b = svalue.get_as<schar>();
	Assert(a == b, "read value same as written");
}

void TestAnyStruct() {
	struct TestStr {
		int a;
		int b;
	};

	TestStr c, d;
	c.a = 12;
	c.b = 21;

	xnode svalue;
	svalue.set_as(c);

	d = svalue.get_as<TestStr>();
	Assert(c.a == d.a);
	Assert(c.b == d.b);
}


void TestRelease() {
	struct TestStr {
		int a;
		int b;
	};

	TestStr c;
	c.a = 12;
	c.b = 21;

	xnode svalue;
	svalue.set_as(c);

	Assert(!svalue.is_null());
	delete svalue.release<TestStr>();
	Assert(svalue.is_null());

}

void TestReleaseNonOwned() {
	xnode svalue;

	char test[] = "Test";
	svalue.set_as(&test[0]);
	Assert(svalue.release<char *>() == nullptr);

	svalue.set_as(5);
	Assert(svalue.release<int>() == nullptr);
}

void TestHoldObj() {
	struct TestStr {
		int a;
		int b;
	};

	std::unique_ptr<TestStr> holder(new TestStr());
	holder->a = 12;
	holder->b = 21;

	xnode value;
	TestStr *ptr = holder.get();
	value.hold(holder.release());

	Assert(value.get_ptr<TestStr>() == ptr);
	Assert(value.get_ptr<TestStr>()->a == 12);
	Assert(value.get_ptr<TestStr>()->b == 21);
	Assert(value.is<TestStr>());
}

void TestHoldInt() {

	std::unique_ptr<int> holder(new int);
	*holder = 12;

	xnode value;
	value.hold(holder.release());

	Assert(value.get_as<int>() == 12);
	Assert(value.is<int>());
}

void TestMove() {
	std::string s("test");
	xnode value = std::move(xnode::value_of(s));
	Assert(value.is<std::string>());
	Assert(value.get_as<std::string>() == "test");
}

void TestIs() {
	xnode value;
	value.set_as(123);
	Assert(value.is<int>());
}

void TestValueOf1Type() {
	xnode value = xnode::value_of(123);
	Assert(value.get_as<int>() == 123);
	Assert(value.is<int>());
}

void TestValueOf2Types() {
	xnode value = xnode::value_of<long>(123);
	Assert(value.is<long>());
	Assert(value.get_as<long>() == 123);
}

void TestEquals() {
	xnode value1 = xnode::value_of(123);
	xnode value2 = xnode::value_of(123);
	xnode value3 = xnode::value_of(3);
	xnode value4;

	Assert(value1 == value2, "check 1");
	Assert(value1 != value3, "check 2");
	Assert(value1 != value4, "check 3");
}

void TestLess() {
	xnode value1 = xnode::value_of(123);
	xnode value2 = xnode::value_of(200);
	xnode value3 = xnode::value_of(3);
	xnode value4;

	Assert(value1 < value2, "check 1");
	Assert(value3 < value2, "check 2");
	Assert(value3 < value1, "check 3");
}

void TestFloatCast() {
    xnode a, b, c;
    a = xnode::value_of(12.1f);
    b = xnode::value_of(3.0);
    Assert(a.is<float>(), "check 1");
    Assert(b.is<double>(), "check 2");
    Assert(a.get_as<int>() == 12, "check 3");

    a.set_as<float>(a.get_as<float>() + b.get_as<float>());
    Assert(a.get_as<int>() == 15, "check 4");
}

// example of polymorphism
// read object as (void *)
// then cast via static cast to base class
// then check if it is a derived class using dynamic_cast
void TestDynamicCastVptr() {

    class BaseNode {
    public:
        virtual ~BaseNode() {}
        virtual int getValue() { return 13; }
    };

    class MyNode : public BaseNode {
    public:
        virtual int getValue() { return 10; }
    };

    class OtherNode : public BaseNode {
    public:
        virtual int getValue() { return 111; }
    };

    std::unique_ptr<MyNode> value(new MyNode());

    xnode wrap;
    wrap.hold(value.release());

    Assert(wrap.is<MyNode>(), "check is 1");
    Assert(wrap.get_ptr<MyNode>()->getValue() == 10, "check value 1");

    void *vptr = wrap.get_vptr();
    Assert(vptr != nullptr, "check ptr 1");

    BaseNode *basePtr = static_cast<BaseNode *>(vptr);
    Assert(basePtr->getValue() == 10, "check value 2");

    MyNode *myPtr = dynamic_cast<MyNode *>(basePtr);
    Assert(myPtr != nullptr, "check dynamic cast 1");

    OtherNode *othPtr = dynamic_cast<OtherNode *>(basePtr);
    Assert(othPtr == nullptr, "check dynamic cast 2");
}

void TestRef() {
    xnode intNode = xnode::value_of(15);
    Assert(intNode.get_as<int>() == 15, "check 1");
    Assert(intNode.get_ref<int>() == 15, "check 2");
    intNode.get_ref<int>() = 100;
    Assert(intNode.get_as<int>() == 100, "check 3");
}

struct callGetRefNull {
    xnode &value_;
    callGetRefNull(xnode &value): value_(value) {
    }

    void operator()() {
        value_.get_ref<xnode_null_value>();
    }
};

void TestRefThrows() {
    xnode value;
    AssertThrows(callGetRefNull(value), "check 1");
    value = xnode::value_of(10);
    Assert(value.get_as<int>() == 10, "check 2");
}

void TestIsConvertable() {
    xnode value = xnode::value_of(3.14f);
    AssertTrue(value.is<float>());
    AssertTrue(value.is_convertable_to<double>(), "is_convertable to double");
    AssertFalse(value.is_convertable_to<void *>(), "is_convertable to void *");
}

struct readByDouble {
    xnode &value_;
    readByDouble(xnode &value): value_(value) {
    }

    void operator()() {
        double d = value_.get_as<double>();
        d += 1.1;
        value_.set_as(d);
    }
};

struct readByDoubleDef {
    xnode &value_;
    double def_value_;

    readByDoubleDef(xnode &value, double def_value): value_(value), def_value_(def_value) {
    }

    void operator()() {
        double d = value_.get_as_def<double>(def_value_);
        d += 1.1;
        d += value_.get_ref_def(def_value_);
        value_.set_as(d);
    }
};

void TestWrongParseThrows() {
    xnode value = xnode::value_of("@!@%!");
    AssertFalse(value.is_convertable_to<double>());
    AssertThrows(readByDouble(value), "check 1");
}

void TestCorrectParse() {
    xnode value = xnode::value_of("1234");
    AssertTrue(value.is_convertable_to<double>());
    AssertNoThrow(readByDouble(value), "check 1");
}

void TestSafeParse() {
    xnode value = xnode::value_of("@&^&!@");
    AssertFalse(value.is_convertable_to<double>());
    AssertNoThrow(readByDoubleDef(value, 2.71), "check 1");
}

void TestLongDoubleNotConvertable() {
    long double d = 12.14;
    xnode value = xnode::value_of(d);
    Assert(value.is<long double>());
    AssertThrows(readByDouble(value));
}

void TestLongDoubleWithPolicyConstruct() {
    typedef basic_xnode<xnode_ld_value_policy> xnode_ld;
    long double d = 12.14;
    xnode_ld value = xnode_ld::value_of(d);
    Assert(value.is<long double>());
    Assert(value.get_as<long double>() > 0.0);
}

void TestLongDoubleWithPolicySet() {
    typedef basic_xnode<xnode_ld_value_policy> xnode_ld;
    long double d = 12.14;
    xnode_ld value;
	value.set_as(d);
    Assert(value.is<long double>());
    Assert(value.get_as<long double>() > 0.0);
}

void TestLongDoubleCastWithPolicyConstruct() {
    typedef basic_xnode<xnode_ld_value_policy> xnode_ld;
    long double d = 12.14;
    xnode_ld value = xnode_ld::value_of(d);
    Assert(value.is<long double>());
    Assert(value.get_as<long double>() > 0.0);
    Assert(value.get_as<double>() > 0.0);
}

void TestLongDoubleCastWithPolicySet() {
    typedef basic_xnode<xnode_ld_value_policy> xnode_ld;
    long double d = 12.14;
    xnode_ld value;
	value.set_as(d);
    Assert(value.is<long double>());
    Assert(value.get_as<long double>() > 0.0);
    Assert(value.get_as<double>() > 0.0);
}

void TestWrongCastThrows() {
    void *vptr = nullptr;
    xnode value = xnode::value_of(vptr);

    AssertFalse(value.is_null(), "is_null");
    AssertTrue(value.is<void *>(), "is void*");
    AssertFalse(value.is_convertable_to<double>(), "is_convertable");
    AssertThrows(readByDouble(value), "read");
}

void TestSafeCastNoThrow() {
    void *vptr = nullptr;
    xnode value = xnode::value_of(vptr);

    AssertFalse(value.is_null(), "is_null");
    AssertTrue(value.is<void *>(), "is void*");
    AssertFalse(value.is_convertable_to<double>());
    AssertNoThrow(readByDoubleDef(value, 13.1));
}

int xnode_test() {
	TEST_PROLOG();
	TEST_FUNC(DefCntr);
	TEST_FUNC(CopyConstr);
	TEST_FUNC(AssignOper);
	TEST_FUNC(Destructor);
	TEST_FUNC(SetAsInt);
	TEST_FUNC(GetAsInt);
	TEST_FUNC(TypeChangeOnSet);
	TEST_FUNC(Reset);
	TEST_FUNC(SetAsString);
	TEST_FUNC(SetValueInt);
	TEST_FUNC(SetValueStr);
	TEST_FUNC(ConvIntToLong);
	TEST_FUNC(ConvIntToString);
	TEST_FUNC(ConvStringToInt);
	TEST_FUNC(CharPtr);
    TEST_FUNC(CharLiteral);
    TEST_FUNC(StringLiteralSetAs);
    TEST_FUNC(StringLiteralValueOf);
    TEST_FUNC(StringLiteralConversions);
    TEST_FUNC(StringLiteralComparisons);
    TEST_FUNC(StringLiteralWithRawPointers);
	TEST_FUNC(AnyScalar);
	TEST_FUNC(AnyStruct);
	TEST_FUNC(Release);
	TEST_FUNC(ReleaseNonOwned);
	TEST_FUNC(HoldObj);
	TEST_FUNC(HoldInt);
	TEST_FUNC(Move);
	TEST_FUNC(Is);
	TEST_FUNC(ValueOf1Type);
	TEST_FUNC(ValueOf2Types);
	TEST_FUNC(Equals);
	TEST_FUNC(Less);
    TEST_FUNC(FloatCast);
    TEST_FUNC(DynamicCastVptr);
    TEST_FUNC(Ref);
    TEST_FUNC(RefThrows);
    TEST_FUNC(IsConvertable);
    TEST_FUNC(WrongParseThrows);
    TEST_FUNC(CorrectParse);
    TEST_FUNC(SafeParse);
    TEST_FUNC(LongDoubleNotConvertable);
    TEST_FUNC(LongDoubleWithPolicyConstruct);
    TEST_FUNC(LongDoubleWithPolicySet);
    TEST_FUNC(LongDoubleCastWithPolicySet);
    TEST_FUNC(LongDoubleCastWithPolicyConstruct);
    TEST_FUNC(WrongCastThrows);
    TEST_FUNC(SafeCastNoThrow);
	TEST_EPILOG();
}

int main()
{
	return xnode_test();
}

