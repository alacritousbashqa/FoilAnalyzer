#include "Plot.h"

int DEFAULT_BORDER[4] = { 0,0,0,0 };

Plot::Plot(wxRect& boundary, double xLim[2], double yLim[2]) 
	: Plot(boundary, xLim, yLim, DEFAULT_BORDER){
}

Plot::Plot(wxRect& boundary, double xLim[2], double yLim[2], int border[4]) {
	this->boundary = boundary;
	this->boundary.SetLeftTop(boundary.GetTopLeft() + wxPoint(border[3], border[0]));
	this->boundary.SetBottomRight(boundary.GetBottomRight() - 2 * wxPoint(border[1], border[2]));
	// Borders
	this->border[0] = border[0];
	this->border[1] = border[1];
	this->border[2] = border[2];
	this->border[3] = border[3];

	// Get the origin location and value
	calculateOrigin(xLim, yLim);

	// Get the axes bounds
	int xBound[2] = { boundary.GetLeft(), boundary.GetRight() };
	int yBound[2] = { boundary.GetTop(), boundary.GetBottom() };

	// Create axes
	horizAxis = new Axis(axisDirection::HORIZONTAL, xBound, origin, vOrigin, xLim, 0.1);
	vertAxis = new Axis(axisDirection::VERTICAL, yBound, origin, vOrigin, yLim, 0.025);
}

void Plot::calculateOrigin(double xLim[2], double yLim[2]) {
	// X component
	if (xLim[0] >= 0) {							// If (0,0) is off-screen left, put origin on left limit
		origin.x = boundary.GetLeft() + 1;
		vOrigin[0] = xLim[0];
	}
	else if (xLim[1] <= 0) {					// If (0,0) is off-screen right, put origin on right limit
		origin.x = boundary.GetRight() - 1;
		vOrigin[0] = xLim[1];
	}
	else {										// Else, find (0,0) with interpolation
		double ratio = abs(xLim[0] / (xLim[1] - xLim[0]));
		origin.x = (int)(ratio * boundary.GetWidth() + boundary.GetX());
		vOrigin[0] = 0.0;
	}
	// Y component
	if (yLim[0] >= 0) {							// If (0,0) is off-screen below, put origin on bottom limit
		origin.y = boundary.GetBottom() - 1;
		vOrigin[1] = yLim[0];
	}
	else if (yLim[1] <= 0) {					// If (0,0) is off-screen above, put origin on top limit
		origin.y = boundary.GetTop() + 1;
		vOrigin[1] = yLim[1];
	}
	else {										// Else, find (0,0) with interpolation
		double ratio = abs(yLim[1] / (yLim[1] - yLim[0]));
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

void Plot::updateBoundaries(wxRect& rect) {
	boundary = rect;
	// Adjust draw area with padding
	boundary.SetLeftTop(boundary.GetTopLeft() + wxPoint(border[3], border[0]));
	boundary.SetBottomRight(boundary.GetBottomRight() - 2 * wxPoint(border[1], border[2]));
	// Set pixel bounds
	int xBound[2] = { boundary.GetLeft(), boundary.GetRight() };
	int yBound[2] = { boundary.GetTop(), boundary.GetBottom() };
	// Set value bounds
	double xLim[2] = { horizAxis->getLowerLimit(), horizAxis->getUpperLimit() };
	double yLim[2] = { vertAxis->getLowerLimit(), vertAxis->getUpperLimit() };
	// Recalculate origin
	calculateOrigin(xLim, yLim);
	// Update axes
	horizAxis->updateAxis(xBound,origin,vOrigin);
	vertAxis->updateAxis(yBound, origin, vOrigin);
}

void Plot::draw(wxDC& dc) {
	// Tell axes to draw, assuming they exist
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

	// Draw white border
	dc.SetPen(wxPen(*wxWHITE,1));
	dc.SetBrush(*wxTRANSPARENT_BRUSH);
	dc.DrawRectangle(boundary);
}

void Plot::drawPoints(wxDC& dc, arma::mat points) {
	if (points.n_rows <= 1) {
		wxLogError("Cannot plot points! There are too few of them (e.g. <= 1)!");
		return;
	}

	// Convert points to pixels, then draw
	arma::umat pixels = pointsToPixels(points);
	for (int i = 0; i < pixels.n_rows-1; i++) {
		dc.DrawLine(pixels(i, 0), pixels(i, 1), pixels(i + 1, 0), pixels(i + 1, 1));
	}
}

void Plot::drawPoints(wxDC& dc, arma::umat points) {
	if (points.n_rows <= 1) {
		wxLogError("Cannot plot points! There are too few of them (e.g. <= 1)!");
		return;
	}

	// Draw given points
	for (int i = 0; i < points.n_rows - 1; i++) {
		dc.DrawLine(points(i, 0), points(i, 1), points(i + 1, 0), points(i + 1, 1));
	}
}

arma::umat Plot::pointsToPixels(arma::mat points) {
	// Assuming x and y in cols 0 and 1, respectively

	arma::umat pixels(size(points), arma::fill::zeros);
	std::map<double, int> hMap = horizAxis->getVLocs();
	std::map<double, int> vMap = vertAxis->getVLocs();

	std::map<double, int>::iterator it;
	for (int i = 0; i < points.n_rows; i++) {
		it = hMap.begin();
		for (; it != hMap.end(); ++it) {
			if (it->first <= points(i, 0)) {
				double xx1 = it->first;
				int x1 = it->second;
				it++;
				double xx2 = it->first;
				int x2 = it->second;

				pixels(i, 0) = ((points(i, 0) - xx1) / (xx2 - xx1))*(x2 - x1) + x1;
				break;
			}
		}

		it = vMap.begin();
		for (; it != vMap.end(); ++it) {
			if (it->first <= points(i, 1)) {
				double yy1 = it->first;
				int y1 = it->second;
				it++;
				double yy2 = it->first;
				int y2 = it->second;

				pixels(i, 1) = ((points(i, 1) - yy1) / (yy2 - yy1))*(y2 - y1) + y1;
				break;
			}
		}
	}

	return pixels;
}