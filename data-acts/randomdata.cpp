#include <NLDatabase.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
using namespace boost;
using namespace boost::filesystem;
using namespace std;
using namespace NL::DB;

Database db("db");

void import() {
  ifstream tags("tags");
  ifstream acts("acts");
  ifstream category("category");
  ifstream stats("actress-stats");
  ifstream directories("dirs");

  db.begin();
  db.query("delete from vids");
  db.query("delete from vidacts");
  db.query("delete from vidtags");
  db.query("delete from acts");
  db.query("delete from tags");
  db.query("delete from sqlite_sequence where name=?").execute("vids");
  db.query("delete from sqlite_sequence where name=?").execute("vidtags");
  db.query("delete from sqlite_sequence where name=?").execute("vidacts");
  db.query("delete from sqlite_sequence where name=?").execute("tags");
  db.query("delete from sqlite_sequence where name=?").execute("acts");

  string line;
  while(!tags.eof()) {
    getline(tags, line);
    if(line.size() > 1)
      db.query("insert into tags(name) values(?)").execute(line); cout << line << '\n';
  }

  while(!acts.eof()) {
    getline(acts, line);
    if(line.size() > 1)
      db.query("insert into acts(name) values(?)").execute(line); cout << line << '\n';
  }

  while(!category.eof()) {
    getline(category, line);
    if(line.size() > 1) {

      vector<string> part;
      boost::split(part, line, boost::is_any_of(","));

      string category = part.at(0);
      boost::trim(category);

      db.query("insert into category(name) values(?)").execute(category);
      int cid = db.query("select _id from category where name = ?").select_single(category).column_int(0);

      for(int i = 1; i < part.size(); i++) {
	db.query("insert into tags(cid,name) values(?,?)").execute(cid, boost::trim_copy(part.at(i)));
      }

    }
  }

  while(!stats.eof()) {
    getline(stats, line);
    if(line.size() > 1) {

      vector<string> part;
      boost::split(part, line, boost::is_any_of(","));

      string category = part.at(0);
      boost::trim(category);

      db.query("insert into category(name) values(?)").execute(category);
      int cid = db.query("select _id from category where name = ?").select_single(category).column_int(0);

      for(int i = 1; i < part.size(); i++) {
	db.query("insert into tags(cid,name) values(?,?)").execute(cid, boost::trim_copy(part.at(i)));
      }

    }
  }

  while(!directories.eof()) {
    getline(directories,line);
    if(line.size() > 1) {
      path dir(line);

      for(recursive_directory_iterator iter(dir), end; iter != end; ++iter) {
	if(is_regular_file(iter->status())) {
	  auto title = iter->path().stem().string();
	  auto ext   = boost::to_lower_copy(iter->path().extension().string());
	  auto fpath = iter->path().string();

	  if(ext == ".avi" || ext == ".mp4" || ext == ".wmv" || ext == ".mkv" || ext == ".mpg"
	      || ext == ".mpeg" || ext == ".flv" || ext == ".mov" || ext == ".asf" || ext == ".rmvb"
	      || ext == ".ogm") {

	    db.query( "INSERT INTO vids(title) VALUES(?)" ).execute(title);
	  }

	}
      }

    }

  }
  db.commit();
}

void random_data() {
  db.begin();
  int total_vids = db.query("select count(*) from vids").select_single().column_int(0);
  int total_tags = db.query("select count(*) from tags").select_single().column_int(0);
  int total_acts = db.query("select count(*) from acts").select_single().column_int(0);

  random_device rd;
  mt19937 e1(rd());
  mt19937 e2(rd());
  mt19937 e3(rd());

  uniform_int_distribution<int> dist_tids(1, total_tags);
  uniform_int_distribution<int> dist_aids(1, total_acts);
  uniform_int_distribution<int> ntags(2, 6);

  for(int i = 1; i < total_vids; i++) {
    for(int y = 1; y < ntags(e3); y++) {
      //cout << "tid: " << dist_tids(e1) << ", aid: " << dist_aids(e2) << '\n';
      db.query("insert into vidtags(vid,tid) values(?,?)").execute(i, dist_tids(e1));
      db.query("insert into vidacts(vid,aid) values(?,?)").execute(i, dist_aids(e2));
    }
  }

  db.commit();
}

void import2() {
  db.begin();
  int total_vids = db.query("select count(*) from vids").select_single().column_int(0);
  int total_tags = db.query("select count(*) from tags").select_single().column_int(0);
  int total_acts = db.query("select count(*) from acts").select_single().column_int(0);

  vector<int> tids;
  for(const auto& id: db.query("select _id from tags where cid = ? or cid = ? or cid = ? or cid = ? or cid = ?").select(4,5,6,7,8)) {
    //cout << id.column_int(0) << '\n';
    tids.emplace_back(id.column_int(0));
  }

  // for(int i = 0; i < total_acts; i++) {
  //   int ntags = 1 + rand() % (10 - 1 + 1);

  //   for(int y = 0; y < ntags; y++) {
  //     int idx = rand() % tids.size();
  //     db.query("insert into acttags(aid,tid) values(?,?)").execute(i, tids.at(idx));
  //   }
  // }

  for(int i = 1; i < total_vids; i++) {
    for(int y = 0; y < 10; y++) {
      int tid = 1 + rand() % total_tags;
      int aid = 1 + rand() % total_acts;
      //cout << "tid: " << tid << ", aid: " << aid << '\n';
      db.query("insert into vidtags(vid,tid) values(?,?)").execute(i, tid);
      db.query("insert into vidacts(vid,aid) values(?,?)").execute(i, aid);
    }
  }
  db.commit();
}

int main(int argc, char **argv) {
  srand( time(NULL));

  import2();
  //import();
  //random_data();
  return 0;
}
