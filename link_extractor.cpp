#include "link_extractor.h"
#include <regex>
#include <unordered_set>
#include <algorithm>
#include <iostream>

std::vector<std::string> extractLinks(const std::string& webpage, const std::string& baseDomain) {

    //std::cout << webpage << std::endl;
    std::vector<std::string> links;
    std::unordered_set<std::string> uniqueLinks;
    std::regex linkRegex("<a\\s+(?:[^>]*?\\s+)?href=([\"'])(.*?)\\1");

    auto begin = std::sregex_iterator(webpage.begin(), webpage.end(), linkRegex);
    auto end = std::sregex_iterator();

    for (auto it = begin; it != end; ++it) {
        std::string link = it->str(2);
        // we print the link to see what we are getting
        //std::cout << link << std::endl;

        // Clean and normalize the link
        link = normalizeLink(link, baseDomain);
        //std::cout << link << std::endl;


        // Extract the domain from the link
        std::string linkDomain = extractDomain(link);

        // Check if the link belongs to the base domain or is a relative URL
        if (isValidUrl(link)) {
            // Add the link to the unique set
            uniqueLinks.insert(link);
        }
    }

    // Copy the unique links to the vector
    links.assign(uniqueLinks.begin(), uniqueLinks.end());



    return links;
}

std::string normalizeLink(std::string link, const std::string& baseDomain) {
    // Remove any trailing slash unless it's a root slash
    if (!link.empty() && link.back() == '/' && link != "/") {
        link.pop_back();
    }

    // Add trailing slash for base URL (domain only)
    if (link.find("://") != std::string::npos && link.find('/', link.find("://") + 3) == std::string::npos) {
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

    // If it is a relative link, we add the base domain
        
    if (link.find("://") == std::string::npos) {
        if (link.empty() || link[0] != '/') {
            link = '/' + link;
        }
        link = "https://" + baseDomain + link;
    }

    if (link.substr(0, 7) == "http://") {
        link = "https://" + link.substr(7);
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