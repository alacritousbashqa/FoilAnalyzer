// FoilParameters.h
// Author: Mark Bashqawi

// Holds data to be seen across the program, including what airfoils are currently loaded. It also holds the code 
// for the AirfoilStruct which is how loaded airfoils are stored.

#pragma once

#include <iostream>
#include <armadillo>
#include <vector>

struct AirfoilStruct {
	std::string name;
	std::string code;
	int nPanels;
	arma::mat points;
};

static std::vector<AirfoilStruct*> loadedAirfoils;