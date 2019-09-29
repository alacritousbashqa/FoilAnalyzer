#include <wx/sizer.h>

#include "MainMenu.h"

StartPanel::StartPanel(wxWindow* parent)
	:wxPanel(parent, wxID_ANY) {

	// Sizer controlling the overall layout of the main menu panel
	wxBoxSizer *mmTopSizer = new wxBoxSizer(wxVERTICAL);

	// Top padding
	mmTopSizer->AddStretchSpacer();

	// Title: static text
	wxStaticText* mmTitleText = new wxStaticText(this, wxID_ANY, "Foil Analyzer");
	mmTopSizer->Add(mmTitleText, wxSizerFlags().Align(wxALIGN_CENTRE_HORIZONTAL).Border(wxALL, 50));
	wxFont myFont(20, wxFontFamily::wxFONTFAMILY_DECORATIVE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	mmTitleText->SetFont(myFont);
	mmTitleText->SetForegroundColour(wxColour(*wxWHITE));

	// Viewer, Analyzer, and Exit buttons in horizontal box sizer
	wxBoxSizer *mmButtonBox = new wxBoxSizer(wxHORIZONTAL);
	mmButtonBox->Add(
		new wxButton(this, wxID_ANY, "Airfoil Viewer"),
		wxSizerFlags().Border(wxALL, 7));
	mmButtonBox->Add(
		new wxButton(this, wxID_ANY, "Airfoil Analyzer"),
		wxSizerFlags().Border(wxALL, 7));
	mmButtonBox->Add(
		new wxButton(this, EXIT_ID, "Exit"),
		wxSizerFlags().Border(wxALL, 7));

	mmTopSizer->Add(mmButtonBox, wxSizerFlags().Center());

	// Padding between center buttons and bottom controls
	mmTopSizer->AddStretchSpacer();

	// About button on bottom right
	mmTopSizer->Add(
		new wxButton(this, wxID_ANY, "About"),
		wxSizerFlags().Border(wxALL, 15).Right());

	this->SetSizer(mmTopSizer);

	// Black background
	SetBackgroundColour(wxColour(*wxBLACK));

	// ------ Bind button events to functions ------
	// Exit Button
	Connect(EXIT_ID, wxEVT_BUTTON, wxCommandEventHandler(StartPanel::onExitButton));

}

// Closes application
void StartPanel::onExitButton(wxCommandEvent& event) {
	GetParent()->Close(true);
}

// Wrapper holding information on main menu panel and associated widgets
MainMenu::MainMenu(wxFrame* topFrame) {
	initializeMainMenu(topFrame);
}

// Creates the main menu panel and sets up its widgets
bool MainMenu::initializeMainMenu(wxFrame* topFrame) {
	mainMenuPanel = new StartPanel(topFrame);

	return true;
}