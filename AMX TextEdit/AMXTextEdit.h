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
#include <wx/textfile.h>
#include <wx/fontdlg.h>
#include <wx/stc/stc.h>
#include <wx/fdrepdlg.h>

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

class AMXTextEdit : public wxFrame
{
	wxMenuBar *mainMenu;
	wxNotebook *mainBook;
	wxFindReplaceData frData;
	wxFindReplaceDialog *frDlg;
	int lastFindPos;

	AMXPage* NewPage(wxNotebook* book);
	void AddNewPage();
	void EnableEditMenus(bool e);
	int DoFind(wxString needle, int flags);
	bool DoReplace(wxString str, wxString rep, int flags, bool replaceAll);

public:
	AMXTextEdit(const wxString& title);
	~AMXTextEdit() { };

	void OnClose(wxCloseEvent& event);
	
	void SaveFile(AMXPage* page);
	void Exit(wxCommandEvent& event);
	void About(wxCommandEvent& event);
	void New(wxCommandEvent& event);
	void Open(wxCommandEvent& event);
	void Save(wxCommandEvent& event);
	void Edit(wxCommandEvent& event);
	void Font(wxCommandEvent& event);
	void SetName(wxCommandEvent& event);

	void OnFind(wxFindDialogEvent& event);
	void OnReplace(wxFindDialogEvent& event);
	void OnReplaceAll(wxFindDialogEvent& event);
	void OnFindClose(wxFindDialogEvent& event);
};

const int ID_MENU_SELECTFONT = 101;

const int ID_BTN_BASE = 200;


#endif
