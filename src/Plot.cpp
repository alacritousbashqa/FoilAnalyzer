#include "Plot.h"

int DEFAULT_BORDER[4] = { 0,0,0,0 };

Plot::Plot(wxRect& boundary, double xLim[2], double yLim[2]) 
	: Plot(boundary, xLim, yLim, DEFAULT_BORDER){
}

Plot::Plot(wxRect& boundary, double xLim[2], double yLim[2], int border[4]) {
	this->boundary = boundary;
	// Borders
	this->border[0] = border[0];
	this->border[1] = border[1];
	this->border[2] = border[2];
	this->border[3] = border[3];

	calculateOrigin(xLim, yLim);

	int xBound[2] = { boundary.GetLeft(), boundary.GetRight() };
	int yBound[2] = { boundary.GetTop(), boundary.GetBottom() };

	// Create axes
	horizAxis = new Axis(axisDirection::HORIZONTAL, xBound, origin, vOrigin, xLim, 0.1);
	vertAxis = new Axis(axisDirection::VERTICAL, yBound, origin, vOrigin, yLim, 0.02);
}

void Plot::calculateOrigin(double xLim[2], double yLim[2]) {
	// X component
	if (xLim[0] >= 0) {
		origin.x = boundary.GetLeft();
		vOrigin[0] = xLim[0];
	}
	else if (xLim[1] <= 0) {
		origin.x = boundary.GetRight();
		vOrigin[0] = xLim[1];
	}
	else {
		double ratio = abs((double)xLim[0]) / ((double)xLim[1] - (double)xLim[0]);
		origin.x = (int)(ratio * boundary.GetWidth() + boundary.GetX());
		vOrigin[0] = 0.0;
	}
	// Y component
	if (yLim[0] >= 0) {
		origin.y = boundary.GetBottom();
		vOrigin[1] = yLim[0];
	}
	else if (yLim[1] <= 0) {
		origin.y = boundary.GetTop();
		vOrigin[1] = yLim[1];
	}
	else {
		double ratio = abs((double)yLim[1]) / ((double)yLim[1] - (double)yLim[0]);
		origin.y = (int)(ratio * boundary.GetHeight() + boundary.GetY());
		vOrigin[1] = 0;
	}
}

Axis* Plot::getHAxis() {
	return horizAxis;
}

Axis* Plot::getVAxis() {
	return vertAxis;
}

void Plot::draw(wxDC& dc) {
	if (!horizAxis) {
		wxLogError("horizAxis is a null pointer!");
	}
	else
		horizAxis->draw(dc);
	if (!vertAxis) {
		wxLogError("vertAxis is a null pointer!");
	}
	else
		vertAxis->draw(dc);
}