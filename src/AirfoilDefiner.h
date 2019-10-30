// AirfoilDefiner.h
// Author: Mark Bashqawi

// Dialog widget which asks the user to input a NACA code for airfoil generation.

#pragma once

#include <wx/wxprec.h>

class AirfoilDefiner : public wxDialog {
	int textH = GetTextExtent("A").GetHeight();

	int nPanels;
	int type;
	std::string code;		// NACA code (and later other airfoils)
	int modalCode;

	wxTextCtrl* tc;
	wxTextCtrl* tc1;

	void onOK(wxCommandEvent& event);
	void onCancel(wxCommandEvent& event);
public:
	AirfoilDefiner(const wxString& title);
	std::string getCode();
	int getNPanels();
	int getType();
};