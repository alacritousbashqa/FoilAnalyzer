#include "AirfoilViewer.h"

ViewerPanel::ViewerPanel(wxWindow* parent)
	: wxPanel(parent){

	// Sizer that controls the overall layout of the airfoil viewer
	avTopSizer = new wxBoxSizer(wxVERTICAL);

	// Drawing area for the airfoil plot
	avDrawArea = avTopSizer->Add(new wxPanel(this,7), wxEXPAND);
	// List to hold loaded airfoils
	wxListBox* airfoilListBox = new wxListBox(this, -1);
	avTopSizer->Add(airfoilListBox, 0, wxEXPAND);

	wxBoxSizer* buttonBox = new wxBoxSizer(wxHORIZONTAL);
	buttonBox->Add(new wxButton(this, BACK_ID, "Main Menu"), wxSizerFlags().Left());

	avTopSizer->Add(buttonBox);

	this->SetSizer(avTopSizer);

	// Black Background
	SetBackgroundColour(wxColour(*wxBLACK));

	// ------ Bind button events to functions ------
	// Main Menu Button
	Connect(BACK_ID, wxEVT_BUTTON, wxCommandEventHandler(ViewerPanel::onViewerBackButton));
	Connect(GetId(), wxEVT_PAINT, wxPaintEventHandler(ViewerPanel::onPaintEvent));
}

wxBoxSizer* ViewerPanel::getTopSizer() {
	return avTopSizer;
}

void ViewerPanel::onPaintEvent(wxPaintEvent& event) {
	wxPaintDC pdc(this);
	drawAxes(pdc);
}

void ViewerPanel::drawAxes(wxPaintDC& dc) {
}

AirfoilViewer::AirfoilViewer(wxWindow* parent) {
	initializeProgram(parent);
}

wxPanel* AirfoilViewer::getTopPanel() {
	return viewerPanel;
}

bool AirfoilViewer::initializeProgram(wxWindow* parent) {
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