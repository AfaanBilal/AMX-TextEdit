/*
 *    AMX TextEdit
 *    Author: Afaan Bilal
 *    https://afaan.ml/AMX-TextEdit
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
#include <wx/utils.h>
#include <wx/busyinfo.h>

#include "AMXPage.h"
#include "AMXGotoLineDlg.h"
#include "AMXAboutDlg.h"
#include "AMXCCPPErrorDlg.h"

WX_DECLARE_STRING_HASH_MAP(wxString, wxStringHashMap);

enum
{
	MARGIN_LINE_NUMBERS,
	MARGIN_FOLD
};

enum Language
{
    C,
    CPP,
    PHP,
    JS
};

class AMXTextEdit : public wxFrame
{
	wxMenuBar *mainMenu;
	wxNotebook *mainBook;
	wxFindReplaceData frData;
	wxFindReplaceDialog *frDlg;
	AMXCCPPErrorDlg *errorDlg;
    wxStringHashMap keywords;
	int lastFindPos;

	AMXPage* NewPage(wxNotebook* book);
	void AddNewPage();
    void AssignKeyWords();
	void EnableEditMenus(bool e);
	void EnableCCPPMenus(bool e);
	void EnableSyntaxHighlighting(bool e, Language lang);
	void EnableCodeFolding(bool e);
	void EnableAutoIndent(bool e);
	void EnableIndentGuides(bool e);
	void EnableCPPMode(bool e);

	bool CompileCCPP(wxString filename);
	void RunCCPP(wxString filename);

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
	void PageChanged(wxBookCtrlEvent& event);

	void SaveFile(AMXPage* page);
	void Exit(wxCommandEvent& event);
	void About(wxCommandEvent& event);
	void New(wxCommandEvent& event);
	void Open(wxCommandEvent& event);
	void Save(wxCommandEvent& event);
	void Edit(wxCommandEvent& event);
	void Options(wxCommandEvent& event);
	void CCPP(wxCommandEvent& event);
	void SetName(wxCommandEvent& event);

	void OnFind(wxFindDialogEvent& event);
	void OnReplace(wxFindDialogEvent& event);
	void OnReplaceAll(wxFindDialogEvent& event);
	void OnFindClose(wxFindDialogEvent& event);

	void OnContextMenu(wxContextMenuEvent& event);
	void OnMarginClick(wxStyledTextEvent& event);
	void OnCharAdded(wxStyledTextEvent& event);
	void OnChange(wxStyledTextEvent& event);
};

const int ID_MENU_SELECTFONT = 101;
const int ID_MENU_GOTOLINE = 102;
const int ID_POPUPMENU_CLOSE = 103;

const int ID_MENU_TABSTOP = 201;
const int ID_MENU_TABSBOTTOM = 202;
const int ID_MENU_ENABLECPP = 203;
const int ID_MENU_ENABLECF = 204;
const int ID_MENU_ENABLEAI = 205;
const int ID_MENU_ENABLEIG = 206;

const int ID_MENU_SYNHIGHLIGHT_C = 207;
const int ID_MENU_SYNHIGHLIGHT_CPP = 208;
const int ID_MENU_SYNHIGHLIGHT_PHP = 209;

const int ID_MENU_COMPILE_RUN = 301;
const int ID_MENU_COMPILE = 302;
const int ID_MENU_RUN = 303;

#endif

