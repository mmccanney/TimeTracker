///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wxMainFrame.h"

///////////////////////////////////////////////////////////////////////////

MyFrame::MyFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	m_menubar1 = new wxMenuBar( 0 );
	mnuAdmin = new wxMenu();
	wxMenuItem* mni_editJobs;
	mni_editJobs = new wxMenuItem( mnuAdmin, ID_EDIT_JOBS, wxString( wxT("Edit Jobs") ) , wxEmptyString, wxITEM_NORMAL );
	mnuAdmin->Append( mni_editJobs );

	wxMenuItem* mniEditTodaysJobHistory;
	mniEditTodaysJobHistory = new wxMenuItem( mnuAdmin, ID_EDIT_TODAYS_JOB_HISTORY, wxString( wxT("Edit todays job history") ) , wxEmptyString, wxITEM_NORMAL );
	mnuAdmin->Append( mniEditTodaysJobHistory );

	m_menubar1->Append( mnuAdmin, wxT("Admin") );

	mnuReports = new wxMenu();
	wxMenuItem* mniHistorySummary;
	mniHistorySummary = new wxMenuItem( mnuReports, ID_HISTORY_SUMMARY, wxString( wxT("History Summary...") ) , wxEmptyString, wxITEM_NORMAL );
	mnuReports->Append( mniHistorySummary );

	m_menubar1->Append( mnuReports, wxT("Reports") );

	this->SetMenuBar( m_menubar1 );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );

	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Logging:") ), wxVERTICAL );

	m_txtCurrentJob = new wxStaticText( sbSizer1->GetStaticBox(), wxID_ANY, wxT("- no logging -"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL );
	m_txtCurrentJob->Wrap( -1 );
	sbSizer1->Add( m_txtCurrentJob, 0, wxALL|wxEXPAND, 10 );


	bSizer5->Add( sbSizer1, 1, wxEXPAND|wxRIGHT|wxLEFT, 5 );

	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Elapsed:") ), wxVERTICAL );

	m_txtElapsed = new wxStaticText( sbSizer2->GetStaticBox(), wxID_ANY, wxT("00:00:00"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtElapsed->Wrap( -1 );
	sbSizer2->Add( m_txtElapsed, 0, wxALL, 10 );


	bSizer5->Add( sbSizer2, 0, wxEXPAND|wxRIGHT|wxLEFT, 5 );


	bSizer1->Add( bSizer5, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );

	m_cbJobsMru = new wxComboBox( this, wxID_ANY, wxT("..."), wxDefaultPosition, wxDefaultSize, 0, NULL, 0|wxTAB_TRAVERSAL );
	bSizer4->Add( m_cbJobsMru, 1, wxALL, 5 );

	m_btnStartStop = new wxButton( this, wxID_ANY, wxT("Start"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_btnStartStop, 0, wxALL, 5 );


	bSizer1->Add( bSizer4, 0, wxEXPAND, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	mnuAdmin->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MyFrame::mni_editJobsOnMenuSelection ), this, mni_editJobs->GetId());
	mnuAdmin->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MyFrame::mniEditTodaysJobHistoryOnMenuSelection ), this, mniEditTodaysJobHistory->GetId());
	mnuReports->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MyFrame::mniHistorySummaryOnMenuSelection ), this, mniHistorySummary->GetId());
	m_btnStartStop->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyFrame::m_btnStartStopOnButtonClick ), NULL, this );
}

MyFrame::~MyFrame()
{
	// Disconnect Events
	m_btnStartStop->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyFrame::m_btnStartStopOnButtonClick ), NULL, this );

}
