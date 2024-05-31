#include "webpage_downloader.h"
#include <curl/curl.h>
#include <stdexcept>
#include <string>

// Callback function to write the response data
size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* response) {
    size_t totalSize = size * nmemb;
    response->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

// Function to download a webpage with a specified maximum redirect depth
std::string downloadWebpage(const std::string& url) {
    const int maxRedirectDepth = 100;
    CURL* curl = curl_easy_init();
    if (!curl) {
        throw std::runtime_error("Failed to initialize libcurl");
    }

    std::string response;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // Enable following redirects
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, maxRedirectDepth); // Set maximum redirect depth

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        curl_easy_cleanup(curl);
        throw std::runtime_error("Failed to download webpage: " + std::string(curl_easy_strerror(res)));
    }

    curl_easy_cleanup(curl);
    return response;
}
