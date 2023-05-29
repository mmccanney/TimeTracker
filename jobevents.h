#ifndef __jobevents_h__
#define __jobevents_h__

namespace ttj {
//--------------------------------------------------------------
// Observer definitions
class JobStartObserver
{
public:
    JobStartObserver() = default;
    ~JobStartObserver() = default;
    virtual void JobStarted(const char *name, const char* display, int job_id) = 0;
};
class JobStopObserver
{
public:
    JobStopObserver() = default;
    ~JobStopObserver() = default;
    virtual void JobStopped(int job_id) = 0;
};
class JobListObserver
{
public:
    JobListObserver() = default;
    ~JobListObserver() = default;
    virtual void JobListChanged() = 0;
};
class JobLogRecObserver
{
public:
    JobLogRecObserver() = default;
    ~JobLogRecObserver() = default;
    virtual void JobLogRecChanged() = 0;
};

//--------------------------------------------------------------
// subscriptions...
void Subsc_LogStarted(JobStartObserver*);
void Subsc_LogStopped(JobStopObserver*);
void Subsc_JobListChanged(JobListObserver*);
void Subsc_JobLogRecChanged(JobLogRecObserver*);
void Unsubsc_LogStarted(JobStartObserver*);
void Unsubsc_LogStopped(JobStopObserver*);
void Unsubsc_JobListChanged(JobListObserver*);
void Unsubsc_JobLogRecChanged(JobLogRecObserver*);

//--------------------------------------------------------------
// notifiers
void NotifyStartObs(char* name, char* display, int job_id);
void NotifyStopObs(int jid);
void NotifyJobListObs();
void NotifyJobLogRecObs();

}
#endif