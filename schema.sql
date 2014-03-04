CREATE TABLE Vids (
  _id integer primary key autoincrement,
  title text,
  path text,
  hash text,
  rating integer,
  synced integer default 0,
  unique(title,path) ON CONFLICT IGNORE
);
CREATE TABLE VidTags (
  _id integer primary key autoincrement,
  vid integer,
  tid integer,
  cid integer default 1,
  synced integer default 0,
  foreign key(vid) references vids(_id) ON DELETE CASCADE,
  foreign key(tid) references tags(_id) ON DELETE CASCADE,
  foreign key(cid) references category(_id),
  unique(vid,tid) ON CONFLICT IGNORE
);
CREATE TABLE VidActs (
  _id integer primary key autoincrement,
  vid integer,
  aid integer,
  synced integer default 0,
  foreign key(vid) references vids(_id) ON DELETE CASCADE,
  foreign key(aid) references acts(_id) ON DELETE CASCADE,
  unique(vid,aid) ON CONFLICT IGNORE
);
CREATE TABLE ActTags (
  _id integer primary key autoincrement,
  aid integer,
  tid integer,
  synced integer default 0,
  foreign key(aid) references aids(_id) ON DELETE CASCADE,
  foreign key(tid) references tags(_id) ON DELETE CASCADE,
  unique(aid,tid) ON CONFLICT IGNORE
);
CREATE TABLE Category (
  _id integer primary key autoincrement,
  name text,
  unique(name) ON CONFLICT IGNORE
);
CREATE TABLE Tags (
  _id integer primary key autoincrement,
  cid integer default 1,
  name text,
  description text,
  foreign key(cid) references category(_id),
  unique(name) ON CONFLICT IGNORE
);
CREATE TABLE Acts (
  _id integer primary key autoincrement,
  name text,
  foreign_name text,
  unique(name) ON CONFLICT IGNORE
);
CREATE TABLE Settings (
  key int,
  value text,
  primary key(key,value)
);

CREATE VIEW TermsView as select name from tags union all select name from acts;

CREATE VIEW LibraryView as
select
vids._id as vid,
vids.title as title,
vids.rating as rating,
group_concat(DISTINCT t.name) as tags,
group_concat(DISTINCT a.name) as acts,
vids.path as path
from ( vids
  left join vidacts va on vids._id = va.vid
  left join vidtags vt on vids._id = vt.vid
  left join acts a on va.aid = a._id
  left join tags t on vt.tid = t._id
) group by vids.title order by vids.title;

CREATE VIEW ActressView as
select
acts._id as aid,
acts.name as name,
group_concat(tags.name) as tags
from( acts
  left join acttags on acttags.aid = acts._id
  left join tags on acttags.tid = tags._id
) group by acts.name order by acts.name;

CREATE VIEW ActList as
select
vidacts.vid as vid,
acts._id as aid,
acts.name as act
from acts
inner join vidacts on vidacts.aid = acts._id
order by act;

CREATE VIEW TagList as
select
vidtags.vid as vid,
tags._id as tid,
tags.name as tag
from tags
inner join vidtags on vidtags.tid = tags._id
order by tag;

CREATE VIEW TagCategory as
select
tags._id as tid,
category.name as category,
tags.name as tag
from tags
inner join category on tags.cid = category._id
order by category;

CREATE VIEW SyncTags as
select
vids.title as title,
tags.name as tag
from vidtags
inner join tags on vidtags.tid = tags._id
inner join vids on vids._id = vidtags.vid
where vidtags.synced = 0;

CREATE VIEW SyncActs as
select
vids.title as title,
acts.name as act
from vidacts
inner join acts on vidacts.aid = acts._id
inner join vids on vids._id = vidacts.vid
where vidacts.synced = 0;

CREATE VIEW SyncActTags as
select
acts.name as name,
group_concat(DISTINCT tags.name) as tags
from acttags
inner join acts on acttags.aid = acts._id
inner join tags on tags._id = acttags.tid
where acttags.synced = 0;

CREATE VIEW TagFilter as select _id,name from tags order by name;
CREATE VIEW ActFilter as select _id,name from acts order by name;

CREATE VIRTUAL TABLE search using fts4(title,tags,acts, tokenize=porter);

CREATE TRIGGER update_fts_tags AFTER INSERT ON vidtags
BEGIN
  delete from search where docid = NEW.vid;
  insert into search(docid,title,tags,acts) select vid as docid,title,tags,acts from LibraryView where docid = NEW.vid;
END;

CREATE TRIGGER update_fts_acts AFTER INSERT ON vidacts
BEGIN
  delete from search where docid = NEW.vid;
  insert into search(docid,title,tags,acts) select vid as docid,title,tags,acts from LibraryView where docid = NEW.vid;
END;

pragma foreign_keys = on;
