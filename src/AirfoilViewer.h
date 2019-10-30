// AirfoilViewer.h
// Author: Mark Bashqawi

// This holds all widgets and data related to the Airfoil Viewer program which allows the user to define an
// airfoil based on a NACA code and draw it to a normalized plot. Multiple airfoils can be loaded and each
// can be toggled to show on the plot and can have their color changed.

#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/listctrl.h>
#include <wx/clrpicker.h>

#include "FoilParameters.h"
#include "AirfoilGenerator.h"
#include "faProgram.h"
#include "FoilParameters.h"
#include "Plot.h"

// Airfoil List Struct: holds reference to an airfoil struct and its associated checkbox and color picker for use in the list
struct AirfoilListStruct {
	AirfoilStruct* airfoil;
	wxCheckBox* checkBox;
	wxColourPickerCtrl* colorPicker;
};

class ViewerPanel : public wxPanel {
	wxBoxSizer* avTopSizer;
	wxSizerItem* avDrawArea;

	// Scrolled list panel and sizer
	wxScrolledWindow* scrolledWindow;
	wxBoxSizer* scrolledBoxSizer;

	// Performs the math to generate the points of an airfoil from a NACA code
	AirfoilGenerator* foilGen;

	Plot* airfoilPlot;

	// Master list of airfoil lst structs
	std::vector<AirfoilListStruct> afListMembers;
public:
	wxBoxSizer * getTopSizer();
	ViewerPanel(wxWindow* parent);
	~ViewerPanel();
	void onPaintEvent(wxPaintEvent& event);
private:
	void onViewerBackButton(wxCommandEvent& event); // Defined in Main.cpp
	void onDefineAirfoil(wxCommandEvent& event);
	void onShowChecked(wxCommandEvent& event);
	void onColorPicked(wxColourPickerEvent& event);
	//void drawTicks(wxDC& dc, wxPoint& origin, wxPoint& beg, wxPoint& end, int dir, int n);
	//void drawTick(wxDC& dc, wxPoint pos, int dir);
	//void drawAxes(wxPaintDC& dc);
};

// Button IDs for event connection
enum {
	BACK_ID = 2,
	DEFINE_AF_ID = 3,
	CHECKBOXES_ID = 4,
	COLORPICKER_ID = 5
};

class AirfoilViewer : public faProgram {
	ViewerPanel* viewerPanel;
public:
	AirfoilViewer(wxWindow* parent);
	~AirfoilViewer();
	wxPanel* getTopPanel();
	bool initializeProgram(wxWindow* parent);
	// Shows and enables the viewer top level panel
	void show(bool show = true);
};
