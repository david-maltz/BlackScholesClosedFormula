// Objective: Implement the Greeks class

// Include the necessary header files
#include "Option.hpp"
#include "EuropeanOption.hpp"
#include "Matrix.hpp"
#include "Greeks.hpp"
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
Greeks::Greeks() : delta(0), gamma(0), EO(EuropeanOption()) {}


// Parameter constructor
Greeks::Greeks(EuropeanOption EO1, double S) : delta(EO1.Delta(S)), gamma(EO1.Gamma(S)), EO(EO1) {}


// Destructor
Greeks::~Greeks() {}


// Copy constructor
Greeks::Greeks(const Greeks& source) : delta(source.delta), gamma(source.gamma), EO(source.EO) {}


// Assignment operator
Greeks& Greeks::operator = (const Greeks& source) {

	if (this == &source) {
		return *this;
	}

	delta = source.delta;
	gamma = source.gamma;
	EO = source.EO;

	return *this;
}


// The Delta method: uses the EuropeanOption Delta method
double Greeks::Delta(const double& S) const {
	return EO.Delta(S);
}


// The Gamma method: uses the EuropeanOption Gamma method
double Greeks::Gamma(const double& S) const {
	return EO.Gamma(S);
}


// The SpotPricePricer has a mesh vector as an argument
// The method computes the delta and gamma of call and put prices of the EuropeanOption at each spot price
boost::tuple<vector<double>, vector<double>, vector<double>, vector<double>> Greeks::SpotPricePricer(std::vector<double> spot) {

	// Initialize 3 vectors for the spot prices, the delta for the call, the delta for the put, and the gamma
	vector<double> delta_calls;
	vector<double> delta_puts;
	vector<double> gamma;


	// Loop through the total number of increments
	for (int i = 0; i < spot.size(); ++i) {

		// Push back the 3 vectors with the updated spot price and its corresponding call and put price

		// Price the delta of the call
		delta_calls.push_back(this->Delta(spot[i]));

		// Change EO to a put
		EO.Toggle();

		// Price the delta of the put
		delta_puts.push_back(this->Delta(spot[i]));

		// Price the gamma
		gamma.push_back(this->Gamma(spot[i]));

		// Change EO back to a call
		EO.Toggle();

	}

	// Insert the 4 vectors into a tuple
	boost::tuple<vector<double>, vector<double>, vector<double>, vector<double>> tups(spot, delta_calls, delta_puts, gamma);

	// Return the tuple
	return tups;
}



// Delta Approximation method
double Greeks::Delta(const double& S, const double& h) const {
	return EO.Delta(S, h);
}



// Gamma Approximation method
double Greeks::Gamma(const double& S, const double& h) const {
	return EO.Gamma(S, h);
}



// The SpotPricePricer has a mesh vector as an argument with the h value for the approximation
// The method computes the approximate delta and gamma of call and put prices of the EuropeanOption at each spot price
boost::tuple<vector<double>, vector<double>, vector<double>, vector<double>> Greeks::SpotPricePricer(std::vector<double> spot, const double& h) {

	// Initialize 3 vectors for the delta for the call, the delta for the put, and the gamma
	vector<double> delta_calls;
	vector<double> delta_puts;
	vector<double> gamma;


	// Loop through the total number of increments
	for (int i = 0; i < spot.size(); ++i) {

		// Push back the 3 vectors with the updated spot price and its corresponding call and put price

		// Price the delta of the call
		delta_calls.push_back(this->Delta(spot[i], h));

		// Change EO into a put
		EO.Toggle();

		// Price the delta of the put
		delta_puts.push_back(this->Delta(spot[i], h));

		// Price the gamma
		gamma.push_back(this->Gamma(spot[i], h));

		// Change EO back to a call
		EO.Toggle();

	}

	// Insert the 4 vectors into a tuple
	boost::tuple<vector<double>, vector<double>, vector<double>, vector<double>> tups(spot, delta_calls, delta_puts, gamma);

	// Return the tuple
	return tups;
}



// This encapsulated method compares the exact greeks with the approximated greeks based on the h size
void Greeks::CompareGreeksMethods(const double& S, const double& n) {

	// Initialize a double h
	double h = 0.1;

	// Initialize doubles that will hold the exact delta and gamma values and the approximate delta and gamma values
	double DC, DP, GC, GP;
	double aDC, aDP, aGC, aGP;

	// Loop through the h values n times
	for (int i = 0; i < n; ++i) {

		std::cout << "h is " << h << endl;

		// Set the values

		DC = EO.Delta(S);
		GC = EO.Gamma(S);
		aDC = EO.Delta(S, h);
		aGC = EO.Gamma(S, h);

		// Change EO to a put
		EO.Toggle();

		DP = EO.Delta(S);
		GP = EO.Gamma(S);
		aDP = EO.Delta(S, h);
		aGP = EO.Gamma(S, h);

		// Output the exact and approximate values and the differences between them

		std::cout << "Delta of call: " << DC << " Gamma of call: " << GC << endl;
		std::cout << "Approximate Delta of call: " << aDC << " Approximate Gamma of call: " << aGC << endl;
		std::cout << "The errors for the Delta calls is: " << abs(DC - aDC) << endl;
		std::cout << "The errors for the Gamma calls is: " << abs(GC - aGC) << endl;


		std::cout << endl;

		std::cout << "Delta of put: " << DP << " Gamma of put: " << GP << endl;
		std::cout << "Approximate Delta of put: " << aDP << " Approximate Gamma of put: " << aGP << endl;
		std::cout << "The errors for the Delta puts is: " << abs(DP - aDP) << endl;
		std::cout << "The errors for the Gamma puts is: " << abs(GP - aGP) << endl;


		// Change EO to a call
		EO.Toggle();

		// Multiply h by itself for the next loop
		h *= h;

		std::cout << endl;

	}
}



// This encapsulated method compares SpotPricePricer's impact on the exact greeks with the approximated greeks based on the h size
void Greeks::CompareGreeksMethods(std::vector<double> spot, const double& m) {

	// Initialize a double h
	double h = 0.1;

	// Loop through the h values m times
	for (int i = 0; i < m; ++i) {

		std::cout << "h is " << h << endl;

		// Create 2 tuples consisting of a vector of spot prices, a vector of call deltas, a vector of put deltas, and a vector of gammas: one for the exact values and one for the approximate values
		boost::tuple<vector<double>, vector<double>, vector<double>, vector<double>> tup1 = this->SpotPricePricer(spot);
		boost::tuple<vector<double>, vector<double>, vector<double>, vector<double>> tup2 = this->SpotPricePricer(spot, h);


		// Output the exact and approximate values from the SpotPricePricer and the check differences between them

		std::cout << "Exact: " << endl;
		std::cout << "Spot prices: " << '\t' << "Delta Call:" << '\t' << "Delta Put: " << '\t' << "Gamma: " << endl;

		// Loop through the tuple and output the spot prices with its corresponding call and put prices
		for (int i = 0; i < spot.size(); i++) {
			std::cout << boost::tuples::get<0>(tup1)[i] << '\t' << '\t' << boost::tuples::get<1>(tup1)[i] << '\t' << '\t' << boost::tuples::get<2>(tup1)[i] << '\t' << '\t' << boost::tuples::get<3>(tup1)[i] << '\t' << '\t' << endl;
		}

		std::cout << endl;

		std::cout << "Approximate: " << endl;
		std::cout << "Spot prices: " << '\t' << "Delta Call:" << '\t' << "Delta Put: " << '\t' << "Gamma: " << endl;
		// Loop through the tuple and output the spot prices with its corresponding call and put prices
		for (int i = 0; i < spot.size(); i++) {
			std::cout << boost::tuples::get<0>(tup2)[i] << '\t' << '\t' << boost::tuples::get<1>(tup2)[i] << '\t' << '\t' << boost::tuples::get<2>(tup2)[i] << '\t' << '\t' << boost::tuples::get<3>(tup2)[i] << '\t' << '\t' << endl;
		}

		std::cout << endl;

		std::cout << "Errors:" << endl;
		// Loop through the tuples and output their differences
		for (int i = 0; i < spot.size(); i++) {
			std::cout << abs(boost::tuples::get<1>(tup1)[i] - boost::tuples::get<1>(tup2)[i]) << '\t' << '\t' << abs(boost::tuples::get<2>(tup1)[i] - boost::tuples::get<2>(tup2)[i]) << '\t' << '\t'
				<< abs(boost::tuples::get<3>(tup1)[i] - boost::tuples::get<3>(tup2)[i]) << '\t' << '\t' << endl;
		}

		// Multiply h by itself for the next loop
		h *= h;

		std::cout << endl;
	}
}