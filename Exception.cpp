// Objective: Implement the classes for the Exception.hpp file

// Include necessary header files
#include "Exception.hpp"
#include <iostream>
using namespace std;
#include <sstream>


// The OutOfBoundsException constructor with an argument
// Set m_index as the "error indexed number"
OutOfBoundsException::OutOfBoundsException(const int err) : ArrayException(), m_index(err) {}


// Demonstrate polymorphism by overriding the GetMessage() method
std::string OutOfBoundsException::GetMessage() const {
	std::stringstream s;
	s << "OH NO! Bound error!" << endl;
	return s.str();
}
