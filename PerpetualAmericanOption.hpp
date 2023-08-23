// Objective: Create the PerpetualAmericanOption derived class

// Ensure no errors if the header file is used twice
#ifndef PERPETUALAMERICANOPTION_HPP
#define PERPETUALAMERICANOPTION_HPP

// Include header files
#include "Option.hpp"
#include <iostream>
#include <vector>
using namespace std;
#include <boost/math/distributions/normal.hpp>
#include "boost/tuple/tuple.hpp"
#include "boost/tuple/tuple_io.hpp"
using boost::tuple;



// Create the PerpetualAmericanOption class as a derived class from Option
class PerpetualAmericanOption : public Option {
private:

	// The private attributes will be the strike price, the risk free rate, the cost of carry rate, the volatility, and the dividend rate

	// Strike price
	double K;

	// Risk free rate;
	double r;

	// Cost of carry rate;
	double b;

	// Volatility
	double sig;

	// Dividend rate
	double q;

public:

	// Default constructor
	PerpetualAmericanOption();

	// Parameter constructor
	PerpetualAmericanOption(double K, double r, double b, double sig, double q);

	// Destructor
	virtual ~PerpetualAmericanOption();

	// Copy constructor
	PerpetualAmericanOption(const PerpetualAmericanOption& source);

	// Assignment operator
	PerpetualAmericanOption& operator = (const PerpetualAmericanOption& source);

	// Modifiers for the private attributes
	void StrikePrice(const double& K);
	void RiskFreeRate(const double& r);
	void CostOfCarry(const double& b);
	void Volatility(const double& sig);
	void Dividend(const double& q);

	// Accessors for the private attributes
	double StrikePrice() const;
	double RiskFreeRate() const;
	double CostOfCarry() const;
	double Volatility() const;
	double Dividend() const;

	// Accessor for all private attributes
	std::string Description() const;

	// Price method
	double Price(const double& S) const;

	// The SpotPricePricer accepts a mesh vector as an argument and increments the spot price accordingly
	// The method computes the call and put prices of the EuropeanOption at each spot price
	boost::tuple<vector<double>, vector<double>, vector<double>> SpotPricePricer(std::vector<double> spot);

};

#endif
