CREATE TABLE Vids (
  _id integer primary key autoincrement,
  title text,
  path text,
  hash text,
  filesize integer,
  unique(title,path) ON CONFLICT IGNORE
);
CREATE TABLE VidTags (
  _id integer primary key autoincrement,
  title text,
  tid integer,
  unique(title,tid) ON CONFLICT IGNORE
);
CREATE TABLE VidActs (
  _id integer primary key autoincrement,
  title text,
  aid integer,
  unique(title,aid) ON CONFLICT IGNORE
);
CREATE TABLE Tags (
  _id integer primary key autoincrement,
  name text,
  unique(name) ON CONFLICT IGNORE
);
CREATE TABLE Acts (
  _id integer primary key autoincrement,
  name text,
  cup_size char(1),
  status text,
  unique(name) ON CONFLICT IGNORE
);
CREATE TABLE ImportDirs (
  _id integer primary key autoincrement,
  name text,
  unique(name) ON CONFLICT IGNORE
);
CREATE TABLE Settings (
  _id integer primary key autoincrement,
  username text,
  password text,
  session text,
  thumb_percentage int,
  thumb_spacing int,
  thumb_title_overlay int,
  thumb_width int,
  thumb_height int,
  thumb_location text,
  cover_location text,
  unique(profile) ON CONFLICT IGNORE
);

CREATE VIEW TagsForVid as select title,name from VidTags inner join tags where tags._id = vidtags.tid;
CREATE VIEW ActsForVid as select title,name from VidActs inner join acts where acts._id = vidacts.aid;
CREATE VIEW SearchView as select name from tags union all select name from acts;
CREATE VIEW vidactsView as select vidacts.title  as title, group_concat(acts.name) as act from vidacts inner join acts on acts._id = vidacts.aid group by title;
CREATE VIEW vidtagsView as select vidtags.title as title, group_concat(tags.name) as tag from vidtags inner join tags on tags._id = vidtags.tid group by title;
