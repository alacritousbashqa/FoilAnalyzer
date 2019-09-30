// Main.cpp
// Handles the initiation of the program and starts the GUI; Start here

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/sizer.h>

#include "MainMenu.h"
#include "AirfoilViewer.h"

class FoilApp : public wxApp {
public:
	virtual bool OnInit();
};

// Frame which stores and shows the panels for each program in the application
class TopFrame : public wxFrame {
public:
	TopFrame(const wxString &title, const wxPoint &pos, const wxSize &size);
	MainMenu* mMenu;
	AirfoilViewer* aViewer;
	wxBoxSizer* topSizer;
	void initializeTopFrame();
	void switchPanels(int panelID);
};

wxIMPLEMENT_APP(FoilApp);
bool FoilApp::OnInit() {

	// Create the top frame and start with the main menu
	TopFrame* topFrame = new TopFrame("FoilAnalysis", wxPoint(0,0), wxSize(1200,700));
	topFrame->initializeTopFrame();

	topFrame->Show(true);
	return true;
}

//===========================================================================================
//
//		FRAME FUNCTIONS
//
//===========================================================================================

TopFrame::TopFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
	: wxFrame(NULL, wxID_ANY, title, pos, size) {

	// Adds basic test menu bar to the frame
	wxMenu *menuFile = new wxMenu;
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT);
	wxMenu *menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT);
	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	menuBar->Append(menuHelp, "&Help");
	SetMenuBar(menuBar);
}

void TopFrame::initializeTopFrame() {
	// Topmost sizer for the program that defines the layout of the top frame
	topSizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(topSizer);

	// Program Classes
	mMenu = new MainMenu(this);
	aViewer = new AirfoilViewer(this);

	// Add the Main Menu
	topSizer->Add(mMenu->getTopPanel(), 1, wxGROW);
}

void TopFrame::switchPanels(int panelID) {
	switch (panelID) {
	case 2:
		mMenu->show(false);
		topSizer->Prepend(aViewer->getTopPanel(), 1, wxGROW);
		aViewer->show();
		break;
	}
}

void StartPanel::onViewerButton(wxCommandEvent& event) {
	dynamic_cast<TopFrame*>(GetParent())->switchPanels(2);
}