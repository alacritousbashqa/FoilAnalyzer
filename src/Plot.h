// Plot.h
// Author: Mark Bashqawi

// Plot holds all data required to show a plot and convert between numerical values to pixel locations. It holds 
// the axis objects and dictates how and when things are drawn, such as the axis ticks and airfoil plots.

#pragma once

#include <armadillo>

#include "Axis.h"

class Plot {
	int border[4];									// Padding on left, top, right, bottom of plot
	Axis* horizAxis;								// Horizontal axis pointer
	Axis* vertAxis;									// Vertical axis pointer
	std::string title;								// Plot title
	wxPoint origin;									// Location of origin (pixels)
	double vOrigin[2]{};							// Origin value (values), i.e. (0.0,0.0)
	wxRect boundary;								// Bounds of plot in which everything is drawn, e.g. bounds after border is applied
	wxRect drawArea;
	bool showLabelX;
	bool showLabelY;
	bool showTitle;

	// Converts the given points to their pixel location equivalents based on the axes steps and limits
	arma::umat pointsToPixels(arma::mat points);
	// Calculates the origin location (origin) and origin value (vOrigin) from the axes limits
	void calculateOrigin(double xLim[2], double yLim[2]);
public:
	// CONSTRUCTORS

	Plot(wxRect& boundary, double xLim[2], double yLim[2]);
	Plot(wxRect& boundary, double xLim[2], double yLim[2], int border[4]);
	//-------------------------------------------------------------------------------------------------------------
	// GETTERS

	// Returns a pointer to the horizontal axis
	Axis* getHAxis();
	// Returns a pointer to the vertical axis
	Axis* getVAxis();
	//-------------------------------------------------------------------------------------------------------------

	// Updates the boundaries with the new one defined by a wxRect
	void updateBoundaries(wxRect& rect);
	// Tells the axes to draw and draws the plot border
	void draw(wxDC& dc);
	// Draws the given points (values) onto the plot. The points are converted to pixel locations first.
	void drawPoints(wxDC& dc, arma::mat points);
	// Draws the given points (pixels) onto the plot
	void drawPoints(wxDC& dc, arma::umat points);
	//-------------------------------------------------------------------------------------------------------------
};