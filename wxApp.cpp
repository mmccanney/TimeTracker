//
// Created by mmccanney on 4/10/23.
//
#include "wxApp.h"
#include "wxViews.h"
#include "jobs.h"
#include "tcontroller.h"

bool MyApp::OnInit()
{

    wxSize sz { 500, 160 };
    auto *frame = new MainView(nullptr, -1, "Time Tracker", wxDefaultPosition, sz, wxDEFAULT_FRAME_STYLE);
    frame->Show(true);

    // open the database file
    int rc = sqlite3_open(ttj::JobsDbName, &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    ttj::Init(db);

    // create the Controller portion of MVC
    Controller = new tController();

    // now let the View and Controller know about each other
    Controller->SetView(frame);
    frame->SetController(Controller);

    return true;
}

MyApp::~MyApp() {
    if (db) {
        ttj::StopCurrentJob();
        sqlite3_close(db);
    }
}
