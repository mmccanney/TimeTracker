#pragma once

#include <sqlite3.h> 

const int SQL_SUCCESS         = 0;
const int SQL_ERROR_NO_DB     = -1;
const int SQL_ERROR           = -2;
const int SQL_ERROR_BAD_PARAM = -3;
const int SQL_NO_RESULTS      = -4;

const int MAX_FIELD_SIZE_NAME    = 100;
const int MAX_FIELD_SIZE_DISPLAY = 100;
const int MAX_FIELD_SIZE_NOTES   = 1000;

namespace ttj {

    static char JobsDbName[] = "jobs.db";

    struct JobRec {
        int jid;
        char name[MAX_FIELD_SIZE_NAME];
        char display[MAX_FIELD_SIZE_DISPLAY];
    };

    struct JobHistorySummaryRec {
        char name[MAX_FIELD_SIZE_NAME];
        char display[MAX_FIELD_SIZE_DISPLAY];
        char elapsed[20];
    };

    struct RawJobHistoryRec {
        //returns: rid, jid, name, starttime, stoptime, elapsed_seconds
        int rid;
        int jid;
        char name[MAX_FIELD_SIZE_NAME];
        char display[MAX_FIELD_SIZE_DISPLAY];
        char start[20];
        char stop[20];
        unsigned int secs;
    };




    // sql result handlers
    static int printcallback(void *NotUsed, int argc, char **argv, char **azColName);
    static int GetJobsList(void *NotUsed, int argc, char **argv, char **azColName);
    static int GetSingleIntValue(void *IntPtr, int argc, char **argv, char **azColName);
    static int GetCharStringValue(void *CharPtr, int argc, char **argv, char **azColName);

    // database validation / management
    void Init(sqlite3* db_ptr);
    void ValidateDataStructure();
    int CreateJobsTable();
    int CreateJobLogTable();

    // job maintenance functions
    int GetJobId(const char* name);
    int AddJob(const char* name, const char* display_txt, int* job_id);
    int RetireJob(int jid);
    int UnRetireJob(int jid);
    int DeleteJob(int jid);
    int GetActiveJobs(std::vector<JobRec>* results, unsigned int limit = 0);
    int GetRetiredJobs(std::vector<JobRec>* results);

    int GetJobName(int job_id, char* name);
    int GetJobDisplay(int job_id, char* name);
    int GetJobNotes(int job_id, char* notes);
    int UpdateJobName(int job_id, char* display_text);
    int UpdateJobDisplay(int job_id, char* display_text);
    int UpdateJobNotes(int job_id, char* notes);

    int GetJobLastStartTime(int job_id, char* notes);

    // job logging functions
    int StartJob(int job_id);
    int StopJob(int job_id);
    int StopCurrentJob();
    int GetCurrentJob(int* job_id);
    int InsertJobLog(int job_id, const char* start_time, const char* stop_time);
    int InsertJobLog(int job_id, const char* start_time, int duration_secs);
    int InsertJobLogSpace(const char* start_time, const char* stop_time);
    int InsertJobLogSpace(const char* start_time, int duration_secs);
    int AdjustJobLogStart(int record_id, const char* new_start_time);
    int AdjustJobLogStop(int record_id, const char* new_stop_time);

    // job log reporting functions
    int ReportJobsDuring(const char* start_time, const char* stop_time, 
        std::vector<JobHistorySummaryRec>* results);
    int GetLogHistory(const char* start_time, const char* stop_time, 
        std::vector<JobRec>* results);

    // utility functions
    // TODO: may want to move these to a separate file
    void SecondsToDurationStr(int seconds, char* elapsed);
    int SecondsFromHMS(int hours, int mins, int secs);
    int SecondsFromDHMS(int days, int hours, int mins, int secs);

}
