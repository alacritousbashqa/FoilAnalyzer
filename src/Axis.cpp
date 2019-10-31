#include "Axis.h"

double DEFAULT_LIMITS[2] = { -1.0,-1.0 };

Axis::Axis(axisDirection dir, int boundary[2], wxPoint& origin, double vOrigin[2])
	: Axis(dir, boundary, origin, vOrigin, DEFAULT_LIMITS, 0.1){
}

Axis::Axis(axisDirection dir, int boundary[2], wxPoint& origin, double vOrigin[2], double limits[2], double step) {
	this->dir = dir;
	this->boundary[0] = boundary[0];
	this->boundary[1] = boundary[1];
	this->origin = origin;
	this->vOrigin[0] = vOrigin[0];
	this->vOrigin[1] = vOrigin[1];
	this->limits[0] = limits[0];
	this->limits[1] = limits[1];
	this->step = step;
	if (dir == HORIZONTAL)
		this->label = "x";
	else
		this->label = "y";

	calculateVLocs();
}

Axis::~Axis() {
	valueLocs.clear();
}

void Axis::calculateVLocs() {
	valueLocs.clear(); // Clears the current list, might change later to just update it if the step size and limits did not change
	// If this is a horizontal axis...
	if (dir == HORIZONTAL) {
		int m_p = (limits[1] - vOrigin[0]) / step;	// Number of ticks for values greater than 0
		int m_n = (vOrigin[0] - limits[0]) / step;	// Number of ticks for values less than 0
		pStep = 0;									// Pixel step size
		// Use either m_p or m_n depending on which is not 0. If both are 0, display an error and return.
		if (m_p)
			pStep = (boundary[1] - origin.x) / m_p;
		else if (m_n)
			pStep = (origin.x - boundary[0]) / m_n;
		else {
			wxLogError("Number of ticks is zero! Perhaps the axis limits are not right?");
			return;
		}

		// Set the values at the limits to the pixel locations at the boundaries. Set the origin location.
		valueLocs[limits[0]] = boundary[0];
		valueLocs[limits[1]] = boundary[1];
		valueLocs[vOrigin[0]] = origin.x;

		// Finally, calculate the other tick value-locations with respect to the origin for consistent spacing on each side of the origin
		for (int i = 1; i <= m_p; i++) {
			valueLocs[vOrigin[0] + i * step] = origin.x + i * pStep;
		}
		for (int i = -1; i >= -m_n; i--) {
			valueLocs[vOrigin[0] + i * step] = origin.x + i * pStep;
		}
	}
	// If this is a vertical axis...
	else {
		int m_p = (limits[1] - vOrigin[1]) / step;	// Number of ticks for values greater than 0
		int m_n = (vOrigin[1] - limits[0]) / step;	// Number of ticks for values less than 0
		pStep = 0;									// Pixel step size
		// Use either m_p or m_n depending on which is not 0. If both are 0, display an error and return.
		if(m_p)
			pStep = (boundary[0] - origin.y) / m_p;
		else if (m_n)
			pStep = (origin.y - boundary[1]) / m_n;
		else {
			wxLogError("Number of ticks is zero! Perhaps the axis limits are not right?");
			return;
		}

		// Set the values at the limits to the pixel locations at the boundaries. Set the origin location.
		valueLocs[limits[0]] = boundary[1];
		valueLocs[limits[1]] = boundary[0];
		valueLocs[vOrigin[1]] = origin.y;

		// Finally, calculate the other tick value-locations with respect to the origin for consistent spacing on each side of the origin
		for (int i = 1; i <= m_p; i++) {
			valueLocs[vOrigin[1] + i * step] = origin.y + i * pStep;
		}
		for (int i = -1; i >= -m_n; i--) {
			valueLocs[vOrigin[1] + i * step] = origin.y + i * pStep;
		}
	}
}

void Axis::setBoundary(int boundary[2]) {
	this->boundary[0] = boundary[0];
	this->boundary[1] = boundary[1];
}

void Axis::draw(wxDC& dc) {
	dc.SetPen(wxPen(*wxRED, 1));
	dc.SetTextForeground(*wxRED);
	std::map<double, int>::iterator it = valueLocs.begin();

	// Horizontal Axis
	if (dir == HORIZONTAL) {
		dc.DrawLine(boundary[0], origin.y, boundary[1], origin.y); // Draw axis line
		for (; it != valueLocs.end(); ++it) { // Draw axis ticks
			dc.DrawLine(it->second, origin.y + 5, it->second, origin.y - 5); // Draw the tick
			int x_adj = 0;			// Label x offset
			int y_adj = 6;			// Label y offset
			int label_length = 5;	// Precision of label
			if (std::next(it, 1) == valueLocs.end()) // If this is the rightmost label, adjust to be within bounds
				x_adj = -15;
			if (it->first == 0.0){	// If this is at 0, move from vertical axis and only show one digit
				x_adj = 7;
				label_length = 1;
			}
			if (it->first > 0)		// If the number is positive, show only 2 decimal digits
				label_length = 4;

			dc.DrawText(std::to_string(it->first).substr(0, label_length), it->second + x_adj, origin.y + y_adj);
		}
	}
	// Vertical Axis
	else {
		dc.DrawLine(origin.x, boundary[0], origin.x, boundary[1]);
		for (; it != valueLocs.end(); ++it) {
			dc.DrawLine(origin.x + 5, it->second, origin.x - 5, it->second);
			int x_adj = -32;				// Label x offset
			int y_adj = 0;					// Label y offset
			int label_length = 5;			// Precision of label
			if (it->first == 0.0) {			// Don't draw the label because it is already drawn for the horizontal axis
				continue;
			}
			if (it == valueLocs.begin())	// If this is the bottom label, adjust to be within bounds
				y_adj = -13;
			if (it->first > 0) {			// If the number is positive, show only 2 decimal digits
				x_adj = -27;
				label_length = 4;
			}

			dc.DrawText(std::to_string(it->first).substr(0, label_length), origin.x + x_adj, it->second + y_adj);
		}
	}
	
}


double Axis::getLowerLimit() {
	return limits[0];
}
double Axis::getUpperLimit() {
	return limits[1];
}

std::map<double, int> Axis::getVLocs() {
	return valueLocs;
}

void Axis::setOrigin(wxPoint origin) {
	this->origin.x = origin.x;
	this->origin.y = origin.y;
}

void Axis::setVOrigin(double vOrigin[2]) {
	this->vOrigin[0] = vOrigin[0];
	this->vOrigin[1] = vOrigin[1];
}

void Axis::updateAxis(int boundary[2], wxPoint origin, double vOrigin[2]) {
	setBoundary(boundary);
	setOrigin(origin);
	setVOrigin(vOrigin);
	calculateVLocs();
}

std::string Axis::getLabel() {
	return label;
}

void Axis::setLabel(std::string l) {
	label = l;
}