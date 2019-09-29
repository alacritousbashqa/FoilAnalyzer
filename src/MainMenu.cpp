#include <wx/sizer.h>

#include "MainMenu.h"

StartPanel::StartPanel(wxWindow* parent)
	:wxPanel(parent, wxID_ANY) {

	wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

	// Top padding
	topsizer->AddStretchSpacer();

	// Title: static text
	wxStaticText* sText = new wxStaticText(this, wxID_ANY, "Foil Analyzer");
	topsizer->Add(sText, wxSizerFlags().Align(wxALIGN_CENTRE_HORIZONTAL).Border(wxALL, 50));
	wxFont myFont(20, wxFontFamily::wxFONTFAMILY_DECORATIVE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	sText->SetFont(myFont);
	sText->SetForegroundColour(wxColour(*wxWHITE));

	// Viewer, Analyzer, and Exit buttons in horizontal box sizer
	wxBoxSizer *button_box = new wxBoxSizer(wxHORIZONTAL);
	button_box->Add(
		new wxButton(this, wxID_ANY, "Airfoil Viewer"),
		wxSizerFlags().Border(wxALL, 7));
	button_box->Add(
		new wxButton(this, wxID_ANY, "Airfoil Analyzer"),
		wxSizerFlags().Border(wxALL, 7));
	button_box->Add(
		new wxButton(this, EXIT_ID, "Exit"),
		wxSizerFlags().Border(wxALL, 7));

	topsizer->Add(button_box, wxSizerFlags().Center());

	// Padding between center buttons and bottom controls
	topsizer->AddStretchSpacer();

	// About button on bottom right
	topsizer->Add(
		new wxButton(this, wxID_ANY, "About"),
		wxSizerFlags().Border(wxALL, 15).Right());

	this->SetSizer(topsizer);

	// Black background
	SetBackgroundColour(wxColour(*wxBLACK));

	Connect(EXIT_ID, wxEVT_BUTTON, wxCommandEventHandler(StartPanel::OnExitButton));

}


void StartPanel::OnExitButton(wxCommandEvent& event) {
	GetParent()->Close(true);
}

MainMenu::MainMenu(wxFrame* topFrame) {
	InitializeMainMenu(topFrame);
}

bool MainMenu::InitializeMainMenu(wxFrame* topFrame) {
	mainmenuPanel = new StartPanel(topFrame);

	return true;
}