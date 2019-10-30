// AirfoilDefiner.h
// Author: Mark Bashqawi

// Dialog widget which asks the user to input a NACA code for airfoil generation.

#pragma once

#include <wx/wxprec.h>

enum {
	OK_ID = 1,
	CANCEL_ID = 2,
	CODE_TEXT_CTRL_ID = 3
};

class AirfoilDefiner : public wxDialog {
	int textH = GetTextExtent("A").GetHeight();

	std::string code;		// NACA code (and later other airfoils)
	std::string name;		// Unique identifier for this airfoil
	int nPanels;			// Number of panels to generate
	int type;				// 4 or 5 digit NACA series
	int modalCode;

	wxTextCtrl* tc;			// Code text control
	wxTextCtrl* tc1;		// Number of panels text control
	wxTextCtrl* nameTC;		// Name text control

	void onCodeChange(wxCommandEvent& event);

	void onOK(wxCommandEvent& event);
	void onCancel(wxCommandEvent& event);
public:
	AirfoilDefiner(const wxString& title);
	std::string getCode();
	std::string getName();
	int getNPanels();
	int getType();
};