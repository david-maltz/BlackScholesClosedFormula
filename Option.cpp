// Objective: Implement the Option class as an ABC

// Include header files
#include "Option.hpp"
#include <iostream>
#include <boost/math/distributions/normal.hpp>


// The default constructor
// Options will always start off as calls
Option::Option() : option_type("Call") {}


// Destructor
Option::~Option() {}


// Copy constructor
Option::Option(const Option& source) : option_type(source.option_type) {}


// Assignment operator
Option& Option::operator = (const Option& source) {
	if (this == &source) {
		return *this;
	}

	option_type = source.option_type;

	return *this;
}


// Toggle method
// Converts call from put and put from call
// Ensures that option_type can only be a call or a put
void Option::Toggle() {

	if (option_type == "Call") {
		option_type = "Put";
	}

	else {
		option_type = "Call";
	}
}


// Accessor method for the option type
std::string Option::OptionType() const {
	return option_type;
}

