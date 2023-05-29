#include <iostream>
#include <vector>
#include <cstdio>
#include <cstring>
//#include <ctime>
#include <cstdlib>
#include <sqlite3.h>

#include "jobs.h"
#include "jobevents.h"

//#define __zcDEBUG__

using std::vector;

namespace ttj {

    namespace {
        int rc;
        char* zErrMsg = nullptr;
        char sql[1600];
        sqlite3* db = nullptr;
    }

    // DB Structure Functions =====================================================
    void Init(sqlite3* db_ptr) {
        db = db_ptr;
        ValidateDataStructure();
    }
    void ValidateDataStructure() {
        CreateJobsTable();
        CreateJobLogTable();
    }
    int CreateJobsTable() {
        if (!db) return SQL_ERROR_NO_DB;
        sprintf(sql, "CREATE TABLE IF NOT EXISTS Jobs(" \
                     "    jid INTEGER PRIMARY KEY AUTOINCREMENT," \
                     "    name VARCHAR(%d) NOT NULL," \
                     "    display VARCHAR(%d) NULL," \
                     "    notes VARCHAR(%d) NULL," \
                     "    last DATETIME NULL," \
                     "    active INT NULL );", MAX_FIELD_SIZE_NAME, 
                     MAX_FIELD_SIZE_DISPLAY, MAX_FIELD_SIZE_NOTES);
        rc = sqlite3_exec(db, sql, nullptr, nullptr, &zErrMsg);
        if (rc != SQLITE_OK) return SQL_ERROR;
        return SQL_SUCCESS;
    }
    int CreateJobLogTable() {
        if (!db) return SQL_ERROR_NO_DB;
        sprintf(sql, "CREATE TABLE IF NOT EXISTS JobLog(" \
              "rid INTEGER PRIMARY KEY AUTOINCREMENT," \
              "jid INT NOT NULL," \
              "starttime DATETIME NULL," \
              "stoptime DATETIME NULL," \
              "elapsed INT NULL);");
        rc = sqlite3_exec(db, sql, nullptr, nullptr, &zErrMsg);
        if (rc != SQLITE_OK) return SQL_ERROR;
        
        return SQL_SUCCESS;
    }


    // DB Return Value Functions ==================================================
    static int printcallback(void *NotUsed, int argc, char **argv, char **azColName) {
        std::cout << "next row " << std::endl;
        int i;
        for(i = 0; i<argc; i++) {
            std::cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << std::endl;
        }
        std::cout << std::endl;
        return 0;
    }
    static int GetSingleIntValue(void *IntPtr, int argc, char **argv, char **azColName) {
        int *int_ptr = (int *)IntPtr;
        if (argc == 1) *(int *)IntPtr = atoi(argv[0]);
        else *(int *)IntPtr = 0;
        return 0;
    }
    static int GetCharStringValue(void *CharPtr, int argc, char **argv, char **azColName) {
        char *ptr = (char*)CharPtr;
        if (argc == 1) strcpy(ptr, argv[0]);
        else ptr[0] = 0;
        return 0;
    }
    static int GetJobsList(void *rec_ptr, int argc, char **argv, char **azColName) {
        auto* ptr = (std::vector<JobRec>*)rec_ptr;
        int i;
        // expects: jid, name, display
        if (argc >= 3) {
            JobRec rec{};
            rec.jid = argv[0] ? atoi(argv[0]) : 0;
            if (argv[1]) strcpy(rec.name, argv[1]); else rec.name[0] = 0;
            if (argv[2]) strcpy(rec.display, argv[2]); else rec.display[0] = 0;
            ptr->push_back(rec);
        }
        return 0;
    }
    static int ProcessJobHistory(void *rec_ptr, int argc, char **argv, char **azColName) {
        auto* ptr = (std::vector<RawJobHistoryRec>*)rec_ptr;
        int i;
        if (argc == 7) {
            // results should come in as: rid, jid, name, display, starttime, stoptime, elapsed_seconds
            RawJobHistoryRec rec{};
            rec.rid = argv[0] ? atoi(argv[0]) : 0;
            rec.jid = argv[1] ? atoi(argv[1]) : 0;
            if (argv[2]) strcpy(rec.name, argv[2]); else rec.name[0] = 0;
            if (argv[3]) strcpy(rec.display, argv[3]); else rec.display[0] = 0;
            if (argv[4]) strcpy(rec.start, argv[4]); else rec.start[0] = 0;
            if (argv[5]) strcpy(rec.stop, argv[5]); else rec.stop[0] = 0;
            rec.secs = argv[6] ? atoi(argv[6]) : 0;
            ptr->push_back(rec);
        }
        return 0;
    }


    // Job Management Functions ===================================================
    int AddJob(const char* name, const char* display_txt, int* job_id) {
        if (!db) return SQL_ERROR_NO_DB;

        if (job_id) {
            *job_id = GetJobId(name);
            if (*job_id > 0) return SQL_SUCCESS;
        }
        else {
            int jid = GetJobId(name);
            if (jid > 0) return SQL_SUCCESS;
        }

        if (display_txt)
            sprintf(sql, "INSERT INTO Jobs (name, display, active) VALUES ('%s','%s',1);",
                name, display_txt);
        else    
            sprintf(sql, "INSERT INTO Jobs (name, active) VALUES ('%s',1);", name);


        rc = sqlite3_exec(db, sql, nullptr, nullptr, &zErrMsg);
        if (rc != SQLITE_OK) return SQL_ERROR;

        if (job_id) {
            *job_id = GetJobId(name);
        }

        NotifyJobListObs();

        return SQL_SUCCESS;
    }
    int RetireJob(int job_id) {
        if (!db) return SQL_ERROR_NO_DB;
        if (job_id < 1) return SQL_ERROR_BAD_PARAM;

        sprintf(sql, "UPDATE Jobs SET active = 0 WHERE jid = %d;", job_id);

        rc = sqlite3_exec(db, sql, nullptr, nullptr, &zErrMsg);
        if (rc != SQLITE_OK) return SQL_ERROR;

        NotifyJobListObs();

        return SQL_SUCCESS;
    }
    int UnRetireJob(int job_id) {
        if (!db) return SQL_ERROR_NO_DB;
        if (job_id < 1) return SQL_ERROR_BAD_PARAM;

        sprintf(sql, "UPDATE Jobs SET active = 1 WHERE jid = %d;", job_id);

        rc = sqlite3_exec(db, sql, nullptr, nullptr, &zErrMsg);
        if (rc != SQLITE_OK) return SQL_ERROR;

        NotifyJobListObs();

        return SQL_SUCCESS;
    }
    int DeleteJob(int job_id) {
        if (!db) return SQL_ERROR_NO_DB;
        if (job_id < 1) return SQL_ERROR_BAD_PARAM;

        sprintf(sql, "DELETE FROM Jobs WHERE jid = %d; DELETE FROM JobLog WHERE jid = %d;", 
            job_id, job_id);

        rc = sqlite3_exec(db, sql, nullptr, nullptr, &zErrMsg);
        if (rc != SQLITE_OK) return SQL_ERROR;

        NotifyJobListObs();

        return SQL_SUCCESS;
    }
    int GetJobId(const char* name) {
        // this differs from the other in that it will return the jid
        if (!db) return SQL_ERROR_NO_DB;

        int val = 0;
        sprintf(sql, "SELECT jid FROM Jobs WHERE name = '%s'", name);
        rc = sqlite3_exec(db, sql, GetSingleIntValue, &val, &zErrMsg);
        if (rc != SQLITE_OK) return SQL_ERROR;
        return val;
    }
    int GetActiveJobs(std::vector<JobRec>* results, uint limit) {
        if (!db) return SQL_ERROR_NO_DB;
        if (!results || limit > 32000) return SQL_ERROR_BAD_PARAM;

        // clear that results
        results->clear();

        sprintf(sql, "SELECT jid, name, display FROM Jobs WHERE active <> 0 ORDER BY last DESC");
        if (limit > 0) {
            char szlim[13];     // max string " limit 32000" size = 12 + \0
            sprintf(szlim," LIMIT %u", limit);
            strcat(sql, szlim);
        }

        rc = sqlite3_exec(db, sql, GetJobsList, results, &zErrMsg);
        if (rc != SQLITE_OK) return SQL_ERROR;

        return SQL_SUCCESS;
    }
    int GetRetiredJobs(std::vector<JobRec>* results) {
        if (!db) return SQL_ERROR_NO_DB;
        if (!results) return SQL_ERROR_BAD_PARAM;

        // clear that results
        results->clear();

        sprintf(sql, "SELECT jid, name, display FROM Jobs WHERE active = 0 ORDER BY last");

        rc = sqlite3_exec(db, sql, GetJobsList, results, &zErrMsg);
        if (rc != SQLITE_OK) return SQL_ERROR;

        return SQL_SUCCESS;
    }

    // Job Editing Functions ======================================================
    int GetJobName(int job_id, char* name) {
        if (!db) return SQL_ERROR_NO_DB;
        if (job_id < 1 || !name) return SQL_ERROR_BAD_PARAM;

        name[0] = 0;    // clear the name

        sprintf(sql, "SELECT name FROM Jobs WHERE jid = %d", job_id);
        rc = sqlite3_exec(db, sql, GetCharStringValue, name, &zErrMsg);
        if (rc != SQLITE_OK) return SQL_ERROR;

        if (strlen(name) < 1) return SQL_NO_RESULTS;

        return SQL_SUCCESS;
    }
    int GetJobDisplay(int job_id, char* display_text) {
        if (!db) return SQL_ERROR_NO_DB;
        if (job_id < 1 || !display_text) return SQL_ERROR_BAD_PARAM;

        display_text[0] = 0;    // clear the display_text

        sprintf(sql, "SELECT COALESCE(" \
                     "    (SELECT display FROM Jobs WHERE jid = %d AND LENGTH(display) > 0)," \
                     "     name)" \
                     "FROM Jobs WHERE jid = %d;", job_id, job_id);
        rc = sqlite3_exec(db, sql, GetCharStringValue, display_text, &zErrMsg);
        if (rc != SQLITE_OK) return SQL_ERROR;

        if (strlen(display_text) < 1) return SQL_NO_RESULTS;

        return SQL_SUCCESS;
    }
    int GetJobNotes(int job_id, char* notes) {
        if (!db) return SQL_ERROR_NO_DB;
        if (job_id < 1 || !notes) return SQL_ERROR_BAD_PARAM;

        notes[0] = 0;    // clear the notes

        sprintf(sql, "SELECT notes FROM Jobs WHERE jid = %d;", job_id);
        rc = sqlite3_exec(db, sql, GetCharStringValue, notes, &zErrMsg);
        if (rc != SQLITE_OK) return SQL_ERROR;

        if (strlen(notes) < 1) return SQL_NO_RESULTS;

        return SQL_SUCCESS;
    }
    int GetJobLastStartTime(int job_id, char* date_string) {
        if (!db) return SQL_ERROR_NO_DB;
        if (job_id < 1 || !date_string) return SQL_ERROR_BAD_PARAM;

        date_string[0] = 0;    // clear the notes

        sprintf(sql, "SELECT last FROM Jobs WHERE jid = %d;", job_id);
        rc = sqlite3_exec(db, sql, GetCharStringValue, date_string, &zErrMsg);
        if (rc != SQLITE_OK) return SQL_ERROR;

        if (strlen(date_string) < 1) return SQL_NO_RESULTS;

        return SQL_SUCCESS;
    }
    int UpdateJobName(int job_id, char* new_name) {
        if (!db) return SQL_ERROR_NO_DB;
        if (job_id < 1 || !new_name) return SQL_ERROR_BAD_PARAM;

        if (strlen(new_name) < 1 || strlen(new_name) > MAX_FIELD_SIZE_NAME) return SQL_ERROR_BAD_PARAM;

        sprintf(sql, "UPDATE Jobs SET name = '%s' WHERE jid = %d", new_name, job_id);
        rc = sqlite3_exec(db, sql, nullptr, nullptr, &zErrMsg);
        if (rc != SQLITE_OK) return SQL_ERROR;

        NotifyJobListObs();

        return SQL_SUCCESS;
    }
    int UpdateJobDisplay(int job_id, char* new_display_text) {
        if (!db) return SQL_ERROR_NO_DB;
        if (job_id < 1 || !new_display_text) return SQL_ERROR_BAD_PARAM;

        if (!new_display_text || strlen(new_display_text) == 0)
            sprintf(sql, "UPDATE Jobs SET display = NULL WHERE jid = %d", job_id);
        else {
            if (strlen(new_display_text) > MAX_FIELD_SIZE_DISPLAY) return SQL_ERROR_BAD_PARAM;
            sprintf(sql, "UPDATE Jobs SET display = '%s' WHERE jid = %d", 
                new_display_text, job_id);
        }
        rc = sqlite3_exec(db, sql, nullptr, nullptr, &zErrMsg);
        if (rc != SQLITE_OK) return SQL_ERROR;

        NotifyJobListObs();

        return SQL_SUCCESS;
    }
    int UpdateJobNotes(int job_id, char* new_notes) {
        if (!db) return SQL_ERROR_NO_DB;
        if (job_id < 1 || !new_notes) return SQL_ERROR_BAD_PARAM;

        if (!new_notes || strlen(new_notes) == 0)
            sprintf(sql, "UPDATE Jobs SET notes = NULL WHERE jid = %d", job_id);
        else {
            if (strlen(new_notes) > MAX_FIELD_SIZE_NOTES) return SQL_ERROR_BAD_PARAM;
            sprintf(sql, "UPDATE Jobs SET notes = '%s' WHERE jid = %d", new_notes, job_id);
        }
        rc = sqlite3_exec(db, sql, nullptr, nullptr, &zErrMsg);
        if (rc != SQLITE_OK) return SQL_ERROR;
        return SQL_SUCCESS;
    }


    // Job Usage Functions ========================================================
    int StartJob(int job_id) {
        if (!db) return SQL_ERROR_NO_DB;
        if (job_id < 1) return SQL_ERROR_BAD_PARAM;

        sprintf(sql, "UPDATE JobLog SET stoptime = datetime('now', 'localtime') "\
            "WHERE stoptime IS NULL; " \
            "INSERT INTO JobLog (jid,starttime) VALUES (%d, datetime('now','localtime')); "\
            "UPDATE Jobs SET last = datetime('now', 'localtime') WHERE jid = %d;", job_id, job_id);
        rc = sqlite3_exec(db, sql, nullptr, nullptr, &zErrMsg);
        if (rc != SQLITE_OK) return SQL_ERROR;

        char name[MAX_FIELD_SIZE_NAME];
        char display[MAX_FIELD_SIZE_DISPLAY];
        GetJobName(job_id, name);
        GetJobDisplay(job_id, display);
        NotifyStartObs(name, display, job_id);
        NotifyJobListObs();     // because the order will change (for MRU lists)
        
        return SQL_SUCCESS;
    }
    int StopJob(int job_id) {
        if (!db) return SQL_ERROR_NO_DB;
        if (job_id < 1) return SQL_ERROR_BAD_PARAM;

        sprintf(sql, "UPDATE JobLog SET stoptime = datetime('now', 'localtime') "\
            "WHERE jid = %d AND stoptime IS NULL; ", job_id);
        rc = sqlite3_exec(db, sql, nullptr, nullptr, &zErrMsg);
        if (rc != SQLITE_OK) return SQL_ERROR;

        NotifyStopObs(job_id);

        return SQL_SUCCESS;
    }
    int StopCurrentJob() {
        if (!db) return SQL_ERROR_NO_DB;

        int job_id;
        GetCurrentJob(&job_id);

        return StopJob(job_id);
    }
    int GetCurrentJob(int* job_id) {
        if (!db) return SQL_ERROR_NO_DB;
        if (!job_id) return SQL_ERROR_BAD_PARAM;

        sprintf(sql, "SELECT COALESCE(jid, 0) FROM JobLog " \
                     "WHERE starttime IS NOT NULL AND stoptime IS NULL " \
                     "ORDER BY starttime DESC " \
                     "LIMIT 1");
        rc = sqlite3_exec(db, sql, GetSingleIntValue, job_id, &zErrMsg);
        if (rc != SQLITE_OK) return SQL_ERROR;
        return SQL_SUCCESS;
    }
    int InsertJobLog(int job_id, const char* start_time, const char* stop_time) {
        if (!db) return SQL_ERROR_NO_DB;
        if (job_id < 1 || !start_time || !stop_time) return SQL_ERROR_BAD_PARAM;
        // minimum expected time stamp format = YYYY-MM-DD HH:MM:SS = 19 characters
        if (strlen(start_time) < 19 || strlen(stop_time) < 19) return SQL_ERROR_BAD_PARAM;

        // delete any entries that fall __within__ the new timeframe
        sprintf(sql, "DELETE FROM JobLog " \
                     "WHERE starttime > datetime('%s') AND stoptime <= datetime('%s');",
                     start_time, stop_time);
        rc = sqlite3_exec(db, sql, nullptr, nullptr, &zErrMsg);
        if (rc != SQLITE_OK) return SQL_ERROR;

        // update any entry that overlaps the new entries start time
        sprintf(sql, "UPDATE JobLog SET stoptime = datetime('%s') " \
                     "WHERE datetime('%s') >= starttime AND datetime('%s') <= stoptime;",
                     start_time, start_time, start_time);
        rc = sqlite3_exec(db, sql, nullptr, nullptr, &zErrMsg);
        if (rc != SQLITE_OK) return SQL_ERROR;

        // update any entry that overlaps the new entries stop time
        sprintf(sql, "UPDATE JobLog SET starttime = datetime('%s') " \
                     "WHERE datetime('%s') >= starttime AND datetime('%s') <= stoptime;",
                     stop_time, stop_time, stop_time);
        rc = sqlite3_exec(db, sql, nullptr, nullptr, &zErrMsg);
        if (rc != SQLITE_OK) return SQL_ERROR;

        // add the new entry
        sprintf(sql, "INSERT INTO JobLog (jid, starttime, stoptime) VALUES(%d, "\
            "datetime('%s'), datetime('%s'));",
                     job_id, start_time, stop_time);
        rc = sqlite3_exec(db, sql, nullptr, nullptr, &zErrMsg);
        if (rc != SQLITE_OK) return SQL_ERROR;

        return SQL_SUCCESS;
    }
    int InsertJobLog(int job_id, const char* start_time, int duration_secs) {
        if (!db) return SQL_ERROR_NO_DB;
        if (job_id < 1 || !start_time || duration_secs < 1) return SQL_ERROR_BAD_PARAM;
        // minimum expected time stamp format = YYYY-MM-DD HH:MM:SS = 19 characters
        if (strlen(start_time) < 19) return SQL_ERROR_BAD_PARAM;

        // delete any entries that fall __within__ the new timeframe
        sprintf(sql, "DELETE FROM JobLog " \
                     "WHERE starttime > datetime('%s') AND stoptime <= datetime('%s','+%d seconds');",
                     start_time, start_time, duration_secs);
        rc = sqlite3_exec(db, sql, nullptr, nullptr, &zErrMsg);
        if (rc != SQLITE_OK) return SQL_ERROR;

        // update any entry that overlaps the new entries start time
        sprintf(sql, "UPDATE JobLog SET stoptime = datetime('%s') " \
                     "WHERE datetime('%s') >= starttime AND datetime('%s') <= stoptime;",
                     start_time, start_time, start_time);
        rc = sqlite3_exec(db, sql, nullptr, nullptr, &zErrMsg);
        if (rc != SQLITE_OK) return SQL_ERROR;

        // update any entry that overlaps the new entries stop time
        sprintf(sql, "UPDATE JobLog SET starttime = datetime('%s','+%d seconds') " \
                     "WHERE datetime('%s','+%d seconds') >= starttime AND datetime('%s','+%d seconds') <= stoptime;",
                     start_time, duration_secs, start_time, duration_secs, start_time, duration_secs);
        rc = sqlite3_exec(db, sql, nullptr, nullptr, &zErrMsg);
        if (rc != SQLITE_OK) return SQL_ERROR;

        // add the new entry
        sprintf(sql, "INSERT INTO JobLog (jid, starttime, stoptime) VALUES(%d, "\
            "datetime('%s'), datetime('%s','+%d seconds'));",
                     job_id, start_time, start_time, duration_secs);
        rc = sqlite3_exec(db, sql, nullptr, nullptr, &zErrMsg);
        if (rc != SQLITE_OK) return SQL_ERROR;

        return SQL_SUCCESS;
    }

    int InsertJobLogSpace(const char* start_time, const char* stop_time) {
        if (!db) return SQL_ERROR_NO_DB;
        if (!start_time || !stop_time) return SQL_ERROR_BAD_PARAM;
        // minimum expected time stamp format = YYYY-MM-DD HH:MM:SS = 19 characters
        if (strlen(start_time) < 19 || strlen(stop_time) < 19) return SQL_ERROR_BAD_PARAM;

        // delete any entries that fall __within__ the new timeframe
        sprintf(sql, "DELETE FROM JobLog " \
                     "WHERE starttime > datetime('%s') AND stoptime <= datetime('%s');",
                     start_time, stop_time);
        rc = sqlite3_exec(db, sql, nullptr, nullptr, &zErrMsg);
        if (rc != SQLITE_OK) return SQL_ERROR;

        // update any entry that overlaps the new entries start time
        sprintf(sql, "UPDATE JobLog SET stoptime = datetime('%s') " \
                     "WHERE datetime('%s') >= starttime AND datetime('%s') <= stoptime;",
                     start_time, start_time, start_time);
        rc = sqlite3_exec(db, sql, nullptr, nullptr, &zErrMsg);
        if (rc != SQLITE_OK) return SQL_ERROR;

        // update any entry that overlaps the new entries stop time
        sprintf(sql, "UPDATE JobLog SET starttime = datetime('%s') " \
                     "WHERE datetime('%s') >= starttime AND datetime('%s') <= stoptime;",
                     stop_time, stop_time, stop_time);
        rc = sqlite3_exec(db, sql, nullptr, nullptr, &zErrMsg);
        if (rc != SQLITE_OK) return SQL_ERROR;

        return SQL_SUCCESS;
    }
    int InsertJobLogSpace(const char* start_time, int duration_secs) {
        if (!db) return SQL_ERROR_NO_DB;
        if (!start_time || duration_secs < 1) return SQL_ERROR_BAD_PARAM;
        // minimum expected time stamp format = YYYY-MM-DD HH:MM:SS = 19 characters
        if (strlen(start_time) < 19) return SQL_ERROR_BAD_PARAM;

        // delete any entries that fall __within__ the new timeframe
        sprintf(sql, "DELETE FROM JobLog " \
                     "WHERE starttime > datetime('%s') AND stoptime <= datetime('%s','+%d seconds');",
                     start_time, start_time, duration_secs);
        rc = sqlite3_exec(db, sql, nullptr, nullptr, &zErrMsg);
        if (rc != SQLITE_OK) return SQL_ERROR;

        // update any entry that overlaps the new entries start time
        sprintf(sql, "UPDATE JobLog SET stoptime = datetime('%s') " \
                     "WHERE starttime <= datetime('%s') AND stoptime > datetime('%s');",
                     start_time, start_time, start_time);
        rc = sqlite3_exec(db, sql, nullptr, nullptr, &zErrMsg);
        if (rc != SQLITE_OK) return SQL_ERROR;

        // update any entry that overlaps the new entries stop time
        sprintf(sql, "UPDATE JobLog SET starttime = datetime('%s','+%d seconds') " \
                     "WHERE starttime <= datetime('%s','+%d seconds') AND stoptime >= datetime('%s','+%d seconds');",
                     start_time, duration_secs, start_time, duration_secs, start_time, duration_secs);
        rc = sqlite3_exec(db, sql, nullptr, nullptr, &zErrMsg);
        if (rc != SQLITE_OK) return SQL_ERROR;

        return SQL_SUCCESS;
    }
    int AdjustJobLogStart(int record_id, const char* new_start_time) {
        if (!db) return SQL_ERROR_NO_DB;
        if (record_id < 1 || !new_start_time) return SQL_ERROR_BAD_PARAM;
        // minimum expected time stamp format = YYYY-MM-DD HH:MM:SS = 19 characters
        if (strlen(new_start_time) < 19) return SQL_ERROR_BAD_PARAM;

        sprintf(sql, "UPDATE JobLog SET starttime = datetime('%s') WHERE rid = %d", 
            new_start_time, record_id);
        rc = sqlite3_exec(db, sql, nullptr, nullptr, &zErrMsg);
        if (rc != SQLITE_OK) return SQL_ERROR;
        return SQL_SUCCESS;
    }
    int AdjustJobLogStop(int record_id, const char* new_stop_time) {
        if (!db) return SQL_ERROR_NO_DB;
        if (record_id < 1 || !new_stop_time) return SQL_ERROR_BAD_PARAM;
        // minimum expected time stamp format = YYYY-MM-DD HH:MM:SS = 19 characters
        if (strlen(new_stop_time) < 19) return SQL_ERROR_BAD_PARAM;

        sprintf(sql, "UPDATE JobLog SET stoptime = datetime('%s') WHERE rid = %d", 
            new_stop_time, record_id);
        rc = sqlite3_exec(db, sql, nullptr, nullptr, &zErrMsg);
        if (rc != SQLITE_OK) return SQL_ERROR;
        return SQL_SUCCESS;
    }


    // Log Reporting Functions ====================================================
    int ReportJobsDuring(const char* start_time, const char* stop_time, std::vector<JobHistorySummaryRec>* results) {

        if (!db) return SQL_ERROR_NO_DB;
        if (!start_time || !stop_time || !results) return SQL_ERROR_BAD_PARAM;
        // minimum expected time stamp format = YYYY-MM-DD HH:MM:SS = 19 characters
        if (strlen(start_time) < 19 || strlen(stop_time) < 19) return SQL_ERROR_BAD_PARAM;

        // clear that results
        results->clear();

        // actual character count after the sprintf should be ~1189
        // The following is 4 queries to calculate the elapsed time. These do:

        // 1. find any job the spans across the starting time but not the stop time
        //      these need the elapsed time trimmed to ignore time outside the requested window
        sprintf(sql, "UPDATE JobLog SET [elapsed] = strftime('%%s', stoptime) - strftime('%%s', '%s') " \
                     "WHERE starttime < datetime('%s') AND " \
                           "stoptime > datetime ('%s') AND " \
                           "stoptime <= datetime ('%s'); ",
                     start_time, start_time, start_time, stop_time);
        rc = sqlite3_exec(db, sql, nullptr, nullptr, &zErrMsg);
        if (rc != SQLITE_OK) return SQL_ERROR;

        // 2. find any job that starts before the window and ends after the window (ie: entirely spans the window)
        //      these need the elapsed time trimmed to equal to the window span
        sprintf(sql, "UPDATE JobLog SET [elapsed] = strftime('%%s', '%s') - strftime('%%s', '%s') " \
                     "WHERE starttime < datetime('%s') AND " \
                           "stoptime > datetime ('%s'); ",
                     stop_time, start_time, start_time, stop_time);
        rc = sqlite3_exec(db, sql, nullptr, nullptr, &zErrMsg);
        if (rc != SQLITE_OK) return SQL_ERROR;

        // 3. find any job that starts in the window but ends after the window.
        //      these need the elapsed time trimmed to ignore outside the requested window
        sprintf(sql, "UPDATE JobLog SET [elapsed] = strftime('%%s', '%s') - strftime('%%s', starttime) " \
                     "WHERE starttime >= datetime('%s') AND " \
                           "starttime < datetime ('%s') AND " \
                           "stoptime > datetime ('%s'); ",
                     stop_time, start_time, stop_time, stop_time);
        rc = sqlite3_exec(db, sql, nullptr, nullptr, &zErrMsg);
        if (rc != SQLITE_OK) return SQL_ERROR;

        // 4. find all jobs that fit within the window
        sprintf(sql, "UPDATE JobLog SET [elapsed] = strftime('%%s', stoptime) - strftime('%%s', starttime) " \
                     "WHERE starttime >= datetime('%s') AND " \
                           "stoptime <= datetime ('%s'); ",
                     start_time, stop_time);
        rc = sqlite3_exec(db, sql, nullptr, nullptr, &zErrMsg);
        if (rc != SQLITE_OK) return SQL_ERROR;

        // now get all those jobs summed up
        sprintf(sql, "SELECT 0, JobLog.jid, name, display, 0, 0, SUM(elapsed) AS [elapsed] " \
                     "FROM JobLog INNER JOIN Jobs ON JobLog.jid = Jobs.jid WHERE " \
                         "(starttime < datetime('%s') AND stoptime > datetime ('%s')) OR " \
                         "(starttime < datetime('%s') AND stoptime > datetime ('%s')) OR " \
                         "(starttime >= datetime('%s') AND stoptime <= datetime ('%s')) " \
                     "GROUP BY JobLog.jid ORDER BY JobLog.jid;",
                     start_time, start_time,
                     stop_time, stop_time,
                     start_time, stop_time);

        std::vector<RawJobHistoryRec> raw_list;
        rc = sqlite3_exec(db, sql, ProcessJobHistory, &raw_list, &zErrMsg);
        if (rc != SQLITE_OK) return SQL_ERROR;

        if (!raw_list.empty()) {

            // translate this into a dataset that is usable to the outside world
            std::vector<RawJobHistoryRec>::iterator itr;
            for (itr = raw_list.begin(); itr != raw_list.end(); itr++) {
                JobHistorySummaryRec nr{};
                strcpy(nr.name, (*itr).name);
                if ((*itr).display[0]) strcpy(nr.display, (*itr).display); else nr.display[0] = 0;
                char ts[22]; SecondsToDurationStr((*itr).secs, ts);
                strcpy(nr.elapsed, ts);
                results->push_back(nr);
            }
        }

        return SQL_SUCCESS;
    }
    int GetLogHistory(const char* start_time, const char* stop_time, std::vector<JobRec>* results) {
        if (!db) return SQL_ERROR_NO_DB;
        if (!start_time || !stop_time || !results) return SQL_ERROR_BAD_PARAM;
        // minimum expected time stamp format = YYYY-MM-DD HH:MM:SS = 19 characters
        if (strlen(start_time) < 19 || strlen(stop_time) < 19) return SQL_ERROR_BAD_PARAM;

        // return all the job logs between start and stop, including any overlapping jobs

        // clear the results
        results->clear();

        // returns: rid, jid, name, starttime, stoptime, elapsed_seconds
        sprintf(sql, "SELECT rid, JobLog.jid, name, starttime, stoptime, " \
                     "       strftime('%%s', stoptime) - strftime('%%s', starttime) as [elasped] " \
                     "FROM JobLog INNER JOIN Jobs ON JobLog.jid = Jobs.jid " \
                     "WHERE starttime >= datetime('%s') AND starttime <= datetime('%s') AND " \
                     "      stoptime >= datetime ('%s') AND stoptime <= datetime('%s') " \
                     "ORDER BY starttime;",
                     start_time, stop_time, start_time, stop_time);

        rc = sqlite3_exec(db, sql, GetJobsList, results, &zErrMsg);
        if (rc != SQLITE_OK) return SQL_ERROR;

        return SQL_SUCCESS;
    }




    // Misc Helper Functions ======================================================
    void SecondsToDurationStr(int seconds, char* time_string) {
        int h = seconds / 3600;                 // 3600 seconds per hour
        int m = (seconds - (h * 3600)) / 60;    // 60 seconds per minute
        int s = seconds - (h * 3600 + m * 60);
        sprintf(time_string, "%02d:%02d:%02d", h, m, s);
    }
    int SecondsFromHMS(int hours, int mins, int secs) {
        // 60 secs = 1 min
        // 60 min = 1 hour = 3600
        // 24 hours = 1 day = 86400
        return SecondsFromDHMS(0, hours, mins, secs);
    }
    int SecondsFromDHMS(int days, int hours, int mins, int secs) {
        // 60 secs = 1 min
        // 60 min = 1 hour = 3600
        // 24 hours = 1 day = 86400
        return days * 86400 + hours * 3600 + mins * 60 + secs;
    }
}