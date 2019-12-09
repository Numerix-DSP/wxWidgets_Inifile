/* Extended text control functions

The line control class is virtually identical to the wxTextCtrl class
except that when it gets the focus, the entire text string will be selected.
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

#include "linectrl.h"


/**/
/********************************************************
* Function : LineCtrl::LineCtrl ()
*
* Parameters :
*
* Return value :
*	void
*
* Description : Line contron constructor
*
********************************************************/

LineCtrl::LineCtrl (wxWindow *parent, wxWindowID id, const wxString& value,
               const wxPoint& pos, const wxSize& size, long style,
               const wxValidator& validator, const wxString& name)
		 : wxTextCtrl (parent, id, value, pos, size, style, validator, name)

{
	
}

/********************************************************
* Function : LineCtrl::OnSetFocus ()
*
* Parameters :
*
* Return value :
*	void
*
* Description : Selects the entire string when control selected
*
********************************************************/

void LineCtrl::OnSetFocus (wxFocusEvent& event)

{
//	wxMessageBox ("Got Here", "LineCtrl::OnSetFocus", wxOK | wxICON_EXCLAMATION, NULL);

	SetSelection (-1, -1);	// Select all of the text
	event.Skip();
}


BEGIN_EVENT_TABLE (LineCtrl, wxWindow)
	EVT_SET_FOCUS (LineCtrl::OnSetFocus)
END_EVENT_TABLE()


