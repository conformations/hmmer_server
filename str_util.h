#ifndef STR_UTIL_H_
#define STR_UTIL_H_

#include <string>
#include <vector>

// Populates tokens with the result of splitting line by expr
void tokenize(const std::string& line,
              const std::string& regex,
              std::vector<std::string>* tokens);

// Removes all instances of s from str
void remove_all(const std::string& s, std::string* str);

// Replaces all instances of find with replace in str
void replace_all(const std::string& find, const std::string& replace, std::string* str);

// Returns a copy of str with newline characters removed
std::string remove_newlines(const std::string& str);

// Reads the contents of the specified file into lines
void read_file(const char* file, std::vector<std::string>* lines);

// Writes contents to a file with the specified name
void write_file(const std::string& contents, const char* file);

#endif  // STR_UTIL_H_
