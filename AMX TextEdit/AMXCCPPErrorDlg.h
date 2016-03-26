/*
*    AMX TextEdit
*    Author: Afaan Bilal
*    https://afaan.ml/AMX-TextEdit
*    https://www.coderevolution.tk
*
*/

#ifndef AMXCCPPERRORDLG_H
#define AMXCCPPERRORDLG_H

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/hyperlink.h>

class AMXCCPPErrorDlg : public wxDialog
{
public:
	AMXCCPPErrorDlg(wxWindow* parent, wxArrayString errors, wxPoint pos, int width) : wxDialog(parent, -1, "AMX TextEdit", pos, wxSize(width, 160))
	{
		SetBackgroundColour(wxColour(255, 255, 255));

		wxBoxSizer* hbox = new wxBoxSizer(wxVERTICAL);

		wxStaticText* title = new wxStaticText(this, -1, wxT("ERRORS"));
		title->SetFont(wxFont(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
		title->SetForegroundColour(wxColour(150, 0, 0));
		hbox->Add(title, 0, wxALL | wxALIGN_LEFT, 5);

		wxListBox* errorList = new wxListBox(this, -1, wxDefaultPosition, wxSize(-1, 150), errors, wxLB_HSCROLL);
				
		hbox->Add(errorList, 0, wxALL | wxEXPAND, 5);
		
		SetSizer(hbox);
	}
	~AMXCCPPErrorDlg() { }
};

#endif
