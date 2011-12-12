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

#endif  // PARSE_H_
