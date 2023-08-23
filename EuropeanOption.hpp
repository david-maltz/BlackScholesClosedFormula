// Objective: Create the EuropeanOption derived class

// Ensure no errors if the header file is used twice
#ifndef EUROPEANOPTION_HPP
#define EUROPEANOPTION_HPP

// Include header files
#include "Option.hpp"
#include <iostream>
#include <vector>
using namespace std;
#include <boost/math/distributions/normal.hpp>
#include "boost/tuple/tuple.hpp"
#include "boost/tuple/tuple_io.hpp"
using boost::tuple;


// Create the EuropeanOption class as a derived class from Option
class EuropeanOption : public Option {
private:

	// The private attributes will be the strike price, the expiry, the risk free rate, the cost of carry rate, the volatility, and the dividend rate

	// Strike price
	double K;

	// Expiry
	double T;

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
	EuropeanOption();

	// Parameter constructor
	EuropeanOption(double K, double T, double r, double b, double sig, double q);

	// Destructor
	virtual ~EuropeanOption();

	// Copy constructor
	EuropeanOption(const EuropeanOption& source);

	// Assignment operator
	EuropeanOption& operator = (const EuropeanOption& source);

	// Modifiers for the private attributes
	void StrikePrice(const double& K);
	void Expiry(const double& T);
	void RiskFreeRate(const double& r);
	void CostOfCarry(const double& b);
	void Volatility(const double& sig);
	void Dividend(const double& q);

	// Accessors for the private attributes
	double StrikePrice() const;
	double Expiry() const;
	double RiskFreeRate() const;
	double CostOfCarry() const;
	double Volatility() const;
	double Dividend() const;

	// Accessor for all private attributes
	std::string Description() const;

	// Price method
	double Price(const double& S) const;

	// PCP_Price method
	double PCP_Price(const double& S) const;

	// Internal Put-Call Parity confirmation
	std::string InternalConfirmPCP(const double& S) const;

	// External Put-Call Parity confirmation (checks with another European Option)
	std::string ConfirmPCP(const EuropeanOption& other, const double& S) const;

	// Method the encapsulates Group A Exercise 1.a
	void ReadBatches(const double& S) const;

	// The SpotPricePricer accepts a mesh vector as an argument and increments the spot price accordingly
	// The method computes the the call and put prices of the EuropeanOption at each spot price
	boost::tuple<vector<double>, vector<double>, vector<double>> SpotPricePricer(std::vector<double> spot);

	// The Delta method
	double Delta(const double& S) const;

	// The Gamma method
	double Gamma(const double& S) const;

	// Computes the approximation of delta
	double Delta(const double& S, const double& h) const;

	// Computes the approximation of gamma
	double Gamma(const double& S, const double& h) const;


};

#endif