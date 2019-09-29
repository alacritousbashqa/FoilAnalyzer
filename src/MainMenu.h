#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#define TITLE "Foil Analyzer"

/* Upper level panel that holds the layout for the main menu.*/
class StartPanel : public wxPanel{
public:
	StartPanel(wxWindow* parent);
private:
	void onExitButton(wxCommandEvent& event);
};

enum {
	EXIT_ID = 3
};

/* Constructs the layout of the main menu panel upon app initialization.
It also stores a reference to the main menu panel.*/
class MainMenu {
	StartPanel* mainMenuPanel;
public:
	MainMenu(wxFrame* topFrame);
	bool initializeMainMenu(wxFrame* topFrame);
};