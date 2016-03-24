/*
*    AMX TextEdit
*    Author: Afaan Bilal
*    https://www.coderevolution.tk
*
*/

#ifndef AMXABOUTDLG_H
#define AMXABOUTDLG_H

#include <wx/wx.h>
#include <wx/hyperlink.h>

class AMXAboutDlg : public wxDialog
{
public:
	AMXAboutDlg(wxWindow* parent) : wxDialog(parent, -1, "AMX TextEdit - About", wxDefaultPosition, wxSize(-1, 360))
	{
		SetBackgroundColour(wxColour(255, 255, 255));

		wxBoxSizer* hbox = new wxBoxSizer(wxVERTICAL);
		
		wxStaticText* title = new wxStaticText(this, -1, wxT("AMX TextEdit"));
		title->SetFont(wxFont(28, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
		title->SetForegroundColour(wxColour(33, 106, 197));
		hbox->Add(title, 0, wxALL | wxALIGN_CENTER, 5);

		wxStaticText* author = new wxStaticText(this, -1, wxT("by Afaan Bilal"));
		author->SetFont(wxFont(16, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
		author->SetForegroundColour(wxColour(52, 177, 74));
		hbox->Add(author, 0, wxALL | wxALIGN_CENTER, 5);

		wxStaticText* desc = new wxStaticText(this, -1, wxT("AMX TextEdit is a simple, fast, tabbed text editor."));
		desc->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
		hbox->Add(desc, 0, wxALL | wxALIGN_CENTER, 5);
		
		wxHyperlinkCtrl* projectLink = new wxHyperlinkCtrl(this, -1, wxT("afaan.ml/AMX-TextEdit"), wxT("https://afaan.ml/AMX-TextEdit"));
		projectLink->SetFont(wxFont(14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
		hbox->Add(projectLink, 0, wxALL | wxALIGN_CENTER, 5);

		wxHyperlinkCtrl* blogLink = new wxHyperlinkCtrl(this, -1, wxT("www.coderevolution.tk"), wxT("https://www.coderevolution.tk"));
		blogLink->SetFont(wxFont(14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
		hbox->Add(blogLink, 0, wxALL | wxALIGN_CENTER, 5);

		wxHyperlinkCtrl* amxLink = new wxHyperlinkCtrl(this, -1, wxT("www.amxinfinity.tk"), wxT("https://www.amxinfinity.tk"));
		amxLink->SetFont(wxFont(14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
		hbox->Add(amxLink, 0, wxALL | wxALIGN_CENTER, 5);

		wxHyperlinkCtrl* gpLink = new wxHyperlinkCtrl(this, -1, wxT("google.com/+AfaanBilal"), wxT("https://google.com/+AfaanBilal"));
		gpLink->SetFont(wxFont(14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
		hbox->Add(gpLink, 0, wxALL | wxALIGN_CENTER, 5);

		wxHyperlinkCtrl* mlLink = new wxHyperlinkCtrl(this, -1, wxT("afaan.ml"), wxT("https://afaan.ml"));
		mlLink->SetFont(wxFont(14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
		hbox->Add(mlLink, 0, wxALL | wxALIGN_CENTER, 5);

		wxStaticText* copyright = new wxStaticText(this, -1, wxT("© 2016 Afaan Bilal, AMX Infinity!"));
		copyright->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
		hbox->Add(copyright, 0, wxALL | wxALIGN_CENTER, 10);

		SetSizer(hbox);
		
		Center();
		ShowModal();

		this->Destroy();
	}
	~AMXAboutDlg() { }
};

#endif
