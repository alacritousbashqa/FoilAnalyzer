#pragma once

#include <armadillo>

#include "Axis.h"

class Plot {
	int border[4];
	Axis* horizAxis;
	Axis* vertAxis;
	std::string title;
	wxPoint origin;
	double vOrigin[2]{};
	wxRect boundary;
	arma::umat pointsToPixels(arma::mat points);

	void calculateOrigin(double xLim[2], double yLim[2]);
public:
	Plot(wxRect& boundary, double xLim[2], double yLim[2]);
	Plot(wxRect& boundary, double xLim[2], double yLim[2], int border[4]);
	void updateBoundaries(wxRect& rect);
	void draw(wxDC& dc);
	void drawPoints(wxDC& dc, arma::mat points);
	Axis* getHAxis();
	Axis *getVAxis();
};