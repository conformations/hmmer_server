#include "str_util.h"

#include <fstream>
#include <string>
#include <vector>

#include <boost/algorithm/string/erase.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <glog/logging.h>

void tokenize(const std::string& line,
              const std::string& expr,
              std::vector<std::string>* tokens) {
  CHECK_NOTNULL(tokens);
  tokens->clear();
  boost::regex pattern(expr);
  boost::algorithm::split_regex(*tokens, line, pattern);
}

void remove_all(const std::string& s, std::string* str) {
  CHECK_NOTNULL(str);
  boost::erase_all(*str, s);
}

void replace_all(const std::string& find, const std::string& replace, std::string* str) {
  CHECK_NOTNULL(str);
  boost::replace_all(*str, find, replace);
}

std::string remove_newlines( std::string const & str ) {
  std::string copy = str;
  copy.erase(std::remove(copy.begin(), copy.end(), '\n'), copy.end());
  return copy;
}

void write_file(const std::string& contents, const char* file) {
  CHECK_NOTNULL(file);
  std::ofstream f(file);
  f << contents;
  f.close();
}

void read_file(const char* file, std::vector<std::string>* lines) {
  CHECK_NOTNULL(file);
  CHECK_NOTNULL(lines);

  lines->clear();

  std::string line;

  std::ifstream f(file);
  while (f.good()) {
    getline(f, line);
    lines->push_back(line);
  }
}
