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
#include "Plot.h"

struct AirfoilListStruct {
	AirfoilStruct* airfoil;
	wxCheckBox* checkBox;
	wxColourPickerCtrl* colorPicker;
};

class ViewerPanel : public wxPanel {
	wxBoxSizer* avTopSizer;
	wxSizerItem* avDrawArea;

	wxFlexGridSizer* fgs;
	wxPanel* flexGridPanel;

	wxListView* afListBox;
	AirfoilGenerator* foilGen;

	Plot* airfoilPlot;

	std::vector<AirfoilListStruct> afListMembers;
public:
	wxBoxSizer * getTopSizer();
	ViewerPanel(wxWindow* parent);
	void onPaintEvent(wxPaintEvent& event);
private:
	void onViewerBackButton(wxCommandEvent& event); // Defined in Main.cpp
	void onDefineAirfoil(wxCommandEvent& event);
	void onShowChecked(wxCommandEvent& event);
	//void drawTicks(wxDC& dc, wxPoint& origin, wxPoint& beg, wxPoint& end, int dir, int n);
	//void drawTick(wxDC& dc, wxPoint pos, int dir);
	//void drawAxes(wxPaintDC& dc);
};

// Button IDs for event connection
enum {
	BACK_ID = 2,
	DEFINE_AF_ID = 3
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