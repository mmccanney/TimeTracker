DELETE FROM JobLog WHERE starttime IS NULL OR starttime > '2020-07-08 01:00:00';
-- full insert
INSERT INTO JobLog (jid, starttime, stoptime) VALUES(11, datetime('2020-07-08 08:00:00'), datetime('2020-07-08 08:59:59'));
-- insert only START
INSERT INTO JobLog (jid, starttime) VALUES(11, datetime('2020-07-08 08:00:00'));
-- insert only STOP - this should never happen
INSERT INTO JobLog (jid, stoptime) VALUES(11, datetime('2020-07-08 08:59:59'));
-- insert just a JID = this should never happen
INSERT INTO JobLog (jid) VALUES(11);


/*
-- using uTimeNoNull : fails > updates all rows
UPDATE JobLog SET stoptime = datetime('2020-07-08 09:00:00') WHERE rid = 67


-- using uTimeNoNull
UPDATE JobLog SET starttime = datetime('2020-07-08 08:00:00') WHERE rid = 68


-- fix just the jid
UPDATE JobLog SET starttime = datetime('2020-07-08 08:00:00') WHERE rid = 69
UPDATE JobLog SET stoptime = datetime('2020-07-08 09:00:00') WHERE rid = 69

*/