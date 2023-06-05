#include "tcontroller.h"
#include <iostream>
#include <cstring>

tController::tController() {
    CurrentJobId = 0;
    CurrentSecs = 0;
    
    // subscribe to events
    ttj::Subsc_LogStarted(this);
    ttj::Subsc_LogStopped(this);
    ttj::Subsc_JobListChanged(this);
    ttj::Subsc_JobLogRecChanged(this);

    // force a load of the MRU jobs
    //JobListChanged(); clang tidy says to not call virt mem funcs within ctor. moved to wxApp

    //tmrConnection = Glib::signal_timeout().connect(sigc::mem_fun(*this, &tController::Time1sElapsed), 1000);
}
tController::~tController() {
    // unsubscribe to events
    ttj::Unsubsc_LogStarted(this);
    ttj::Unsubsc_LogStopped(this);
    ttj::Unsubsc_JobListChanged(this);
    ttj::Unsubsc_JobLogRecChanged(this);

    //tmrConnection.disconnect();
}

void tController::SetView(MainView *pmv) {
    pView = pmv;
}

//=============================================================================
// Events from the model
void tController::JobStarted(const char *name, const char* display, int job_id) {
    CurrentJobId = job_id;
    CurrentSecs = 0;
    char text[MAX_FIELD_SIZE_NAME + MAX_FIELD_SIZE_DISPLAY + 4];
    if (std::strlen(display) > 0)
        sprintf(text, "%s (%s)", display, name);
    else
        std::strcpy(text, name);
    pView->SetLoggingDisplay(text);
    pView->SetButtonStop();
}
bool tController::Time1sElapsed() {
    if (pView && CurrentJobId) {
        CurrentSecs++;
        char tstr[20];
        ttj::SecondsToDurationStr(CurrentSecs, tstr);
        pView->UpdateTimeElapsed(tstr);
    }
    return true;    // true means this mem func will be called again
}
void tController::JobStopped(int job_id) {
    pView->ClearLoggingDisplay();
    CurrentJobId = 0;
    CurrentSecs = 0;
    pView->SetButtonStart();
}
void tController::JobListChanged() {
    int rc = ttj::GetActiveJobs(&Jobs);
    if (!rc) {
        pView->UpdateMruJobsList(Jobs);
    }
}
void tController::JobLogRecChanged() {

}


//=============================================================================
// the View calls these
void tController::StartLogging(int index, bool use_list = true) {
    if (use_list) {
        // this means that the index is an index into the Jobs vector, not an actual job id
        ttj::JobRec jr = Jobs[index];
        ttj::StartJob(jr.jid);
    }
    else {
        // otherwise "index" is the job_id
        ttj::StartJob(index);
    }
}
void tController::StopLogging() const {
    ttj::StopJob(CurrentJobId);
}
void tController::Shutdown() {
    ttj::StopCurrentJob();
}
void tController::GetAllJobs(std::vector<ttj::JobRec>* rec_list) {
    rec_list->clear();
    *rec_list = Jobs;
}


