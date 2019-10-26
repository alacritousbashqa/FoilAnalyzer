#include "AirfoilDefiner.h"

AirfoilDefiner::AirfoilDefiner(const wxString& title)
	: wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(200, 230)) {

	type = -1;

	wxPanel *panel = new wxPanel(this, -1);

	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

	wxStaticBox *st = new wxStaticBox(panel, -1, wxT("NACA Type"),
		wxPoint(5, 5), wxSize(170, textH * 6 + 20));
	wxStaticText stext(panel, -1, wxT("Enter a NACA 4 or 5 digit code:"),
		wxPoint(15, 2 * textH), wxSize(150, 40));
	tc = new wxTextCtrl(panel, -1, wxT(""),
		wxPoint(15, 5 * textH), wxSize(150,24));

	wxButton *okButton = new wxButton(this, 1, wxT("Ok"),
		wxDefaultPosition, wxSize(70, 30));
	wxButton *closeButton = new wxButton(this, 2, wxT("Cancel"),
		wxDefaultPosition, wxSize(70, 30));
	
	this->SetSize(200, 14 * textH);

	hbox->Add(okButton, 1);
	hbox->Add(closeButton, 1, wxLEFT, 5);

	vbox->Add(panel, 1);
	vbox->Add(hbox, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

	SetSizer(vbox);

	Connect(1, wxEVT_BUTTON, wxCommandEventHandler(AirfoilDefiner::onOK));
	Connect(2, wxEVT_BUTTON, wxCommandEventHandler(AirfoilDefiner::onCancel));

	Centre();
	modalCode = ShowModal();

	Destroy();
}

void AirfoilDefiner::onOK(wxCommandEvent& event) {
	text = tc->GetLineText(0);
	if (text != "" && std::all_of(text.begin(), text.end(), ::isdigit)) {
		if (text.length() == 4) {
			type = 4;
			EndModal(modalCode);
		}
		else if (text.length() == 5) {
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

std::string AirfoilDefiner::getText() {
	return text;
}

int AirfoilDefiner::getType() {
	return type;
}