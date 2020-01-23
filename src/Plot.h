// Plot.h
// Author: Mark Bashqawi

// Plot holds all data required to show a plot and convert between numerical values to pixel locations. It holds 
// the axis objects and dictates how and when things are drawn, such as the axis ticks and airfoil plots.

#pragma once

#include <armadillo>

#include "Axis.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

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
	int aspectRatio[2];

	// Converts the given points to their pixel location equivalents based on the axes steps and limits
	arma::umat pointsToPixels(arma::mat points);
	// Calculates the origin location (origin) and origin value (vOrigin) from the axes limits
	void calculateOrigin(double xLim[2], double yLim[2]);

	void setHStep(double st);
	void setVStep(double st);
public:
	// CONSTRUCTORS

	Plot(wxRect& boundary, double xLim[2], double yLim[2], bool showX = false, bool showY = false, bool showTitle = false);
	Plot(wxRect& boundary, double xLim[2], double yLim[2], int border[4], bool showX = false, bool showY = false, bool showTitle = false);

	// DESTRUCTOR

	~Plot();

	//-------------------------------------------------------------------------------------------------------------
	// GETTERS

	// Returns a pointer to the horizontal axis
	Axis* getHAxis();
	// Returns a pointer to the vertical axis
	Axis* getVAxis();
	std::string getTitle();
	std::string getHLabel();
	std::string getVLabel();
	double getHStep();
	double getVStep();
	// Returns a pointer to the apect ratio array
	int* getAspectRatio();
	//-------------------------------------------------------------------------------------------------------------
	// SETTERS

	void setTitle(std::string t);
	void setHLabel(std::string hl);
	void setVLabel(std::string vl);
	/* Sets the axes steps to st: [horizontal, vertical]
	*/
	void setAxesSteps(double st[2]);
	/* Sets the aspect ratio to ar
	For no aspect ratio, at least one of the values can be set to an integer <= 0,
	otherwise all positive integers are valid inputs for an aspect ratio
	*/
	void setAspectRatio(int ar[2]);
	/* Sets the aspect ratio to [arx, ary]
	For no aspect ratio, at least one of the values can be set to an integer <= 0,
	otherwise all positive integers are valid inputs for an aspect ratio
	 */
	void setAspectRatio(int arx, int ary);
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