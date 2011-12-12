#include "hmmer.pb.h"
#include "util.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <boost/regex.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <glog/logging.h>

void tokenize(const std::string& line,
              const std::string& expr,
              std::vector<std::string>* tokens) {
  CHECK_NOTNULL(tokens);
  tokens->clear();
  boost::regex pattern(expr);
  boost::algorithm::split_regex(*tokens, line, pattern);
}

double string_to_double(std::string const & str) {
  return std::strtod(str.c_str(),0);
}

std::string remove_newlines( std::string const & str ) {
  std::string copy = str;
  copy.erase(std::remove(copy.begin(), copy.end(), '\n'), copy.end());
  return copy;
}

int string_to_int(std::string const & str) {
  return std::strtol(str.c_str(),0,0);
}

void parse_output(char* filename, Response* response) {
  using std::string;
  using std::vector;
  CHECK_NOTNULL(filename);
  CHECK_NOTNULL(response);

  vector<string> lines;
  read_file(filename, &lines);

  static string const prefix = "  == domain";
  static string const finished_tag = "Internal pipeline statistics summary:";

  bool parsing = false;
  Response_Alignment* current_aln = NULL;

  for (vector<string>::const_iterator i = lines.begin(); i != lines.end(); ++i) {
    if (i->substr(0,prefix.size()) == prefix) {
      current_aln = response->add_alignments();

      vector<string> tokens;
      tokenize(*i, "\\s+", &tokens);

      double const evalue(string_to_double(tokens[8]));
      double log_evalue = -1;
      if ( evalue > 0 ) log_evalue = std::log(evalue);
      current_aln->set_ln_evalue(log_evalue);
      current_aln->set_bitscore(string_to_double(tokens[4]));

      parsing = true;
    } else if ( i->substr(0,finished_tag.size()) == finished_tag ) {
      break; // finished parsing
    } else if ( i->substr(0,2) == ">>" || remove_newlines(*i).length() == 0 ) {
      parsing = false;
    } else if ( parsing ) {
      string query_line = remove_newlines(*i); ++i;
      string cons_line = remove_newlines(*i); ++i;
      string templ_line = remove_newlines(*i); ++i;
      string prob_line = remove_newlines(*i); ++i;

      boost::trim(query_line);
      boost::trim(cons_line);
      boost::trim(templ_line);
      boost::trim(prob_line);
  
      vector<string> tokens;
      tokenize(query_line, "\\s+", &tokens);
      CHECK(tokens.size() == 4) << "Incorrect number of tokens-- " << tokens.size();

      // 0 -> id
      // 1 -> start
      // 2 -> sequence
      // 3 -> stop
      if ( current_aln->query_start() == 0 ) {
        current_aln->set_query_start(string_to_int(tokens[1]));
      }
      current_aln->set_aligned_query( current_aln->aligned_query() + tokens[2] );
  
      tokens.clear();
      tokenize(templ_line, "\\s+", &tokens);
      CHECK(tokens.size() == 4) << "Incorrect number of tokens-- " << tokens.size(); 

      current_aln->set_id(tokens[0]);
      if ( current_aln->template_start() == 0 ) {
        current_aln->set_template_start(string_to_int(tokens[1]));
      }
      current_aln->set_aligned_template( current_aln->aligned_template() + tokens[2] );
    }
  }
}
