#include "Plot.h"

Plot::Plot(wxRect& boundary, double xLim[2], double yLim[2]) {
	// Default: no borders
	int bords[4] = { 0,0,0,0 };
	Plot::Plot(boundary, xLim, yLim, bords);
}

Plot::Plot(wxRect& boundary, double xLim[2], double yLim[2], int border[4]) {
	Plot::boundary = boundary;
	// Borders
	Plot::border[0] = border[0];
	Plot::border[1] = border[1];
	Plot::border[2] = border[2];
	Plot::border[3] = border[3];

	calculateOrigin(xLim, yLim);

	int xBound[2] = { boundary.GetLeft(), boundary.GetRight() };
	int yBound[2] = { boundary.GetTop(), boundary.GetBottom() };

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

void Plot::draw(wxDC& dc) {
	horizAxis->draw(dc);
	vertAxis->draw(dc);
}