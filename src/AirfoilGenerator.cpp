#include "AirfoilGenerator.h"

#include <wx/wxprec.h>

arma::mat AirfoilGenerator::generate4Digit(std::string code, int n) {

	// If the code was not 4 digits...
	if (code.length() != 4) {
		wxLogError("Entered NACA code invalid! Code should be 4 digits!");
		return arma::mat(1,2,arma::fill::zeros);
	}

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
		case 2:
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

arma::mat AirfoilGenerator::generate5Digit(std::string code, int n) {

	// If the code was not 5 digits...
	if (code.length() != 5) {
		wxLogError("Entered NACA code invalid! Code should be 5 digits!");
		return arma::mat(1, 2, arma::fill::zeros);
	}

	// Defaults in case something goes wrong
	double l = 6.0/20.0, p = 3.0/20.0, t = 0.12;
	int q = 0;

	// Parse the code for the camber and thickness
	for (int i = 0; i < code.length() - 1; i++) {
		switch (i) {
		case 0:
			l = 3.0 * std::stod(code.substr(0, 1)) / 20.0;
			break;
		case 1:
			p = std::stod(code.substr(1, 1)) / 20.0;
			break;
		case 2:
			q = std::stoi(code.substr(2, 1));
		case 4:
			t = std::stod(code.substr(3, 2)) / 100.0;
			break;
		}
	}

	double r = 0.2025, k1 = 15.957, k_ratio = 0.00677;

	// Determine the coefficients from max camber position and standard/reflex
	if (!q) {
		if (p == .05) {
			r = 0.0580;
			k1 = 361.4;
		}
		if (p == .10) {
			r = 0.126;
			k1 = 51.64;
		}
		if (p == .15) {
			r = 0.2025;
			k1 = 15.957;
		}
		if (p == .20) {
			r = 0.29;
			k1 = 6.643;
		}
		if (p == .25) {
			r = 0.391;
			k1 = 3.23;
		}
	}
	else {
		if (p == .10) {
			r = 0.13;
			k1 = 51.99;
			k_ratio = 0.000764;
		}
		if (p == .15) {
			r = 0.217;
			k1 = 15.793;
			k_ratio = 0.00677;
		}
		if (p == .20) {
			r = 0.318;
			k1 = 5.52;
			k_ratio = 0.0303;
		}
		if (p == .25) {
			r = 0.441;
			k1 = 3.191;
			k_ratio = 0.1355;
		}
	}

	// Let the panels vary with cosine of chord position
	arma::vec beta = arma::linspace(0, arma::datum::pi, n / 2);
	arma::vec x = 0.5*(1 - cos(beta));

	// Get all x's left of max camber position
	arma::uvec leftXi = arma::find(x < r);
	// Camber characteristics
	arma::vec yc;
	arma::vec dyc;

	// Standard
	if (!q) {
		// If cambered
		if (!leftXi.is_empty()) {
			arma::vec x1 = x(arma::span(0, leftXi(leftXi.back())));
			arma::vec x2 = x(arma::span(leftXi(leftXi.back()) + 1, x.size() - 1));
			arma::vec yc1 = k1 / 6.0 * (pow(x1, 3.0) - 3 * r*pow(x1, 2.0) + pow(r, 2.0)*(3 - r)*x1);
			arma::vec yc2 = k1 / 6.0*pow(r, 3.0)*(1 - x2);
			yc = arma::join_cols(yc1, yc2);
			arma::vec dyc1 = k1 / 6.0 * (3.0 * pow(x1, 2.0) - 6 * r * x1 + pow(r, 2.0)*(3 - r));
			arma::vec dyc2 = (-k1 / 6.0*pow(r, 3.0))*arma::ones(x2.n_rows, 1);
			dyc = arma::join_cols(dyc1, dyc2);
		}
		// If symmetric
		else {
			yc = k1 / 6.0*pow(r, 3.0)*(1 - x);
			dyc = (-k1 / 6.0*pow(r, 3.0))*arma::ones(x.n_rows, 1);
		}
	}
	// Reflex
	else {
		// If cambered
		if (!leftXi.is_empty()) {
			arma::vec x1 = x(arma::span(0, leftXi(leftXi.back())));
			arma::vec x2 = x(arma::span(leftXi(leftXi.back()) + 1, x.size() - 1));
			arma::vec yc1 = k1 / 6.0*(pow(x1 - r, 3.0) - k_ratio * pow(1 - r, 3.0)*x1 - pow(r, 3.0)*x1 + pow(r, 3.0));
			arma::vec yc2 = k1 / 6.0*(k_ratio*pow(x2 - r, 3.0) - k_ratio * pow(1 - r, 3.0)*x2 - pow(r, 3.0)*x2 + pow(r, 3.0));
			yc = arma::join_cols(yc1, yc2);
			arma::vec dyc1 = k1 / 6.0*(3.0*pow(x1 - r, 2.0) - k_ratio * pow(1 - r, 3.0) - pow(r, 3.0));
			arma::vec dyc2 = k1 / 6.0*(3.0*k_ratio*pow(x2 - r, 2.0) - k_ratio * pow(1 - r, 3.0) - pow(r, 3.0));
			dyc = arma::join_cols(dyc1, dyc2);
		}
		// If symmetric
		else {
			yc = k1 / 6.0*(k_ratio*pow(x - r, 3.0) - k_ratio * pow(1 - r, 3.0)*x - pow(r, 3.0)*x + pow(r, 3.0));
			dyc = k1 / 6.0*(3.0*k_ratio*pow(x - r, 2.0) - k_ratio * pow(1 - r, 3.0) - pow(r, 3.0));
		}
	}

	yc = l / 0.3*yc;
	dyc = l / 0.3*dyc;

	// Coefficients for generating the thickness distribution of a NACA digit airfoil
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