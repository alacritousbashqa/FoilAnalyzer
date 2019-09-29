#include "AirfoilViewer.h"

ViewerPanel::ViewerPanel(wxWindow* parent) {

}

AirfoilViewer::AirfoilViewer(wxWindow* parent) {

}

bool AirfoilViewer::initializeAirfoilViewer(wxWindow* parent) {
	viewerPanel = new ViewerPanel(parent);

	if (viewerPanel)
		return true;
	return false;
}