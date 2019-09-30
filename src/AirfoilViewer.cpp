#include "AirfoilViewer.h"

ViewerPanel::ViewerPanel(wxWindow* parent)
	: wxPanel(parent){

	// Sizer that controls the overall layout of the airfoil viewer
	wxBoxSizer* avTopSizer = new wxBoxSizer(wxVERTICAL);

	// Drawing area for the airfoil plot
	wxSizerItem* avDrawArea = avTopSizer->Add(new wxPanel(this,-1,wxDefaultPosition,wxSize(800,500)), wxSizerFlags().Center());
	// List to hold loaded airfoils
	wxListBox* airfoilListBox = new wxListBox(this, -1);
	avTopSizer->Add(airfoilListBox, wxSizerFlags().Expand());

	this->SetSizer(avTopSizer);

}

AirfoilViewer::AirfoilViewer(wxWindow* parent) {
	initializeAirfoilViewer(parent);
}

ViewerPanel* AirfoilViewer::getTopPanel() {
	return viewerPanel;
}

bool AirfoilViewer::initializeAirfoilViewer(wxWindow* parent) {
	viewerPanel = new ViewerPanel(parent);

	if (viewerPanel) {
		show(false);
		return true;
	}
	return false;
}

void AirfoilViewer::show(bool show) {
	if (viewerPanel) {
		viewerPanel->Show(show);
		viewerPanel->Enable(show);
	}
	else
		return; // Add error reporting here
}