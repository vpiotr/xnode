Introduction
================
This is readme file for xnode - variant-like data type container for dynamic-type variables in C++.

License
================
Software is published under BSD license, see LICENSE.txt for details.

Project home
================
https://github.com/vpiotr/xnode

Purpose
================
* for support of variables with dynamic type
* for on-the-fly conversion between string-int-long etc. 
* for support of dynamic tuples (as arguments or return values)
* for support of optional named parameters with default values
* for processing of dynamic structures like JSON, XML or INI files with values where type information can be missing

Features
================
* handles the following value types: null, scalar, pointer, owned object
* size of object is minimal (2x pointers inside)
* scalar types are stored without dynamic allocation
* supports storing of value for any struct, class or scalar without changing single line in library
* supports polymorphism

Benefits
================
* dynamic typing

		xnode value;
        Assert(value.is_null()); 
        
		value.set_as(5);
		Assert(value.type() == typeid(int));

		value.set_as(false);
		Assert(value.is<bool>()));

* perform on-fly transparent conversion & type checking

	    xnode ivalue;
	    std::string s("123");

		ivalue.set_as(s);
		Assert(ivalue.is<std::string>());

		cout << "int value: " << ivalue.get_as<int>() << endl;
        Assert(ivalue.is_convertable_to<int>());
	
* handle optional named parameters without builder pattern

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

* acquire ownership of & auto-delete objects

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

* store pointers (non-owned blocks of memory)

		void TestReleaseNonOwned() {
			xnode svalue;
		
			char test[] = "Test";
			svalue.set_as(&test[0]);
			Assert(svalue.release<char *>() == nullptr);
		
			svalue.set_as(5);
			Assert(svalue.release<int>() == nullptr);
		}


Conversion
======================
Example conversion matrix defined in xnode_builtins.h specify conversion between types:

* bool
* float, double
* char, short, int, long, long long
* unsigned: char, short, int, long, long long
* std::string

Conversion between types can be user-defined (see xnode_long_double.h & TestLongDoubleWithPolicy).
		
Supported environments
======================
* compiler: any compiler with C++11 support 
* tested with VS 2015 C++ compiler 

External dependencies
=====================
* just C++ standard library, including std::string
        
Other similar efforts
========================
* boost::any
* boost::variant

Building
====================
In order to build the test application just add all *.cpp files to your project 
from directory "\test" and add "\include" to include path.

Contact information
====================
* [Google+](https://plus.google.com/114326541605789029332/) 
* [LinkedIn](http://pl.linkedin.com/pub/piotr-likus/2/307/7b9/)

Documentation
====================
To build documentation, use Code::Blocks 13 and command:

    DoxyBlocks \ Extract documentation

Output will be generated into: "./help/html/" directory in html format.

Release History
====================
See the [CHANGELOG](doc/CHANGELOG).

    