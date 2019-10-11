#pragma once

#include <armadillo>

class AirfoilGenerator {
public:
	static arma::mat generate4Digit(std::string code, int n);
};