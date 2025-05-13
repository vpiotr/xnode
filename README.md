# Introduction
xnode is a lightweight, header-only C++ library for dynamically typed values with automatic conversion between types.

[![CI Build Status](https://github.com/vpiotr/xnode/actions/workflows/ci.yml/badge.svg)](https://github.com/vpiotr/xnode/actions/workflows/ci.yml)

# License
BSD

# Project home
https://github.com/vpiotr/xnode

# Purpose
Support for dynamic typing in C++.

# Features
* dynamically typed values
* type checking
* type conversion
* STL compatibility
* header-only library
* template based design
* extensible for custom type conversions
* property lists and vector containers for advanced data structures
* no external dependencies beyond C++ STL

# Benefits
* dynamic typing

		xnode value;
        Assert(value.is_null()); 
        
		value.set_as(5);
		Assert(value.type() == typeid(int));

		value.set_as(false);
		Assert(value.is<bool>());

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
	
			const xobject &list = font.get_ref<xobject>();
	
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

* store pointers (non-owned blocks of memory)

		void TestReleaseNonOwned() {
			xnode svalue;
		
			char test[] = "Test";
			svalue.set_as(&test[0]);
			Assert(svalue.release<char *>() == nullptr);
		
			svalue.set_as(5);
			Assert(svalue.release<int>() == nullptr);
		}


# Conversion
Example conversion matrix defined in xnode_builtins.h specify conversion between types:

* bool
* float, double
* char, short, int, long, long long
* unsigned: char, short, int, long, long long
* std::string

Conversion between types can be user-defined (see xnode_long_double.h & TestLongDoubleWithPolicy).
		
# Supported environments
* compiler: any compiler with C++11 support 
* tested with:
  * VS 2015 C++ compiler 
  * GCC 4.8.1 (under MinGW)
  * Recent compilers (GCC, Clang)

# External dependencies
* just C++ standard library, including std::string
        
# Other similar efforts
* boost::any
* boost::variant

# Building
xnode is a header-only library, so you can just include the headers in your project.

## Using CMake (Recommended)
xnode uses modern CMake for its build system. To build and install:

```bash
# Create build directory
mkdir build
cd build

# Configure (customize your options as needed)
cmake ..

# Build the tests
cmake --build .

# Run the tests
ctest

# Install the library (may require elevated privileges)
cmake --install .
```

### CMake Options
- `XNODE_BUILD_TESTS` - Build tests (ON by default)

## Using the library in your CMake project
After installing, you can use the library in your own CMake project:

```cmake
find_package(xnode REQUIRED)
target_link_libraries(your_target PRIVATE xnode::xnode)
```

## Manual Integration
For simple projects, you can just copy the headers from the `include` directory into your project.

# Contact information
* [LinkedIn](http://pl.linkedin.com/pub/piotr-likus/2/307/7b9/)

# Documentation
To build documentation, use Code::Blocks 13 and command:

    DoxyBlocks \ Extract documentation

Output will be generated into: "./help/html/" directory in html format.

# Release History
See the [CHANGELOG](doc/CHANGELOG).

