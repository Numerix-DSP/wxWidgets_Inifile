/* .ini file read / write functions

The .ini file class allows the processing of a .ini file that will allow the storing
and retrieval of status and configuration information. The information is stored in a
given file name and has the format :
[KeyString]
InformationString

Where the KeyString signifies the type of information that is being stored and the
InformationString contains the specific information being stored.

InformationStrings can be overwritten with new data and new KeyStrings are appended to
the end of the file with the appropriate data.
*/


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

#include <wx/textfile.h>
#include <wx/thread.h>
#include "inifile.h"


/**/
/********************************************************
* Function : IniFile::IniFile ()
*
* Parameters :
*
* Return value :
*	void
*
* Description : ini file constructor
*
********************************************************/

IniFile::IniFile (wxString FileName)
{
	IniFileName = wxFindAppPath (wxTheApp->argv[0], wxGetCwd()) + wxFILE_SEP_PATH + FileName;

// Debug
//	wxMessageBox (IniFileName, _T("INI File Name"), wxOK | wxICON_INFORMATION, NULL);
}

/**/
/********************************************************
* Function : IniFile::~IniFile ()
*
* Parameters :
*
* Return value :
*	void
*
* Description : ini file destructor
*
********************************************************/

IniFile::~IniFile()
{

}


/**/
/********************************************************
* Function : IniFile::ReadIniStringFromFile ()
*
* Parameters :
*
* Return value :
*	void
*
* Description : Read a configuration parameter from ini file
*
********************************************************/

bool IniFile::ReadIniStringFromFile (wxString Key, wxString * String)

{
	wxString KeySearchString = wxString ("[") + Key + wxString ("]");

// Debug
//	wxMessageBox(_T(KeySearchString), _T("Search string"), wxOK | wxICON_INFORMATION, NULL);

						// Search for required string
	wxTextFile file(_T(IniFileName));

	if (!file.Exists ())
	{
		wxMessageBox(_T("The requested initialization file does not\nappear to be installed"), _T("File I/O error"), wxOK | wxICON_INFORMATION, NULL);
		return (FALSE);         
	}


	if (file.Open ())
	{
		wxString s;

		for ( s = file.GetFirstLine(); !file.Eof(); s = file.GetNextLine() )
		{
				// Check each line against Key, if same, add new line and delete old one
			if (s.c_str() == KeySearchString)
			{
				*String = file.GetNextLine();		// Get String

				if (file.Close() == FALSE)
				{
					wxMessageBox(_T("Can not close .ini file"),
						_T("File I/O error"), wxOK | wxICON_INFORMATION, NULL);
					return (FALSE);         
				}
				return (TRUE);
			}
		}
				// Ordinarily should do last line here but last line can't be a KEY
	}
	else
	{
		wxMessageBox(_T("Can not open .ini file"),
					 _T("File I/O error"), wxOK | wxICON_INFORMATION, NULL);
		return (FALSE);
	}

	// If KEY not present, KEY has not been found

	if (file.Close() == FALSE)
	{
		wxMessageBox(_T("Can not close .ini file"),
			_T("File I/O error"), wxOK | wxICON_INFORMATION, NULL);
		return (FALSE);         
	}

	return (FALSE);
}


/**/
/********************************************************
* Function : IniFile::WriteIniStringToFile ()
*
* Parameters :
*
* Return value :
*	void
*
* Description : Write a configuration parameter to ini file
*
********************************************************/

bool IniFile::WriteIniStringToFile (wxString Key, wxString String)
{
	wxString KeySearchString = wxString ("[") + Key + wxString ("]");

	bool KeyPresentFlag = FALSE;				// Flag set if key found


// Debug
//	wxMessageBox(_T(KeySearchString), _T("Search string"), wxOK | wxICON_INFORMATION, NULL);

						// Search for required string
//	wxMutexLocker lock(s_mutexProtectingTheGlobalData);
	s_mutexProtectingTheGlobalData.Lock();	

	wxTextFile file(_T(IniFileName));

	if (!file.Exists ())
	{
				// The requested .ini file was not installed so we will create it
		if (file.Create() == FALSE)
		{
			wxMessageBox(_T("Can not create .ini file"),
				_T("File I/O error"), wxOK | wxICON_INFORMATION, NULL);
			s_mutexProtectingTheGlobalData.Unlock();	
			return (FALSE);         
		}

	}

	else
	{
		if (file.Open())
		{
			wxString s;

			for ( s = file.GetFirstLine(); !file.Eof(); s = file.GetNextLine() )
			{
					// Check each line against Key, if same, add new line and delete old one
				if (s.c_str() == KeySearchString)
				{
					// If KEY is present, remove old string and update it with new one
					s = file.GetNextLine();
					file.RemoveLine (file.GetCurrentLine ());
					file.InsertLine (String, file.GetCurrentLine ());

					KeyPresentFlag = TRUE;			// Set flag to say that key was located
				}
			}
					// Ordinarily should do last line here but last line can't be a KEY
		        file.GetLastLine();					// Make sure we are at the end of the file
		}
		else
		{
			wxMessageBox(_T("Can not open .ini file"),
						 _T("File I/O error"), wxOK | wxICON_INFORMATION, NULL);
			s_mutexProtectingTheGlobalData.Unlock();	
			return (FALSE);
		}
	}

			// If KEY not present, write KEY and String at the bottom
	if (KeyPresentFlag == FALSE)
	{
		file.AddLine (KeySearchString);
		file.AddLine (String);
		file.AddLine ("");
	}

	if (file.Write() == FALSE)
	{
		wxMessageBox(_T("Can not write .ini file"),
			_T("File I/O error"), wxOK | wxICON_INFORMATION, NULL);
		s_mutexProtectingTheGlobalData.Unlock();	
		return (FALSE);         
	}

	if (file.Close() == FALSE)
	{
		wxMessageBox(_T("Can not close .ini file"),
			_T("File I/O error"), wxOK | wxICON_INFORMATION, NULL);
		s_mutexProtectingTheGlobalData.Unlock();	
		return (FALSE);         
	}

	s_mutexProtectingTheGlobalData.Unlock();	
	return (TRUE);
	
}

