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
		new wxButton(this, wxID_ANY, "Exit"),
		wxSizerFlags().Border(wxALL, 7));

	topsizer->Add(button_box, wxSizerFlags().Center());

	// Bottom padding
	topsizer->AddStretchSpacer();

	this->SetSizer(topsizer);

	topsizer->SetSizeHints(this);

	SetBackgroundColour(wxColour(*wxBLACK)); // Black background

}

MainMenu::MainMenu(wxFrame* topFrame) {
	InitializeMainMenu(topFrame);
}

bool MainMenu::InitializeMainMenu(wxFrame* topFrame) {
	mainmenuPanel = new StartPanel(topFrame);

	return true;
}