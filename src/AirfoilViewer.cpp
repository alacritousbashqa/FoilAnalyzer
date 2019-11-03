#include "AirfoilViewer.h"

std::vector<AirfoilStruct*> loadedAirfoils;

ViewerPanel::ViewerPanel(wxWindow* parent)
	: wxPanel(parent, -1, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE){

	// Sizer that controls the overall layout of the airfoil viewer
	avTopSizer = new wxBoxSizer(wxVERTICAL);

	// Drawing area for the airfoil plot
	avDrawArea = avTopSizer->Add(new wxPanel(this,7), wxEXPAND);

	// Sizer that ensures the scroll box fills its given area
	wxBoxSizer* listHBox = new wxBoxSizer(wxHORIZONTAL);

	// Holds list of loaded airfoil and allows scrolling when there's too many
	scrolledBoxSizer = new wxBoxSizer(wxVERTICAL);
	scrolledWindow = new wxScrolledWindow(this, -1, wxDefaultPosition, wxSize(-1,100), wxVSCROLL);
	scrolledWindow->SetBackgroundColour(wxColour(*wxWHITE));
	scrolledWindow->SetScrollRate(5, 5);
	listHBox->Add(scrolledWindow, 1, wxEXPAND);
	scrolledWindow->SetSizer(scrolledBoxSizer);
	avTopSizer->Add(listHBox, 1, wxLEFT | wxRIGHT | wxBOTTOM | wxEXPAND, 10);

	// Box that holds "back to main menu" button at bottom of screen
	wxBoxSizer* buttonBox = new wxBoxSizer(wxHORIZONTAL);
	buttonBox->Add(new wxButton(this, BACK_ID, "Main Menu"), wxSizerFlags().Left());
	buttonBox->Add(new wxButton(this, DEFINE_AF_ID, "Define Airfoil"), wxSizerFlags().Left());

	avTopSizer->Add(buttonBox, 0, wxLEFT | wxBOTTOM, 10);

	this->SetSizer(avTopSizer);

	// Black Background
	SetBackgroundColour(wxColour(*wxBLACK));

	// Airfoil Plot which holds the axes data and draws airfoils onto them
	wxRect plotRect(avDrawArea->GetRect().GetLeft(), avDrawArea->GetRect().GetTop(), parent->GetSize().GetWidth()-20, 500);
	double xLim[2] = { -0.1,1.1 };
	double yLim[2] = { -0.20,0.20 };
	int bords[4] = { 50,50,50,50 };
	airfoilPlot = new Plot(plotRect, xLim, yLim, bords, true, true, true);
	airfoilPlot->setTitle("Airfoil Viewer Plot");
	airfoilPlot->setHLabel("x/c");
	airfoilPlot->setVLabel("y/c");
	int ar[] = { 3, 1 };
	airfoilPlot->setAspectRatio(ar);

	// Generates the points for an airfoil to be plotted on the airfoilPlot
	foilGen = new AirfoilGenerator();

	// ------ Bind events to functions ------
	// Main Menu Button
	Connect(BACK_ID, wxEVT_BUTTON, wxCommandEventHandler(ViewerPanel::onViewerBackButton));
	// Define airfoil button
	Connect(DEFINE_AF_ID, wxEVT_BUTTON, wxCommandEventHandler(ViewerPanel::onDefineAirfoil));
	// Airfoil list checkbox
	Connect(CHECKBOXES_ID, wxEVT_CHECKBOX, wxCommandEventHandler(ViewerPanel::onShowChecked));
	// Airfoil list color picker
	Connect(COLORPICKER_ID, wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler(ViewerPanel::onColorPicked));
	// Delete airfoil button
	Connect(DELETE_ID, wxEVT_BUTTON, wxCommandEventHandler(ViewerPanel::onDelButton));
	// Event Paint
	Connect(GetId(), wxEVT_PAINT, wxPaintEventHandler(ViewerPanel::onPaintEvent));
}

ViewerPanel::~ViewerPanel(){
	delete airfoilPlot;
	delete foilGen;
	for (AirfoilListStruct afs : afListMembers) {
		delete afs.airfoil;
		delete afs.checkBox;
		delete afs.colorPicker;
		delete afs.delButton;
	}
	afListMembers.clear();
	for (AirfoilStruct *as : loadedAirfoils) {
		if(!as)
			delete as;
	}
	loadedAirfoils.clear();
}

wxBoxSizer* ViewerPanel::getTopSizer() {
	return avTopSizer;
}

enum {
	axesHORIZ = 0,
	axesVERT = 1
};

// Called on refresh; paints the axes and any loaded airfoils to be shown
void ViewerPanel::onPaintEvent(wxPaintEvent& event) {
	wxPaintDC pdc(this);

	// Update the plot on resize and redraw axes and plots
	wxRect plotRect(avDrawArea->GetRect().GetLeft(), avDrawArea->GetRect().GetTop(), this->GetParent()->GetSize().GetWidth()-20, avTopSizer->GetChildren().front()->GetRect().GetHeight());
	airfoilPlot->updateBoundaries(plotRect);
	airfoilPlot->draw(pdc);
	// Draw the loaded airfoils
	for (AirfoilListStruct als : afListMembers) {
		if (als.checkBox->GetValue()) {
			pdc.SetPen(wxPen(als.colorPicker->GetColour(), 1));
			airfoilPlot->drawPoints(pdc, als.airfoil->points);
		}
	}
}

// When the button to define an airfoil is pressed. Opens up the NACA code dialog.
void ViewerPanel::onDefineAirfoil(wxCommandEvent& event) {
	// Create and open airfoil definer dialog
	AirfoilDefiner defineDialog("NACA Airfoil Definer");
	// Get code from dialog on dialog close
	std::string temp = defineDialog.getCode();
	// Get NACA code type (4 or 5 digit) on dialog close
	int type = defineDialog.getType();
	// Get the number of panels to generate on dialog close
	int nPanels = defineDialog.getNPanels();
	// Get the unique name of the airfoil on dialog close
	std::string name = defineDialog.getName();

	AirfoilStruct* newItem = defineDialog.getNewItem();

	this->SetFocus();

	// If the dialog returns type = -1, it was closed without a valid code, so break
	if (type == -1) {
		return;
	}

	// If the dialog returns nPanels = -1, it was closed without a valid code, so break
	if (nPanels == -1) {
		return;
	}

	// If the dialog says to not create a new item, AKA overwrite a current list item and redraw plot
	if (newItem) {
		AirfoilListStruct als = getListMemberFromAirfoil(newItem);
		// No valid AirfoilListStruct in afListMembers was found
		if (!als.airfoil) {
			wxLogError("AirfoilStruct was a nullptr! Could not find airfoil list member from given airfoil struct!");
			return;
		}
		als.airfoil->code = temp;
		als.airfoil->nPanels = nPanels;
		if (type == 4) {
			als.airfoil->points = foilGen->generate4Digit(temp, nPanels);
		}
		else {
			als.airfoil->points = foilGen->generate5Digit(temp, nPanels);
		}

		als.codeText->SetLabelText(("NACA " + temp).c_str()); // Set new code in list
		
		this->Refresh(); // Redraw

		return;
	}

	// Create a new airfoil struct
	AirfoilStruct* afs = new AirfoilStruct();
	afs->code = temp;
	afs->name = name;
	afs->nPanels = nPanels;
	// Generate the correct points based on the entered code type
	if (type == 4) {
		afs->points = foilGen->generate4Digit(temp, nPanels);
	}
	else {
		afs->points = foilGen->generate5Digit(temp, nPanels);
	}
	loadedAirfoils.emplace_back(afs); // Add to global master list
	// Create a new list struct
	wxBoxSizer* hBox = new wxBoxSizer(wxHORIZONTAL);
	AirfoilListStruct als;
	als.airfoil = afs;
	als.checkBox = new wxCheckBox(scrolledWindow, CHECKBOXES_ID, "Show?");
	als.checkBox->SetValue(true);
	als.nameText = new wxStaticText(scrolledWindow, -1, afs->name, wxDefaultPosition, wxDefaultSize);
	als.codeText = new wxStaticText(scrolledWindow, -1, ("NACA " + afs->code).c_str());
	als.colorPicker = new wxColourPickerCtrl(scrolledWindow, COLORPICKER_ID);
	als.colorPicker->SetColour(wxColour(*wxWHITE));
	als.delButton = new wxButton(scrolledWindow, DELETE_ID, "Delete");
	als.delButton->SetName(afs->name);
	afListMembers.push_back(als); // Add to class master list
		
	// Add the new airfoil widgets to a box sizer (adds a new row)
	hBox->Add(als.checkBox, 1, wxEXPAND | wxLEFT, 10);
	hBox->Add(als.nameText, 3, wxALIGN_CENTER_VERTICAL);
	hBox->Add(als.codeText, 2, wxALIGN_CENTER_VERTICAL);
	hBox->Add(als.colorPicker, 1, wxALIGN_CENTER_VERTICAL);
	hBox->Add(als.delButton, 1, wxALIGN_CENTER_VERTICAL);
	hBox->Add(new wxPanel(scrolledWindow, -1));
	hBox->Layout();

	// Add the new airfoil row to the scroll box and tell the scroll box to update its size
	scrolledBoxSizer->Add(hBox);
	scrolledWindow->FitInside();
	scrolledBoxSizer->Layout();
	
	// Tell the viewer panel to redraw the new airfoil
	this->Refresh();
}

// When the checkbox changes state, tell the viewer panel to redraw
void ViewerPanel::onShowChecked(wxCommandEvent& event) {
	this->Refresh();
}
// When the color picker changes state, tell the viewer panel to redraw
void ViewerPanel::onColorPicked(wxColourPickerEvent& event) {
	this->Refresh();
}

void ViewerPanel::onDelButton(wxCommandEvent& event) {
}

AirfoilListStruct ViewerPanel::getListMemberFromAirfoil(AirfoilStruct* afs) {
	for (AirfoilListStruct als : afListMembers) {
		if (als.airfoil = afs) {
			return als;
		}
	}
	AirfoilListStruct als;
	als.airfoil = nullptr;
	return als;
}

AirfoilViewer::AirfoilViewer(wxWindow* parent) {
	initializeProgram(parent);
}

AirfoilViewer::~AirfoilViewer() {
	delete viewerPanel;
}

wxPanel* AirfoilViewer::getTopPanel() {
	return viewerPanel;
}

bool AirfoilViewer::initializeProgram(wxWindow* parent) {
	// Construct the top panel for the airfoil viewer
	viewerPanel = new ViewerPanel(parent);

	if (viewerPanel) {
		show(false);
		return true;
	}
	wxLogError("Top level panel could not be constructed!");
	return false;
}

void AirfoilViewer::show(bool show) {
	if (viewerPanel) {
		viewerPanel->Show(show);
		viewerPanel->Enable(show);
	}
	else {
		wxLogError("Cannot show airfoil viewer! Associated top-level panel does not exist!");
		return;
	}
}