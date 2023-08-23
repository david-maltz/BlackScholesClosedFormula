// Objective: Implement the Vector and Matrix classes

// Include the necessary header files
#include "Option.hpp"
#include "EuropeanOption.hpp"
#include "PerpetualAmericanOption.hpp"
#include "Matrix.hpp"
#include "Exception.hpp"
#include "Greeks.hpp"
#include <iostream>
using namespace std;
#include <vector>
#include "boost/tuple/tuple.hpp"
#include "boost/tuple/tuple_io.hpp"
#include <boost/math/distributions/normal.hpp>



// The default constructor
// Sets the size of the Vector to 0 and the style to nothing
Vector::Vector() : m_vector(0), m_style("") {}


// Parameter constructor
// Sets the Vector's size and its private attributes from a EuropeanOption
// 0: S (Spot price)
// 1: K (Strike price)
// 2: T (Expiry)
// 3: r (Risk free rate)
// 4: b (Cost of carry rate)
// 5: sig (Volatility)
// 6: q (Dividend rate)
Vector::Vector(double S, EuropeanOption option) : m_vector(7), m_style("EO") {
	m_vector[0] = S;
	m_vector[1] = option.StrikePrice();
	m_vector[2] = option.Expiry();
	m_vector[3] = option.RiskFreeRate();
	m_vector[4] = option.CostOfCarry();
	m_vector[5] = option.Volatility();
	m_vector[6] = option.Dividend();
}


// Parameter constructor
// Sets the Vector's size and its private attributes from a PerpetualAmericanOption
// 0: S (Spot price)
// 1: K (Strike price)
// 2: T (Expiry)
// 3: r (Risk free rate)
// 4: b (Cost of carry rate)
// 5: sig (Volatility)
// 6: q (Dividend rate)
Vector::Vector(double S, PerpetualAmericanOption option) : m_vector(6), m_style("PAO") {
	m_vector[0] = S;
	m_vector[1] = option.StrikePrice();
	m_vector[2] = option.RiskFreeRate();
	m_vector[3] = option.CostOfCarry();
	m_vector[4] = option.Volatility();
	m_vector[5] = option.Dividend();
}


// Destructor
Vector::~Vector() {}


// Copy constructor
Vector::Vector(const Vector& source) : m_vector(source.m_vector), m_style(source.m_style) {}


// Assignment operator
Vector& Vector::operator = (const Vector& source) {

	if (this == &source) {
		return *this;
	}

	m_vector = source.m_vector;
	m_style = source.m_style;

	return *this;
}


// []-operator
// Throws an OutOfBoundsException if the index is out of bounds
double& Vector::operator [] (int index) {

	if (m_style == "EO" && index > 6 || index < 0) throw OutOfBoundsException(index);

	else if (m_style == "PAO" && index > 5 || index < 0) throw OutOfBoundsException(index);

	return m_vector[index];
}


// Returns the size of the vector
int Vector::Size() const {
	return m_vector.size();
}


// Returns the option style
std::string Vector::OptionStyle() {
	return m_style;
}


// Converts a vector to a EuropeanOption
EuropeanOption Vector::ConvertToEO() {

	EuropeanOption EO(m_vector[1], m_vector[2], m_vector[3], m_vector[4], m_vector[5], m_vector[6]);

	return EO;
}


// Converts a vector to an PerpetualAmericanOption
PerpetualAmericanOption Vector::ConvertToPAO() {

	PerpetualAmericanOption PAO(m_vector[1], m_vector[2], m_vector[3], m_vector[4], m_vector[5]);

	return PAO;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Default constructor
// Set with size 1 and a Vector of 0's
Matrix::Matrix() : m_matrix(1) {
	m_matrix.push_back(Vector());
}


// Parameter constructor for a EuropeanOption
// v: original Vector
// index: the parameter of the option that is being incremented
// b: the ending point of the increments
// n: the size of the matrix
Matrix::Matrix(EuropeanOption EO, double S, int index, double b, double n) : m_matrix() {

	// Initialize a Vector
	Vector v(S, EO);

	// Initialize a as the indexed parameter from the Vector
	double a = v[index];

	// Create the mesh size
	double h = (b - a) / n;

	// Insert v as the first row of the Matrix
	m_matrix.push_back(v);

	// Loop through the intended size of the Matrix
	for (int i = 0; i < n; ++i) {

		// Loop through the size of each Vector
		// Increment the indexed parameter and leave the rest the same
		for (int j = 0; j < 7; ++j) {
			if (j == index) {
				v[j] += h;
			}
		}

		// Add the updated Vector to the next row of the Matrix
		m_matrix.push_back(v);
	}
}


// Parameter constructor for a PerpetualAmericanOption
// v: original Vector
// index: the parameter of the option that is being incremented
// b: the ending point of the increments
// n: the size of the matrix
Matrix::Matrix(PerpetualAmericanOption PAO, double S, int index, double b, double n) : m_matrix() {

	// Initialize a Vector
	Vector v(S, PAO);

	// Initialize a as the indexed parameter from the Vector
	double a = v[index];

	// Create the mesh size
	double h = (b - a) / n;

	// Insert v as the first row of the Matrix
	m_matrix.push_back(v);

	// Loop through the intended size of the Matrix
	for (int i = 0; i < n; ++i) {

		// Loop through the size of each Vector
		// Increment the indexed parameter and leave the rest the same
		for (int j = 0; j < 6; ++j) {
			if (j == index) {
				v[j] += h;
			}
		}

		// Add the updated Vector to the next row of the Matrix
		m_matrix.push_back(v);
	}
}


// Destructor
Matrix::~Matrix() {}


// Copy constructor
Matrix::Matrix(const Matrix& source) : m_matrix(source.m_matrix) {}


// Assignment operator
Matrix& Matrix::operator = (const Matrix& source) {

	if (this == &source) {
		return *this;
	}

	m_matrix.clear();

	m_matrix = source.m_matrix;

	return *this;
}


// []-operator
// Throws an OutOfBoundsException if the index is out of bounds
Vector& Matrix::operator [] (int index) {

	if (this->GetRow(0).OptionStyle() == "EO" && index > 6 || index < 0) throw OutOfBoundsException(index);

	else if (this->GetRow(0).OptionStyle() == "PAO" && index > 5 || index < 0) throw OutOfBoundsException(index);

	return m_matrix[index];
}


// Returns the indexed row of the Matrix
Vector Matrix::GetRow(const int& index) {
	return m_matrix[index];
}


// Returns the number of rows of the Matrix
int Matrix::GetSize() const {
	return m_matrix.size();
}


// The MatrixPricer method for Options
// The method prices a EuropeanOption from the rows of the Matrix (Vector) and returns a tuple with a vector of calls and a vector of puts
// If it is necessary to add another derived options class, add another else if statement that initializes that derived Option object
boost::tuple<vector<double>, vector<double>> Matrix::MatrixPricer() {

	// Initialize 2 vectors: one for the call prices and one for the put prices
	vector<double> calls;
	vector<double> puts;

	// If the option type of the first vector is a EuropeanOption, proceed in the if block
	if (m_matrix[0].OptionStyle() == "EO") {

		// Initialize a EuropeanOption
		EuropeanOption option;

		// Loop through the total number of rows of the matrix
		for (int i = 0; i < m_matrix.size(); ++i) {

			// Assign the EuropeanOption that is obtained from the Matrix's row (Vector)
			option = m_matrix[i].ConvertToEO();

			// Push back the 2 vector with the option prices calculated from each of the Matrix's row vectors

			calls.push_back(option.Price(m_matrix[i][0]));

			// Change the EuropeanOption from a call to a put
			option.Toggle();

			puts.push_back(option.Price(m_matrix[i][0]));

		}
	}

	// If the option type of the first vector is a PerpetualAmericanOption, proceed in the else if block
	else if (m_matrix[0].OptionStyle() == "PAO") {

		// Initialize an PerpetualAmericanOption
		PerpetualAmericanOption option;

		// Loop through the total number of rows of the matrix
		for (int i = 0; i < m_matrix.size(); ++i) {

			// Assign the AmericanOption that is obtained from the Matrix's row (Vector)
			option = m_matrix[i].ConvertToPAO();

			// Push back the 2 vector with the option prices calculated from each of m's row vectors

			calls.push_back(option.Price(m_matrix[i][0]));

			// Change the PerpetualAmericanOption from a call to a put
			option.Toggle();

			puts.push_back(option.Price(m_matrix[i][0]));

		}
	}

	// If the option type is neither of the two option, set the two vectors as 0 vectors
	else {
		for (int i = 0; i < m_matrix.size(); ++i) {
			calls.push_back(0);
			puts.push_back(0);
		}
	}

	// Insert the 2 vectors into a tuple
	boost::tuple<vector<double>, vector<double>> tup(calls, puts);

	// Return the tuple
	return tup;
}



// The MatrixPricer method for the Greeks, overloaded the previous MatrixPricer
// The method calculate the delta and gamma for a call and put from the rows of the Matrix (Vector)
// The method returns a tuple with a vector of delta calls and delta puts and a vector of gamma calls/puts
boost::tuple<vector<double>, vector<double>, vector<double>> Matrix::MatrixPricer(Greeks g) {

	// Initialize 3 vectors: one for the delta call prices, one for the delta put prices, and one for the gammas
	vector<double> delta_calls;
	vector<double> delta_puts;
	vector<double> gamma;

	// Initialize a EuropeanOption and a Greek
	EuropeanOption EO;

	// Loop through the total number of rows of matrix m
	for (int i = 0; i < m_matrix.size(); ++i) {

		EO = m_matrix[i].ConvertToEO();

		// Push back the 3 vector with the deltas and gamma calculated from each of m's row vectors

		delta_calls.push_back(EO.Delta(m_matrix[i][0]));

		// Change the EuropeanOption from a call to a put
		EO.Toggle();

		delta_puts.push_back(EO.Delta(m_matrix[i][0]));

		gamma.push_back(EO.Gamma(m_matrix[i][0]));

	}

	// Insert the 3 vectors into a tuple
	boost::tuple<vector<double>, vector<double>, vector<double>> tup(delta_calls, delta_puts, gamma);

	// Return the tuple
	return tup;
}



// Outputs the matrix
void Matrix::OutputMatrix(const double& n) {

	for (int i = 0; i < n; ++i) {

		for (int j = 0; j < this->GetRow(0).Size(); ++j) {

			std::cout << m_matrix[i][j] << " ";
		}

		std::cout << endl;
	}

	std::cout << endl;
}



// Method that outputs a matrix of option parameters
void Matrix::ReadChange(const int& n) {
	this->OutputMatrix(n);
	boost::tuple<vector<double>, vector<double>> tup = this->MatrixPricer();
	ReadTuple(tup, n);
}



// Method that outputs a matrix of option parameters for a Greeks object
void Matrix::ReadChange(Greeks g, const int& n) {
	this->OutputMatrix(n);
	boost::tuple<vector<double>, vector<double>, vector<double>> tup = this->MatrixPricer(g);
	ReadTuple(tup, n);
}




// Create a global function to read through the option pricing MatrixPricer tuple without overlap in the main function
void ReadTuple(boost::tuple<vector<double>, vector<double>> tup, const double& n) {

	// Output the first vector (calls) of the tuple
	std::cout << "Call Prices: " << endl;
	for (int i = 0; i < n; i++) {
		std::cout << boost::tuples::get<0>(tup)[i] << endl;
	}

	std::cout << endl;

	// Output the second vector (puts) of the tuple
	std::cout << "Put Prices: " << endl;
	for (int i = 0; i < n; i++) {
		std::cout << boost::tuples::get<1>(tup)[i] << endl;
	}

	std::cout << endl;
}



// Create a global function to read through the greek calculating MatrixPricer tuple without overlap in the main function
void ReadTuple(boost::tuple<vector<double>, vector<double>, vector<double>> tup, const double& n) {

	// Output the first vector (delta calls) of the tuple
	std::cout << "Call Deltas: " << endl;
	for (int i = 0; i < n; i++) {
		std::cout << boost::tuples::get<0>(tup)[i] << endl;
	}

	std::cout << endl;

	// Output the second vector (delta puts) of the tuple
	std::cout << "Put Deltas: " << endl;
	for (int i = 0; i < n; i++) {
		std::cout << boost::tuples::get<1>(tup)[i] << endl;
	}

	std::cout << endl;

	// Output the second vector (puts) of the tuple
	std::cout << "Gammas: " << endl;
	for (int i = 0; i < n; i++) {
		std::cout << boost::tuples::get<2>(tup)[i] << endl;
	}

	std::cout << endl;
}