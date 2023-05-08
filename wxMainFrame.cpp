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
	wxMenuItem* mniAdmin_EditJobs;
	mniAdmin_EditJobs = new wxMenuItem( mnuAdmin, ID_EDIT_JOBS, wxString( wxT("Edit Jobs") ) , wxEmptyString, wxITEM_NORMAL );
	mnuAdmin->Append( mniAdmin_EditJobs );

	wxMenuItem* mniAdmin_EditJobHistory;
	mniAdmin_EditJobHistory = new wxMenuItem( mnuAdmin, ID_EDIT_TODAYS_JOB_HISTORY, wxString( wxT("Edit todays job history") ) , wxEmptyString, wxITEM_NORMAL );
	mnuAdmin->Append( mniAdmin_EditJobHistory );

	m_menubar1->Append( mnuAdmin, wxT("Admin") );

	mnuReports = new wxMenu();
	wxMenuItem* mniReports_HistorySummary;
	mniReports_HistorySummary = new wxMenuItem( mnuReports, ID_HISTORY_SUMMARY, wxString( wxT("History Summary...") ) , wxEmptyString, wxITEM_NORMAL );
	mnuReports->Append( mniReports_HistorySummary );

	m_menubar1->Append( mnuReports, wxT("Reports") );

	this->SetMenuBar( m_menubar1 );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );

	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Logging:") ), wxVERTICAL );

	txtCurrentJob = new wxStaticText( sbSizer1->GetStaticBox(), wxID_ANY, wxT("- no logging -"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL );
	txtCurrentJob->Wrap( -1 );
	sbSizer1->Add( txtCurrentJob, 0, wxALL|wxEXPAND, 10 );


	bSizer5->Add( sbSizer1, 1, wxEXPAND|wxRIGHT|wxLEFT, 5 );

	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Elapsed:") ), wxVERTICAL );

	txtElapsed = new wxStaticText( sbSizer2->GetStaticBox(), wxID_ANY, wxT("00:00:00"), wxDefaultPosition, wxDefaultSize, 0 );
	txtElapsed->Wrap( -1 );
	sbSizer2->Add( txtElapsed, 0, wxALL, 10 );


	bSizer5->Add( sbSizer2, 0, wxEXPAND|wxRIGHT|wxLEFT, 5 );


	bSizer1->Add( bSizer5, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );

	cbJobsMru = new wxComboBox( this, wxID_ANY, wxT("..."), wxDefaultPosition, wxDefaultSize, 0, NULL, 0|wxTAB_TRAVERSAL );
	bSizer4->Add( cbJobsMru, 1, wxALL, 5 );

	btnStartStop = new wxButton( this, wxID_ANY, wxT("Start"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( btnStartStop, 0, wxALL, 5 );


	bSizer1->Add( bSizer4, 0, wxEXPAND, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();
	tmrElapsed.SetOwner( this, wxID_ANY );

	this->Centre( wxBOTH );

    dlgJobs = new dlgJobsList(this);

	// Connect Events
	mnuAdmin->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MyFrame::mniAdmin_EditJobs_Click ), this, mniAdmin_EditJobs->GetId());
	mnuAdmin->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MyFrame::mniAdmin_EditJobHistory_Click ), this, mniAdmin_EditJobHistory->GetId());
	mnuReports->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MyFrame::mniReports_HistorySummary_Click ), this, mniReports_HistorySummary->GetId());
	cbJobsMru->Connect( wxEVT_COMBOBOX_DROPDOWN, wxCommandEventHandler( MyFrame::cbJobsMru_ListDown ), NULL, this );
	btnStartStop->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyFrame::btnStartStop_Click ), NULL, this );
	this->Connect( wxID_ANY, wxEVT_TIMER, wxTimerEventHandler( MyFrame::tmrElapsed_Tick ) );
}

MyFrame::~MyFrame()
{
	// Disconnect Events
	cbJobsMru->Disconnect( wxEVT_COMBOBOX_DROPDOWN, wxCommandEventHandler( MyFrame::cbJobsMru_ListDown ), NULL, this );
	btnStartStop->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyFrame::btnStartStop_Click ), NULL, this );
	this->Disconnect( wxID_ANY, wxEVT_TIMER, wxTimerEventHandler( MyFrame::tmrElapsed_Tick ) );

}
void MyFrame::tmrElapsed_Tick( wxTimerEvent& event ) {
    ElapsedSecs++;
    std::string s = std::to_string(ElapsedSecs);
    txtElapsed->SetLabel(s);
    //event.Skip();
}
void MyFrame::btnStartStop_Click( wxCommandEvent& event ) {
    //event.Skip();
    if (btnStartStop->GetLabel() == StartText) {
        btnStartStop->SetLabel(StopText);
        ElapsedSecs = 0;
        tmrElapsed.Start(1000);
    }
    else {
        btnStartStop->SetLabel(StartText);
        tmrElapsed.Stop();
    }
}

void MyFrame::mniAdmin_EditJobs_Click( wxCommandEvent& event ) {
    //dlgJobsList* dlg = new dlgJobsList(this);
    dlgJobs->Show();

    //delete dlg;
    //event.Skip();
}


dlgJobsList::dlgJobsList( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );

	m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("Job List"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	bSizer5->Add( m_staticText3, 0, wxALL, 5 );

	lcJobList = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_SINGLE_SEL );
	bSizer5->Add( lcJobList, 0, wxALL, 5 );


	bSizer4->Add( bSizer5, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );

	btnJobAdd = new wxButton( this, wxID_ANY, wxT("Add"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( btnJobAdd, 0, wxALL, 5 );

	btnJobDelete = new wxButton( this, wxID_ANY, wxT("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( btnJobDelete, 0, wxALL, 5 );

	btnJobRetire = new wxButton( this, wxID_ANY, wxT("Retire"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( btnJobRetire, 0, wxALL, 5 );


	bSizer4->Add( bSizer6, 0, wxEXPAND, 5 );


	this->SetSizer( bSizer4 );
	this->Layout();

	this->Centre( wxBOTH );
}

dlgJobsList::~dlgJobsList()
{
}
