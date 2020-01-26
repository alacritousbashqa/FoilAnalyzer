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

extern std::vector<AirfoilStruct*> loadedAirfoils;

inline AirfoilStruct* getAirfoilFromName(std::string name) {
	for (AirfoilStruct* afs : loadedAirfoils) {
		if (afs->name == name)
			return afs;
	}
	return NULL;
}