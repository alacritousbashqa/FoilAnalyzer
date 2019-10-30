// AirfoilDefiner.h
// Author: Mark Bashqawi

// Dialog widget which asks the user to input a NACA code for airfoil generation.

#pragma once

#include <wx/wxprec.h>

class AirfoilDefiner : public wxDialog {
	int textH = GetTextExtent("A").GetHeight();

	std::string text;
	int nPanels;
	int type;
	int modalCode;

	wxRadioButton* rb4;
	wxRadioButton* rb5;
	wxTextCtrl* tc;
	wxTextCtrl* tc1;

	void onOK(wxCommandEvent& event);
	void onCancel(wxCommandEvent& event);
public:
	AirfoilDefiner(const wxString& title);
	std::string getText();
	int getNPanels();
	int getType();
};