// Objective: Test out the functionalities of the different classes

// Include the necessary header files
#include "Option.hpp"
#include "EuropeanOption.hpp"
#include "PerpetualAmericanOption.hpp"
#include "Matrix.hpp"
#include "Exception.hpp"
#include "Greeks.hpp"
#include "boost/tuple/tuple.hpp"
#include "boost/tuple/tuple_io.hpp"
using boost::tuple;
#include <vector>
#include <cmath>
#include <iostream>
using namespace std;


// Global Mesh function
// Takes in a start (a), an end (b), and the total amount (n) and returns a vector of spot prices

std::vector<double> Mesh(const double& a, const double& b, const double& n) {

	// Calculate the interval between each spot price
	double h = (b - a) / n;

	std::vector<double> spot;

	double r = 0;

	// Insert each updated spot price
	for (int i = 0; i < n + 1; ++i) {

		spot.push_back(a + r);

		r += h;
	}

	return spot;
}




int main() {

	// Calculate the call and put prices for 4 batches of option parameters
	// Confirm the Put-Call Parity holds using the EuropeanOption method: InternalConfirmPutCallParity

	// The below description is encapsulated in the EuropeanOption method, ReadBatches:
	// 
	// For each batch, initialize a EuropeanOption with type call and its corresponding spot price
	// 
	// Output the option type and with its price, and using the PutCallParity method, price the call's corresponding put price
	// 
	// Use the InternalConfirmPutCallParity to internally confirm that the Put-Call Parity holds
	// 
	// Initialize a new option and change it to a put
	// 
	// Output the option type and with its price, and using the PutCallParity method, price the put's corresponding call price
	// 
	// Use the InternalConfirmPutCallParity to internally confirm that the Put-Call Parity holds
	// 
	// Use the external ConfirmPutCallParity method alternating between the call and put as another check of the Put-Call parity


	std::cout << "Outputs for Price/Put-Call Parity checking:" << endl << endl;


	// Batch 1: T = 0.25, K = 65, sig = 0.30, r = 0.08, S = 60 (then C = 2.13337, P = 5.84628)

	std::cout << "Batch #1: " << endl << endl;

	EuropeanOption batch1_call(65, 0.25, 0.08, 0.08, 0.3, 0);

	double S = 60;

	batch1_call.ReadBatches(S);

	std::cout << endl << endl;



	// Batch 2: T = 1.0, K = 100, sig = 0.2, r = 0.0, S = 100 (then C = 7.96557, P = 7.96557).

	std::cout << "Batch #2: " << endl << endl;

	EuropeanOption batch2_call(100, 1, 0, 0, 0.2, 0);

	double S2 = 100;

	batch2_call.ReadBatches(S2);

	std::cout << endl << endl;



	//Batch 3: T = 1.0, K = 10, sig = 0.50, r = 0.12, S = 5 (C = 0.204058, P = 4.07326).

	std::cout << "Batch #3: " << endl << endl;

	EuropeanOption batch3_call(10, 1, 0.12, 0.12, 0.5, 0);

	double S3 = 5;

	batch3_call.ReadBatches(S3);

	std::cout << endl << endl;



	// Batch 4: T = 30.0, K = 100.0, sig = 0.30, r = 0.08, S = 100.0 (C = 92.17570, P = 1.24750).

	std::cout << "Batch #4: " << endl << endl;

	EuropeanOption batch4_call(100, 30, 0.08, 0.08, 0.3, 0);

	double S4 = 100;

	batch4_call.ReadBatches(S4);

	std::cout << endl << endl << endl;

	std::cout << "///////////////////////////////////////////////////////////////////////////////////////////" << endl << endl;


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Call the EuropeanOption SpotPricePricer method to calculate the call and put prices for a European Option whose spot price is incremented

	std::cout << "Outputs for European Option SpotPricePricer method:" << endl << endl;
	std::cout << "Ran option with K = 65, T = 0.25, r = 0.08, sig = 0.3 " << endl << endl;

	double start = 50;
	double end = 100;
	double n = 5;

	// Initialize a EuropeanOption
	EuropeanOption option(65, 0.25, 0.08, 0.08, 0.3, 0);

	// Create a mesh vector
	std::vector<double> mesh1 = Mesh(start, end, n);

	// Create a tuple consisting of a vector of spot prices, a vector of call prices, and a vector of put prices
	boost::tuple<vector<double>, vector<double>, vector<double>> tup = option.SpotPricePricer(mesh1);

	std::cout << "Spot prices: " << '\t' << "Call prices :" << '\t' << "Put prices: " << endl;

	// Loop through the tuple and output the spot prices with its corresponding call and put prices
	for (int i = 0; i < n; i++) {
		std::cout << boost::tuples::get<0>(tup)[i] << '\t' << '\t' << boost::tuples::get<1>(tup)[i] << '\t' << '\t' << boost::tuples::get<2>(tup)[i] << '\t' << '\t' << endl;
	}

	std::cout << endl << endl << endl;

	std::cout << "///////////////////////////////////////////////////////////////////////////////////////////" << endl << endl;


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Group A.d1: Call the Matrix MatrixPricer method to price calls/puts based on which parameter is incremented

	// The below description is encapsulated in Matrix methods:
	// For all 7 possible matrices:
	// 
	// Initialize a Matrix with a EuropeanOption, the spot price, the index of the parameter that will be changed, the ending point, and the number of rows of the Matrix
	// 
	// Output the tuple through the ReadTuple method

	std::cout << "Outputs for European Option MatrixPricer method:" << endl << endl;
	std::cout << "Ran option with K = 65, T = 0.25, r = 0.08, sig = 0.3 " << endl << endl;


	std::cout << "Change in spot price: " << endl;
	Matrix m1(option, start, 0, end, n);
	m1.ReadChange(n);


	std::cout << "Change in strike price: " << endl;
	Matrix m2(option, start, 1, end, n);
	m2.ReadChange(n);


	std::cout << "Change in expiry: " << endl;
	Matrix m3(option, start, 2, 1, n);
	m3.ReadChange(n);


	std::cout << "Change in risk free rate: " << endl;
	Matrix m4(option, start, 3, 0.4, n);
	m4.ReadChange(n);


	std::cout << "Change in cost of carry: " << endl;
	Matrix m5(option, start, 4, 0.4, n);
	m5.ReadChange(n);


	std::cout << "Change in volatility: " << endl;
	Matrix m6(option, start, 5, 0.8, n);
	m6.ReadChange(n);



	std::cout << "///////////////////////////////////////////////////////////////////////////////////////////" << endl
		<< "///////////////////////////////////////////////////////////////////////////////////////////" << endl << endl;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Group A.a2: Test out the Delta and Gamma using the European Option parameters listed in the question prompt	

	std::cout << "Outputs for Delta and Gamma calculations:" << endl << endl;
	std::cout << "Ran option with K = 100, T = 0.5, r = 0.1, sig = 0.36 " << endl << endl;

	EuropeanOption option1(100, 0.5, 0.1, 0, 0.36, 0);
	double S1 = 105;

	// Output the deltas and gammas of the EuropeanOption
	// Notice that the gammas for the call and put are the same

	std::cout << "Delta of call: " << option1.Delta(S1) << " Gamma of call : " << option1.Gamma(S1) << endl;

	// Change EO1 to a put
	option1.Toggle();

	std::cout << "Delta of put: " << option1.Delta(S1) << " Gamma of put : " << option1.Gamma(S1) << endl;

	// Change EO1 to a put
	option1.Toggle();

	std::cout << endl;

	std::cout << "///////////////////////////////////////////////////////////////////////////////////////////" << endl << endl;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Group A.b2: Call the Greeks SpotPricePrice method to calculate the call and put deltas and gammas for a European Option whose spot price is incremented

	std::cout << "Outputs for the Greeks SpotPricePricer:" << endl << endl;
	std::cout << "Ran option with K = 100, T = 0.5, r = 0.1, sig = 0.36 " << endl << endl;

	// Initialize a Greek with a EuropeanOption and a starting spot price
	Greeks g(option1, S1);

	// Create a mesh vector
	std::vector<double> mesh2 = Mesh(S1, 130, n);

	// Create a tuple consisting of a vector of spot prices, a vector of call deltas, a vector of put deltas, and a vector of gammas
	boost::tuple<vector<double>, vector<double>, vector<double>, vector<double>> tup8 = g.SpotPricePricer(mesh2);

	std::cout << "Spot prices: " << '\t' << "Delta Call:" << '\t' << "Delta Put: " << '\t' << "Gamma: " << endl;

	// Loop through the tuple and output the spot prices with its corresponding call and put prices
	for (int i = 0; i < n; i++) {
		std::cout << boost::tuples::get<0>(tup8)[i] << '\t' << '\t' << boost::tuples::get<1>(tup8)[i] << '\t' << '\t' << boost::tuples::get<2>(tup8)[i] << '\t' << '\t' << boost::tuples::get<3>(tup8)[i] << '\t' << '\t' << endl;
	}

	std::cout << endl << endl;

	std::cout << "///////////////////////////////////////////////////////////////////////////////////////////" << endl << endl;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Group A.c2: Call the Matrix MatrixPricer method to calculate the deltas and gammas of calls/puts based on which parameter is incremented

	std::cout << "Outputs for Greeks MatrixPricer method:" << endl << endl;
	std::cout << "Ran option with K = 100, T = 0.5, r = 0.1, sig = 0.36 " << endl << endl;

	// The below description is encapsulated in Matrix methods:
	// 
	// For all 7 possible matrices:
	// 
	// Initialize a Matrix with a EuropeanOption, a spot price, the index of the parameter that will be changed, the ending point, and the number of rows of the Matrix
	// 
	// Output the tuple through the ReadTuple method

	std::cout << "Change in spot price: " << endl;
	Matrix m10(option1, S1, 0, 130, n);
	m10.ReadChange(g, n);


	std::cout << "Change in strike price: " << endl;
	Matrix m11(option1, S1, 1, 125, n);
	m11.ReadChange(g, n);


	std::cout << "Change in the expiry: " << endl;
	Matrix m12(option1, S1, 2, 1, n);
	m12.ReadChange(g, n);


	std::cout << "Change in risk free rate: " << endl;
	Matrix m13(option1, S1, 3, 0.4, n);
	m13.ReadChange(g, n);


	std::cout << "Change in cost of carry rate: " << endl;
	Matrix m14(option1, S1, 4, 0.4, n);
	m14.ReadChange(g, n);


	std::cout << "Change in volatility: " << endl;
	Matrix m15(option1, S1, 5, 0.8, n);
	m15.ReadChange(g, n);


	std::cout << endl << endl;

	std::cout << "///////////////////////////////////////////////////////////////////////////////////////////" << endl << endl;


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Group A.d2: Iterate through the h's and output the approximations of delta and gamma for parts a) and b)

	std::cout << "Outputs for Greeks approximations:" << endl << endl;
	std::cout << "Ran option with K = 100, T = 0.5, r = 0.1, sig = 0.36 " << endl << endl;


	// The comparison between the exact greeks and the approximated greeks are encapsulated in the Greeks class
	g.CompareGreeksMethods(S1, 3);

	// The comparison between the exact greeks spot pricer and the approximated greeks spot pricer are encapsulated in the Greeks class
	g.CompareGreeksMethods(mesh2, 3);


	std::cout << endl << endl << endl;

	std::cout << "///////////////////////////////////////////////////////////////////////////////////////////" << endl
		<< "///////////////////////////////////////////////////////////////////////////////////////////" << endl << endl;


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Group B.b: Output the prices of the Perpetual American Option with the following data

	// K = 100, sig = 0.1, r = 0.1, b = 0.02, S = 110 (check C = 18.5035, P = 3.03106)

	std::cout << "Outputs for Perpetual American Option pricing:" << endl << endl;
	std::cout << "Ran option with K = 100, T = 0.1, r = 0.02, sig = 0.1 " << endl << endl;

	// Initialize an AmericanOption and a spot price
	PerpetualAmericanOption AO(100, 0.1, 0.02, 0.1, 0);
	double Spot = 110;

	// Output the call and put prices of the American Option
	std::cout << "The call price of the American Option is: " << AO.Price(Spot) << endl;

	AO.Toggle();

	std::cout << "The put price of the American Option is: " << AO.Price(Spot) << endl;

	AO.Toggle();

	std::cout << endl << endl << endl;

	std::cout << "///////////////////////////////////////////////////////////////////////////////////////////" << endl << endl;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Group B.c: Test the SpotPriceChecker for the PerpetualAmericanOption

	std::cout << "Outputs for Perpetual American Option SpotPricePricer:" << endl << endl;
	std::cout << "Ran option with K = 100, T = 0.1, r = 0.02, sig = 0.1 " << endl << endl;

	// Initialize starting and ending spot prices and the total number of spots
	double start2 = Spot;
	double end2 = 146;
	double n2 = 6;

	// Create a mesh vector
	std::vector<double> mesh3 = Mesh(start2, end2, n2);

	// Create a tuple consisting of a vector of spot prices, a vector of call prices, and a vector of put prices
	boost::tuple<vector<double>, vector<double>, vector<double>> tup30 = AO.SpotPricePricer(mesh3);


	std::cout << "Spot prices: " << '\t' << "Call prices :" << '\t' << "Put prices: " << endl;

	// Loop through the tuple and output the spot prices with its corresponding call and put prices
	for (int i = 0; i < n2; i++) {
		std::cout << boost::tuples::get<0>(tup30)[i] << '\t' << '\t' << boost::tuples::get<1>(tup30)[i] << '\t' << '\t' << boost::tuples::get<2>(tup30)[i] << '\t' << '\t' << endl;
	}

	std::cout << endl << endl << endl;

	std::cout << "///////////////////////////////////////////////////////////////////////////////////////////" << endl << endl;


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Group B.d: Test the MatrixPricer for the AmericanOption

	std::cout << "Outputs for Perpetual American Option MatrixPricer:" << endl << endl;
	std::cout << "Ran option with K = 100, T = 0.1, r = 0.02, sig = 0.1 " << endl;

	// The below description is encapsulated in Matrix methods:
	// 
	// For all 6 possible matrices:
	// 
	// Initialize a Matrix with a AmericanOption, a spot price, the index of the parameter that will be changed, the ending point, and the number of rows of the Matrix
	// 
	// Output the tuple through the ReadTuple method

	std::cout << "Change in spot price: " << endl;
	Matrix m20(AO, start2, 0, end2, n2);
	m20.ReadChange(n2);


	std::cout << "Change in strike price: " << endl;
	Matrix m21(AO, start2, 1, 115, n2);
	m21.ReadChange(n2);


	std::cout << "Change in risk free rate: " << endl;
	Matrix m22(AO, start2, 2, 0.6, n2);
	m22.ReadChange(n2);


	std::cout << "Change in cost of carry rate: " << endl;
	Matrix m23(AO, start2, 3, 0.5, n);
	m23.ReadChange(n2);


	std::cout << "Change in volatility: " << endl;
	Matrix m24(AO, start2, 4, 0.86, n2);
	m24.ReadChange(n2);


	std::cout << endl << endl << endl;

	std::cout << "///////////////////////////////////////////////////////////////////////////////////////////" << endl
		<< "///////////////////////////////////////////////////////////////////////////////////////////" << endl;

	return 0;
}