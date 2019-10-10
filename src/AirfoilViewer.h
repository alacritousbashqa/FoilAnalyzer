#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "faProgram.h"
#include "Plot.h"

class ViewerPanel : public wxPanel {
	wxBoxSizer* avTopSizer;
	wxSizerItem* avDrawArea;

	Plot* airfoilPlot;
public:
	wxBoxSizer * getTopSizer();
	ViewerPanel(wxWindow* parent);
	void onPaintEvent(wxPaintEvent& event);
private:
	void onViewerBackButton(wxCommandEvent& event); // Defined in Main.cpp
	//void drawTicks(wxDC& dc, wxPoint& origin, wxPoint& beg, wxPoint& end, int dir, int n);
	//void drawTick(wxDC& dc, wxPoint pos, int dir);
	//void drawAxes(wxPaintDC& dc);
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