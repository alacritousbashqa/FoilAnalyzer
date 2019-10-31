#include "AirfoilDefiner.h"

AirfoilDefiner::AirfoilDefiner(const wxString& title)
	: wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(200, 230)) {

	// Default values
	type = -1;
	nPanels = 50;
	newListItem = true;

	//Panel that holds the static boxes and text controls
	wxPanel *panel = new wxPanel(this, -1);

	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

	// Static box containing the code and number of panels text controls
	wxStaticBox *st = new wxStaticBox(panel, -1, wxT("NACA Type"),
		wxPoint(5, 5), wxSize(170, textH * 9 + 20));
	wxStaticText stext(panel, -1, wxT("Enter a NACA 4 or 5 digit code:"),
		wxPoint(15, 2 * textH), wxSize(150, 32));
	tc = new wxTextCtrl(panel, CODE_TEXT_CTRL_ID, wxT(""),
		wxPoint(15, 4.5f * textH), wxSize(150,24));
	tc->SetMaxLength(15); // Set code max length
	wxStaticText stext1(panel, -1, wxT("Number of Panels:"),
		wxPoint(15, 7 * textH), wxSize(150, 20));
	tc1 = new wxTextCtrl(panel, -1, std::to_string(nPanels).c_str(),
		wxPoint(15, 8.5f * textH), wxSize(50, 24));
	tc1->SetMaxLength(4); // Set number of panels max length

	// Static box containing the unique identifier (the name) text control
	wxStaticBox *st1 = new wxStaticBox(panel, -1, wxT("Name"),
		wxPoint(5, 11 * textH), wxSize(170, textH * 7));
	wxStaticText stext2(panel, -1, wxT("Enter a unique identifier for this airfoil:"),
		wxPoint(15, 13 * textH), wxSize(150, 32));
	nameTC = new wxTextCtrl(panel, -1, wxT(""),
		wxPoint(15, 15.5f * textH), wxSize(150, 24));
	nameTC->SetMaxLength(15); // Set max length of the name
	
	// Buttons
	wxButton *okButton = new wxButton(this, OK_ID, wxT("Ok"),
		wxDefaultPosition, wxSize(70, 30));
	wxButton *closeButton = new wxButton(this, CANCEL_ID, wxT("Cancel"),
		wxDefaultPosition, wxSize(70, 30));
	
	// Set final size of dialog
	this->SetSize(200, 25 * textH);

	hbox->Add(okButton, 1);
	hbox->Add(closeButton, 1, wxLEFT, 5);

	vbox->Add(panel, 1);
	vbox->Add(hbox, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

	SetSizer(vbox);

	// Connect the buttons to their functions
	Connect(OK_ID, wxEVT_BUTTON, wxCommandEventHandler(AirfoilDefiner::onOK));
	Connect(CANCEL_ID, wxEVT_BUTTON, wxCommandEventHandler(AirfoilDefiner::onCancel));
	// Connect onTextChanged from the code text control to its function
	Connect(CODE_TEXT_CTRL_ID, wxEVT_TEXT, wxCommandEventHandler(AirfoilDefiner::onCodeChange));

	// Center and set focus to the code text control
	Centre();
	modalCode = ShowModal(); // Pause everything except this dialog
	tc->SetFocus();

	Destroy(); // Only gets called after EndModal is called
}

void AirfoilDefiner::onOK(wxCommandEvent& event) {
	code = tc->GetLineText(0);
	std::string tmp = (std::string)tc1->GetLineText(0);
	name = nameTC->GetLineText(0);

	// If number of panels text is not empty and is numeric, set nPanels
	if (tmp != "" && std::all_of(tmp.begin(), tmp.end(), ::isdigit)) {
		nPanels = std::stoi(tmp);
	}
	// Else, set it to an invalid number
	else {
		nPanels = -1;
	}
	// If the name is empty, give an error and return
	if (name == "") {
		wxLogError("No name was entered! Please enter a name for the generated airfoil.");
		return;
	}

	// If the code text is not empty and is numeric...
	if (code != "" && std::all_of(code.begin(), code.end(), ::isdigit)) {
		// If the number of panels is not in the valid range, return with an error 
		if (nPanels < 10 || nPanels > 5000) {
			nPanels = -1;
			wxLogError("An invalid number of panels was entered! Number of panels must be between 10 and 5000!");
			return;
		}
		// If the code is a 4 digit, close dialog
		if (code.length() == 4) {
			type = 4;
			EndModal(modalCode);
		}
		// If the code is a 5 digit, close dialog
		else if (code.length() == 5) {
			type = 5;
			EndModal(modalCode);
		}
		// Else, return an invalid type with error
		else {
			type = -1;
			wxLogError("An invalid length was entered! Please use 4 or 5 digit series only!");
		}
	}
	// Else, return an invalid type with error
	else {
		type = -1;
		wxLogError("Invalid code! Code should be only numbers and be 4 or 5 digits long!");
	}
}

AirfoilStruct* AirfoilDefiner::checkNameUniqueness(std::string name) {
	for (AirfoilStruct* afs : loadedAirfoils) {
		if (afs->name == name) {
			return afs; // Not unique
		}
	}
	return nullptr; // Unique
}

void AirfoilDefiner::onCancel(wxCommandEvent& event) {
	EndModal(modalCode); // Close dialog
}

void AirfoilDefiner::onCodeChange(wxCommandEvent& event) {
	// Clamp text length to max allowed characters
	std::string c = (std::string)tc->GetLineText(0);
	if (c.length() > 10)
		c = c.substr(0, 10);
	// Set name text from code
	nameTC->SetValue("NACA " + c);
}

std::string AirfoilDefiner::getCode() {
	return code;
}

std::string AirfoilDefiner::getName() {
	return name;
}

int AirfoilDefiner::getNPanels() {
	return nPanels;
}

int AirfoilDefiner::getType() {
	return type;
}

bool AirfoilDefiner::getNewItem() {
	return newListItem;
}