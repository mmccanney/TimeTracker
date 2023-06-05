///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wxViews.h"


///////////////////////////////////////////////////////////////////////////

MainView::MainView(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style)
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

	cbJobsMru = new wxComboBox( this, wxID_ANY, wxT("..."), wxDefaultPosition, wxDefaultSize, 0, nullptr, wxCB_READONLY|wxTAB_TRAVERSAL );
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
	mnuAdmin->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainView::mniAdmin_EditJobs_Click ), this, mniAdmin_EditJobs->GetId());
	mnuAdmin->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainView::mniAdmin_EditJobHistory_Click ), this, mniAdmin_EditJobHistory->GetId());
	mnuReports->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainView::mniReports_HistorySummary_Click ), this, mniReports_HistorySummary->GetId());
	cbJobsMru->Connect( wxEVT_COMBOBOX_DROPDOWN, wxCommandEventHandler( MainView::cbJobsMru_ListDown ), nullptr, this );
	btnStartStop->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainView::btnStartStop_Click ), nullptr, this );
	this->Connect( wxID_ANY, wxEVT_TIMER, wxTimerEventHandler( MainView::tmrElapsed_Tick ) );


    ClearLoggingDisplay();
}
MainView::~MainView()
{
	// Disconnect Events
	cbJobsMru->Disconnect( wxEVT_COMBOBOX_DROPDOWN, wxCommandEventHandler(MainView::cbJobsMru_ListDown), nullptr, this );
	btnStartStop->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainView::btnStartStop_Click), nullptr, this );
	this->Disconnect(wxID_ANY, wxEVT_TIMER, wxTimerEventHandler( MainView::tmrElapsed_Tick));

}
void MainView::SetController(tController *pc) {
    pController = pc;
}



void MainView::btnStartStop_Click(wxCommandEvent& event) {
    // what does the button say?
    if (btnStartStop->GetLabel() == StartText) {
        if (cbJobsMru->GetSelection() < MruJobsSize) {
            pController->StartLogging(cbJobsMru->GetSelection(), true);
            tmrElapsed.Start(1000);
        }
    }
    else {
        pController->StopLogging();
        tmrElapsed.Stop();
    }
}
void MainView::ClearLoggingDisplay() {
    txtCurrentJob->SetLabel("-- no logging --");
    txtElapsed->SetLabel("00:00:00");
    SetButtonStart();
}
void MainView::SetLoggingDisplay(const char* display) {
    txtCurrentJob->SetLabel(display);
}
void MainView::UpdateTimeElapsed(const char* time_elapsed) {
    txtElapsed->SetLabel(time_elapsed);
}
void MainView::SetButtonStart() {
    btnStartStop->SetLabel(StartText);
    cbJobsMru->Show();
}
void MainView::SetButtonStop() {
    btnStartStop->SetLabel(StopText);
    cbJobsMru->Show(false);
}
void MainView::SetMruJobsSize(int qty) {
    // don't let the user select a crazy value
    if (qty > 1 && qty < 20) MruJobsSize = qty;
}
int MainView::GetMruJobsSize() { return MruJobsSize; };
void MainView::ClearMruJobsList() {
    cbJobsMru->Clear();
}
void MainView::UpdateMruJobsList(std::vector<ttj::JobRec>& rec_list) {
    if (!cbJobsMru) return;

    ClearMruJobsList();

    int x = 0;
    // populate the popup menu
    auto itr = rec_list.begin();
    while (itr != rec_list.end() && x < MruJobsSize) {
        ttj::JobRec j = *itr;
        char text[MAX_FIELD_SIZE_NAME + MAX_FIELD_SIZE_DISPLAY + 4];
        if (strlen(j.display) > 0) sprintf(text, "%s (%s)", j.display, j.name);
        else sprintf(text, "%s", j.name);
        cbJobsMru->Append(text);
        itr++; x++;
    }
    if (MruJobsSize < rec_list.size()) cbJobsMru->Append("...");
}
void MainView::cbJobsMru_ListDown(wxCommandEvent& event) {
    if (cbJobsMru->GetSelection() >= MruJobsSize) {
        //
    }
}
void MainView::ShowAllJobsForSelection() {
    std::vector<ttj::JobRec> rec_list;
    pController->GetAllJobs(&rec_list);
//    if (pJobSelDlg) {
//        pJobSelDlg->UpdateJobsList(rec_list);
//        int res = pJobSelDlg->run();
//        if (res == Gtk::RESPONSE_OK) {
//            int jid = pJobSelDlg->SelectedJobId;
//            if (jid > 0) pController->StartLogging(jid, false);
//        }
//        else {
//            // clear the ... from the combo box, otherwise if the user wants to
//            // view the "All Jobs" screen again, they can't until they select a different job
//            pMruJobs->unset_active();
//        }
//
//        pJobSelDlg->hide();
//    }
}







void MainView::tmrElapsed_Tick(wxTimerEvent& event) {
    if (pController) pController->Time1sElapsed();
}

void MainView::mniAdmin_EditJobs_Click(wxCommandEvent& event) {
    //dlgJobsList* dlg = new dlgJobsList(this);     // can't do this as it'll leak memory (ie: who's deleting it?)
    dlgJobs->Show();

    //delete dlg;
    //event.Skip();
}





//==============================================================================================
//==============================================================================================

dlgJobsList::dlgJobsList(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style)
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

//dlgJobsList::~dlgJobsList()
//= default;
