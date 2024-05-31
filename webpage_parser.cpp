#include "webpage_parser.h"
#include <regex>

std::string extractTitle(const std::string& webpage) {
    std::regex titleRegex("<title>(.*?)</title>");
    std::smatch match;
    if (std::regex_search(webpage, match, titleRegex)) {
        return match[1].str();
    }
    return "";
}

std::string extractContentSummary(const std::string& webpage) {
    // Extract the meta description as the content summary
    std::regex metaDescriptionRegex("<meta\\s+name=[\"']description[\"']\\s+content=[\"'](.*?)[\"']");
    std::smatch match;
    if (std::regex_search(webpage, match, metaDescriptionRegex)) {
        return match[1].str();
    }
    return "";
}