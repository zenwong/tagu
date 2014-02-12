#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <unistd.h>
#include <fcntl.h>
#include <NLDatabase.h>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <sstream>
using namespace std;
using namespace NL::DB;
using namespace boost::filesystem;
using namespace boost;

Database db("db");

void import() {
  vector<string> dirs;
  ifstream infile;
  infile.open("dirs");
  string line;
  smatch match;
  char buf[9999];
  int fd;
  stringstream ss;

  while(!infile.eof()) {
    getline(infile, line);
    if(line.size() > 2) {
      dirs.emplace_back(line);
    }
    //cout << line << '\n';
  }

  db.query("delete from vids").execute();

  db.begin();
  for(auto& d: dirs) {
    path dir(d);

    for(recursive_directory_iterator iter(dir), end; iter != end; ++iter) {
      if(is_regular_file(iter->status())) {
	auto title = boost::to_lower_copy(iter->path().leaf().string());
	auto ext   = boost::to_lower_copy(iter->path().extension().string());
	auto fpath = iter->path().string();

	if(ext == ".avi" || ext == ".mp4" || ext == ".wmv" || ext == ".mkv" || ext == ".mpg"
	    || ext == ".mpeg" || ext == ".flv" || ext == ".mov" || ext == ".asf" || ext == ".rmvb"
	    || ext == ".ogm") {

	  db.query( "INSERT INTO vids(title,path) VALUES(?,?)" ).execute(title, fpath, ss.str());
	}

      }
    }
  }

  db.commit();
}

int main(int argc, char **argv) {
  import();

  return 0;
}
