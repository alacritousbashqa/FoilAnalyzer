#pragma once

#include <map>
#include <vector>

#include <wx/wxprec.h>

enum axisDirection {
	HORIZONTAL,
	VERTICAL
};

class Axis {
	double limits[2];					// Lower and upper bounds (values)
	double step;						// Tick step size (value)
	int boundary[2];					// Lower and upper bounds (pixels)
	axisDirection dir;					// Axis direction: horizontal or vertical
	std::string label;					// Axis label, e.g. t
	wxPoint origin;						// Location of origin (pixels)
	double vOrigin[2]{};				// Origin value (value), e.g. (0.0,0.0)

	std::map<double, int> valueLocs;	// Maps values to their pixel locations, e.g. tick locations

	// Calculates and stores the axis values to their pixel locations
	void calculateVLocs();
public:
	// CONSTRUCTORS

	Axis(axisDirection dir, int boundary[2], wxPoint& origin, double vOrigin[2]);
	Axis(axisDirection dir, int boundary[2], wxPoint& origin, double vOrigin[2], double limits[2], double step);
	//-------------------------------------------------------------------------------------------------------------
	// GETTERS

	// Returns the smallest value plotted on the axis
	double getLowerLimit();
	// Returns the largest value plotted on the axis
	double getUpperLimit();
	// Get a map of the axis value-locations: value->pixel
	std::map<double, int> getVLocs();
	//-------------------------------------------------------------------------------------------------------------
	// SETTERS

	// Sets the axis boundary (in pixels) to the new value
	void setBoundary(int boundary[2]);
	// Set the origin pixel position
	void setOrigin(wxPoint origin);
	// Set the origin value
	void setVOrigin(double vOrigin[2]);
	//-------------------------------------------------------------------------------------------------------------

	// Updates the boundary, origin pixel location, and origin value, then recalculates the value-locations
	void updateAxis(int boundary[2], wxPoint origin, double vOrigin[2]);
	// Draws the axis line, the ticks, and the labels for the axis
	void draw(wxDC& dc);
};