///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/combobox.h>
#include <wx/button.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

#define ID_EDIT_JOBS 1000
#define ID_EDIT_TODAYS_JOB_HISTORY 1001
#define ID_HISTORY_SUMMARY 1002

///////////////////////////////////////////////////////////////////////////////
/// Class MyFrame
///////////////////////////////////////////////////////////////////////////////
class MyFrame : public wxFrame
{
	private:

	protected:
		wxMenuBar* m_menubar1;
		wxMenu* mnuAdmin;
		wxMenu* mnuReports;
		wxStaticText* m_txtCurrentJob;
		wxStaticText* m_txtElapsed;
		wxComboBox* m_cbJobsMru;
		wxButton* m_btnStartStop;

		// Virtual event handlers, overide them in your derived class
		virtual void mni_editJobsOnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void mniEditTodaysJobHistoryOnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void mniHistorySummaryOnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_btnStartStopOnButtonClick( wxCommandEvent& event ) { event.Skip(); }


	public:

		MyFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,140 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~MyFrame();

};

