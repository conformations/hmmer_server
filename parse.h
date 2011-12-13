#ifndef PARSE_H_
#define PARSE_H_

#include <string>
#include <vector>

class Response;
void parse_output(char* filename, Response* response);

// Populates tokens with the result of splitting line by expr
void tokenize(const std::string& line,
              const std::string& regex,
              std::vector<std::string>* tokens);

// Removes all instances of s from str
void remove_all(const std::string& s, std::string* str);

// Replaces all instances of find with replace in str
void replace_all(const std::string& find, const std::string& replace, std::string* str);

#endif  // PARSE_H_
