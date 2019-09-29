#include "AirfoilViewer.h"

ViewerPanel::ViewerPanel(wxWindow* parent)
	: wxPanel(parent){
}

AirfoilViewer::AirfoilViewer(wxWindow* parent) {
	initializeAirfoilViewer(parent);
}

bool AirfoilViewer::initializeAirfoilViewer(wxWindow* parent) {
	viewerPanel = new ViewerPanel(parent);

	if (viewerPanel)
		return true;
	return false;
}