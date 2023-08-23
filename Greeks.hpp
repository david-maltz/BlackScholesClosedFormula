// Objective: Create the Greek class using aggregation to compute the greeks for a European Option

// Ensure no errors if the header file is used twice
#ifndef GREEKS_HPP
#define GREEKS_HPP

// Include header files
#include "Option.hpp"
#include "EuropeanOption.hpp"
#include <iostream>
#include <vector>
using namespace std;
#include <boost/math/distributions/normal.hpp>
#include "boost/tuple/tuple.hpp"
#include "boost/tuple/tuple_io.hpp"
using boost::tuple;



// Create the Greeks class
class Greeks {
private:

	// The private attributes are delta, gamma, and a EuropeanOption object
	double delta;
	double gamma;

	// The Greeks class uses aggregation from the EuropeanOption class
	EuropeanOption EO;

public:

	// Default constructor
	Greeks();

	// Parameter constructor
	Greeks(EuropeanOption EO, double S);

	// Destructor
	virtual ~Greeks();

	// Copy constructor
	Greeks(const Greeks& source);

	// Assignment operator
	Greeks& operator = (const Greeks& source);

	// Delta method
	double Delta(const double& S) const;

	// Gamma method
	double Gamma(const double& S) const;

	// The SpotPricePricer creates a mesh using the arguments and increments the spot price accordingly
	// The method inserts each spot price into a vector while also computes for the delta and gamma of call and put prices of the EuropeanOption at each spot price
	boost::tuple<vector<double>, vector<double>, vector<double>, vector<double>> SpotPricePricer(std::vector<double> spot);

	// Delta approximation method
	double Delta(const double& S, const double& h) const;

	// Gamma approximation method
	double Gamma(const double& S, const double& h) const;

	// The SpotPricePricer method for the approximate greeks
	boost::tuple<vector<double>, vector<double>, vector<double>, vector<double>> SpotPricePricer(std::vector<double> spot, const double& k);

	// Void method called in the main() that outputs the differences between the exact and approximate greeks
	void CompareGreeksMethods(const double& S, const double& m);

	// Void method called in the main() that outputs the differences between the exact and approximate greeks when using their SpotPricePricer methods
	void CompareGreeksMethods(std::vector<double> spot, const double& m);
};



#endif
