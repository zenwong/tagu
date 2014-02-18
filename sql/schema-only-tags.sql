CREATE TABLE Vids (
  _id integer primary key autoincrement,
  title text,
  path text,
  hash text,
  rating integer,
  synced integer default 0,
  remote integer default 0,
  unique(title,path) ON CONFLICT IGNORE
);
CREATE TABLE VidTags (
  _id integer primary key autoincrement,
  vid int,
  tid integer,
  synced integer default 0,
  remote integer default 0,
  votes integer default 1,
  foreign key(vid) references vids(_id) ON DELETE CASCADE,
  unique(vid,tid) ON CONFLICT IGNORE
);
CREATE TABLE Tags (
  _id integer primary key autoincrement,
  name text,
  cid integer default 0,
  synced integer default 0,
  remote integer default 0,
  unique(name) ON CONFLICT IGNORE
);
CREATE TABLE Alias (
  _id integer primary key autoincrement,
  tid integer,
  name text,
  unique(tid,name) ON CONFLICT IGNORE
);
CREATE TABLE Recommendations (
  _id integer primary key autoincrement,
  vid integer,
  vid2 integer,
  synced integer default 0,
  unique(vid,vid2) ON CONFLICT IGNORE
);
CREATE TABLE Category (
  _id integer primary key autoincrement,
  name text,
  unique(name) ON CONFLICT IGNORE
);

CREATE VIEW LibraryView as
select
vids._id as vid,
vids.title as title,
vids.rating as rating,
group_concat(DISTINCT t.name) as tags,
vids.path as path
from ( vids
  left join vidtags vt on vids._id = vt.vid
  left join tags t on vt.tid = t._id
) group by vids.title order by vids.title;

CREATE VIRTUAL TABLE search using fts4(title,tags, tokenize=porter);

CREATE TRIGGER update_fts_tags AFTER INSERT ON vidtags
BEGIN
  delete from search where docid = NEW.vid;
  insert into search(docid,title,tags) select vid as docid,title,tags from LibraryView where docid = NEW.vid;
END;
