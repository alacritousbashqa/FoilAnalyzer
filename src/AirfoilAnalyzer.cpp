#include <wx/statline.h>

#include "AirfoilAnalyzer.h"

//std::vector<AirfoilStruct*> loadedAirfoils;

AnalyzerPanel::AnalyzerPanel(wxWindow* parent) 
	: wxPanel(parent, -1, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE) {

	// Sizer that controls the overall layout of the airfoil viewer
	aaTopSizer = new wxBoxSizer(wxHORIZONTAL);

	// Drawing areas for the graphs and airfoil plots
	drawAreaBoxSizer = new wxBoxSizer(wxVERTICAL);
	aaGraphArea = drawAreaBoxSizer->Add(new wxPanel(this,7), wxEXPAND);
	aaAirfoilArea = drawAreaBoxSizer->Add(new wxPanel(this,8), wxEXPAND);

	//-----------------------    OPTIONS PANEL    ------------------------------------------------------
	optionsBoxSizer = new wxBoxSizer(wxVERTICAL);
	wxPanel* optionsPanel = new wxPanel(this, -1, wxDefaultPosition, wxSize(300,500));
	optionsPanel->SetSizer(optionsBoxSizer);
	optionsPanel->SetBackgroundColour(wxColour(*wxLIGHT_GREY));

	optionsBoxSizer->Add(new wxStaticText(optionsPanel, -1, "Analysis Options", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL), 0, wxALIGN_CENTER | wxTOP, 10);	// Box Title
	optionsBoxSizer->Add(new wxStaticLine(optionsPanel, -1, wxDefaultPosition, wxSize(270, 2)), 0, wxALIGN_CENTER);																// Title Bar
		// AIRFOIL SELECTION
	wxStaticBoxSizer *airfoilBox = new wxStaticBoxSizer(wxVERTICAL, optionsPanel, "Airfoil");
	optionsBoxSizer->Add(airfoilBox, 0, wxALIGN_CENTER | wxTOP, 15);
	airfoilBox->Add(new wxStaticText(airfoilBox->GetStaticBox(), -1, "Choose thine foil:"));
	airfoilChoice = new wxChoice(airfoilBox->GetStaticBox(), CHOICE_ID);
	airfoilBox->Add(airfoilChoice);
		// ANALYSIS DEFINITION
	wxStaticBoxSizer *analysisBox = new wxStaticBoxSizer(wxVERTICAL, optionsPanel, "Analysis Definition");
	optionsBoxSizer->Add(analysisBox, 0, wxALIGN_CENTER | wxTOP, 15);
	analysisBox->Add(new wxStaticText(analysisBox->GetStaticBox(), -1, "Setup analysis parameters here"));

	optionsBoxSizer->AddStretchSpacer();																																		// Space between options and bottom button
	optionsBoxSizer->Add(new wxButton(optionsPanel, -1, "Run Analysis"), 0, wxALIGN_CENTER | wxBOTTOM, 10);																		// Run button
	//--------------------------------------------------------------------------------------------------

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
	// Airfoil Choice Box
	Connect(CHOICE_ID, wxEVT_CHOICE, wxCommandEventHandler(AnalyzerPanel::onChoiceChanged));
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

	if (chosenAirfoil) {
		pdc.SetPen(wxPen(wxColour(*wxWHITE), 1));
		aaAirfoilPlot->drawPoints(pdc, chosenAirfoil->points);
	}
}

void AnalyzerPanel::onChoiceChanged(wxCommandEvent& event) {
	std::string name = airfoilChoice->GetString(airfoilChoice->GetSelection()).ToStdString();
	chosenAirfoil = getAirfoilFromName(name);
	this->Refresh();
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

void AnalyzerPanel::updateChoiceList() {
	airfoilChoice->Clear();
	for (int i = 0; i < loadedAirfoils.size(); i++) {
		airfoilChoice->Append(loadedAirfoils[i]->name);
	}
	if(chosenAirfoil)
		airfoilChoice->SetStringSelection(chosenAirfoil->name);
}

void AirfoilAnalyzer::show(bool show) {
	if (analyzerPanel) {
		analyzerPanel->Show(show);
		analyzerPanel->Enable(show);
		analyzerPanel->updateChoiceList();
	}
	else {
		wxLogError("Cannot show airfoil analyzer! Associated top-level panel does not exist!");
		return;
	}
}