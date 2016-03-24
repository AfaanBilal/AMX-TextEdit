/*
*    AMX TextEdit
*    Author: Afaan Bilal
*    https://www.coderevolution.tk
*
*/

#ifndef AMXPAGE_H
#define AMXPAGE_H

#include <wx/wx.h>

class AMXPage : public wxWindow
{
public:
	AMXPage(wxWindow* parent, wxWindowID id) : wxWindow(parent, id) { }
	~AMXPage() { delete this->txtBody; }
	int id;
	bool saved;
	wxString filename;
	wxStyledTextCtrl* txtBody;
};

#endif
