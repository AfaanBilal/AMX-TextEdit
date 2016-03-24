/*
*    AMX TextEdit
*    Author: Afaan Bilal
*    https://www.coderevolution.tk
*
*/

#ifndef AMXGOTOLINEDLG_H
#define AMXGOTOLINEDLG_H

#include <wx/wx.h>

class AMXGotoLineDlg : public wxDialog
{
public:
	wxTextCtrl* lineNum;
	AMXGotoLineDlg(wxWindow* parent) : wxDialog(parent, -1, "AMX TextEdit - Go to Line", wxDefaultPosition, wxSize(280, 70))
	{
		wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);
		hbox->Add(new wxStaticText(this, -1, wxT("Line number: ")), 0, wxALL | wxEXPAND, 5);
		this->lineNum = new wxTextCtrl(this, -1, wxT(""));
		hbox->Add(this->lineNum, 0, wxALL | wxEXPAND, 5);
		hbox->Add(new wxButton(this, wxID_OK, wxT("GO")), 0, wxALL | wxEXPAND, 5);
		SetSizer(hbox);
	}
	~AMXGotoLineDlg() { delete this->lineNum; }
};

#endif
