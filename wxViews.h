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
#include <wx/timer.h>
#include <wx/frame.h>
#include <wx/listctrl.h>
#include <wx/dialog.h>
#include "tcontroller.h"
#include <vector>

///////////////////////////////////////////////////////////////////////////

#define ID_EDIT_JOBS 1000
#define ID_EDIT_TODAYS_JOB_HISTORY 1001
#define ID_HISTORY_SUMMARY 1002

class dlgJobsList;
class tController;


///////////////////////////////////////////////////////////////////////////////
/// Class MainView
///////////////////////////////////////////////////////////////////////////////
class MainView : public wxFrame
{
	private:
        const std::string StartText {"Start"};
        const std::string StopText {"Stop"};

	protected:
		wxMenuBar* m_menubar1;
		wxMenu* mnuAdmin;
		wxMenu* mnuReports;
		wxStaticText* txtCurrentJob;
		wxStaticText* txtElapsed;
		wxComboBox* cbJobsMru;
		wxButton* btnStartStop;
		wxTimer tmrElapsed;
        dlgJobsList* dlgJobs;
        int ElapsedSecs = 0;
        tController* pController {};

		// Virtual event handlers, overide them in your derived class
		virtual void mniAdmin_EditJobs_Click( wxCommandEvent& event );
		virtual void mniAdmin_EditJobHistory_Click( wxCommandEvent& event ) { event.Skip(); }
		virtual void mniReports_HistorySummary_Click( wxCommandEvent& event ) { event.Skip(); }
		virtual void cbJobsMru_ListDown( wxCommandEvent& event ) { event.Skip(); }
		virtual void btnStartStop_Click( wxCommandEvent& event );
		virtual void tmrElapsed_Tick( wxTimerEvent& event );


	public:

		explicit MainView( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,140 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~MainView();

        void SetController(tController*);
        void UpdateMruJobsList(std::vector<ttj::JobRec>);
};

///////////////////////////////////////////////////////////////////////////////
/// Class dlgJobsList
///////////////////////////////////////////////////////////////////////////////
class dlgJobsList : public wxDialog
{
	private:

	protected:
		wxStaticText* m_staticText3;
		wxListCtrl* lcJobList;
		wxButton* btnJobAdd;
		wxButton* btnJobDelete;
		wxButton* btnJobRetire;

	public:

		explicit dlgJobsList( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Jobs List"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 315,518 ), long style = wxDEFAULT_DIALOG_STYLE );
		~dlgJobsList() = default;

};

