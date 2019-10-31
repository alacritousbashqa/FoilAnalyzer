#pragma once

#include <wx/wxprec.h>

#include "FoilParameters.h"

enum {
	OVERWRITE_ID = 1,
	CANCEL_OVER_ID = 2
};

class OverwriteNameDialog : public wxDialog {
	int textH = GetTextExtent("A").GetHeight();	// Text height for positioning widgets instead of hard coded positions

	bool overwrite;		// Should the airfoil be overwritten?
	int modalCode;

	// Called when the overwrite button is pressed. Checks and returns the values from the text controls.
	void onOverwrite(wxCommandEvent& event);
	// Called when the cancel button is pressed. Returns invalid values.
	void onCancel(wxCommandEvent& event);

public:
	OverwriteNameDialog(wxWindow* parent, const wxString& title, std::string name);

	bool getOverwite();
};