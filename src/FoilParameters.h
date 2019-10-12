#pragma once

#include <iostream>
#include <armadillo>
#include <vector>

struct AirfoilStruct {
	std::string code;
	int nPanels;
	arma::mat points;
};

static std::vector<AirfoilStruct> loadedAirfoils;