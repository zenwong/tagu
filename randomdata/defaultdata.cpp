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
  ifstream acts("jav-acts");
  ifstream category("category");
  ifstream stats("actress-stats");
  ifstream directories("dirs");

  db.begin();
  db.query("delete from vids");
  db.query("delete from vidacts");
  db.query("delete from vidtags");
  db.query("delete from acts");
  db.query("delete from tags");
  db.query("delete from sqlite_sequence").execute();

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

  db.commit();
}

int main(int argc, char **argv) {
  import();
  return 0;
}
