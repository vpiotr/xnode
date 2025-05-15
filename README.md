# Introduction
xnode is a lightweight, header-only C++ library for dynamically typed values with automatic conversion between types.

[![CI Build Status](https://github.com/vpiotr/xnode/actions/workflows/ci.yml/badge.svg)](https://github.com/vpiotr/xnode/actions/workflows/ci.yml)

# License
BSD

# Project home
https://github.com/vpiotr/xnode

# Purpose
Bringing the power and flexibility of dynamic typing to modern C++, allowing you to write more expressive and adaptable code.

# Key Features
* **Zero-cost Abstractions**: Header-only with no external dependencies beyond STL
* **Seamless Type Flexibility**: Change types at runtime with automatic conversion
* **Modern C++ Design**: Template-based, STL-compatible architecture
* **Rich Data Structures**: Property lists and vector containers with insertion-order preservation
* **Extensible Type System**: Easily add support for your custom types
* **Developer-Friendly API**: Intuitive interface with fluent builder methods

# Why xnode?
* **Simplify Dynamic Data Handling**

		xnode value;        
		value.set_as(5);                 // Store as integer
		value.set_as(false);             // Change to boolean
		value.set_as("dynamic typing");  // Change to string
		
		// Type safety when you need it
		Assert(value.is<std::string>());

* **Smart Type Conversions** - Automatic, transparent, and safe

	    xnode value;
	    value.set_as("123");  // Store as string
	    
	    // Automatic conversion when needed
	    int num = value.get_as<int>();  // Converts to 123
	    
	    // Type safety built-in
	    Assert(value.is<std::string>());
	    Assert(value.is_convertable_to<int>());
	
* **Elegant Named Parameters** - Cleaner than builder pattern, more flexible than structs

		std::string printInFont(const xnode &font, const std::string &text) {
			const xobject &options = font.get_ref<xobject>();
			
			// Default values handled elegantly with get_def
			std::ostringstream out;
			out << "text in font [";
			out << "color:" << options.get_def("color", xnode::value_of(0x00ff00)).get_as<int>();
			out << ", font_name:" << options.get_def("font_name", xnode::value_of("courier")).get_as<std::string>();
			out << ", size:" << options.get_def("size", xnode::value_of(10)).get_as<int>();
			out << ", bold:" << options.get_def("bold", xnode::value_of(false)).get_as<bool>();
			out << "] = " << text;
			
			return out.str();  
		}
		
		// Create options with fluent static 'of' method:
		auto result = printInFont(
			xnode::value_of(xobject::of(
				"color", xnode::value_of(0xff0000),
				"font_name", xnode::value_of(std::string("arial")),
				"size", xnode::value_of(12),
				"bold", xnode::value_of(true)
			)), 
			"Hello World!"
		);

* **Smart Object Ownership** - Automatic memory management for dynamic objects

		// Let xnode handle object lifecycle
		void useObjectOwnership() {
			struct CustomObject {
				int data1;
				int data2;
			};
		
			// Create an object to be owned by xnode
			std::unique_ptr<CustomObject> obj(new CustomObject{42, 100});
		
			// Transfer ownership to xnode
			xnode container;
			container.hold(obj.release());
		
			// Use the object safely
			CustomObject* ptr = container.get_ptr<CustomObject>();
			std::cout << "Values: " << ptr->data1 << ", " << ptr->data2 << std::endl;
			
			// Object is automatically deleted when container goes out of scope
		}

* **Flexible Memory Models** - Store owned objects or reference existing memory

		// Work with existing memory without ownership concerns
		void workWithExistingData() {
			// External data we want to reference
			char existingData[] = "Hello xnode!";
			
			// Store a pointer without taking ownership
			xnode reference;
			reference.set_as(existingData);
			
			// Use the data through xnode
			char* data = reference.get_as<char*>();
			std::cout << data << std::endl; // Prints "Hello xnode!"
			
			// xnode won't attempt to free this memory
		}


# Type Conversion System

xnode comes with a rich set of built-in conversions for seamless interoperability:

* **Fundamental Types**: All C++ numeric types (integer and floating-point)
* **Logical Values**: bool with smart conversion from/to other types
* **Text Support**: Full std::string integration
* **Custom Types**: Easily extend with your own conversion rules

Create your own conversion paths by implementing a simple policy class (see `xnode_long_double.h` for a practical example).
		
# Compatibility

* **Standards**: C++11 and later
* **Platforms**: Cross-platform (Windows, Linux, macOS)
* **Compilers**: 
  * Modern GCC/Clang toolchains
  * Microsoft Visual Studio 2015+
  * Any compiler with good C++11 support

# Advantages Over Alternatives

* **Lighter than Boost**: Self-contained with no external dependencies
* **More Dynamic than std::any**: Built-in type conversion
* **More Flexible than std::variant**: Change types at runtime
* **More Modern than void\***: Type-safe with no casting required

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

