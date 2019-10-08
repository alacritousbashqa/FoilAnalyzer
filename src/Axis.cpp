#include "Axis.h"

	double lims[2] = { -1.0,1.0 };
	Axis::Axis(dir, boundary, origin, vOrigin, lims, 0.1);
double DEFAULT_LIMITS[2] = { -1.0,-1.0 };

Axis::Axis(axisDirection dir, int boundary[2], wxPoint& origin, double vOrigin[2])
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

}
void Axis::calculateVLocs() {
}
void Axis::draw(wxDC& dc) {
	dc.SetPen(wxPen(*wxRED, 1));
	dc.SetTextForeground(*wxRED);
	dc.DrawLine(100, 100, 80, 90);
	//dc.DrawText(std::to_string((int)dir), 50,50);
	//std::map<double, int>::iterator it = valueLocs.begin();

	//if (dir == HORIZONTAL) {
	//	dc.DrawLine(boundary[0], origin.y, boundary[1], origin.y); // Draw axis line
	//	for (; it != valueLocs.end(); ++it) { // Draw axis ticks
	//		dc.DrawLine(it->second, origin.y + 5, it->second, origin.y - 5);
	//		dc.DrawText(std::to_string(it->first).substr(0,4), it->second, origin.y + 7);
	//	}
	//}
	//else {
	//	dc.DrawLine(origin.x, boundary[0], origin.x, boundary[1]);
	//	for (; it != valueLocs.end(); ++it) {
	//		dc.DrawLine(origin.x + 5, it->second, origin.x - 5, it->second);
	//	}
	//}
	
}