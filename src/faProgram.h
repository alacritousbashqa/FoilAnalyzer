// faProgram.h
// Author: Mark Bashqawi

// Pure virtual class which is the base of all sub-programs: Main Menu, Airfoil Viewer, Airfoil Analyzer, etc.

#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class faProgram {
public:
	virtual wxPanel* getTopPanel() = 0;
	virtual bool initializeProgram(wxWindow* parent) = 0;
	virtual void show(bool show = true) = 0;
};