#ifndef INIFILE_H
#define INIFILE_H

class IniFile
{
public:
	IniFile (wxString FileName);
	~IniFile ();

	bool ReadIniStringFromFile (wxString Key, wxString * String);
	bool WriteIniStringToFile (wxString Key, wxString String);

private:
	wxString IniFileName;
};

#endif

