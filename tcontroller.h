#ifndef __tcontroller_h__
#define __tcontroller_h__

#include <vector>
#include "jobs.h"
#include "jobevents.h"
#include "wxViews.h"

using std::vector;

//class tView;
class MainView;

class tController : public ttj::JobStartObserver, public ttj::JobStopObserver,
                    public ttj::JobListObserver, public ttj::JobLogRecObserver
{
public:
    tController();
    ~tController();


    void JobStarted(const char *name, const char* display, int job_id) override;
    void JobStopped(int job_id) override;
    void JobListChanged() override;
    void JobLogRecChanged() override;

    void StartLogging(int index, bool use_short_list);
    void StopLogging();
    void Shutdown();
    void GetAllJobs(vector<ttj::JobRec>* rec_list);
    
    //Glib::Timer tmrSecs;
    //sigc::connection tmrConnection;
    bool Time1sElapsed();

    void SetView(MainView*);
private:
    vector<ttj::JobRec> Jobs;

    MainView* pView {};

    int CurrentJobId;
    long CurrentSecs; 

};





#endif