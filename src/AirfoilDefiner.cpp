#include "AirfoilDefiner.h"

AirfoilDefiner::AirfoilDefiner(const wxString& title)
	: wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(200, 230)) {

	type = -1;
	nPanels = 50;

	wxPanel *panel = new wxPanel(this, -1);

	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

	wxStaticBox *st = new wxStaticBox(panel, -1, wxT("NACA Type"),
		wxPoint(5, 5), wxSize(170, textH * 9 + 20));
	wxStaticText stext(panel, -1, wxT("Enter a NACA 4 or 5 digit code:"),
		wxPoint(15, 2 * textH), wxSize(150, 32));
	tc = new wxTextCtrl(panel, CODE_TEXT_CTRL_ID, wxT(""),
		wxPoint(15, 4.5f * textH), wxSize(150,24));
	tc->SetMaxLength(15);
	wxStaticText stext1(panel, -1, wxT("Number of Panels:"),
		wxPoint(15, 7 * textH), wxSize(150, 20));
	tc1 = new wxTextCtrl(panel, -1, std::to_string(nPanels).c_str(),
		wxPoint(15, 8.5f * textH), wxSize(50, 24));
	tc1->SetMaxLength(4);

	wxStaticBox *st1 = new wxStaticBox(panel, -1, wxT("Name"),
		wxPoint(5, 11 * textH), wxSize(170, textH * 7));
	wxStaticText stext2(panel, -1, wxT("Enter a unique identifier for this airfoil:"),
		wxPoint(15, 13 * textH), wxSize(150, 32));
	nameTC = new wxTextCtrl(panel, -1, wxT(""),
		wxPoint(15, 15.5f * textH), wxSize(150, 24));
	nameTC->SetMaxLength(15);

	wxButton *okButton = new wxButton(this, OK_ID, wxT("Ok"),
		wxDefaultPosition, wxSize(70, 30));
	wxButton *closeButton = new wxButton(this, CANCEL_ID, wxT("Cancel"),
		wxDefaultPosition, wxSize(70, 30));
	
	this->SetSize(200, 25 * textH);

	hbox->Add(okButton, 1);
	hbox->Add(closeButton, 1, wxLEFT, 5);

	vbox->Add(panel, 1);
	vbox->Add(hbox, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

	SetSizer(vbox);

	Connect(OK_ID, wxEVT_BUTTON, wxCommandEventHandler(AirfoilDefiner::onOK));
	Connect(CANCEL_ID, wxEVT_BUTTON, wxCommandEventHandler(AirfoilDefiner::onCancel));

	Connect(CODE_TEXT_CTRL_ID, wxEVT_TEXT, wxCommandEventHandler(AirfoilDefiner::onCodeChange));

	Centre();
	modalCode = ShowModal();
	tc->SetFocus();

	Destroy();
}

void AirfoilDefiner::onOK(wxCommandEvent& event) {
	code = tc->GetLineText(0);
	std::string tmp = tc1->GetLineText(0);
	name = nameTC->GetLineText(0);

	if (tmp != "" && std::all_of(tmp.begin(), tmp.end(), ::isdigit)) {
		nPanels = std::stoi(tmp);
	}
	else {
		nPanels = -1;
	}
	// If the name is empty, give an error and return
	if (name == "") {
		wxLogError("No name was entered! Please enter a name for the generated airfoil.");
		return;
	}

	if (code != "" && std::all_of(code.begin(), code.end(), ::isdigit)) {
		if (nPanels < 10 || nPanels > 5000) {
			nPanels = -1;
			wxLogError("An invalid number of panels was entered! Number of panels must be between 10 and 5000!");
			return;
		}
		if (code.length() == 4) {
			type = 4;
			EndModal(modalCode);
		}
		else if (code.length() == 5) {
			type = 5;
			EndModal(modalCode);
		}
		else {
			type = -1;
			wxLogError("An invalid length was entered! Please use 4 or 5 digit series only!");
		}
	}
	else {
		type = -1;
		wxLogError("Invalid code! Code should be only numbers and be 4 or 5 digits long!");
	}
}

void AirfoilDefiner::onCancel(wxCommandEvent& event) {
	EndModal(modalCode);
}

void AirfoilDefiner::onCodeChange(wxCommandEvent& event) {
	nameTC->SetValue("NACA " + tc->GetLineText(0));
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