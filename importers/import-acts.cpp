#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include "../NLDatabase/NLDatabase.h"
using namespace std;
using namespace boost;
using namespace NL::DB;

Database db("db");

void toTitleCase(string& strToConvert) {
  bool thisWordCapped = false;
  to_lower(strToConvert);

  for (unsigned int i = 0; i < strToConvert.length(); i++) {
    if ((ispunct(strToConvert[i])) || (isspace(strToConvert[i])))
      thisWordCapped = false;

    if ((thisWordCapped == false) && (isalpha(strToConvert[i]))) {
      strToConvert[i] = toupper(strToConvert[i]);
      thisWordCapped = true;
    }
  }
}

int main(int argc, char **argv) {
  ifstream file;
  stringstream ss;
  file.open("act-names");
  stringstream sstr;
  ss << file.rdbuf();
  string content = ss.str();
  file.close();

  vector<string> tokens;
  split(tokens, content, is_any_of(","));

  db.begin();
  for(auto token: tokens) {
    boost::replace_all(token, "_", " ");
    trim(token);
    toTitleCase(token);
    if(token.size() > 3) {
      db.query("insert into acts(name) values(?)").execute(token);
    }
  }
  db.commit();

  return 0;
}
