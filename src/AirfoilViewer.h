#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class ViewerPanel : public wxPanel {
public:
	ViewerPanel(wxWindow* parent);
};

class AirfoilViewer {
	ViewerPanel* viewerPanel;
public:
	AirfoilViewer(wxWindow* parent);
	ViewerPanel* getTopPanel();
	bool initializeAirfoilViewer(wxWindow* parent);
	// Shows and enables the viewer top level panel
	void show(bool show = true);
};