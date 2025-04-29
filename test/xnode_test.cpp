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
#include "xnode_vector.h"
#include "xnode_property_list.h"
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

	std::auto_ptr<TestStr> holder(new TestStr());
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

	std::auto_ptr<int> holder(new int);
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

template<typename T>
xnode mysum(xnode x, xnode y) {
	xnode result;
	T value = x.get_as<T>() + y.get_as<T>();
	result.set_as(value);
	return result;
}

void TestVectorSum() {
	xnode_vector v;
	v.push_back(xnode::value_of(1));
	v.push_back(xnode::value_of(8));
	v.push_back(xnode::value_of(2));
	v.push_back(xnode::value_of<long>(3));
	v.push_back(xnode::value_of<long>(4));

	xnode sum = std::accumulate(v.begin(), v.end(), xnode::value_of(0), mysum<int>);
	int isum = sum.get_as<int>();
	Assert(isum == 18);
}

void TestVectorSort() {
	xnode_vector v;
	v.push_back(xnode::value_of(1));
	v.push_back(xnode::value_of(18));
	v.push_back(xnode::value_of(128));
	v.push_back(xnode::value_of(3));
	v.push_back(xnode::value_of(23));

	std::sort(v.begin(), v.end());
	Assert(std::is_sorted(v.begin(), v.end()));
}

void TestPropertyListPutGet() {
	xnode_property_list<std::string> v;
	v.put("test1", xnode::value_of(12));
	v.put("test2", xnode::value_of(12L));
	v.put("test3", xnode::value_of(std::string("ala")));

	Assert(v.contains("test1"));
	Assert(v.get("test2").get_as<int>() == 12);
	Assert(v.get_ptr("test3") != nullptr);
	Assert(v.get_ptr("test3")->get_as<std::string>() == "ala");
}

void TestPropertyListKeysInOrder() {
	xnode_property_list<std::string> v;
	v.put("z", xnode::value_of(12));
	v.put("a", xnode::value_of(1L));
	v.put("ba", xnode::value_of(std::string("ala")));
	v.put("d", xnode::value_of(3));

	std::vector<std::string> keys = v.get_keys();
	Assert(keys[0] == "z");
	Assert(keys[1] == "a");
	Assert(keys[2] == "ba");
	Assert(keys[3] == "d");

	v.remove("a");
	keys = v.get_keys();
	Assert(keys[2] == "d");
	Assert(v.get("d").is<int>());
	Assert(v.get("d").get_as<int>() == 3);
}

void TestPropertyListValuesInOrder() {
	xnode_property_list<std::string> v;
	v.put("z", xnode::value_of(12));
	v.put("a", xnode::value_of(1L));
	v.put("ba", xnode::value_of(std::string("ala")));
	v.put("d", xnode::value_of(3));

	std::vector<xnode> values = v.get_values();
	Assert(values[0].get_as<int>() == 12);
	Assert(values[1].get_as<int>() == 1);
	Assert(values[2].get_as<std::string>() == "ala");
	Assert(values[3].get_as<int>() == 3);

	v.remove("a");
	values = v.get_values();
	Assert(values[2].get_as<int>() == 3);
}

void TestPropertyListSum() {
	xnode_property_list<std::string> v;
	v.put("z", xnode::value_of(12));
	v.put("a", xnode::value_of(1L));
	v.put("ba", xnode::value_of(7));
	v.put("d", xnode::value_of(3));

	auto keys = v.get_keys();
    Assert(keys.size() == 4);

	int sum = 0;
	for (std::string key : keys)
		sum += v.get(key).get_as<int>();

	Assert(sum == 23);
}

void TestPropertyListKeysNoCopy() {
    // faster version of get_keys

	xnode_property_list<std::string> v;
	v.put("z", xnode::value_of(12));
	v.put("a", xnode::value_of(1L));
	v.put("ba", xnode::value_of(7));
	v.put("bc", xnode::value_of(1));

    v.remove("a");

    xnode_property_list<std::string>::key_list_type keys_helper;
	auto keys = v.get_keys(keys_helper);

    Assert(keys.size() == 3);
}

void TestTreeOfNodes() {
	xnode root;
	xnode_vector v;
	xnode value;

	v.push_back(xnode::value_of(3));
	v.push_back(xnode::value_of(5));

	xnode_named_list pl;
	pl.put("z", xnode::value_of(12));
	pl.put("a", xnode::value_of(1L));

	xnode_vector v2;
	v2.push_back(xnode::value_of(1));
	v2.push_back(xnode::value_of(1L));
	v2.push_back(xnode::value_of(false));

	v.push_back(xnode::value_of(pl));
	v.push_back(xnode::value_of(v2));

	root.set_as<xnode_vector>(v);

	Assert(root.is<xnode_vector>());
	Assert(root.get_ptr<xnode_vector>()->size() == 4);
	Assert(root.get_ptr<xnode_vector>()->at(0).is<int>());
	Assert(root.get_ptr<xnode_vector>()->at(1).is<int>());
	Assert(root.get_ptr<xnode_vector>()->at(2).is<xnode_named_list>());
	Assert(root.get_ptr<xnode_vector>()->at(3).is<xnode_vector>());

	Assert(root.get_ptr<xnode_vector>()->at(2).get_ptr<xnode_named_list>()->contains("z"));
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

bool checkStructOk(const xnode &value) {
    // d,z are required int values
    bool result =
        value.is<xnode_named_list>() &&
        value.get_ptr<xnode_named_list>()->contains("z") &&
        value.get_ptr<xnode_named_list>()->contains("d") &&
        value.get_ptr<xnode_named_list>()->get("z").is<int>() &&
        value.get_ptr<xnode_named_list>()->get("d").is<int>();

    // a is optional long
    if (result)
        if (value.get_ptr<xnode_named_list>()->contains("a"))
            result = value.get_ptr<xnode_named_list>()->get("a").is<long>();

    return result;
}

std::string printInFont(const xnode &font, const std::string &text) {
	std::ostringstream out;

    const xnode_named_list &list = font.get_ref<xnode_named_list>();

    out << "text in font [";
    out << "color:" << list.get_def("color", xnode::value_of(0x00ff00)).get_as<int>();
    out << ", font_name:" << list.get_def("font_name", xnode::value_of("courier")).get_as<std::string>();
    out << ", size:" << list.get_def("size", xnode::value_of(10)).get_as<int>();
    out << ", bold:" << list.get_def("bold", xnode::value_of(false)).get_as<bool>();
    out << "] = " << text;

	return(out.str());
}

void TestOptionalNamedParams() {
	xnode_named_list v;
	v.put("color", xnode::value_of(0xff0000));
	v.put("font_name", xnode::value_of(std::string("arial")));
	v.put("size", xnode::value_of(12));

    xnode value = std::move(xnode::value_of(v));

    std::string s = printInFont(value, "test");
    Assert(s.find("bold:") != std::string::npos);
}

void TestDefNamedParams() {
	xnode_named_list v;
	v.put("z", xnode::value_of(12));
	v.put("a", xnode::value_of(1L));
	v.put("d", xnode::value_of(7));

    Assert(v.get_def("c", xnode::value_of(-1)).get_as<int>() == -1);
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

    std::auto_ptr<MyNode> value(new MyNode());

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
	std::cout << "DEBUG: TestLongDoubleCastWithPolicyConstruct step 0" << std::endl;
    typedef basic_xnode<xnode_ld_value_policy> xnode_ld;
    long double d = 12.14;
    xnode_ld value = xnode_ld::value_of(d);
	std::cout << "DEBUG: TestLongDoubleCastWithPolicyConstruct step 1" << std::endl;
    Assert(value.is<long double>());
	std::cout << "DEBUG: TestLongDoubleCastWithPolicyConstruct step 2" << std::endl;
    Assert(value.get_as<long double>() > 0.0);
	std::cout << "DEBUG: TestLongDoubleCastWithPolicyConstruct step 3" << std::endl;
    Assert(value.get_as<double>() > 0.0);
	std::cout << "DEBUG: TestLongDoubleCastWithPolicyConstruct step 4" << std::endl;
}

void TestLongDoubleCastWithPolicySet() {
	std::cout << "DEBUG: TestLongDoubleCastWithPolicySet step 0" << std::endl;
    typedef basic_xnode<xnode_ld_value_policy> xnode_ld;
    long double d = 12.14;
    xnode_ld value;
	value.set_as(d);
	std::cout << "DEBUG: TestLongDoubleCastWithPolicySet step 1" << std::endl;
    Assert(value.is<long double>());
	std::cout << "DEBUG: TestLongDoubleCastWithPolicySet step 2" << std::endl;
    Assert(value.get_as<long double>() > 0.0);
	std::cout << "DEBUG: TestLongDoubleCastWithPolicySet step 3" << std::endl;
    Assert(value.get_as<double>() > 0.0);
	std::cout << "DEBUG: TestLongDoubleCastWithPolicySet step 4" << std::endl;
}

void TestWrongCastThrows() {
    void *vptr = NULL;
    xnode value = xnode::value_of(vptr);

    AssertFalse(value.is_null(), "is_null");
    AssertTrue(value.is<void *>(), "is void*");
    AssertFalse(value.is_convertable_to<double>(), "is_convertable");
    AssertThrows(readByDouble(value), "read");
}

void TestSafeCastNoThrow() {
    void *vptr = NULL;
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
	TEST_FUNC(VectorSum);
	TEST_FUNC(VectorSort);
	TEST_FUNC(PropertyListPutGet);
	TEST_FUNC(PropertyListKeysInOrder);
	TEST_FUNC(PropertyListValuesInOrder);
	TEST_FUNC(PropertyListSum);
    TEST_FUNC(PropertyListKeysNoCopy);
	TEST_FUNC(TreeOfNodes);
    TEST_FUNC(FloatCast);
    TEST_FUNC(OptionalNamedParams);
    TEST_FUNC(DefNamedParams);
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
    TEST_FUNC(LongDoubleCastWithPolicyConstruct);
    TEST_FUNC(LongDoubleCastWithPolicySet);
    TEST_FUNC(WrongCastThrows);
    TEST_FUNC(SafeCastNoThrow);
	TEST_EPILOG();
}

int main()
{
	return xnode_test();
}

