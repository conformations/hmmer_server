#include "hmmer.pb.h"
#include "str_util.h"
#include "util.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/lexical_cast.hpp>
#include <glog/logging.h>

static const std::string prefix = "  == domain";
static const std::string sentinel = "Internal pipeline statistics summary:";

void parse_output(char* filename, Response* response) {
  using std::string;
  using std::vector;
  CHECK_NOTNULL(filename);
  CHECK_NOTNULL(response);

  vector<string> lines;
  read_file(filename, &lines);

  bool parsing = false;
  Response_Alignment* current_aln = NULL;

  for (size_t i = 0; i < lines.size(); ++i) {
    string line = lines[i];

    if (line == sentinel)
      break;

    if (boost::starts_with(line, prefix)) {
      current_aln = response->add_alignments();

      vector<string> tokens;
      tokenize(line, "\\s+", &tokens);

      double evalue = boost::lexical_cast<double>(tokens[9]);
      current_aln->set_ln_evalue(std::log(evalue));

      double bitscore = boost::lexical_cast<double>(tokens[5]);
      current_aln->set_bitscore(bitscore);

      parsing = true;
    } else if ( boost::starts_with(line, ">>") || remove_newlines(lines[i]).length() == 0 ) {
      parsing = false;
    } else if ( parsing ) {
      string query_line = remove_newlines(lines[i++]);
      string cons_line  = remove_newlines(lines[i++]);
      string templ_line = remove_newlines(lines[i++]);
      string prob_line  = remove_newlines(lines[i++]);

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
        unsigned idx = boost::lexical_cast<unsigned>(tokens[1]);
        current_aln->set_query_start(idx);
      }
      current_aln->set_aligned_query( current_aln->aligned_query() + tokens[2] );
  
      tokens.clear();
      tokenize(templ_line, "\\s+", &tokens);
      CHECK(tokens.size() == 4) << "Incorrect number of tokens-- " << tokens.size(); 

      current_aln->set_id(tokens[0]);
      if ( current_aln->template_start() == 0 ) {
        unsigned idx = boost::lexical_cast<unsigned>(tokens[1]);
        current_aln->set_template_start(idx);
      }
      current_aln->set_aligned_template( current_aln->aligned_template() + tokens[2] );
    }
  }
}
