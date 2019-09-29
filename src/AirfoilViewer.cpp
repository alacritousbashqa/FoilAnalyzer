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

bool AirfoilViewer::initializeAirfoilViewer(wxWindow* parent) {
	viewerPanel = new ViewerPanel(parent);

	if (viewerPanel)
		return true;
	return false;
}