// Objective: Implement the PerpetualAmericanOption class

// Include the necessary header files
#include "Option.hpp"
#include "PerpetualAmericanOption.hpp"
#include <iostream>
using namespace std;
#include <boost/math/distributions.hpp>
#include <boost/math/distributions/normal.hpp>
#include "boost/tuple/tuple.hpp"
#include "boost/tuple/tuple_io.hpp"
using boost::tuple;
using namespace boost::math;
#include <cmath>
#include <vector>



// Default constructor
PerpetualAmericanOption::PerpetualAmericanOption() : Option(), K(0), r(0), b(0), sig(0), q(0) {}


// Parameter constructor
PerpetualAmericanOption::PerpetualAmericanOption(double strike, double rate, double carry, double sigma, double div) : Option(), K(strike), r(rate), b(carry), sig(sigma), q(div) {}


// Destructor
PerpetualAmericanOption::~PerpetualAmericanOption() {}


// Copy constructor
PerpetualAmericanOption::PerpetualAmericanOption(const PerpetualAmericanOption& source) : Option(), K(source.K), r(source.r), b(source.b), sig(source.sig), q(source.q) {}


// Assignment operator
PerpetualAmericanOption& PerpetualAmericanOption::operator = (const PerpetualAmericanOption& source) {
	if (this == &source) {
		return *this;
	}

	// Call base class assignment
	Option::operator = (source);

	K = source.K;
	r = source.r;
	b = source.b;
	sig = source.sig;
	q = source.q;


	return *this;
}


// Modifiers for the private attributes

void PerpetualAmericanOption::StrikePrice(const double& newK) {
	K = newK;
}

void PerpetualAmericanOption::RiskFreeRate(const double& newr) {
	r = newr;
}

void PerpetualAmericanOption::CostOfCarry(const double& newb) {
	b = newb;
}

void PerpetualAmericanOption::Volatility(const double& newsig) {
	sig = newsig;
}

void PerpetualAmericanOption::Dividend(const double& newq) {
	q = newq;
}


// Accessors for the private attributes

double PerpetualAmericanOption::StrikePrice() const {
	return K;
}

double PerpetualAmericanOption::RiskFreeRate() const {
	return r;
}

double PerpetualAmericanOption::CostOfCarry() const {
	return b;
}

double PerpetualAmericanOption::Volatility() const {
	return sig;
}

double PerpetualAmericanOption::Dividend() const {
	return q;
}



// Accessor for all the private attributes
std::string PerpetualAmericanOption::Description() const {

	std::stringstream ss;

	ss << " " << K << " " << r << " " << b << " " << sig << " " << q << " " << Option::OptionType() << endl;

	return ss.str();
}



// Prices the PerpetualAmericanOption
double PerpetualAmericanOption::Price(const double& S) const {

	double sig2 = sig * sig;
	double fac = b / sig2 - 0.5;
	fac *= fac;


	// If the option type is a call, compute for a call
	if (Option::OptionType() == "Call") {


		double y1 = 0.5 - b / sig2 + sqrt(fac + 2.0 * r / sig2);


		if (1.0 == y1)
			return S;

		double fac2 = ((y1 - 1.0) * S) / (y1 * K);
		double c = K * pow(fac2, y1) / (y1 - 1.0);
		return c;
	}

	// Otherwise, the option type is a put and compute for a put

	double y2 = 0.5 - b / sig2 - sqrt(fac + 2.0 * r / sig2);

	if (0.0 == y2)
		return S;

	double fac2 = ((y2 - 1.0) * S) / (y2 * K);
	double p = K * pow(fac2, y2) / (1.0 - y2);

	return p;
}



// The SpotPricePricer accepts a mesh as an argument and increments the spot price accordingly
// The method computes the call and put prices of the EuropeanOption at each spot price
boost::tuple<vector<double>, vector<double>, vector<double>> PerpetualAmericanOption::SpotPricePricer(std::vector<double> spot) {

	// Initialize 2 vectors for the call and put prices
	vector<double> calls;
	vector<double> puts;

	// Loop through the total number of increments
	for (int i = 0; i < spot.size(); ++i) {

		// Push back the 2 vectors with the updated spot price and its corresponding call and put price

		// Prices the call price
		calls.push_back(this->Price(spot[i]));

		// Changes the AmericanOption into a put
		this->Toggle();

		// Prices the put price
		puts.push_back(this->Price(spot[i]));

		// Changes the AmericanOption into a call
		this->Toggle();

	}

	// Insert the 3 vectors into a tuple
	boost::tuple<vector<double>, vector<double>, vector<double>> tups(spot, calls, puts);

	// Return the tuple
	return tups;
}


