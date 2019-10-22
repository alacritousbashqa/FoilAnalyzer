#pragma once

#include <wx/wxprec.h>

class AirfoilDefiner : public wxDialog {
	std::string text;
	int type;
	int modalCode;

	wxRadioButton* rb4;
	wxRadioButton* rb5;
	wxTextCtrl* tc;

	void onOK(wxCommandEvent& event);
	void onCancel(wxCommandEvent& event);
public:
	AirfoilDefiner(const wxString& title);
	std::string getText();
	int getType();
};