#ifndef LINK_EXTRACTOR_H
#define LINK_EXTRACTOR_H

#include <string>
#include <vector>

std::vector<std::string> extractLinks(const std::string& webpage, const std::string& baseDomain);
std::string normalizeLink(std::string link);
std::string extractDomain(const std::string& url);
bool isValidUrl(const std::string& url);
#endif
