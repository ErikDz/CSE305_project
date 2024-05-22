#include "link_extractor.h"
#include <regex>
#include <unordered_set>
#include <algorithm>

std::vector<std::string> extractLinks(const std::string& webpage, const std::string& baseDomain) {
    std::vector<std::string> links;
    std::unordered_set<std::string> uniqueLinks;
    std::regex linkRegex("<a\\s+(?:[^>]*?\\s+)?href=([\"'])(.*?)\\1");

    auto begin = std::sregex_iterator(webpage.begin(), webpage.end(), linkRegex);
    auto end = std::sregex_iterator();

    for (auto it = begin; it != end; ++it) {
        std::string link = it->str(2);

        // Clean and normalize the link
        link = normalizeLink(link);

        // Extract the domain from the link
        std::string linkDomain = extractDomain(link);

        // Check if the link belongs to the base domain or is a relative URL
        if ((linkDomain == baseDomain || linkDomain.empty()) && isValidUrl(link)) {
            // Add the link to the unique set
            uniqueLinks.insert(link);
        }
    }

    // Copy the unique links to the vector
    links.assign(uniqueLinks.begin(), uniqueLinks.end());

    return links;
}

std::string normalizeLink(std::string link) {
        // Remove any trailing slash
    if (!link.empty() && link.back() == '/') {
        link.pop_back();
    }

    // Add trailing slash for base URL (domain only)
    if (link.find("://") != std::string::npos && link.find('/') == link.find("://") + 2) {
        link += '/';
    }

    // Remove any query parameters
    size_t queryPos = link.find('?');
    if (queryPos != std::string::npos) {
        link = link.substr(0, queryPos);
    }

    // Remove any fragments
    size_t fragmentPos = link.find('#');
    if (fragmentPos != std::string::npos) {
        link = link.substr(0, fragmentPos);
    }

    return link;
}

std::string extractDomain(const std::string& url) {
    std::regex domainRegex("https?://([^/]+)");
    std::smatch match;
    if (std::regex_search(url, match, domainRegex)) {
        return match[1].str();
    }
    return "";
}

bool isValidUrl(const std::string& url) {
    // Implement a function to validate the URL format
    // Example validation: check if the URL starts with "http://" or "https://"
    return url.find("http://") == 0 || url.find("https://") == 0;
}