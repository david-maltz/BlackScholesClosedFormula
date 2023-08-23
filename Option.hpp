// Objective: Create the Option base class

// In case Option.hpp is used multiple times
#ifndef OPTION_HPP
#define OPTION_HPP

// Include header files
#include <iostream>
#include <boost/math/distributions/normal.hpp>
#include "boost/tuple/tuple.hpp"
#include "boost/tuple/tuple_io.hpp"



// Create the Option class
// The class will be the base class for different types of Options, such as European Options and Perpetual American Options
class Option {
private:

	// The private attribute will be the option type
	std::string option_type;

public:

	// Default constructor
	Option();

	// Destructor
	virtual ~Option();

	// Copy constructor
	Option(const Option& source);

	// Assignment operator
	Option& operator = (const Option& source);

	// Pricing method
	// Set the Price method to 0 to make Option an Abstract Base Class
	// This will allow its derived classes to have different Pricing methods
	virtual double Price(const double& S) const = 0;

	// Toggle method to switch option type
	void Toggle();

	// OptionType accessor
	std::string OptionType() const;

};



#endif
