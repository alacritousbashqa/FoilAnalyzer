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
	wxFont titleFont(20, wxFontFamily::wxFONTFAMILY_DECORATIVE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	mmTitleText->SetFont(titleFont);
	mmTitleText->SetForegroundColour(wxColour(*wxWHITE));

	// Viewer, Analyzer, and Exit buttons in horizontal box sizer
	wxBoxSizer *mmButtonBox = new wxBoxSizer(wxHORIZONTAL);
	mmButtonBox->Add(
		new wxButton(this, VIEWER_BUTTON_ID, "Airfoil Viewer"),
		wxSizerFlags().Border(wxALL, 7));
	wxButton* afAnalyzerB = new wxButton(this, ANALYZER_BUTTON_ID, "Airfoil Analyzer");
	//afAnalyzerB->Disable();
	mmButtonBox->Add(
		afAnalyzerB,
		wxSizerFlags().Border(wxALL, 7));
	mmButtonBox->Add(
		new wxButton(this, EXIT_ID, "Exit"),
		wxSizerFlags().Border(wxALL, 7));

	mmTopSizer->Add(mmButtonBox, wxSizerFlags().Center());

	// Padding between center buttons and bottom controls
	mmTopSizer->AddStretchSpacer();

	// About button on bottom right
	mmTopSizer->Add(
		new wxButton(this, ABOUT_ID, "About"),
		wxSizerFlags().Border(wxALL, 15).Right());

	this->SetSizer(mmTopSizer);

	// Black background
	SetBackgroundColour(wxColour(*wxBLACK));

	// ------ Bind button events to functions ------
	// Exit Button
	Connect(EXIT_ID, wxEVT_BUTTON, wxCommandEventHandler(StartPanel::onExitButton));
	Connect(VIEWER_BUTTON_ID, wxEVT_BUTTON, wxCommandEventHandler(StartPanel::onViewerButton));
	Connect(ANALYZER_BUTTON_ID, wxEVT_BUTTON, wxCommandEventHandler(StartPanel::onAnalyzerButton));
	Connect(ABOUT_ID, wxEVT_BUTTON, wxCommandEventHandler(StartPanel::onAboutButton));



	wxLog* log = new wxLogWindow(NULL, "Log");
	wxLog::SetActiveTarget(log);
}

// Wrapper holding information on main menu panel and associated widgets
MainMenu::MainMenu(wxWindow* topFrame) {
	initializeProgram(topFrame);
}

MainMenu::~MainMenu() {
	delete mainMenuPanel;
}

wxPanel* MainMenu::getTopPanel() {
	return mainMenuPanel;
}

// Creates the main menu panel and sets up its widgets
bool MainMenu::initializeProgram(wxWindow* topFrame) {
	mainMenuPanel = new StartPanel(topFrame);
	if(mainMenuPanel)
		return true;
	return false;
}

void MainMenu::show(bool show) {
	if (mainMenuPanel) {
		mainMenuPanel->Show(show);
		mainMenuPanel->Enable(show);
	}
	else {
		wxLogError("Cannot show main menu! Associated top-level panel does not exist!");
		return;
	}
}