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


	int xBound[2] = { boundary.GetLeft(), boundary.GetRight() };
	int yBound[2] = { boundary.GetTop(), boundary.GetBottom() };

}

void Plot::calculateOrigin(double xLim[2], double yLim[2]) {
}
void Plot::draw(wxDC& dc) {
	horizAxis->draw(dc);
	vertAxis->draw(dc);
}