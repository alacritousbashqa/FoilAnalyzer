#include "AirfoilGenerator.h"

#include <wx/wxprec.h>

arma::mat AirfoilGenerator::generate4Digit(std::string code, int n) {

	// Defaults in case something goes wrong
	double m = .02, p = .4, t = .12;

	// Parse the code for the camber and thickness
	for (int i = 0; i < code.length()-1; i++) {
		switch (i) {
		case 0:
			m = std::stod(code.substr(0, 1)) / 100.0;
			break;
		case 1:
			p = std::stod(code.substr(1, 1)) / 10.0;
			break;
		case 3:
			t = std::stod(code.substr(2, 2)) / 100.0;
			break;
		}
	}

	// Let the panels vary with cosine of chord position
	arma::vec beta = arma::linspace(0, arma::datum::pi, n/2);
	arma::vec x = 0.5*(1 - cos(beta));

	// Get all x's left of max camber position
	arma::uvec leftXi = arma::find(x < p);
	// Camber characteristics
	arma::vec yc;
	arma::vec dyc;

	// If cambered
	if (!leftXi.is_empty()) {
		arma::vec x1 = x(arma::span(0, leftXi(leftXi.back())));
		arma::vec x2 = x(arma::span(leftXi(leftXi.back()) + 1, x.size() - 1));
		arma::vec yc1 = m / (pow(p, 2.0)) * (2 * p*x1 - arma::pow(x1, 2.0));
		arma::vec yc2 = m / pow((1 - p), 2.0) * (1 - 2 * p + 2 * p*x2 - arma::pow(x2, 2.0));
		yc = arma::join_cols(yc1, yc2);
		arma::vec dyc1 = 2 * m / (pow(p, 2.0)) * (p - x1);
		arma::vec dyc2 = 2 * m / pow((1 - p), 2.0) * (p - x2);
		dyc = arma::join_cols(dyc1, dyc2);
	}
	// If symmetric
	else{
		yc = m / pow((1 - p), 2.0) * (1 - 2 * p + 2 * p*x - arma::pow(x, 2.0));
		dyc = 2 * m / pow((1 - p), 2.0) * (p - x);
	}

	// Coefficients for generating the thickness distribution of a NACA 4 digit airfoil
	double a0 = 0.2969;
	double a1 = -0.126;
	double a2 = -0.3516;
	double a3 = 0.2843;
	double a4 = -0.1036;

	arma::vec yt = t / 0.2 * (a0 * pow(x, 0.5) + a1 * x + a2 * pow(x, 2.0) + a3 * pow(x, 3.0) + a4 * pow(x, 4.0));

	arma::vec theta = atan(dyc);

	// Upper and lower x and y
	arma::vec xu = x - yt % sin(theta);
	arma::vec yu = yc + yt % cos(theta);
	arma::vec xl = x + yt % sin(theta);
	arma::vec yl = yc - yt % cos(theta);

	// All points returned as values, not pixels
	arma::vec X = arma::join_cols(arma::flipud(xu), xl);
	arma::vec Y = arma::join_cols(arma::flipud(yu), yl);

	return arma::join_horiz(X, Y);
}