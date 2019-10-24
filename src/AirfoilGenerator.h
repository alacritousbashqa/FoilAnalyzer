// AirfoilGenerator.h
// Author: Mark Bashqawi

// Handles generation of airfoils points based on a NACA code and number of panels.

#pragma once

#include <armadillo>

class AirfoilGenerator {
public:
	static arma::mat generate4Digit(std::string code, int n);
	static arma::mat generate5Digit(std::string code, int n);
};