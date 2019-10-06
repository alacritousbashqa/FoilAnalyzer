#include "Axis.h"

Axis::Axis(axisDirection dir, int boundary[2], wxPoint& origin, int vOrigin[2]) {
	double lims[2] = { -1.0,1.0 };
	Axis::Axis(dir, boundary, origin, vOrigin, lims, 0.1);
}

Axis::Axis(axisDirection dir, int boundary[2], wxPoint& origin, int vOrigin[2], double limits[2], double step) {
	Axis::dir = dir;
	Axis::boundary[0] = boundary[0];
	Axis::boundary[1] = boundary[1];
	Axis::origin = origin;
	Axis::vOrigin[0] = vOrigin[0];
	Axis::vOrigin[1] = vOrigin[1];
	Axis::limits[0] = limits[0];
	Axis::limits[1] = limits[1];
	Axis::step = step;

}
void Axis::calculateVLocs() {
}
void Axis::draw(wxDC& dc) {
	
}