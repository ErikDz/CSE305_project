#ifndef WEBPAGE_PARSER_H
#define WEBPAGE_PARSER_H

#include <string>

std::string extractTitle(const std::string& webpage);
std::string extractContentSummary(const std::string& webpage);

#endif