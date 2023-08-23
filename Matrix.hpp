// Objective: Create the Vector class and the Matrix class

// Ensure no errors if the header file is used twice
#ifndef MATRIX_HPP
#define MATRIX_HPP

// Include the necessary header files
#include "Option.hpp"
#include "EuropeanOption.hpp"
#include "PerpetualAmericanOption.hpp"
#include "Greeks.hpp"
#include <iostream>
using namespace std;
#include <vector>
#include "boost/tuple/tuple.hpp"
#include "boost/tuple/tuple_io.hpp"
#include <boost/math/distributions/normal.hpp>



// Create the Vector class
class Vector {
private:

	// The private attributes are a vector of doubles that hold the option parameter and a string that indicates the style of option
	// If it is necessary to add another derived options class, another parameter constructor should be added and a string should be chosen to represent that option

	std::vector<double> m_vector;
	std::string m_style;

public:

	// Default constructor
	Vector();

	// Parameter constructor for a EuropeanOption
	Vector(double S, EuropeanOption option);

	// Parameter constructor for a PerpetualAmericanOption
	Vector(double S, PerpetualAmericanOption option);

	// Destructor
	virtual ~Vector();

	// Copy constructor
	Vector(const Vector& source);

	// Assignment operator
	Vector& operator = (const Vector& source);

	// [] operator
	double& operator [] (int index);

	// Returns the size of the vector
	int Size() const;

	// Converts a vector to a EuropeanOption
	EuropeanOption ConvertToEO();

	// Converts a vector to a PerpetualAmericanOption
	PerpetualAmericanOption ConvertToPAO();

	// Outputs the option type
	std::string OptionStyle();

};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Create the Matrix class
class Matrix {

private:

	// The private attribute is a vector of the Vector class
	// A Matrix object will be nx7 matrix
	std::vector<Vector> m_matrix;


public:

	// Default constructor
	Matrix();

	// Parameter constructor for a EuropeanOption
	Matrix(EuropeanOption EO, double S, int index, double b, double n);

	// Parameter constructor for a PerpetualAmericanOption
	Matrix(PerpetualAmericanOption AO, double S, int index, double b, double n);

	// Destructor
	virtual ~Matrix();

	// Copy constructor
	Matrix(const Matrix& source);

	// Assignment operator
	Matrix& operator = (const Matrix& source);

	// []-operator
	Vector& operator [] (int index);

	// Accessor to return the selected row (Vector) of the Matrix
	Vector GetRow(const int& index);

	// Returns the number of rows in the Matrix
	int GetSize() const;

	// Prices the call/put options of a Matrix with Vectors as rows
	boost::tuple<vector<double>, vector<double>> MatrixPricer();

	// Calculates the call/put options' deltas and gammas of a Matrix with Vectors as rows given a Greek
	boost::tuple<vector<double>, vector<double>, vector<double>> MatrixPricer(Greeks g);

	// Outputs the total matrix
	void OutputMatrix(const double& n);

	// Method that outputs a Matrix's option parameters
	void ReadChange(const int& n);

	// Method that outputs a Matrix's option for a Greeks object
	void ReadChange(Greeks g, const int& n);
};


// A global function that reads through the option pricing MatrixPricer tuple without overlap in the main function
void ReadTuple(boost::tuple<vector<double>, vector<double>> tup, const double& n);

// A global function that reads through the greek calculating MatrixPricer tuple without overlap in the main function
void ReadTuple(boost::tuple<vector<double>, vector<double>, vector<double>> tup, const double& n);



#endif