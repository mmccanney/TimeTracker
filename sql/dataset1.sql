DELETE FROM Jobs;
UPDATE sqlite_sequence SET seq = 0 WHERE name="Jobs";

INSERT INTO Jobs (name, display, active) VALUES ('IJ1111', 'Test job1', 1);
INSERT INTO Jobs (name, display, active) VALUES ('IJ2222', null, 1);
INSERT INTO Jobs (name, display, active) VALUES ('IJ3333', 'Test job3', 1);


DELETE FROM JobLog;
UPDATE sqlite_sequence SET seq = 0 WHERE name="JobLog";

INSERT INTO JobLog (jid, starttime, stoptime) VALUES(3, datetime('2020-07-06 23:00:00'), datetime('2020-07-07 03:00:00'));

INSERT INTO JobLog (jid, starttime, stoptime) VALUES(1, datetime('2020-07-07 08:00:00'), datetime('2020-07-07 09:00:00'));
INSERT INTO JobLog (jid, starttime, stoptime) VALUES(2, datetime('2020-07-07 10:00:00'), datetime('2020-07-07 10:30:00'));
INSERT INTO JobLog (jid, starttime, stoptime) VALUES(3, datetime('2020-07-07 11:00:00'), datetime('2020-07-07 15:00:00'));

INSERT INTO JobLog (jid, starttime, stoptime) VALUES(1, datetime('2020-07-08 11:00:00'), datetime('2020-07-08 11:30:00'));
INSERT INTO JobLog (jid, starttime, stoptime) VALUES(2, datetime('2020-07-08 14:00:00'), datetime('2020-07-08 15:30:00'));
INSERT INTO JobLog (jid, starttime, stoptime) VALUES(3, datetime('2020-07-08 16:00:00'), datetime('2020-07-08 18:00:00'));

INSERT INTO JobLog (jid, starttime, stoptime) VALUES(1, datetime('2020-07-09 08:00:00'), datetime('2020-07-09 09:30:00'));
INSERT INTO JobLog (jid, starttime, stoptime) VALUES(2, datetime('2020-07-09 18:00:00'), datetime('2020-07-10 02:00:00'));



/*
Results:

rid   jid   starttime             stoptime              elapsed
------------------------------------------------------------------
1     3     2020-07-06 23:00:00   2020-07-07 03:00:00   0
2     1     2020-07-07 08:00:00   2020-07-07 09:00:00   0
3     2     2020-07-07 10:00:00   2020-07-07 10:30:00   0
4     3     2020-07-07 11:00:00   2020-07-07 15:00:00   0
5     1     2020-07-08 11:00:00   2020-07-08 11:30:00   0
6     2     2020-07-08 14:00:00   2020-07-08 15:30:00   0
7     3     2020-07-08 16:00:00   2020-07-08 18:00:00   0
8     1     2020-07-09 08:00:00   2020-07-09 09:30:00   0
9     2     2020-07-09 18:00:00   2020-07-10 02:00:00   0


*/