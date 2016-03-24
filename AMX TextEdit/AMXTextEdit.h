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

#include "AMXPage.h"
#include "AMXGotoLineDlg.h"
#include "AMXAboutDlg.h"

enum
{
	MARGIN_LINE_NUMBERS,
	MARGIN_FOLD
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
	void AssignEventHandlers();
	void CreateMenus();
	
	int DoFind(wxString needle, int flags);
	bool DoReplace(wxString str, wxString rep, int flags, bool replaceAll);

	void CloseCurrentPage();

	void Redraw();

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
	void Options(wxCommandEvent& event);
	void SetName(wxCommandEvent& event);

	void OnFind(wxFindDialogEvent& event);
	void OnReplace(wxFindDialogEvent& event);
	void OnReplaceAll(wxFindDialogEvent& event);
	void OnFindClose(wxFindDialogEvent& event);

	void OnContextMenu(wxContextMenuEvent& event);
	void OnMarginClick(wxStyledTextEvent& event);

	void EnableCPPSyntaxHighlighting(bool e);
	void EnableCodeFolding(bool e);
	
};

const int ID_MENU_SELECTFONT = 101;
const int ID_MENU_GOTOLINE = 102;
const int ID_POPUPMENU_CLOSE = 103;

const int ID_MENU_TABSTOP = 201;
const int ID_MENU_TABSBOTTOM = 202;
const int ID_MENU_ENABLECPP = 203;
const int ID_MENU_ENABLECF = 204;

#endif
