/*
	LineCtrl include file
*/


#ifndef		_LINECTRL_H
#define		_LINECTRL_H

class LineCtrl : public wxTextCtrl

{
public:
    LineCtrl (wxWindow *parent, wxWindowID id,
               const wxString& value = wxEmptyString,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = 0,
               const wxValidator& validator = wxDefaultValidator,
               const wxString& name = wxTextCtrlNameStr);


	void OnSetFocus (wxFocusEvent& event);

    DECLARE_EVENT_TABLE()
};

#endif			// End of #ifndef _LINECTRL_H


