// Main.cpp
// Handles the initiation of the program and starts the GUI; Start here

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/sizer.h>

#include "MainMenu.h"
#include "AirfoilViewer.h"

// Holds IDs for each program panel to help in panel switching
enum ProgPanelIDs {
	MAIN_MENU_ID = 1,
	VIEWER_ID = 2,
	ANALYZER_ID = 3
};

class FoilApp : public wxApp {
public:
	virtual bool OnInit();
};

// Frame which stores and shows the panels for each program in the application
class TopFrame : public wxFrame {
public:
	TopFrame(const wxString &title, const wxPoint &pos, const wxSize &size);
	~TopFrame();

	// Program class variables
	MainMenu* mMenu;
	AirfoilViewer* aViewer;
	wxBoxSizer* topSizer;
	faProgram* currentProgram;
	wxMenuBar *menuBar;
	wxMenu *menuAfPlot = new wxMenu;

	void initializeTopFrame();
	void switchPanels(int panelID);
};

wxIMPLEMENT_APP(FoilApp);
bool FoilApp::OnInit() {

	// Create the top frame
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
	menuFile->Append(wxID_EXIT);
	wxMenu *menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT);
	menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	menuBar->Append(menuHelp, "&Help");
	SetMenuBar(menuBar);

	Connect(wxID_EXIT, wxEVT_MENU, wxCommandEventHandler(StartPanel::onExitButton));
	Connect(wxID_ABOUT, wxEVT_MENU, wxCommandEventHandler(StartPanel::onAboutButton));
}

TopFrame::~TopFrame() {
	delete mMenu;
	delete aViewer;
}

// Creates the top sizer, main menu, and initializes the program classes
void TopFrame::initializeTopFrame() {
	// Topmost sizer for the program that defines the layout of the top frame
	topSizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(topSizer);

	// Program Classes
	mMenu = new MainMenu(this);
	aViewer = new AirfoilViewer(this);

	// Add the program panels; only the Main Menu is visible by default
	topSizer->Add(mMenu->getTopPanel(), 1, wxGROW);
	topSizer->Add(aViewer->getTopPanel(), 1, wxGROW);

	currentProgram = mMenu;
}

// Hides the other panels and shows the panel of the specified ID
void TopFrame::switchPanels(int panelID) {
	switch (panelID) {
	case MAIN_MENU_ID:
		aViewer->show(false);
		mMenu->show();
		topSizer->Layout();
		currentProgram = mMenu;
		menuBar->Remove(menuBar->FindMenu("Plot"));
		break;
	case VIEWER_ID:
		mMenu->show(false);
		aViewer->show();
		topSizer->Layout();
		currentProgram = aViewer;
		menuBar->Append(menuAfPlot, "&Plot");
		break;
	}
}

//===========================================================================================
//
//		START PANEL BUTTON BINDINGS
//
//===========================================================================================

void StartPanel::onAboutButton(wxCommandEvent& event) {
	wxMessageBox(wxT("FoilAnalyzer is a program that can plot NACA 4 and 5 digit airfoils based on a user \
	inputted code. The goal of this project is to be able to perform potential panel and viscous flow calculations\
	 on the generated airfoils to produce polars and pressure distributions."), "About", wxOK | wxICON_INFORMATION);
}

// Closes application
void StartPanel::onExitButton(wxCommandEvent& event) {
	GetParent()->Close(true);
}

//===========================================================================================
//
//		PANEL SWITCHING BIND FUNCTIONS
//
//===========================================================================================

// When the 'Airfoil Viewer' button is pressed in the main menu, show the viewer program
void StartPanel::onViewerButton(wxCommandEvent& event) {
	dynamic_cast<TopFrame*>(GetParent())->switchPanels(VIEWER_ID);
}

// When the 'Main Menu' button is pressed in the airfoil viewer, show the main menu
void ViewerPanel::onViewerBackButton(wxCommandEvent& event) {
	dynamic_cast<TopFrame*>(GetParent())->switchPanels(MAIN_MENU_ID);
}