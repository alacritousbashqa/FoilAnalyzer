#include "AirfoilDefiner.h"

AirfoilDefiner::AirfoilDefiner(const wxString& title)
	: wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(200, 230)) {

	wxPanel *panel = new wxPanel(this, -1);

	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

	wxStaticBox *st = new wxStaticBox(panel, -1, wxT("NACA Type"),
		wxPoint(5, 5), wxSize(170, 125));
	rb4 = new wxRadioButton(panel, -1,
		wxT("NACA 4 Digit"), wxPoint(15, 30), wxDefaultSize, wxRB_GROUP);

	rb5 = new wxRadioButton(panel, -1,
		wxT("NACA 5 Digit"), wxPoint(15, 55));
	rb5->Disable();
	tc = new wxTextCtrl(panel, -1, wxT(""),
		wxPoint(15, 95), wxSize(150,24));

	wxButton *okButton = new wxButton(this, 1, wxT("Ok"),
		wxDefaultPosition, wxSize(70, 30));
	wxButton *closeButton = new wxButton(this, 2, wxT("Cancel"),
		wxDefaultPosition, wxSize(70, 30));

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
	EndModal(modalCode);
}

void AirfoilDefiner::onCancel(wxCommandEvent& event) {
	EndModal(modalCode);
}

std::string AirfoilDefiner::getText() {
	return text;
}