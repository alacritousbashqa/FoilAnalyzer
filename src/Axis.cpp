#include "Axis.h"

Axis::Axis(axisDirection dir, int boundary[2], wxPoint& origin, int vOrigin[2]) {
	double lims[2] = { -1.0,1.0 };
	Axis::Axis(dir, boundary, origin, vOrigin, lims, 0.1);
}

Axis::Axis(axisDirection dir, int boundary[2], wxPoint& origin, int vOrigin[2], double limits[2], double step) {
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
	
}