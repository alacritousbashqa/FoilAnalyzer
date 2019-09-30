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