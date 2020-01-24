#include "AirfoilAnalyzer.h"

AnalyzerPanel::AnalyzerPanel(wxWindow* parent) 
	: wxPanel(parent, -1, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE) {

	// Sizer that controls the overall layout of the airfoil viewer
	aaTopSizer = new wxBoxSizer(wxHORIZONTAL);

	// Drawing areas for the graphs and airfoil plots
	drawAreaBoxSizer = new wxBoxSizer(wxVERTICAL);
	aaGraphArea = drawAreaBoxSizer->Add(new wxPanel(this,7), wxEXPAND);
	aaAirfoilArea = drawAreaBoxSizer->Add(new wxPanel(this,8), wxEXPAND);

	// Sizer for the options menu
	optionsBoxSizer = new wxBoxSizer(wxVERTICAL);
	wxPanel* optionsPanel = new wxPanel(this, -1, wxDefaultPosition, wxSize(300,500));
	optionsPanel->SetSizer(optionsBoxSizer);
	optionsPanel->SetBackgroundColour(wxColour(*wxLIGHT_GREY));

	aaTopSizer->Add(drawAreaBoxSizer, 1, wxLEFT | wxRIGHT | wxBOTTOM | wxEXPAND, 10);
	aaTopSizer->Add(optionsPanel, 0, wxLEFT | wxRIGHT | wxBOTTOM | wxEXPAND, 10);

	// Box that holds "back to main menu" button at bottom of screen
	wxBoxSizer* buttonBox = new wxBoxSizer(wxHORIZONTAL);
	buttonBox->Add(new wxButton(this, BACK_ID, "Main Menu"), wxSizerFlags().Left());

	drawAreaBoxSizer->Add(buttonBox, 0, wxLEFT | wxBOTTOM, 10);

	this->SetSizer(aaTopSizer);

	// Black Background
	SetBackgroundColour(wxColour(*wxBLACK));

	// Graphs Plot which holds the axes data and draws the Cp graph onto them
	wxRect plotRect(aaGraphArea->GetRect().GetLeft(), aaGraphArea->GetRect().GetTop(), drawAreaBoxSizer->GetSize().GetWidth() - 300, 100);
	double xLim[2] = { -0.1,1.1 };
	double yLim[2] = { -5.0,5.0 };
	int bords[4] = { 10,10,40,10 };
	cpPlot = new Plot(plotRect, xLim, yLim, bords, true, true, true);
	double steps[2] = { 0.10,1.0 };
	cpPlot->setAxesSteps(steps);
	cpPlot->setTitle("Cp vs x/c");
	cpPlot->setHLabel("x/c");
	cpPlot->setVLabel("Cp");
	int ar[] = { 3, 1 };
	cpPlot->setAspectRatio(ar);

	// Graphs Plot which holds the axes data and draws airfoils onto them
	wxRect airfoilRect(aaAirfoilArea->GetRect().GetLeft(), aaAirfoilArea->GetRect().GetTop(), drawAreaBoxSizer->GetSize().GetWidth() - 300, 100);
	yLim[0] = -0.20;
	yLim[1] = 0.20;
	aaAirfoilPlot = new Plot(airfoilRect, xLim, yLim, bords, true, true, true);
	aaAirfoilPlot->setTitle("");
	aaAirfoilPlot->setHLabel("x/c");
	aaAirfoilPlot->setVLabel("y/c");
	aaAirfoilPlot->setAspectRatio(ar);

	// ------ Bind events to functions ------
	// Main Menu Button
	Connect(BACK_ID, wxEVT_BUTTON, wxCommandEventHandler(AnalyzerPanel::onAnalyzerBackButton));
	// Event Paint
	Connect(GetId(), wxEVT_PAINT, wxPaintEventHandler(AnalyzerPanel::onPaintEvent));
}

AnalyzerPanel::~AnalyzerPanel() {

}

// Called on refresh; paints the axes and any loaded airfoils to be shown
void AnalyzerPanel::onPaintEvent(wxPaintEvent& event) {
	wxPaintDC pdc(this);

	// Update the plot on resize and redraw axes and plots
	wxRect plotRect(aaGraphArea->GetRect().GetLeft(), aaGraphArea->GetRect().GetTop(), drawAreaBoxSizer->GetSize().GetWidth() - 50, aaTopSizer->GetSize().GetHeight() / 2);
	cpPlot->updateBoundaries(plotRect);
	cpPlot->draw(pdc);

	wxRect airfoilRect(aaAirfoilArea->GetRect().GetLeft(), aaAirfoilArea->GetRect().GetTop(), drawAreaBoxSizer->GetSize().GetWidth() - 50, aaTopSizer->GetSize().GetHeight() / 2);
	aaAirfoilPlot->updateBoundaries(airfoilRect);
	aaAirfoilPlot->draw(pdc);
}

wxBoxSizer* AnalyzerPanel::getTopSizer() {
	return aaTopSizer;
}

AirfoilAnalyzer::AirfoilAnalyzer(wxWindow* parent) {
	initializeProgram(parent);
}

AirfoilAnalyzer::~AirfoilAnalyzer() {
	delete analyzerPanel;
}

wxPanel* AirfoilAnalyzer::getTopPanel() {
	return analyzerPanel;
}

bool AirfoilAnalyzer::initializeProgram(wxWindow* parent) {
	// Construct the top panel for the airfoil analyzer
	analyzerPanel = new AnalyzerPanel(parent);

	if (analyzerPanel) {
		show(false);
		return true;
	}
	wxLogError("Top level panel could not be constructed!");
	return false;
}

void AirfoilAnalyzer::show(bool show) {
	if (analyzerPanel) {
		analyzerPanel->Show(show);
		analyzerPanel->Enable(show);
	}
	else {
		wxLogError("Cannot show airfoil analyzer! Associated top-level panel does not exist!");
		return;
	}
}