/*
 *    AMX TextEdit
 *    Author: Afaan Bilal
 *    https://afaan.ml/AMX-TextEdit
 *    https://www.coderevolution.tk
 *
 */

#include "AMXTextEdit.h"

AMXTextEdit::AMXTextEdit(const wxString& title)
: wxFrame(NULL, -1, title, wxPoint(-1, -1), wxSize(640, 600))
{
	SetIcon(wxIcon(wxT("MAIN_ICON")));
	lastFindPos = -1;

	CreateMenus();

	mainBook = new wxNotebook(this, -1, wxDefaultPosition, wxDefaultSize, wxNB_TOP);

	AssignEventHandlers();

	Centre();

	AddNewPage();
}

void AMXTextEdit::CreateMenus()
{
	mainMenu = new wxMenuBar;

	wxMenu* fileMenu = new wxMenu;
	fileMenu->Append(new wxMenuItem(fileMenu, wxID_NEW, wxT("&New\tCtrl+N"), wxT("Create a new text document")));
	fileMenu->Append(new wxMenuItem(fileMenu, wxID_OPEN, wxT("&Open\tCtrl+O"), wxT("Open an existing text document")));
	fileMenu->Append(new wxMenuItem(fileMenu, wxID_SAVE, wxT("&Save\tCtrl+S"), wxT("Save the current text document")));
	fileMenu->Append(new wxMenuItem(fileMenu, wxID_SAVEAS, wxT("&Save As...\tCtrl+Shift+S"), wxT("Save the current text document with a new name")));
	fileMenu->Append(new wxMenuItem(fileMenu, -1, wxEmptyString, wxEmptyString, wxITEM_SEPARATOR));
	fileMenu->Append(new wxMenuItem(fileMenu, wxID_EXIT, wxT("E&xit\tCtrl+Q"), wxT("Exit the program")));
	mainMenu->Append(fileMenu, wxT("&File"));

	wxMenu* editMenu = new wxMenu;
	editMenu->Append(new wxMenuItem(editMenu, wxID_UNDO, wxT("Undo\tCtrl+Z"), wxT("Undo last action")));
	editMenu->Append(new wxMenuItem(editMenu, wxID_REDO, wxT("Redo\tCtrl+Y"), wxT("Revert last undo")));
	editMenu->Append(new wxMenuItem(editMenu, -1, wxEmptyString, wxEmptyString, wxITEM_SEPARATOR));
	editMenu->Append(new wxMenuItem(editMenu, wxID_SELECTALL, wxT("Select All\tCtrl+A"), wxT("Select all text")));
	editMenu->Append(new wxMenuItem(editMenu, wxID_COPY, wxT("Copy\tCtrl+C"), wxT("Copy selected text")));
	editMenu->Append(new wxMenuItem(editMenu, wxID_CUT, wxT("Cut\tCtrl+X"), wxT("Cut selected text")));
	editMenu->Append(new wxMenuItem(editMenu, wxID_PASTE, wxT("Paste\tCtrl+V"), wxT("Paste copied text")));
	editMenu->Append(new wxMenuItem(editMenu, -1, wxEmptyString, wxEmptyString, wxITEM_SEPARATOR));
	editMenu->Append(new wxMenuItem(editMenu, wxID_FIND, wxT("Find\tCtrl+F"), wxT("Find something")));
	editMenu->Append(new wxMenuItem(editMenu, wxID_REPLACE, wxT("Replace\tCtrl+H"), wxT("Replace something")));
	editMenu->Append(new wxMenuItem(editMenu, -1, wxEmptyString, wxEmptyString, wxITEM_SEPARATOR));
	editMenu->Append(new wxMenuItem(editMenu, ID_MENU_GOTOLINE, wxT("Go to line\tCtrl+G"), wxT("Go to a specific line number")));
	mainMenu->Append(editMenu, wxT("&Edit"));

	wxMenu* optionsMenu = new wxMenu;
	optionsMenu->Append(new wxMenuItem(optionsMenu, ID_MENU_SELECTFONT, wxT("Choose font"), wxT("Select a font")));

	wxMenuItem* menuTabPos = new wxMenuItem(optionsMenu, -1, wxT("Tabs position"));
	
	wxMenu* menuTabPosOps = new wxMenu;
	menuTabPosOps->Append(new wxMenuItem(menuTabPosOps, ID_MENU_TABSTOP, wxT("Top"), wxT("Set tabs on top"), wxITEM_RADIO));
	menuTabPosOps->Append(new wxMenuItem(menuTabPosOps, ID_MENU_TABSBOTTOM, wxT("Bottom"), wxT("Set tabs on bottom"), wxITEM_RADIO));
	menuTabPos->SetSubMenu(menuTabPosOps);

	optionsMenu->Append(menuTabPos);
	optionsMenu->Append(new wxMenuItem(optionsMenu, ID_MENU_ENABLECPP, wxT("C/C++ Syntax Highlighting"), wxT("Toggle syntax highlighting for C/C++"), wxITEM_CHECK));
	optionsMenu->Append(new wxMenuItem(optionsMenu, ID_MENU_ENABLECF, wxT("Code Folding"), wxT("Toggle code folding"), wxITEM_CHECK));
	mainMenu->Append(optionsMenu, wxT("&Options"));

	wxMenu* ccppMenu = new wxMenu;
	ccppMenu->Append(new wxMenuItem(ccppMenu, ID_MENU_COMPILE_RUN, wxT("Compile and Run"), wxT("Compile and execute the C/C++ file")));
	ccppMenu->Append(new wxMenuItem(ccppMenu, ID_MENU_COMPILE, wxT("Compile"), wxT("Compile the C/C++ file")));
	ccppMenu->Append(new wxMenuItem(ccppMenu, ID_MENU_RUN, wxT("Run"), wxT("Execute the C/C++ file")));
	mainMenu->Append(ccppMenu, wxT("C/C++"));

	wxMenu* helpMenu = new wxMenu;
	helpMenu->Append(new wxMenuItem(helpMenu, wxID_ABOUT, wxT("&About"), wxT("About AMX TextEdit")));
	mainMenu->Append(helpMenu, wxT("&Help"));

	SetMenuBar(mainMenu);

	CreateStatusBar();

	EnableEditMenus(false);
	EnableCCPPMenus(false);
}

void AMXTextEdit::AssignEventHandlers()
{
	Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(AMXTextEdit::OnClose));
	Connect(wxEVT_NOTEBOOK_PAGE_CHANGED, wxBookCtrlEventHandler(AMXTextEdit::PageChanged));

	Connect(wxID_ABOUT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(AMXTextEdit::About));
	Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(AMXTextEdit::Exit));
	Connect(wxID_NEW, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(AMXTextEdit::New));
	Connect(wxID_OPEN, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(AMXTextEdit::Open));
	
	Connect(wxID_SAVE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(AMXTextEdit::Save));
	Connect(wxID_SAVEAS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(AMXTextEdit::Save));

	Connect(wxID_UNDO, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(AMXTextEdit::Edit));
	Connect(wxID_REDO, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(AMXTextEdit::Edit));
	Connect(wxID_SELECTALL, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(AMXTextEdit::Edit));
	Connect(wxID_COPY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(AMXTextEdit::Edit));
	Connect(wxID_CUT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(AMXTextEdit::Edit));
	Connect(wxID_PASTE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(AMXTextEdit::Edit));
	Connect(wxID_FIND, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(AMXTextEdit::Edit));
	Connect(wxID_REPLACE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(AMXTextEdit::Edit));
	Connect(ID_MENU_GOTOLINE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(AMXTextEdit::Edit));
	Connect(ID_POPUPMENU_CLOSE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(AMXTextEdit::Edit));

	Connect(ID_MENU_SELECTFONT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(AMXTextEdit::Options));
	Connect(ID_MENU_TABSTOP, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(AMXTextEdit::Options));
	Connect(ID_MENU_TABSBOTTOM, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(AMXTextEdit::Options));
	Connect(ID_MENU_ENABLECPP, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(AMXTextEdit::Options));
	Connect(ID_MENU_ENABLECF, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(AMXTextEdit::Options));

	Connect(ID_MENU_COMPILE_RUN, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(AMXTextEdit::CCPP));
	Connect(ID_MENU_COMPILE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(AMXTextEdit::CCPP));
	Connect(ID_MENU_RUN, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(AMXTextEdit::CCPP));

	Connect(wxEVT_FIND, wxFindDialogEventHandler(AMXTextEdit::OnFind));
	Connect(wxEVT_FIND_NEXT, wxFindDialogEventHandler(AMXTextEdit::OnFind));
	Connect(wxEVT_FIND_REPLACE, wxFindDialogEventHandler(AMXTextEdit::OnReplace));
	Connect(wxEVT_FIND_REPLACE_ALL, wxFindDialogEventHandler(AMXTextEdit::OnReplaceAll));
	Connect(wxEVT_FIND_CLOSE, wxFindDialogEventHandler(AMXTextEdit::OnFindClose));

	Connect(wxEVT_CONTEXT_MENU, wxContextMenuEventHandler(AMXTextEdit::OnContextMenu));
	Connect(wxEVT_STC_MARGINCLICK, wxStyledTextEventHandler(AMXTextEdit::OnMarginClick));
}

bool AMXTextEdit::CompileCCPP(wxString filename)
{
	wxString compiler = (filename.EndsWith(".c") || filename.EndsWith(".C")) ? wxT("gcc") : wxT("g++");
	wxString cmd = wxString::Format("%s -o \"%s.exe\" \"%s\"", compiler, filename.BeforeLast('.'), filename);
	
	wxArrayString output, errors;
	
	wxBeginBusyCursor();
	{
		wxBusyInfo info
			(
			wxBusyInfoFlags()
			.Parent(this)
			.Title("<b>AMX TextEdit</b>")
			.Text("Compiling, please wait...")
			.Foreground(*wxWHITE)
			.Background(wxColour(48,115,182))
			.Transparency(4 * wxALPHA_OPAQUE / 5)
			);
		wxExecute(cmd, output, errors);
	}
	wxEndBusyCursor();

	wxString outputS = wxEmptyString;
	wxString errorS = wxEmptyString;

	if (output.GetCount() > 0)
	{
		for (size_t i = 0; i < output.GetCount(); i++)
		{
			outputS += wxString::Format("\n%s", output[i]);
		}

		wxMessageBox(wxT("Output:\n") + outputS, wxT("AMX TextEdit"));
	}

	if (errors.GetCount() > 0)
	{
		wxSize curSize = GetSize();
		wxPoint curPos = GetPosition();
		wxPoint dlgPos = wxPoint(curPos.x + 10, curPos.y + curSize.GetHeight() - 180);
		errorDlg = new AMXCCPPErrorDlg(this, errors, dlgPos, curSize.GetWidth() - 20);
		errorDlg->Show();
		return false;
	}
	else
	{
		GetStatusBar()->SetStatusText(wxT("Compilation successful!"));
		return true;
	}
}

void AMXTextEdit::RunCCPP(wxString filename)
{
	if (wxFile::Exists(filename.BeforeLast('.') + wxT(".exe")))
	{
		wxSystem(wxString::Format("%s.exe", filename.BeforeLast('.')));
	}
	else
	{
		wxMessageBox(wxT("The file is not compiled!"), wxT("AMX TextEdit"));
	}
}

int AMXTextEdit::DoFind(wxString needle, int flags)
{
	AMXPage* page = (AMXPage*)(mainBook->GetCurrentPage());
	
	int pos;

	if (lastFindPos == -1)
	{
		lastFindPos = pos = page->txtBody->FindText(0, page->txtBody->GetTextLength(), needle, flags);
	}
	else
	{
		lastFindPos = pos = page->txtBody->FindText(lastFindPos + 1, page->txtBody->GetTextLength(), needle, flags);
	}

	if (pos != -1)
	{
		page->txtBody->SetFocus();
		page->txtBody->SetSelectionStart(pos);
		page->txtBody->SetSelectionEnd(pos + needle.Length());
	}

	return pos;
}

bool AMXTextEdit::DoReplace(wxString str, wxString rep, int flags, bool replaceAll = false)
{
	AMXPage* page = (AMXPage*)(mainBook->GetCurrentPage());
	
	if (page->txtBody->GetSelectedText() == str)
	{
		page->txtBody->ReplaceSelection(rep);
		DoFind(str, flags);
		return true;
	}

	int pos = DoFind(str, flags);
	
	if (replaceAll)
	{
		bool anyReplaced = false;

		while (pos != -1)
		{
			page->txtBody->Replace(pos, pos + str.Length(), rep);
			pos = DoFind(str, flags);
			anyReplaced = true;
		}

		return anyReplaced;
	}

	if (pos == -1) return false;
	
	page->txtBody->Replace(pos, pos + str.Length(), rep);
	
	return true;
}

void AMXTextEdit::EnableEditMenus(bool e = true)
{
	FindItemInMenuBar(wxID_SAVE)->Enable(e);
	FindItemInMenuBar(wxID_SAVEAS)->Enable(e);
	FindItemInMenuBar(wxID_UNDO)->Enable(e);
	FindItemInMenuBar(wxID_REDO)->Enable(e);
	FindItemInMenuBar(wxID_SELECTALL)->Enable(e);
	FindItemInMenuBar(wxID_COPY)->Enable(e);
	FindItemInMenuBar(wxID_CUT)->Enable(e);
	FindItemInMenuBar(wxID_PASTE)->Enable(e);
	FindItemInMenuBar(ID_MENU_SELECTFONT)->Enable(e);
	FindItemInMenuBar(wxID_FIND)->Enable(e);
}

void AMXTextEdit::EnableCCPPMenus(bool e = true)
{
	FindItemInMenuBar(ID_MENU_COMPILE_RUN)->Enable(e);
	FindItemInMenuBar(ID_MENU_COMPILE)->Enable(e);
	FindItemInMenuBar(ID_MENU_RUN)->Enable(e);
}

void AMXTextEdit::EnableCPPSyntaxHighlighting(bool e = true)
{
	AMXPage* page = (AMXPage*)(mainBook->GetCurrentPage());

	page->ccppSyntaxHighlighting = e;
	if (e)
	{
		page->txtBody->SetLexer(wxSTC_LEX_CPP);
		page->txtBody->StyleSetForeground(wxSTC_C_STRING, wxColour(150, 0, 0));
		page->txtBody->StyleSetForeground(wxSTC_C_PREPROCESSOR, wxColour(165, 105, 0));
		page->txtBody->StyleSetForeground(wxSTC_C_IDENTIFIER, wxColour(40, 0, 60));
		page->txtBody->StyleSetForeground(wxSTC_C_NUMBER, wxColour(150, 0, 0));
		page->txtBody->StyleSetForeground(wxSTC_C_CHARACTER, wxColour(150, 0, 0));
		page->txtBody->StyleSetForeground(wxSTC_C_WORD, wxColour(0, 0, 150));
		page->txtBody->StyleSetForeground(wxSTC_C_WORD2, wxColour(0, 150, 0));
		page->txtBody->StyleSetForeground(wxSTC_C_COMMENT, wxColour(150, 150, 150));
		page->txtBody->StyleSetForeground(wxSTC_C_COMMENTLINE, wxColour(150, 150, 150));
		page->txtBody->StyleSetForeground(wxSTC_C_COMMENTDOC, wxColour(150, 150, 150));
		page->txtBody->StyleSetForeground(wxSTC_C_COMMENTDOCKEYWORD, wxColour(0, 0, 200));
		page->txtBody->StyleSetForeground(wxSTC_C_COMMENTDOCKEYWORDERROR, wxColour(0, 0, 200));
		page->txtBody->StyleSetBold(wxSTC_C_WORD, true);
		page->txtBody->StyleSetBold(wxSTC_C_WORD2, true);
		page->txtBody->StyleSetBold(wxSTC_C_COMMENTDOCKEYWORD, true);

		page->txtBody->SetKeyWords(0, wxT("auto break case char const continue default do double else enum extern float \
										   for goto if int long register return short signed sizeof static struct switch  \
										   typedef union unsigned void volatile while \
										   asm bool catch class const_cast delete dynamic_cast explicit false friend inline \
										   mutable namespace new operator private public protected reinterpret_cast static_cast \
										   template this throw true try typeid typename using virtual wchar_t \
										   and and_eq bitand bitor compl not not_eq or or_eq xor xor_eq"));
		page->txtBody->SetKeyWords(1, wxT("cin cout endl include INT_MIN INT_MAX iomanip iostream main MAX_RAND npos NULL std string"));
	}
	else
	{
		page->txtBody->StyleClearAll();
	}
}

void AMXTextEdit::EnableCodeFolding(bool e = true)
{
	AMXPage* page = (AMXPage*)(mainBook->GetCurrentPage());

	page->codeFolding = e;
	if (e)
	{
		page->txtBody->SetMarginType(MARGIN_FOLD, wxSTC_MARGIN_SYMBOL);
		page->txtBody->SetMarginWidth(MARGIN_FOLD, 15);
		page->txtBody->SetMarginMask(MARGIN_FOLD, wxSTC_MASK_FOLDERS);
		page->txtBody->StyleSetBackground(MARGIN_FOLD, wxColor(200, 200, 200));
		page->txtBody->SetMarginSensitive(MARGIN_FOLD, true);

		// Properties found from http://www.scintilla.org/SciTEDoc.html
		page->txtBody->SetProperty(wxT("fold"), wxT("1"));
		page->txtBody->SetProperty(wxT("fold.comment"), wxT("1"));
		page->txtBody->SetProperty(wxT("fold.compact"), wxT("1"));

		wxColor grey(100, 100, 100);
		page->txtBody->MarkerDefine(wxSTC_MARKNUM_FOLDER, wxSTC_MARK_ARROW);
		page->txtBody->MarkerSetForeground(wxSTC_MARKNUM_FOLDER, grey);
		page->txtBody->MarkerSetBackground(wxSTC_MARKNUM_FOLDER, grey);

		page->txtBody->MarkerDefine(wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_ARROWDOWN);
		page->txtBody->MarkerSetForeground(wxSTC_MARKNUM_FOLDEROPEN, grey);
		page->txtBody->MarkerSetBackground(wxSTC_MARKNUM_FOLDEROPEN, grey);

		page->txtBody->MarkerDefine(wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_EMPTY);
		page->txtBody->MarkerSetForeground(wxSTC_MARKNUM_FOLDERSUB, grey);
		page->txtBody->MarkerSetBackground(wxSTC_MARKNUM_FOLDERSUB, grey);

		page->txtBody->MarkerDefine(wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_ARROW);
		page->txtBody->MarkerSetForeground(wxSTC_MARKNUM_FOLDEREND, grey);
		page->txtBody->MarkerSetBackground(wxSTC_MARKNUM_FOLDEREND, _T("WHITE"));

		page->txtBody->MarkerDefine(wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_ARROWDOWN);
		page->txtBody->MarkerSetForeground(wxSTC_MARKNUM_FOLDEROPENMID, grey);
		page->txtBody->MarkerSetBackground(wxSTC_MARKNUM_FOLDEROPENMID, _T("WHITE"));

		page->txtBody->MarkerDefine(wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_EMPTY);
		page->txtBody->MarkerSetForeground(wxSTC_MARKNUM_FOLDERMIDTAIL, grey);
		page->txtBody->MarkerSetBackground(wxSTC_MARKNUM_FOLDERMIDTAIL, grey);

		page->txtBody->MarkerDefine(wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_EMPTY);
		page->txtBody->MarkerSetForeground(wxSTC_MARKNUM_FOLDERTAIL, grey);
		page->txtBody->MarkerSetBackground(wxSTC_MARKNUM_FOLDERTAIL, grey);
	}
	else
	{
		page->txtBody->SetMarginWidth(MARGIN_FOLD, 0);
	}
}

void AMXTextEdit::AddNewPage()
{
	AMXPage *newPage = NewPage(mainBook);
	mainBook->InsertPage(newPage->id, newPage, "Untitled", true);
	newPage->txtBody->SetFocus();
}

void AMXTextEdit::Redraw()
{
	if (this->IsMaximized())
	{
		this->Restore();
		this->Maximize();
		return;
	}

	int w, h;
	GetSize(&w, &h);
	this->SetSize(w - 1, h - 1);
	this->SetSize(w, h);
}

void AMXTextEdit::EnableCPPMode(bool e = true)
{
	AMXPage* page = (AMXPage*)(mainBook->GetCurrentPage());

	if (page->filename.EndsWith(wxT(".c")) ||
		page->filename.EndsWith(wxT(".C")) ||
		page->filename.EndsWith(wxT(".cpp")) ||
		page->filename.EndsWith(wxT(".CPP")) ||
		page->filename.EndsWith(wxT(".h")) ||
		page->filename.EndsWith(wxT(".H"))
		)
	{
		EnableCPPSyntaxHighlighting(e);
		EnableCodeFolding(e);
		FindItemInMenuBar(ID_MENU_ENABLECPP)->Check(e);
		FindItemInMenuBar(ID_MENU_ENABLECF)->Check(e);
		page->ccpp = e;
		EnableCCPPMenus(e);
	}
}

void AMXTextEdit::CloseCurrentPage()
{
	AMXPage* page = (AMXPage*)(mainBook->GetCurrentPage());

	mainBook->DeletePage(page->id);
	
	if (mainBook->GetPageCount() < 1)
	{
		AddNewPage();
	}
}

void AMXTextEdit::SaveFile(AMXPage* page)
{
	wxFile file;
	file.Create(page->filename, true);

	if (file.IsOpened())
		file.Write(page->txtBody->GetValue());

	file.Close();

	page->saved = true;
	mainBook->SetPageText(page->id, page->filename.AfterLast('\\'));
	GetStatusBar()->SetStatusText(wxT("Save successfull!"));
}

void AMXTextEdit::OnMarginClick(wxStyledTextEvent& event)
{
	AMXPage* page = (AMXPage*)(mainBook->GetCurrentPage());

	if (event.GetMargin() == MARGIN_FOLD)
	{
		int lineClick = page->txtBody->LineFromPosition(event.GetPosition());
		int levelClick = page->txtBody->GetFoldLevel(lineClick);

		if ((levelClick & wxSTC_FOLDLEVELHEADERFLAG) > 0)
		{
			page->txtBody->ToggleFold(lineClick);
		}
	}
}

void AMXTextEdit::OnClose(wxCloseEvent& event)
{
	wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Are you sure to quit?"), wxT("AMX TextEdit"), wxYES_NO | wxNO_DEFAULT | wxICON_WARNING);

	int ret = dial->ShowModal();
	dial->Destroy();

	if (ret == wxID_YES) {
		Destroy();
	}
	else {
		event.Veto();
	}
}

void AMXTextEdit::About(wxCommandEvent& event)
{
	(new AMXAboutDlg(this))->Show(true);
}

void AMXTextEdit::Exit(wxCommandEvent& event)
{
	Close();
}

void AMXTextEdit::New(wxCommandEvent& event)
{
	AddNewPage();
}

void AMXTextEdit::Open(wxCommandEvent& event)
{
	wxFileDialog * openFileDialog = new wxFileDialog(this, _("Open a file"), "", "", "All Files (*.*)|*.*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (openFileDialog->ShowModal() == wxID_OK){
		AddNewPage();
		AMXPage* page = (AMXPage*)(mainBook->GetCurrentPage());
		page->filename = openFileDialog->GetPath();

		EnableCPPMode(true);

		wxTextFile file(page->filename);

		file.Open();

		page->txtBody->Clear();

		for (wxString s = file.GetFirstLine(); !file.Eof(); s = file.GetNextLine())
		{
			page->txtBody->AppendText(s);
			page->txtBody->AppendText("\n");
		}

		file.Close();

		mainBook->SetPageText(page->id, page->filename.AfterLast('\\'));
		GetStatusBar()->SetStatusText(wxT("Load successfull!"));
	}

	delete openFileDialog;
}

void AMXTextEdit::Save(wxCommandEvent& event)
{
	if (mainBook->GetPageCount() < 1)
	{
		GetStatusBar()->SetStatusText(wxT("Please open or create a new file to save"));
		return;
	}

	AMXPage* page = (AMXPage*)(mainBook->GetCurrentPage());
	switch (event.GetId())
	{

	case wxID_SAVE:
	{
					  if (page->filename != wxEmptyString)
					  {
						  SaveFile(page);
						  break;
					  }
	}

	case wxID_SAVEAS:
	{
						wxFileDialog * saveFileDialog = new wxFileDialog(this, _("Save the file"), "", "", "All files (*.*)|*.*", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
						if (saveFileDialog->ShowModal() == wxID_OK)
						{
							page->filename = saveFileDialog->GetPath();
							//if (!page->filename.EndsWith(wxT(".txt")) && !page->filename.EndsWith(wxT(".TXT")))
							//{
							//	page->filename += ".txt";
							//}

							EnableCPPMode(true);

							SaveFile(page);
						}
						delete saveFileDialog;
						break;
	}

	}

}

void AMXTextEdit::Edit(wxCommandEvent& event)
{
	if (mainBook->GetPageCount() < 1)
	{
		GetStatusBar()->SetStatusText(wxT("Please open or create a new file to edit"));
		return;
	}

	AMXPage* page = (AMXPage*)(mainBook->GetCurrentPage());
	switch (event.GetId())
	{
	case wxID_UNDO:
		if (page->txtBody->CanUndo())
		{
			page->txtBody->Undo();
		}
		break;

	case wxID_REDO:
		if (page->txtBody->CanRedo())
		{
			page->txtBody->Redo();
		}
		break;

	case wxID_SELECTALL:
		page->txtBody->SelectAll();
		break;

	case wxID_COPY:
		if (page->txtBody->CanCopy())
		{
			page->txtBody->Copy();
		}
		break;

	case wxID_CUT:
		if (page->txtBody->CanCut())
		{
			page->txtBody->Cut();
		}
		break;

	case wxID_PASTE:
		if (page->txtBody->CanPaste())
		{
			page->txtBody->Paste();
		}
		break;

	case wxID_FIND:
		if (!page->txtBody->IsEmpty())
		{
			if (frDlg)
			{
				delete frDlg;
				frDlg = NULL;
			}
			else
			{
				frData;
				frDlg = new wxFindReplaceDialog(this, &frData, wxT("AMX TextEdit - Find"));
				frDlg->Show(true);
			}
		}
		else
		{
			GetStatusBar()->SetStatusText("Nothing in there yet. No haystack, no finding needle!");
		}
		break;

	case wxID_REPLACE:
		if (!page->txtBody->IsEmpty())
		{
			if (frDlg)
			{
				delete frDlg;
				frDlg = NULL;
			}
			else
			{
				frData;
				frDlg = new wxFindReplaceDialog(this, &frData, wxT("AMX TextEdit - Find"), wxFR_REPLACEDIALOG);
				frDlg->Show(true);
			}
		}
		else
		{
			GetStatusBar()->SetStatusText("Nothing in there yet. No haystack, no replacing needle!");
		}
		break;

	case ID_MENU_GOTOLINE:
	{
							 AMXGotoLineDlg* dlg = new AMXGotoLineDlg(this);
							 if (dlg->ShowModal() == wxID_OK)
							 {
								 long line = -1;
								 dlg->lineNum->GetValue().ToLong(&line);

								 if (line == -1)
								 {
									 wxMessageBox(wxT("Please enter a valid line number!"));
									 return;
								 }

								 if (line > page->txtBody->GetNumberOfLines())
								 {
									 wxMessageBox(wxString::Format("There are no more than %d lines in the current document!", page->txtBody->GetNumberOfLines()));
									 return;
								 }

								 page->txtBody->GotoLine(line - 1);
							 }
							 dlg->Destroy();
							 delete dlg;
	}
		break;

	case ID_POPUPMENU_CLOSE:
	{
							   wxMessageDialog dial(NULL, wxT("Are you sure?"), wxT("AMX TextEdit"), wxYES_NO | wxNO_DEFAULT | wxICON_WARNING);

							   int ret = dial.ShowModal();

							   if (ret == wxID_YES)
							   {
								   CloseCurrentPage();
							   }
	}
		break;

	}
}

void AMXTextEdit::Options(wxCommandEvent& event)
{
	if (mainBook->GetPageCount() < 1)
	{
		GetStatusBar()->SetStatusText(wxT("Please open or create a new file to choose editor font"));
		return;
	}

	AMXPage* page = (AMXPage*)(mainBook->GetCurrentPage());
	switch (event.GetId())
	{
	case ID_MENU_SELECTFONT:
	{
							   wxFontDialog* fontDialog = new wxFontDialog(this);

							   if (fontDialog->ShowModal() == wxID_OK)
							   {
								   page->txtBody->StyleSetFont(wxSTC_STYLE_DEFAULT, fontDialog->GetFontData().GetChosenFont());
								   GetStatusBar()->SetStatusText(wxT("Font loaded"));
							   }

							   delete fontDialog;
	}
		break;

	case ID_MENU_TABSTOP:
		mainBook->SetWindowStyle(wxNB_TOP);
		Redraw();
		break;

	case ID_MENU_TABSBOTTOM:
		mainBook->SetWindowStyle(wxNB_BOTTOM);
		Redraw();
		break;

	case ID_MENU_ENABLECPP:
		EnableCPPSyntaxHighlighting(FindItemInMenuBar(ID_MENU_ENABLECPP)->IsChecked());
		break;

	case ID_MENU_ENABLECF:
		EnableCodeFolding(FindItemInMenuBar(ID_MENU_ENABLECF)->IsChecked());
		break;

	}
}

void AMXTextEdit::CCPP(wxCommandEvent& event)
{
	AMXPage* page = (AMXPage*)(mainBook->GetCurrentPage());

	if (!page->ccpp)
	{
		wxMessageBox(wxT("The current file is not saved as a C/C++ source file!"), wxT("AMX TextEdit"));
		return;
	}

	if (errorDlg != NULL && errorDlg->IsShown())
	{
		errorDlg->Destroy();
		delete errorDlg;
		errorDlg = NULL;
	}

	SaveFile(page);

	switch (event.GetId())
	{
	case ID_MENU_COMPILE_RUN:
		if (CompileCCPP(page->filename))
		{
			RunCCPP(page->filename);
		}
		break;

	case ID_MENU_COMPILE:
		CompileCCPP(page->filename);
		break;

	case ID_MENU_RUN:
		RunCCPP(page->filename);
		break;
	}
}

void AMXTextEdit::OnFind(wxFindDialogEvent& event)
{
	AMXPage* page = (AMXPage*)(mainBook->GetCurrentPage());

	int pos = DoFind(event.GetFindString(), event.GetFlags());

	if (pos == -1)
	{
		wxMessageBox(wxT("No more matches!"), wxT("AMX TextEdit - Find"));
	}
}

void AMXTextEdit::OnReplace(wxFindDialogEvent& event)
{
	if (!DoReplace(event.GetFindString(), event.GetReplaceString(), event.GetFlags()))
	{
		wxMessageBox(wxT("No more matches."));
	}
}

void AMXTextEdit::OnReplaceAll(wxFindDialogEvent& event)
{
	if (DoReplace(event.GetFindString(), event.GetReplaceString(), event.GetFlags(), true))
	{
		wxMessageBox(wxT("Replacements made."));
	}
	else
	{
		wxMessageBox(wxT("No replacements made."));
	}
}

void AMXTextEdit::OnFindClose(wxFindDialogEvent& event)
{
	lastFindPos = -1;
	frDlg->Destroy();
	frDlg = NULL;
}

void AMXTextEdit::OnContextMenu(wxContextMenuEvent& event)
{
	wxMenu popupMenu;
	popupMenu.Append(ID_POPUPMENU_CLOSE, wxT("Close current tab"));

	AMXPage* page = (AMXPage*)(mainBook->GetCurrentPage());

	PopupMenu(&popupMenu);
}

void AMXTextEdit::PageChanged(wxBookCtrlEvent& event)
{
	AMXPage* page = (AMXPage*)(mainBook->GetCurrentPage());

	EnableCCPPMenus(page->ccpp);
	FindItemInMenuBar(ID_MENU_ENABLECF)->Check(page->codeFolding);
	FindItemInMenuBar(ID_MENU_ENABLECPP)->Check(page->ccppSyntaxHighlighting);
}

AMXPage* AMXTextEdit::NewPage(wxNotebook* book)
{
	EnableEditMenus();

	int currentID = book->GetPageCount();

	AMXPage *page = new AMXPage(book, currentID);

	page->id = currentID;
	page->filename = wxEmptyString;
	page->saved = false;
	page->ccpp = false;
	page->ccppSyntaxHighlighting = false;
	page->codeFolding = false;

	wxBoxSizer* vSizer = new wxBoxSizer(wxVERTICAL);

	page->txtBody = new wxStyledTextCtrl(page, -1, wxDefaultPosition, wxDefaultSize, wxSTC_STYLE_LINENUMBER);
	
	page->txtBody->StyleClearAll();
	page->txtBody->SetMarginWidth(MARGIN_LINE_NUMBERS, 30);
	page->txtBody->SetMarginType(MARGIN_LINE_NUMBERS, wxSTC_MARGIN_NUMBER);

	vSizer->Add(page->txtBody, 1, wxALL | wxEXPAND, 5);
	page->SetSizer(vSizer);

	return page;
}
