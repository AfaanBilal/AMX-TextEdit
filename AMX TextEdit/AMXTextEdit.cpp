/*
 *    AMX TextEdit
 *    Author: Afaan Bilal
 *    https://www.coderevolution.tk
 *
 */

#include "AMXTextEdit.h"

AMXTextEdit::AMXTextEdit(const wxString& title)
: wxFrame(NULL, -1, title, wxPoint(-1, -1), wxSize(580, 640))
{
	SetIcon(wxIcon(wxT("MAIN_ICON")));
	lastFindPos = -1;

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
	editMenu->Append(new wxMenuItem(editMenu, wxID_SELECTALL, wxT("Select All"), wxT("Select all text")));
	editMenu->Append(new wxMenuItem(editMenu, wxID_COPY, wxT("Copy\tCtrl+C"), wxT("Copy selected text")));
	editMenu->Append(new wxMenuItem(editMenu, wxID_CUT, wxT("Cut\tCtrl+X"), wxT("Cut selected text")));
	editMenu->Append(new wxMenuItem(editMenu, wxID_PASTE, wxT("Paste\tCtrl+V"), wxT("Paste copied text")));
	editMenu->Append(new wxMenuItem(editMenu, -1, wxEmptyString, wxEmptyString, wxITEM_SEPARATOR));
	editMenu->Append(new wxMenuItem(editMenu, wxID_FIND, wxT("Find\tCtrl+F"), wxT("Find something")));
	editMenu->Append(new wxMenuItem(editMenu, wxID_REPLACE, wxT("Replace\tCtrl+H"), wxT("Replace something")));
	mainMenu->Append(editMenu, wxT("&Edit"));

	wxMenu* fontMenu = new wxMenu;
	fontMenu->Append(new wxMenuItem(fontMenu, ID_MENU_SELECTFONT, wxT("Choose font"), wxT("Select a font")));
	mainMenu->Append(fontMenu, wxT("&Font"));

	wxMenu* helpMenu = new wxMenu;
	helpMenu->Append(new wxMenuItem(helpMenu, wxID_ABOUT, wxT("&About"), wxT("About AMX TextEdit")));
	mainMenu->Append(helpMenu, wxT("&Help"));

	SetMenuBar(mainMenu);
	CreateStatusBar();

	EnableEditMenus(false);

	mainBook = new wxNotebook(this, -1, wxDefaultPosition, wxDefaultSize, wxNB_TOP);

	Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(AMXTextEdit::OnClose));

	Connect(wxID_ABOUT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(AMXTextEdit::About));
	Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(AMXTextEdit::Exit));
	Connect(wxID_NEW, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(AMXTextEdit::New));
	Connect(wxID_OPEN, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(AMXTextEdit::Open));
	Connect(wxID_SAVE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(AMXTextEdit::Save));
	Connect(wxID_SAVEAS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(AMXTextEdit::Save));
	Connect(wxID_SELECTALL, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(AMXTextEdit::Edit));
	Connect(wxID_COPY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(AMXTextEdit::Edit));
	Connect(wxID_CUT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(AMXTextEdit::Edit));
	Connect(wxID_PASTE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(AMXTextEdit::Edit));
	Connect(wxID_FIND, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(AMXTextEdit::Edit));
	Connect(wxID_REPLACE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(AMXTextEdit::Edit));
	Connect(ID_MENU_SELECTFONT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(AMXTextEdit::Font));

	Connect(wxEVT_FIND, wxFindDialogEventHandler(AMXTextEdit::OnFind));
	Connect(wxEVT_FIND_NEXT, wxFindDialogEventHandler(AMXTextEdit::OnFind));
	Connect(wxEVT_FIND_REPLACE, wxFindDialogEventHandler(AMXTextEdit::OnReplace));
	Connect(wxEVT_FIND_REPLACE_ALL, wxFindDialogEventHandler(AMXTextEdit::OnReplaceAll));
	Connect(wxEVT_FIND_CLOSE, wxFindDialogEventHandler(AMXTextEdit::OnFindClose));

	Centre();

	AddNewPage();
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

void AMXTextEdit::EnableEditMenus(bool e = true)
{
	FindItemInMenuBar(wxID_SAVE)->Enable(e);
	FindItemInMenuBar(wxID_SAVEAS)->Enable(e);
	FindItemInMenuBar(wxID_SELECTALL)->Enable(e);
	FindItemInMenuBar(wxID_COPY)->Enable(e);
	FindItemInMenuBar(wxID_CUT)->Enable(e);
	FindItemInMenuBar(wxID_PASTE)->Enable(e);
	FindItemInMenuBar(ID_MENU_SELECTFONT)->Enable(e);
	FindItemInMenuBar(wxID_FIND)->Enable(e);
}

AMXPage* AMXTextEdit::NewPage(wxNotebook* book)
{
	EnableEditMenus();

	int currentID = book->GetPageCount();

	AMXPage *page = new AMXPage(book, currentID);

	page->id = currentID;
	page->filename = wxEmptyString;

	wxBoxSizer* vSizer = new wxBoxSizer(wxVERTICAL);

	page->txtBody = new wxStyledTextCtrl(page, -1, wxDefaultPosition, wxDefaultSize, wxSTC_STYLE_LINENUMBER);
	page->txtBody->SetMarginType(0, wxSTC_MARGIN_NUMBER);
	page->txtBody->SetMarginWidth(0, 25);
	page->txtBody->StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColour(75, 75, 75));
	page->txtBody->StyleSetBackground(wxSTC_STYLE_LINENUMBER, wxColour(220, 220, 220));
		
	vSizer->Add(page->txtBody, 1, wxALL | wxEXPAND, 5);
	page->SetSizer(vSizer);
	
	return page;
}

void AMXTextEdit::AddNewPage()
{
	AMXPage *newPage = NewPage(mainBook);
	mainBook->InsertPage(newPage->id, newPage, "Untitled", true);
	newPage->txtBody->SetFocus();
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
	wxMessageBox(wxT("AMX TextEdit\nA simple, tabbed text editor.\n\n(c) Afaan Bilal\n\nwww.coderevolution.tk\ngoogle.com/+AfaanBilal"), wxT("About AMX TextEdit"), wxICON_INFORMATION);
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
	wxFileDialog * openFileDialog = new wxFileDialog(this, _("Open a text file"), "", "", "Text Files (*.*)|*.*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (openFileDialog->ShowModal() == wxID_OK){
		AddNewPage();
		AMXPage* page = (AMXPage*)(mainBook->GetCurrentPage());		
		page->filename = openFileDialog->GetPath();
		
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
						wxFileDialog * saveFileDialog = new wxFileDialog(this, _("Save the text file"), "", "", "Text files (*.*)|*.*", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
						if (saveFileDialog->ShowModal() == wxID_OK)
						{
							page->filename = saveFileDialog->GetPath();
							if (!page->filename.EndsWith(wxT(".txt")) && !page->filename.EndsWith(wxT(".TXT")))
							{
								page->filename += ".txt";
							}

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

	}
}

void AMXTextEdit::Font(wxCommandEvent& event)
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
	}
}

