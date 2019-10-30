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
	std::string name;		// Unique identifier for this airfoil
	int modalCode;

	wxTextCtrl* tc;
	wxTextCtrl* tc1;
	wxTextCtrl* nameTC;		// Name text control

	void onOK(wxCommandEvent& event);
	void onCancel(wxCommandEvent& event);
public:
	AirfoilDefiner(const wxString& title);
	std::string getCode();
	std::string getName();
	int getNPanels();
	int getType();
};