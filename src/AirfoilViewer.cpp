#include "AirfoilViewer.h"
#include "AirfoilDefiner.h"

ViewerPanel::ViewerPanel(wxWindow* parent)
	: wxPanel(parent, -1, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE){

	// Sizer that controls the overall layout of the airfoil viewer
	avTopSizer = new wxBoxSizer(wxVERTICAL);

	// Drawing area for the airfoil plot
	avDrawArea = avTopSizer->Add(new wxPanel(this,7), wxEXPAND);
	// List to hold loaded airfoils
	wxListBox* airfoilListBox = new wxListBox(this, -1);
	avTopSizer->Add(airfoilListBox, 0, wxEXPAND);

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
	double yLim[2] = { -0.18,0.18 };
	int bords[4] = { 50,50,50,50 };
	airfoilPlot = new Plot(plotRect, xLim, yLim, bords);

	// Generates the points for an airfoil to be plotted on the airfoilPlot
	foilGen = new AirfoilGenerator();

	// ------ Bind events to functions ------
	// Main Menu Button
	Connect(BACK_ID, wxEVT_BUTTON, wxCommandEventHandler(ViewerPanel::onViewerBackButton));
	// Define airfoil button
	Connect(DEFINE_AF_ID, wxEVT_BUTTON, wxCommandEventHandler(ViewerPanel::onDefineAirfoil));
	// Event Paint
	Connect(GetId(), wxEVT_PAINT, wxPaintEventHandler(ViewerPanel::onPaintEvent));
}

wxBoxSizer* ViewerPanel::getTopSizer() {
	return avTopSizer;
}

enum {
	axesHORIZ = 0,
	axesVERT = 1
};

void ViewerPanel::onPaintEvent(wxPaintEvent& event) {
	wxPaintDC pdc(this);

	// Update the plot on resize and redraw axes and plots
	wxRect plotRect(avDrawArea->GetRect().GetLeft(), avDrawArea->GetRect().GetTop(), this->GetParent()->GetSize().GetWidth()-20, avTopSizer->GetChildren().front()->GetRect().GetHeight());
	airfoilPlot->updateBoundaries(plotRect);
	airfoilPlot->draw(pdc);
	for (AirfoilStruct afs : loadedAirfoils) {
		airfoilPlot->drawPoints(pdc, afs.points);
	}
}

void ViewerPanel::onDefineAirfoil(wxCommandEvent& event) {
	AirfoilDefiner defineDialog("NACA Airfoil Definer");
	std::string temp = defineDialog.getText();
	if (temp != "" && (temp.length() == 4 || temp.length() == 5) && std::all_of(temp.begin(), temp.end(), ::isdigit)) {
		AirfoilStruct afs;
		afs.code = temp;
		afs.nPanels = 50;
		afs.points = foilGen->generate4Digit(temp, 50);
		loadedAirfoils.emplace_back(afs);
		
		//afListBox->InsertItem(0, "");
		//afListBox->SetItem(0, 0, "");
		//afListBox->SetItem(0, 1, temp);
		//new wxStaticText(airfoilListBox, -1, temp);
	}
	
	this->Refresh();
}

//void ViewerPanel::drawAxes(wxPaintDC& dc) {
//	int w = avTopSizer->GetSize().GetWidth();
//	int h = avDrawArea->GetRect().GetHeight();
//	wxPoint top(60, 50);
//	wxPoint bottom(60, h - 50);
//	wxPoint left(20, h / 2);
//	wxPoint right(w - 20, h / 2);
//	wxPoint origin(60, h / 2);
//	dc.SetPen(wxPen(*wxWHITE, 2));
//	dc.DrawLine(top,bottom);
//	dc.DrawLine(left,right);
//	drawTicks(dc, origin, top, bottom, axesVERT, 15);
//	drawTicks(dc, origin, left, right, axesHORIZ, 23);
//}
//
//void ViewerPanel::drawTicks(wxDC& dc, wxPoint& origin, wxPoint& beg, wxPoint& end, int dir, int n){
//	dc.SetPen(wxPen(*wxWHITE, 1));
//	if (n < 1)
//		return;
//	int l = 0;
//	int h = 0;
//	if (dir == axesHORIZ) {
//		l = end.x - beg.x;
//		h = origin.x - beg.x;
//	}
//	else {
//		l = end.y - beg.y;
//		h = origin.y - beg.y;
//	}
//	if (l < 1)
//		return;
//	int a = l / n;
//	for (int i = 0; i < n; i++) {
//		if (dir == axesHORIZ) {
//			int pos = beg.x + i * a + h;
//			if (pos > beg.x + l) {
//				pos -= l;
//			}
//			drawTick(dc, wxPoint(pos, beg.y), dir);
//		}
//		else {
//			int pos = beg.y + i * a + h;
//			if (pos > beg.y + l) {
//				pos -= l;
//			}
//			drawTick(dc, wxPoint(beg.x, pos), dir);
//		}
//	}
//}
//
//void ViewerPanel::drawTick(wxDC& dc, wxPoint pos, int dir) {
//	if (dir == axesHORIZ) {
//		dc.DrawLine(pos.x, pos.y - 7, pos.x, pos.y + 7);
//	}
//	else
//		dc.DrawLine(pos.x - 7, pos.y, pos.x + 7, pos.y);
//}

AirfoilViewer::AirfoilViewer(wxWindow* parent) {
	initializeProgram(parent);
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
