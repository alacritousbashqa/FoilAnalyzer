#include "OverwriteNameDialog.h"

OverwriteNameDialog::OverwriteNameDialog(wxWindow* parent, const wxString& title, std::string name)
	: wxDialog(parent, -1, title, wxDefaultPosition, wxSize(200, 230)) {

	wxPanel *panel = new wxPanel(this, -1);

	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText stext(panel, -1, (name + " already exists. Do you wish to overwrite it?").c_str(),
		wxPoint(15, 2 * textH), wxSize(150, 32));

	// Buttons
	wxButton *overwriteButton = new wxButton(this, OVERWRITE_ID, wxT("Overwrite"),
		wxDefaultPosition, wxSize(70, 30));
	wxButton *closeButton = new wxButton(this, CANCEL_OVER_ID, wxT("Cancel"),
		wxDefaultPosition, wxSize(70, 30));

	// Set final size of dialog
	this->SetSize(200, 11 * textH);

	hbox->Add(overwriteButton, 1);
	hbox->Add(closeButton, 1, wxLEFT, 5);

	vbox->Add(panel, 1, wxALIGN_CENTER);
	vbox->Add(hbox, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

	SetSizer(vbox);

	// Connect the buttons to their functions
	Connect(OVERWRITE_ID, wxEVT_BUTTON, wxCommandEventHandler(OverwriteNameDialog::onOverwrite));
	Connect(CANCEL_OVER_ID, wxEVT_BUTTON, wxCommandEventHandler(OverwriteNameDialog::onCancel));

	// Center
	Centre();
	modalCode = ShowModal(); // Pause everything except this dialog

	Destroy(); // Only gets called after EndModal is called

}

bool OverwriteNameDialog::getOverwrite() {
	return overwrite;
}

void OverwriteNameDialog::onOverwrite(wxCommandEvent& event) {
	overwrite = true;

	EndModal(modalCode);
}

void OverwriteNameDialog::onCancel(wxCommandEvent& event) {
	overwrite = false;
	EndModal(modalCode);
}