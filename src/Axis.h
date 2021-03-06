// Axis.h
// Author: Mark Bashqawi

// Axis holds all data related to value<->pixel conversion, tick distribution, and how the axes are drawn with
// labels.

#pragma once

#include <map>
#include <vector>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

enum axisDirection {
	HORIZONTAL,
	VERTICAL
};

class Axis {
	double limits[2];					// Lower and upper bounds (values)
	double step;						// Tick step size (value)
	int pStep;
	int boundary[2];					// Lower and upper bounds (pixels)
	axisDirection dir;					// Axis direction: horizontal or vertical
	std::string label;					// Axis label, e.g. t
	wxPoint origin;						// Location of origin (pixels)
	double vOrigin[2]{};				// Origin value (value), e.g. (0.0,0.0)

	bool flip;

	std::map<double, int> valueLocs;	// Maps values to their pixel locations, e.g. tick locations

	// Calculates and stores the axis values to their pixel locations
	void calculateVLocs();

	void setStep(double st);
public:
	// CONSTRUCTORS

	Axis(axisDirection dir, int boundary[2], wxPoint& origin, double vOrigin[2]);
	Axis(axisDirection dir, int boundary[2], wxPoint& origin, double vOrigin[2], double limits[2], double step, bool flip);

	// DESTRUCTORS

	~Axis();

	//-------------------------------------------------------------------------------------------------------------
	// GETTERS

	// Returns the smallest value plotted on the axis
	double getLowerLimit();
	// Returns the largest value plotted on the axis
	double getUpperLimit();
	// Get a map of the axis value-locations: value->pixel
	std::map<double, int> getVLocs();
	std::string getLabel();
	int getPixelStep();
	double getStep();
	//-------------------------------------------------------------------------------------------------------------
	// SETTERS

	// Sets the axis boundary (in pixels) to the new value
	void setBoundary(int boundary[2]);
	// Set the origin pixel position
	void setOrigin(wxPoint origin);
	// Set the origin value
	void setVOrigin(double vOrigin[2]);
	void setLabel(std::string l);
	//-------------------------------------------------------------------------------------------------------------

	friend class Plot;

	// Updates the boundary, origin pixel location, and origin value, then recalculates the value-locations
	void updateAxis(int boundary[2], wxPoint origin, double vOrigin[2]);
	// Draws the axis line, the ticks, and the labels for the axis
	void draw(wxDC& dc);
};
