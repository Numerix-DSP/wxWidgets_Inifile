#ifndef TEST_H
#define TEST_H

enum
{
	PANEL_READ_BUTTON = wxID_HIGHEST + 1,
	PANEL_WRITE_BUTTON,
};


class MyPanel : public wxPanel
{
public:
    MyPanel (wxFrame *frame);
    ~MyPanel ();

    void OnReadButtonClicked ();
    void OnWriteButtonClicked ();
	wxString FindExecutablePath (const wxString& argv0, const wxString& cwd);

private:
	IniFile *AppIniFile;

	LineCtrl *WriteKeyLineCtrl;
	LineCtrl *WriteStringLineCtrl;
	
	LineCtrl *ReadKeyLineCtrl;
	LineCtrl *ReadStringLineCtrl;
	
    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(MyPanel, wxPanel)
    EVT_BUTTON      (PANEL_READ_BUTTON,   MyPanel::OnReadButtonClicked)
    EVT_BUTTON      (PANEL_WRITE_BUTTON,   MyPanel::OnWriteButtonClicked)
END_EVENT_TABLE()


class MyApp : public wxApp
{
  public:
	virtual bool OnInit();
};

class MyFrame : public wxFrame
{
public:
	MyFrame ( const wxChar *title, int xpos, int ypos, int width, int height);
	~MyFrame ();

private:
    MyPanel *m_panel;

};

#endif

