// Objective: Create the Exception class using polymorphism

// Ensure no errors if the header file is used twice
#ifndef Exception_HPP
#define Exception_HPP

// Include the standard input/output header for C++
#include <iostream>
using namespace std;
#include <sstream>

// Create the ArrayException class
class ArrayException {
public:

	// Make ArrayException an ABC by setting GetMessage() as a virtual method
	virtual std::string GetMessage() const = 0;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Create the OutOfBoundsException class as an inheritance of ArrayException
class OutOfBoundsException : public ArrayException {
private:

	// The private attribute is the index number of the array that causes the error
	int m_index;

public:

	// A constructor that creates an OutOfBoundsException object with the indexed number as its argument
	OutOfBoundsException(const int);

	// Override ArrayException's GetMessage() method
	std::string GetMessage() const;

};

#endif


