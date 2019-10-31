// AirfoilDefiner.h
// Author: Mark Bashqawi

// Dialog widget which asks the user to input a NACA code for airfoil generation.

#pragma once

#include <wx/wxprec.h>

#include "AirfoilGenerator.h"
#include "FoilParameters.h"

enum {
	OK_ID = 1,
	CANCEL_ID = 2,
	CODE_TEXT_CTRL_ID = 3
};

class AirfoilDefiner : public wxDialog {
	int textH = GetTextExtent("A").GetHeight();	// Text height for positioning widgets instead of hard coded positions

	std::string code;		// NACA code (and later other airfoils)
	std::string name;		// Unique identifier for this airfoil
	int nPanels;			// Number of panels to generate
	int type;				// 4 or 5 digit NACA series
	int modalCode;

	wxTextCtrl* tc;			// Code text control
	wxTextCtrl* tc1;		// Number of panels text control
	wxTextCtrl* nameTC;		// Name text control

	// Checks if the entered name already exists in the loaded airfoils list. Returns false if it already exists, true otherwise.
	AirfoilStruct* checkNameUniqueness(std::string name);

	/* Called when the code text is changed by either the user or by code. 
	Sets the name based on the entered code in the form: NACA {code}.
	*/
	void onCodeChange(wxCommandEvent& event);

	// Called when the OK button is pressed. Checks and returns the values from the text controls.
	void onOK(wxCommandEvent& event);
	// Called when the cancel button is pressed. Returns invalid values.
	void onCancel(wxCommandEvent& event);
public:
	//-------------------------------------------------------------------------------------------------------------
	// CONSTRUCTOR

	AirfoilDefiner(const wxString& title);

	//-------------------------------------------------------------------------------------------------------------
	// GETTERS

	// Returns the airfoil code entered in the top text box
	std::string getCode();
	// Returns the airfoil name entered in the bottom text box
	std::string getName();
	/* Returns the number of panels specified in the middle text box.
	-1 is returned when the entered number is not valid
	*/
	int getNPanels();
	/* Returns the NACA series based on the length of the code text (either 4 or 5 digit series).
	-1 is returned when the type is not valid (neither 4 or 5)
	*/
	int getType();
	//-------------------------------------------------------------------------------------------------------------
};