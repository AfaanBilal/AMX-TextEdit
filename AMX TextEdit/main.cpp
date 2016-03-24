/*
 *    AMX TextEdit
 *    Author: Afaan Bilal
 *    https://afaan.ml/AMX-TextEdit
 *    https://www.coderevolution.tk
 *
 */


#include "main.h"
#include "AMXTextEdit.h"

IMPLEMENT_APP(MainApp)

bool MainApp::OnInit()
{

	AMXTextEdit *amte = new AMXTextEdit(wxT("AMX TextEdit"));
	amte->Show(true);

	return true;
}
