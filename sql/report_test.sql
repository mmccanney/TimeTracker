SELECT * FROM JobLog; --order by jid;

/*
Testing for the Job Summary Report

Use dataset1.sql to set up the data
start time for the report = 2020-07-07 00:00:00
stop time for the report  = 2020-07-10 00:00:00

There are two methods in play
1. calculate [elapsed] on the fly in the select statements
2. update each applicable rows [elapsed] field then query

Method 1 is no longer valid. The final query in the union requires that [elapsed] is
valid for each record. This is true if the triggers are used. However, the spec has
changed and it makes more sense to allow [elapsed] to be altered to simplify
reporting in the future.

*/


-- Method 2: this one recalcs any applicable rows elapsed field first, the collects them all at once

-- 1. find any job the spans across the starting time but not the stop time
--      these need the elapsed time trimmed to ignore time outside the requested window
UPDATE JobLog SET [elapsed] = strftime('%s', stoptime) - strftime('%s', '2020-07-07 00:00:00') 
WHERE 
    starttime < datetime('2020-07-07 00:00:00') AND 
    stoptime > datetime ('2020-07-07 00:00:00') AND 
    stoptime <= datetime ('2020-07-10 00:00:00');
-- should modify rid 1, new elapsed is 10800 (3 hours)


-- 2. find any job that starts before the window and ends after the window (ie: entirely spans the window)
--      these need the elapsed time trimmed to equal to the window span
UPDATE JobLog SET [elapsed] = strftime('%s', '2020-07-10 00:00:00') - strftime('%s', '2020-07-07 00:00:00') 
WHERE 
    starttime < datetime('2020-07-07 00:00:00') AND 
    stoptime > datetime ('2020-07-10 00:00:00');
-- should not modify anything


-- 3. find any job that starts in the window but ends after the window.
--      these need the elapsed time trimmed to ignore outside the requested window
UPDATE JobLog SET [elapsed] = strftime('%s', '2020-07-10 00:00:00') - strftime('%s', starttime)
WHERE 
    starttime >= datetime('2020-07-07 00:00:00') AND 
    starttime < datetime ('2020-07-10 00:00:00') AND
    stoptime > datetime ('2020-07-10 00:00:00');
-- should modify rid 9, new elapsed = 21600 (6 hours)


-- 4. find all jobs that fit within the window
UPDATE JobLog SET [elapsed] = strftime('%s', stoptime) - strftime('%s', starttime)
WHERE 
    starttime >= datetime('2020-07-07 00:00:00') AND 
    stoptime <= datetime ('2020-07-10 00:00:00');
-- should modify rid 2-8 as follows
-- 2 = 3600
-- 3 = 1800
-- 4 = 14400
-- 5 = 1800
-- 6 = 5400
-- 7 = 7200
-- 8 = 5400

-- collect them all now
SELECT jid, sum(elasped) FROM JobLog WHERE
-- spans the start of the window
(starttime < datetime('2020-07-07 00:00:00') AND stoptime > datetime ('2020-07-07 00:00:00'))
OR
-- spans the end of the window
(starttime < datetime('2020-07-10 00:00:00') AND stoptime > datetime ('2020-07-10 00:00:00'))
OR
-- is within the window
(starttime >= datetime('2020-07-07 00:00:00') AND stoptime <= datetime ('2020-07-10 00:00:00'))
GROUP BY jid
ORDER BY jid;
-- should result in
-- 1    10800
-- 2    28800
-- 3    32400


/*
    Method 1 - this one does the Elapsed math on the fly
    deprecated - needs the triggers
    the spec makes this invalid
*/
-- select those that cross the start but not the stop
SELECT jid, strftime('%s', stoptime) - strftime('%s', '2020-07-07 00:00:00') as [elasped]
FROM JobLog 
WHERE starttime < datetime('2020-07-07 00:00:00') AND 
      stoptime >= datetime ('2020-07-07 00:00:00') AND 
	  stoptime <= datetime ('2020-07-09 23:59:59')
--LIMIT 1
UNION
-- select any single entry that crosses the start and the stop
SELECT jid, strftime('%s', '2020-07-09 23:59:59') - strftime('%s', '2020-07-07 00:00:00') AS [elasped]
FROM JobLog 
WHERE starttime <= datetime('2020-07-07 00:00:00') AND 
      stoptime >= datetime ('2020-07-09 23:59:59')
--LIMIT 1
UNION
-- select those that spane the end of the time frame (but the start is within)
SELECT jid, strftime('%s', stoptime) - strftime('%s', '2020-07-09 23:59:59') AS [elasped]
FROM JobLog 
WHERE starttime >= datetime('2020-07-07 00:00:00') AND 
      stoptime >= datetime ('2020-07-09 23:59:59') AND 
	  starttime < datetime ('2020-07-09 23:59:59')
UNION	
-- select only those that fit completely within the time frame
SELECT jid, sum(elapsed) AS [elapsed]
FROM JobLog 
WHERE starttime >= datetime('2020-07-07 00:00:00') AND 
      stoptime <= datetime ('2020-07-09 23:59:59')
GROUP BY jid
ORDER BY jid;
