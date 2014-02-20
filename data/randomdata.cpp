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
  ifstream directories("dirs");
  //regex rx(".*?([a-zA-Z]{2,5})[-]([0-9]{2,4}).*?");
  //smatch match;

  db.begin();
  db.query("delete from vids");
  db.query("delete from vidtags");
  db.query("delete from tags");
  db.query("delete from sqlite_sequence where name=?").execute("vids");
  db.query("delete from sqlite_sequence where name=?").execute("vidtags");
  db.query("delete from sqlite_sequence where name=?").execute("tags");

  db.query("insert into category(name) values(?)").execute("tag");
  db.query("insert into category(name) values(?)").execute("actress");
  db.query("insert into category(name) values(?)").execute("body type");
  db.query("insert into category(name) values(?)").execute("sex act");
  db.query("insert into category(name) values(?)").execute("sex position");

  string line;
  while(!tags.eof()) {
    getline(tags, line);
    if(line.size() > 1)
      db.query("insert into tags(cid,name) values(1,?)").execute(line); //cout << line << '\n';
  }

  while(!acts.eof()) {
    getline(acts, line);
    if(line.size() > 1)
      db.query("insert into tags(cid,name) values(2,?)").execute(line); //cout << line << '\n';
  }
  db.commit();

  db.begin();
  while(!directories.eof()) {
    getline(directories,line);
    if(line.size() > 1) {
      path dir(line);

      for(recursive_directory_iterator iter(dir), end; iter != end; ++iter) {
	if(is_regular_file(iter->status())) {
	  auto title = boost::to_lower_copy(iter->path().stem().string());
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

  random_device rd;
  mt19937 e1(rd());
  mt19937 e2(rd());
  mt19937 e3(rd());

  uniform_int_distribution<int> dist_tids(1, total_tags);
  uniform_int_distribution<int> ntags(2, 6);

  for(int i = 1; i < total_vids; i++) {
    for(int y = 1; y < ntags(e3); y++) {
      //cout << "tid: " << dist_tids(e1) << '\n';
      db.query("insert into vidtags(vid,tid) values(?,?)").execute(i, dist_tids(e1));
    }
  }

  db.commit();
}

int main(int argc, char **argv) {
  import();
  random_data();
  return 0;
}
