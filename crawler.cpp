#include "crawler.h"
#include "webpage_downloader.h"
#include "link_extractor.h"
#include <iostream>
#include <regex>
#include "webpage_parser.h"
#include <fstream>
#include <chrono>

Crawler::Crawler(const std::string& startUrl, int numThreads)
    : startUrl(startUrl), numThreads(numThreads), visitedWebpages(100), pagesVisited(0) {
}

void Crawler::setThreadCount(int count) {
    numThreads = count;
}

void Crawler::run() {
    // we save the base domain on an environment variable
    std::vector<std::thread> threads;
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(&Crawler::crawlLoop, this);
    }

    {
        std::unique_lock<std::mutex> lock(queueMutex);
        urlQueue.push(startUrl);
        queueCondVar.notify_one();
    }

    const int maxPagesToVisit = 1000;

    while (!urlQueue.empty() && pagesVisited < maxPagesToVisit) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    {
        std::unique_lock<std::mutex> lock(queueMutex);
        while (!urlQueue.empty()) {
            urlQueue.pop();
        }
        queueCondVar.notify_all();
    }

    for (auto& thread : threads) {
        thread.join();
    }
}

void Crawler::crawlLoop() {
    while (true) {
        std::string url;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            // Wait for the condition variable to be notified and check the queue is not empty
            queueCondVar.wait(lock, [this] { return !urlQueue.empty(); });

            // When notified, check if the queue is still not empty
            if (urlQueue.empty()) {
                continue; // If the queue is empty, go back to waiting
            }

            url = urlQueue.front();
            urlQueue.pop();
        }

        // Normalize URL
        url = normalizeLink(url, extractDomain(startUrl));

        // Print the thread ID and URL being crawled
        std::cout << "Thread ID: " << std::this_thread::get_id() << std::endl;
        std::cout << "Crawling: " << url << std::endl;

        // Check if the URL has already been visited
        if (visitedWebpages.contains(url)) {
            std::cout << "  Already visited" << std::endl;
            continue;
        }

        try {
            // Download webpage
            auto startDownload = std::chrono::steady_clock::now();
            std::string webpage = downloadWebpage(url);
            auto endDownload = std::chrono::steady_clock::now();
            performanceMetrics["download"] += std::chrono::duration_cast<std::chrono::milliseconds>(endDownload - startDownload);

            // Extract links from the webpage
            auto startExtract = std::chrono::steady_clock::now();
            std::string baseDomain = extractDomain(url);
            std::vector<std::string> links = extractLinks(webpage, baseDomain);
            auto endExtract = std::chrono::steady_clock::now();
            performanceMetrics["extract_links"] += std::chrono::duration_cast<std::chrono::milliseconds>(endExtract - startExtract);

            // Extract title and content summary
            std::string title = extractTitle(webpage);
            std::string contentSummary = extractContentSummary(webpage);

            {
                // Insert the URL into the visited set and increase the pagesVisited count
                visitedWebpages.insert(url);
                ++pagesVisited;
                websiteIndex.push_back({url, title, contentSummary});
            }

            // Add new links to the URL queue
            auto startQueue = std::chrono::steady_clock::now();
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                for (const std::string& link : links) {
                    if (link.empty() || link[0] == '#') {
                        continue;
                    }

                    std::string linkDomain = extractDomain(link);
                    if (!linkDomain.empty() && linkDomain != baseDomain) {
                        continue;
                    }
                    if (!visitedWebpages.contains(link) && !urlQueueContains(link)) {
                        urlQueue.push(link);
                        queueCondVar.notify_all(); // Notify all waiting threads
                    }
                }
            }
            auto endQueue = std::chrono::steady_clock::now();
            performanceMetrics["update_queue"] += std::chrono::duration_cast<std::chrono::milliseconds>(endQueue - startQueue);

        } catch (const std::exception& e) {
            std::cerr << "Error crawling webpage: " << url << " - " << e.what() << std::endl;
        }
    }
}

bool Crawler::urlQueueContains(const std::string& url) {
    std::queue<std::string> tempQueue = urlQueue;
    while (!tempQueue.empty()) {
        if (tempQueue.front() == url) {
            return true;
        }
        tempQueue.pop();
    }
    return false;
}

int Crawler::getPagesVisited() const {
    return pagesVisited;
}

std::string Crawler::extractDomain(const std::string& url) {
    std::regex domainRegex("https?://([^/]+)");
    std::smatch match;
    if (std::regex_search(url, match, domainRegex)) {
        return match[1].str();
    }
    return "";
}

void Crawler::saveWebsiteIndexToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << "[\n";
        for (size_t i = 0; i < websiteIndex.size(); ++i) {
            file << "  {\n";
            file << "    \"url\": \"" << websiteIndex[i].url << "\",\n";
            file << "    \"title\": \"" << websiteIndex[i].title << "\",\n";
            file << "    \"contentSummary\": \"" << websiteIndex[i].contentSummary << "\"\n";
            file << "  }";
            if (i < websiteIndex.size() - 1) {
                file << ",\n";
            } else {
                file << "\n";
            }
        }
        file << "]\n";
        file.close();
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}

void Crawler::performanceAnalysis() {
    std::cout << "Performance Analysis:" << std::endl;
    for (const auto& metric : performanceMetrics) {
        std::cout << "- " << metric.first << ": " << metric.second.count() << " ms" << std::endl;
    }
}
