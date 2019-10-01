#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "faProgram.h"

class ViewerPanel : public wxPanel {
	wxBoxSizer * avTopSizer;
public:
	wxBoxSizer * getTopSizer();
	ViewerPanel(wxWindow* parent);
private:
	void onViewerBackButton(wxCommandEvent& event); // Defined in Main.cpp
};

// Button IDs for event connection
enum {
	BACK_ID = 2
};

class AirfoilViewer : public faProgram {
	ViewerPanel* viewerPanel;
public:
	AirfoilViewer(wxWindow* parent);
	wxPanel* getTopPanel();
	bool initializeProgram(wxWindow* parent);
	// Shows and enables the viewer top level panel
	void show(bool show = true);
};