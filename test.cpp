/* wxWindows application to test Ini file I/O and line control */


// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all <standard< wxWindows headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "inifile.h"
#include "linectrl.h"

#include "test.h"

IMPLEMENT_APP(MyApp)


// `Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
  MyFrame *frame = new MyFrame ("Test", 200, 150, 450, 300);

  frame->Show(TRUE);
  SetTopWindow(frame);
  return TRUE;	
}

MyFrame::MyFrame (const wxChar *title, int xpos, int ypos, int width, int height)
             : wxFrame( (wxFrame *) NULL, -1, title, wxPoint(xpos, ypos), wxSize(width, height))
{

	m_panel = new MyPanel(this);

}

MyFrame::~MyFrame()
{

}


MyPanel::MyPanel (wxFrame *frame)
       : wxPanel (frame, -1)
{
	wxString	ExecutablePathString;

	ExecutablePathString = FindExecutablePath (wxTheApp->argv[0], wxGetCwd());

	if (ExecutablePathString.Last() != wxFILE_SEP_PATH)			// Make sure the last character is a separator
		ExecutablePathString += wxFILE_SEP_PATH;

	AppIniFile = new IniFile (ExecutablePathString + _T("testfile.ini"));
//	AppIniFile = new IniFile (_T("testfile.ini"));

										// Get size of parent from .ini file and re-size
	wxString SizeString;
										// Check to see if size has prevoiusly been saved
	if (AppIniFile->ReadIniStringFromFile (_T("SIZE"), &SizeString) == TRUE)
	{
		wxWindow *Parent = GetParent ();

		int x, y, w, h;
		sscanf (SizeString, "%d, %d, %d, %d", &x, &y, &w, &h);

		wxRect Rect (x, y, w, h);
		
		Parent->SetSize (Rect);

// Debug
//		wxMessageBox (_T(SizeString), _T("Size"), wxOK | wxICON_INFORMATION, this);
	}



    wxBoxSizer *TopSizer = new wxBoxSizer(wxHORIZONTAL);

    wxBoxSizer *LeftSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *RightSizer = new wxBoxSizer(wxVERTICAL);
    TopSizer->Add (LeftSizer, 0, wxALIGN_RIGHT | wxALL, 10 );
    TopSizer->Add (RightSizer, 0, wxALIGN_RIGHT | wxALL, 10 );

    WriteKeyLineCtrl = new LineCtrl (this, -1, "WriteKey");
    WriteStringLineCtrl = new LineCtrl (this, -1, "WriteString");
    wxButton *WriteButton = new wxButton (this, PANEL_WRITE_BUTTON, "Write");

    LeftSizer->Add (WriteKeyLineCtrl, 1, wxEXPAND | wxALL, 5);
    LeftSizer->Add (WriteStringLineCtrl, 1, wxEXPAND | wxALL, 5);
    LeftSizer->Add (WriteButton, 1, wxEXPAND | wxALL, 5);

    ReadKeyLineCtrl = new LineCtrl (this, -1, "ReadKey");
    ReadStringLineCtrl = new LineCtrl (this, -1, "ReadString", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    wxButton *ReadButton = new wxButton (this, PANEL_READ_BUTTON, "Read");

    RightSizer->Add (ReadKeyLineCtrl, 1, wxEXPAND | wxALL, 5);
    RightSizer->Add (ReadStringLineCtrl, 1, wxEXPAND | wxALL, 5);
    RightSizer->Add (ReadButton, 1, wxEXPAND | wxALL, 5);

    SetAutoLayout (TRUE);
    SetSizer (TopSizer);

    TopSizer->SetSizeHints (this);
    TopSizer->Fit (this);

}


MyPanel::~MyPanel()
{
	wxString SizeString;
	wxWindow *Parent = GetParent ();
	wxRect Rect = Parent->GetRect ();

							// Save size of parent window so that it can be reloaded on startup
	SizeString.sprintf ("%d, %d, %d, %d", Rect.GetX (), Rect.GetY (), Rect.GetWidth (), Rect.GetHeight ());

// Debug
//	wxMessageBox (_T(SizeString), _T("Size"), wxOK | wxICON_INFORMATION, this);

	AppIniFile->WriteIniStringToFile (_T("SIZE"), _T(SizeString));
}


void MyPanel::OnReadButtonClicked ()
{
//	wxMessageBox (_T("OnReadButtonClicked"), _T("Got here"), wxOK | wxICON_INFORMATION, this);

	wxString String;

	if (AppIniFile->ReadIniStringFromFile (ReadKeyLineCtrl->GetValue (), &String) == TRUE)
	{
		ReadStringLineCtrl->SetValue (String);
	}
	else
	{
		wxMessageBox (_T(".ini file does not exist or is empty\nor the read string does not exist in the file"), _T("Read button"), wxOK | wxICON_INFORMATION, this);
	}


}


void MyPanel::OnWriteButtonClicked ()
{
//	wxMessageBox (_T("OnWriteButtonClicked"), _T("Got here"), wxOK | wxICON_INFORMATION, this);

	AppIniFile->WriteIniStringToFile (WriteKeyLineCtrl->GetValue (), WriteStringLineCtrl->GetValue ());
}


/**/
/********************************************************
* Function : MyPanel::FindExecutablePath ()
*
* Parameters :
*
* Return value :
*	void
*
* Description : Find the directory path of the executable
*				This function has been taken from the
*				wxWindows web site and modified slightly
*				because it does not scan the environment
*				variable.
* Find the absolute path where this application has been run from.
* argv0 is wxTheApp->argv[0]
* cwd is the current working directory (at startup)
*
********************************************************/

wxString MyPanel::FindExecutablePath (const wxString& argv0, const wxString& cwd)

{
	wxString str;

	if (wxIsAbsolutePath(argv0))
	{
		return wxPathOnly(argv0);
	}
	else
	{
		// Is it a relative path?
		wxString currentDir(cwd);

		if ((wxPathOnly(argv0) == ".") || (wxPathOnly(argv0) == ""))
			return (currentDir);
			
		if (currentDir.Last() != wxFILE_SEP_PATH)
			currentDir += wxFILE_SEP_PATH;

#if defined(__WXGTK__) || defined(__WXMOTIF__)
		str = currentDir + argv0;
#else
		str = currentDir + argv0 + wxString(".exe");
#endif
//	SUF_Debugfprintf ("mainFrame::FindExecutablePath : str = \"%s\"\n", str);
		if (wxFileExists(str))
			return wxPathOnly(str);
	}

	// OK, it's neither an absolute path nor a relative path.
	// Search PATH.

	wxPathList pathList;
	pathList.AddEnvList(wxT("PATH"));
	str = pathList.FindAbsoluteValidPath(argv0);
	if (!str.IsEmpty())
		return wxPathOnly(str);

	// Failed
	return wxEmptyString;
}


