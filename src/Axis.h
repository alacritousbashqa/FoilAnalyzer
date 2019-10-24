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

	void calculateVLocs();				// Calculates and populates value-locations map
public:
	// Constructors
	Axis(axisDirection dir, int boundary[2], wxPoint& origin, double vOrigin[2]);
	Axis(axisDirection dir, int boundary[2], wxPoint& origin, double vOrigin[2], double limits[2], double step);
	// Getters
	double getLowerLimit();
	double getUpperLimit();
	std::map<double, int> getVLocs();
	// Setters
	void setBoundary(int boundary[2]);
	void setOrigin(wxPoint origin);
	void setVOrigin(double vOrigin[2]);

	void updateAxis(int boundary[2], wxPoint origin, double vOrigin[2]);
	// Draw
	void draw(wxDC& dc);
};