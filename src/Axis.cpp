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

	calculateVLocs();
}

void Axis::calculateVLocs() {
	valueLocs.clear();
	if (dir == HORIZONTAL) {
		int m_p = (limits[1] - vOrigin[0]) / step;
		int m_n = (vOrigin[0] - limits[0]) / step;
		int np = 0;
		if (m_p)
			np = (boundary[1] - origin.x) / m_p;
		else if (m_n)
			np = (origin.x - boundary[0]) / m_n;
		else {
			wxLogError("Number of ticks is zero! Perhaps the axis limits are not right?");
			return;
		}

		valueLocs[limits[0]] = boundary[0];
		valueLocs[limits[1]] = boundary[1];
		valueLocs[vOrigin[0]] = origin.x;

		for (int i = 1; i <= m_p; i++) {
			valueLocs[vOrigin[0] + i * step] = origin.x + i * np;
		}
		for (int i = -1; i >= -m_n; i--) {
			valueLocs[vOrigin[0] + i * step] = origin.x + i * np;
		}
	}
	else {
		int m_p = (limits[1] - vOrigin[1]) / step;
		int m_n = (vOrigin[1] - limits[0]) / step;
		int np = 0;
		if(m_p)
			np = (boundary[0] - origin.y) / m_p;
		else if (m_n)
			np = (origin.y - boundary[1]) / m_n;
		else {
			wxLogError("Number of ticks is zero! Perhaps the axis limits are not right?");
			return;
		}

		valueLocs[limits[0]] = boundary[1];
		valueLocs[limits[1]] = boundary[0];
		valueLocs[vOrigin[1]] = origin.y;

		for (int i = 1; i <= m_p; i++) {
			valueLocs[vOrigin[1] + i * step] = origin.y + i * np;
		}
		for (int i = -1; i >= -m_n; i--) {
			valueLocs[vOrigin[1] + i * step] = origin.y + i * np;
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

	if (dir == HORIZONTAL) {
		dc.DrawLine(boundary[0], origin.y, boundary[1], origin.y); // Draw axis line
		for (; it != valueLocs.end(); ++it) { // Draw axis ticks
			dc.DrawLine(it->second, origin.y + 5, it->second, origin.y - 5);
			int x_adj = 0;
			int y_adj = 6;
			int label_length = 5;
			if (std::next(it, 1) == valueLocs.end())
				x_adj = -15;
			if (it->first == 0.0){
				x_adj = 7;
				label_length = 1;
			}
			if (it->first > 0)
				label_length = 4;

			dc.DrawText(std::to_string(it->first).substr(0, label_length), it->second + x_adj, origin.y + y_adj);
		}
	}
	else {
		dc.DrawLine(origin.x, boundary[0], origin.x, boundary[1]);
		for (; it != valueLocs.end(); ++it) {
			dc.DrawLine(origin.x + 5, it->second, origin.x - 5, it->second);
			int x_adj = -32;
			int y_adj = 0;
			int label_length = 5;
			if (it->first == 0.0) { // Don't draw the label because it is already drawn for the horizontal axis
				continue;
			}
			if (it == valueLocs.begin())
				y_adj = -13;
			if (it->first > 0) {
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