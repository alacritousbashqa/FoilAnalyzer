#pragma once

#include "Axis.h"

class Plot {
	int border[4];
	Axis* horizAxis;
	Axis* vertAxis;
	std::string title;
	wxPoint origin;
	double vOrigin[2]{};
	wxRect boundary;

	void calculateOrigin(double xLim[2], double yLim[2]);
public:
	Plot(wxRect& boundary, double xLim[2], double yLim[2]);
	Plot(wxRect& boundary, double xLim[2], double yLim[2], int border[4]);
	void draw(wxDC& dc);
	Axis* getHAxis();
	Axis *getVAxis();
};