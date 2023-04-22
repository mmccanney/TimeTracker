DROP TRIGGER IF EXISTS uTimeNoNull;
CREATE TRIGGER IF NOT EXISTS uTimeNoNull 
AFTER UPDATE OF starttime, stoptime ON JobLog 
WHEN NEW.starttime IS NOT NULL AND NEW.stoptime IS NOT NULL AND NEW.rid IS NOT NULL BEGIN 
    UPDATE JobLog 
    SET elapsed = 
          strftime('%s', NEW.stoptime) - strftime('%s', NEW.starttime) 
    WHERE rid = new.rid;
END;

DROP TRIGGER IF EXISTS uTimeNull;
CREATE TRIGGER IF NOT EXISTS uTimeNull 
AFTER UPDATE OF starttime, stoptime ON JobLog 
WHEN NEW.starttime IS NULL OR NEW.stoptime IS NULL BEGIN 
    UPDATE JobLog 
    SET elapsed = 0 
    WHERE rid = new.rid;
END;

DROP TRIGGER IF EXISTS iTimeNull;
CREATE TRIGGER IF NOT EXISTS iTimeNull 
AFTER INSERT ON JobLog 
WHEN NEW.starttime IS NULL OR NEW.stoptime IS NULL BEGIN 
    UPDATE JobLog 
    SET elapsed = 0 
    WHERE rid = new.rid;
END;

DROP TRIGGER IF EXISTS iTimeNoNull;
CREATE TRIGGER IF NOT EXISTS iTimeNoNull 
AFTER INSERT ON JobLog 
WHEN NEW.starttime IS NOT NULL AND NEW.stoptime IS NOT NULL BEGIN 
    UPDATE JobLog 
    SET elapsed = strftime('%s', NEW.stoptime) - strftime('%s', NEW.starttime) 
    WHERE rid = new.rid;
END;
