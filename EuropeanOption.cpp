// Objective: Implement the EuropeanOption class

// Include the necessary header files
#include "Option.hpp"
#include "EuropeanOption.hpp"
#include <boost/math/distributions.hpp>
#include <boost/math/distributions/normal.hpp>
#include "boost/tuple/tuple.hpp"
#include "boost/tuple/tuple_io.hpp"
using boost::tuple;
using namespace boost::math;
#include <cmath>
#include <vector>



// Default constructor
EuropeanOption::EuropeanOption() : Option(), K(0), T(0), r(0), b(0), sig(0), q(0) {}


// Parameter constructor
EuropeanOption::EuropeanOption(double strike, double expiry, double rate, double carry, double sigma, double div) : Option(), K(strike), T(expiry), r(rate), b(carry), sig(sigma), q(div) {}


// Destructor
EuropeanOption::~EuropeanOption() {}


// Copy constructor
EuropeanOption::EuropeanOption(const EuropeanOption& source) : Option(), K(source.K), T(source.T), r(source.r), b(source.b), sig(source.sig), q(source.q) {}


// Assignment operator
EuropeanOption& EuropeanOption::operator = (const EuropeanOption& source) {
	if (this == &source) {
		return *this;
	}

	// Call base class assignment
	Option::operator = (source);

	K = source.K;
	T = source.T;
	r = source.r;
	b = source.b;
	sig = source.sig;
	q = source.q;

	return *this;
}

// Modifiers for the private attributes

void EuropeanOption::StrikePrice(const double& newK) {
	K = newK;
}

void EuropeanOption::Expiry(const double& newT) {
	T = newT;
}

void EuropeanOption::RiskFreeRate(const double& newr) {
	r = newr;
}

void EuropeanOption::CostOfCarry(const double& newb) {
	b = newb;
}

void EuropeanOption::Volatility(const double& newsig) {
	sig = newsig;
}

void EuropeanOption::Dividend(const double& newq) {
	q = newq;
}


// Accessors for the private attributes

double EuropeanOption::StrikePrice() const {
	return K;
}

double EuropeanOption::Expiry() const {
	return T;
}

double EuropeanOption::RiskFreeRate() const {
	return r;
}

double EuropeanOption::CostOfCarry() const {
	return b;
}

double EuropeanOption::Volatility() const {
	return sig;
}

double EuropeanOption::Dividend() const {
	return q;
}



// Accessor for all the private attributes
std::string EuropeanOption::Description() const {

	std::stringstream ss;

	ss << " " << K << " " << T << " " << r << " " << b << " " << sig << " " << q << " " << Option::OptionType() << endl;

	return ss.str();
}



// Prices the EuropeanOption
double EuropeanOption::Price(const double& S) const {

	// Initialize a standard Gaussian RV
	normal_distribution<> nd(0.0, 1.0);

	// Compute d1 and d2
	double d1 = (log(S / K) + (r + (sig * sig) * 0.5) * T) / (sig * sqrt(T));
	double d2 = d1 - (sig * sqrt(T));

	// If the option type is a call, compute for a call
	if (Option::OptionType() == "Call") {
		return S * cdf(nd, d1) * exp((b - r) * T) - K * exp(-r * T) * cdf(nd, d2);
	}

	// Otherwise, the option type is a put and compute for a put
	return K * cdf(nd, -d2) * exp(-r * T) - S * cdf(nd, -d1) * exp((b - r) * T);
}



// Prices the opposite option type for a EuropeanOption using the Put-Call Parity (PCP)
double EuropeanOption::PCP_Price(const double& S) const {

	// Initialize a standard Gaussian RV
	normal_distribution<> nd(0.0, 1.0);

	// Compute d1 and d2
	double d1 = (log(S / K) + (r + (sig * sig) * 0.5) * T) / (sig * sqrt(T));
	double d2 = d1 - (sig * sqrt(T));

	// If the option type is a call, compute for a put
	if (Option::OptionType() == "Call") {

		return K * cdf(nd, -d2) * exp(-r * T) - S * cdf(nd, -d1) * exp((b - r) * T);
	}

	// Otherwise, the option type is a put and compute for a call
	return S * cdf(nd, d1) * exp((b - r) * T) - K * exp(-r * T) * cdf(nd, d2);
}



// Returns a string that confirms that the PCP is satisfied with the opposite option type
std::string EuropeanOption::InternalConfirmPCP(const double& S) const {

	// Initialize a tolerance that is close to 0
	float tol = 1e-8;

	// Check if the difference is less than the tolerance and the current option type is a call
	// Return a string of confirmation if so
	if (abs((this->Price(S) + K * exp(-r * T)) - (this->PCP_Price(S) + S * exp((b - r) * T))) < tol && Option::OptionType() == "Call") {
		return "The current option is a Call and the Put-Call Parity is preserved.";
	}

	// Check if the difference is less than the tolerance and the current option type is a put
	// Return a string of confirmation if so
	else if (abs((this->PCP_Price(S) + K * exp(-r * T)) - (this->Price(S) + S * exp((b - r) * T))) < tol && Option::OptionType() == "Put") {
		return "The current option is a Put and the Put-Call Parity is preserved.";
	}

	// The PCP does not hold in this case
	return "The Put-Call Parity is not preserved.";
}



// Returns a string that confirms that the PCP is satisfied with another EuropeanOption
std::string EuropeanOption::ConfirmPCP(const EuropeanOption& other, const double& S) const {

	// Initialize a tolerance that is close to 0
	float tol = 1e-8;

	// Check if the difference is less than the tolerance and the current option type is a call
	// Return a string of confirmation if so
	if (abs((this->Price(S) + K * exp(-r * T)) - (other.Price(S) + S * exp((b - r) * T))) < tol && Option::OptionType() == "Call") {
		return "The current option is a Call and the Put-Call Parity is preserved.";
	}

	// Check if the difference is less than the tolerance and the current option type is a put
	// Return a string of confirmation if so
	else if (abs((other.Price(S) + K * exp(-r * T)) - (this->Price(S) + S * exp((b - r) * T))) < tol && Option::OptionType() == "Put") {
		return "The current option is a Put and the Put-Call Parity is preserved.";
	}

	// The PCP does not hold in this case
	return "The Put-Call Parity is not preserved.";
}



// Method that encapsulates the objectives for Group A Exercise 1.a
// Outputs the price of the EuropeanOption's call and put prices
// Confirms that the Put-Call Parity holds using a method that takes the PCP with the object by itself and the object with its call/put counterpart
void EuropeanOption::ReadBatches(const double& S) const {

	std::cout << "The type of option is a " << this->OptionType() << " and is worth " << this->Price(S) << endl;
	std::cout << "According to the Put-Call Parity the price of the corresponding Put should be " << this->PCP_Price(S) << endl;
	std::cout << this->InternalConfirmPCP(S) << endl;


	// Initialize a put counterpart
	EuropeanOption put = *this;

	// Change the option_type from the default call to a put
	put.Toggle();

	std::cout << endl;

	std::cout << "The type of option is a " << put.OptionType() << " and is worth " << put.Price(S) << endl;
	std::cout << "According to the Put-Call Parity the price of the corresponding Call should be " << put.PCP_Price(S) << endl;
	std::cout << put.InternalConfirmPCP(S) << endl;

	std::cout << endl;

	// Confirm the PCP holds from the call and from the put
	std::cout << this->ConfirmPCP(put, S) << endl;
	std::cout << put.ConfirmPCP(*this, S) << endl;
}



// The SpotPricePricer accepts a mesh vector as an arguments and increments the spot price accordingly
// The method computes the the call and put prices of the EuropeanOption at each spot price
boost::tuple<vector<double>, vector<double>, vector<double>> EuropeanOption::SpotPricePricer(std::vector<double> spot) {

	// Initialize 2 vectors for the call and put prices
	vector<double> calls;
	vector<double> puts;


	// Loop through the total number of increments
	for (int i = 0; i < spot.size(); ++i) {

		// Push back the 2 vectors with the updated spot price and its corresponding call and put price

		// Price the call price for the EuropeanOption
		calls.push_back(this->Price(spot[i]));

		// Change the option type to a put
		this->Toggle();

		// Price the put price for the EuropeanOption
		puts.push_back(this->Price(spot[i]));

		// Change the option type to a call
		this->Toggle();

	}

	// Insert the 3 vectors into a tuple
	boost::tuple<vector<double>, vector<double>, vector<double>> tups(spot, calls, puts);

	// Return the tuple
	return tups;
}



// The Delta method
double EuropeanOption::Delta(const double& S) const {

	// Initialize a standard Gaussian RV
	normal_distribution<> nd(0.0, 1.0);

	// Compute d1
	double tmp = sig * sqrt(T);
	double d1 = (log(S / K) + (b + (sig * sig) * 0.5) * T) / tmp;


	// If the option type is a call, compute the delta for a call
	if (Option::OptionType() == "Call") {
		return exp((b - r) * T) * cdf(nd, d1);
	}

	// Otherwise, the option type is a put and compute delta for a put
	return exp((b - r) * T) * (cdf(nd, d1) - 1.0);
}



// The Gamma method
double EuropeanOption::Gamma(const double& S) const {

	// Initialize a standard Gaussian RV
	normal_distribution<> nd(0.0, 1.0);

	// Compute d1
	double tmp = sig * sqrt(T);
	double d1 = (log(S / K) + (b + (sig * sig) * 0.5) * T) / tmp;

	// Since gamma is the same for calls and puts, no need to separate between the option types
	return (pdf(nd, d1) * exp((b - r) * T)) / (S * sig * sqrt(T));
}



// Computes the approximation of delta
double EuropeanOption::Delta(const double& S, const double& h) const {
	return double(this->Price(S + h) - this->Price(S - h)) / double(2 * h);
}



// Computes the approximation of gamma
double EuropeanOption::Gamma(const double& S, const double& h) const {
	return double(this->Price(S + h) - (2 * this->Price(S)) + this->Price(S - h)) / double(h * h);
}