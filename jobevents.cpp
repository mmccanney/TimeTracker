#include <vector>
#include "jobevents.h"

using std::vector;

namespace ttj {

    namespace {
        vector<JobStartObserver*> StartObs;
        vector<JobStopObserver*> StopObs;
        vector<JobListObserver*> JobObs;
        vector<JobLogRecObserver*> JobLogRecObs;
    }


    // Observer pattern subscriptions
    void Subsc_LogStarted(JobStartObserver *o) {
        StartObs.push_back(o);
    }
    void Subsc_LogStopped(JobStopObserver *o) {
        StopObs.push_back(o);
    }
    void Subsc_JobListChanged(JobListObserver *o) {
        JobObs.push_back(o);
    }
    void Subsc_JobLogRecChanged(JobLogRecObserver *o) {
        JobLogRecObs.push_back(o);
    }
    void Unsubsc_LogStarted(JobStartObserver *o) {
        auto i = StartObs.begin();
        while (i != StartObs.end()) {
            if (*i == o) {
                StartObs.erase(i);
                return;
            }
            i++;
        }
    }
    void Unsubsc_LogStopped(JobStopObserver *o) {
        auto i = StopObs.begin();
        while (i != StopObs.end()) {
            if (*i == o) {
                StopObs.erase(i);
                return;
            }
            i++;
        }
    }
    void Unsubsc_JobListChanged(JobListObserver *o) {
        auto i = JobObs.begin();
        while (i != JobObs.end()) {
            if (*i == o) {
                JobObs.erase(i);
                return;
            }
            i++;
        }
    }
    void Unsubsc_JobLogRecChanged(JobLogRecObserver *o) {
        auto i = JobLogRecObs.begin();
        while (i != JobLogRecObs.end()) {
            if (*i == o) {
                JobLogRecObs.erase(i);
                return;
            }
            i++;
        }
    }
    // notifiers
    void NotifyStartObs(char* name, char* display, int job_id) {
        auto i = StartObs.begin();
        while (i != StartObs.end()) {
            (*i)->JobStarted(name, display, job_id);
            i++;
        }
    }
    void NotifyStopObs(int job_id) {
        auto i = StopObs.begin();
        while (i != StopObs.end()) {
            (*i)->JobStopped(job_id);
            i++;
        }
    }
    void NotifyJobListObs(){
        auto i = JobObs.begin();
        while (i != JobObs.end()) {
            (*i)->JobListChanged();
            i++;
        }
    }
    void NotifyJobLogRecObs() {
        auto i = JobLogRecObs.begin();
        while (i != JobLogRecObs.end()) {
            (*i)->JobLogRecChanged();
            i++;
        }
    }

}