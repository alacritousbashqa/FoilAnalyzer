#pragma once

#include <armadillo>
#include <wx/log.h>

class AirfoilGenerator {
public:
	static arma::mat generate4Digit(std::string code, int n);
};
