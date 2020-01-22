// AirfoilAnalyzer.h
// Author: Mark Bashqawi

// This holds all widgets and data related to the Airfoil Analyzer program which allows the user to define an
// analysis for a loaded airfoil, i.e. a potential flow analysis. It can perform inviscid and viscous 
// calculations at a defined speed, Reynold's Number, angle of attack, etc.

#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "FoilParameters.h"
#include "AirfoilViewer.h"
#include "faProgram.h"
#include "Plot.h"

class AnalyzerPanel : public wxPanel{
	wxBoxSizer* aaTopSizer;
	wxBoxSizer* drawAreaBoxSizer; // Sizer for the graphs and airfoil plot areas
	wxSizerItem* aaGraphArea;
	wxSizerItem* aaAirfoilArea;

	// Scrolled list panel and sizer
	wxBoxSizer* optionsBoxSizer;

	// Pressure Coefficient and airfoil plots
	Plot* cpPlot;
	Plot* aaAirfoilPlot;

public:
	wxBoxSizer* getTopSizer();
	AnalyzerPanel(wxWindow* parent);
	~AnalyzerPanel();
	void onPaintEvent(wxPaintEvent& event);

private:
	// Button and other control events
	void onAnalyzerBackButton(wxCommandEvent& event); // Defined in Main.cpp
};

class AirfoilAnalyzer : public faProgram {
	AnalyzerPanel* analyzerPanel;
public:
	AirfoilAnalyzer(wxWindow* parent);
	~AirfoilAnalyzer();
	wxPanel* getTopPanel();
	bool initializeProgram(wxWindow* parent);
	// Shows and enables the viewer top level panel
	void show(bool show = true);
};