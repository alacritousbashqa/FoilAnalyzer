#include "Plot.h"

int DEFAULT_BORDER[4] = { 0,0,0,0 };

Plot::Plot(wxRect& boundary, double xLim[2], double yLim[2], bool showX, bool showY, bool showTitle)
	: Plot(boundary, xLim, yLim, DEFAULT_BORDER){
}

Plot::Plot(wxRect& boundary, double xLim[2], double yLim[2], int border[4], bool showX, bool showY, bool showTitle) {
	this->boundary = boundary;
	int extra = 0;
	if (showTitle) {
		extra = 24;
	}
	this->drawArea.SetTop(boundary.GetTop() + border[1] + extra);
	extra = 0;
	if (showX) {
		extra = 24;
	}
	this->drawArea.SetLeft(boundary.GetLeft() + border[0] + extra);
	extra = 0;
	if (showY) {
		extra = 24;
	}
	this->drawArea.SetBottom(boundary.GetBottom() - border[3] - extra);
	this->drawArea.SetRight(boundary.GetRight() - border[2]);

	boundary.SetLeftTop(boundary.GetTopLeft() + wxPoint(border[3], border[0]));
	boundary.SetBottomRight(boundary.GetBottomRight() - 2 * wxPoint(border[1], border[2]));
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

	this->showLabelX = showX;
	this->showLabelY = showY;
	this->showTitle = showTitle;

	this->title = "Plot";
}

Plot::~Plot() {
	delete horizAxis;
	delete vertAxis;
}

void Plot::calculateOrigin(double xLim[2], double yLim[2]) {
	// X component
	if (xLim[0] >= 0) {							// If (0,0) is off-screen left, put origin on left limit
		origin.x = drawArea.GetLeft() + 1;
		vOrigin[0] = xLim[0];
	}
	else if (xLim[1] <= 0) {					// If (0,0) is off-screen right, put origin on right limit
		origin.x = drawArea.GetRight() - 1;
		vOrigin[0] = xLim[1];
	}
	else {										// Else, find (0,0) with interpolation
		double ratio = abs(xLim[0] / (xLim[1] - xLim[0]));
		origin.x = (int)(ratio * drawArea.GetWidth() + drawArea.GetX());
		vOrigin[0] = 0.0;
	}
	// Y component
	if (yLim[0] >= 0) {							// If (0,0) is off-screen below, put origin on bottom limit
		origin.y = drawArea.GetBottom() - 1;
		vOrigin[1] = yLim[0];
	}
	else if (yLim[1] <= 0) {					// If (0,0) is off-screen above, put origin on top limit
		origin.y = drawArea.GetTop() + 1;
		vOrigin[1] = yLim[1];
	}
	else {										// Else, find (0,0) with interpolation
		double ratio = abs(yLim[1] / (yLim[1] - yLim[0]));
		origin.y = (int)(ratio * drawArea.GetHeight() + drawArea.GetY());
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
	int extra = 0;
	if (showTitle) {
		extra = 24;
	}
	drawArea.SetTop(boundary.GetTop() + border[1] + extra);
	extra = 0;
	if (showLabelX) {
		extra = 24;
	}
	drawArea.SetLeft(boundary.GetLeft() + border[0] + extra);
	extra = 0;
	if (showLabelY) {
		extra = 24;
	}
	drawArea.SetBottom(boundary.GetBottom() - border[3] - extra);
	drawArea.SetRight(boundary.GetRight() - border[2]);
	// Adjust boundaries with padding
	boundary.SetLeftTop(boundary.GetTopLeft() + wxPoint(border[3], border[0]));
	boundary.SetBottomRight(boundary.GetBottomRight() - 2 * wxPoint(border[1], border[2]));
	// Set pixel bounds
	int xBound[2] = { drawArea.GetLeft(), drawArea.GetRight() };
	int yBound[2] = { drawArea.GetTop(), drawArea.GetBottom() };
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
	//dc.DrawRectangle(boundary);
	//dc.DrawRectangle(drawArea);
	dc.SetTextForeground(wxColour(*wxWHITE));
	if (showTitle)
		dc.DrawText(title, wxPoint(drawArea.GetWidth() / 2 + drawArea.GetLeft(), boundary.GetTop() + 5));
	if (showLabelX)
		dc.DrawText(horizAxis->getLabel(), wxPoint(drawArea.GetRight() - 30, origin.y - 26));
	if (showLabelY)
		dc.DrawText(vertAxis->getLabel(), wxPoint(origin.x + 13, drawArea.GetTop() - 9));

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

std::string Plot::getTitle() {
	return title;
}
std::string Plot::getHLabel() {
	return horizAxis->getLabel();
}
std::string Plot::getVLabel() {
	return vertAxis->getLabel();
}

int* Plot::getAspectRatio() {
	return aspectRatio;
}

void Plot::setTitle(std::string t) {
	title = t;
}
void Plot::setHLabel(std::string hl) {
	horizAxis->setLabel(hl);
}
void Plot::setVLabel(std::string vl) {
	vertAxis->setLabel(vl);
}

void Plot::setAspectRatio(int ar[2]) {
	aspectRatio[0] = ar[0];
	aspectRatio[1] = ar[1];
}
void Plot::setAspectRatio(int arx, int ary) {
	aspectRatio[0] = arx;
	aspectRatio[1] = ary;
}