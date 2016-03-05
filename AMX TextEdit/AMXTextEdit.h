/*
 *    AMX TextEdit
 *    Author: Afaan Bilal
 *    https://www.coderevolution.tk
 *
 */

#ifndef AMXTEXTEDIT_H
#define AMXTEXTEDIT_H

#include <wx/wx.h>
#include <wx/notebook.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/textfile.h>
#include <wx/fontdlg.h>

class AMXPage : public wxWindow
{
public:
	AMXPage(wxWindow* parent, wxWindowID id) : wxWindow(parent, id) { }
	
	//wxTextCtrl* txtTitle;
	wxRichTextCtrl* txtBody;
	//wxButton* btnSetName;

	int id;
	//wxString title;
	wxString filename;
	bool saved;
};

class AMXTextEdit : public wxFrame
{
	wxMenuBar *mainMenu;
	wxNotebook *mainBook;

	AMXPage* NewPage(wxNotebook* book);
	void AddNewPage();
	void EnableEditMenus(bool e);

public:
	AMXTextEdit(const wxString& title);
	~AMXTextEdit(){};

	void OnClose(wxCloseEvent& event);
	
	void SaveFile(AMXPage* page);
	void Exit(wxCommandEvent& event);
	void About(wxCommandEvent& event);
	void ShortcutsHandler(wxKeyEvent& event);
	void New(wxCommandEvent& event);
	void Open(wxCommandEvent& event);
	void Save(wxCommandEvent& event);
	void Edit(wxCommandEvent& event);
	void Font(wxCommandEvent& event);
	void SetName(wxCommandEvent& event);

};

const int ID_MENU_SELECTFONT = 101;

const int ID_BTN_BASE = 200;


#endif
