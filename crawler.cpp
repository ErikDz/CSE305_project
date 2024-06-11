// crawler.cpp
#include "crawler.h"
#include "webpage_downloader.h"
#include "link_extractor.h"
#include <iostream>
#include <regex>
#include "webpage_parser.h"
#include <fstream>
#include <chrono>

Crawler::Crawler(const std::string& startUrl, int numThreads)
    : startUrl(startUrl), numThreads(numThreads), pagesVisited(0), visitedWebpages(100), done(false), activeThreads(0) {
}

void Crawler::setThreadCount(int count) {
    numThreads = count;
}

void Crawler::run() {
    std::vector<std::thread> threads;
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(&Crawler::crawlLoop, this);
    }

    {
        std::unique_lock<std::mutex> lock(queueMutex);
        urlQueue.push(startUrl);
        queueCondVar.notify_one();
    }

    // Continuously check if crawling should stop
    while (true) {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            if (urlQueue.empty() && activeThreads == 0) {
                done = true;
                queueCondVar.notify_all();
                break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
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
            queueCondVar.wait(lock, [this] { return !urlQueue.empty() || done; });

            if (done) {
                break;
            }

            url = urlQueue.front();
            urlQueue.pop();
        }

        activeThreads++;

        url = normalizeLink(url, extractDomain(startUrl));

        //std::cout << "Thread ID: " << std::this_thread::get_id() << std::endl;
        //std::cout << std::this_thread::get_id() << " Crawling: " << url << std::endl;

        // Check if the URL has already been visited
        if (visitedWebpages.contains(url)) {
            //std::cout << "  Already visited" << std::endl;
            activeThreads--;
            continue;
        }

        try {
            auto startDownload = std::chrono::steady_clock::now();
            std::string webpage = downloadWebpage(url);
            auto endDownload = std::chrono::steady_clock::now();
            performanceMetrics["download"] += std::chrono::duration_cast<std::chrono::milliseconds>(endDownload - startDownload);

            auto startExtract = std::chrono::steady_clock::now();
            std::string baseDomain = extractDomain(url);
            std::vector<std::string> links = extractLinks(webpage, baseDomain);
            auto endExtract = std::chrono::steady_clock::now();
            performanceMetrics["extract_links"] += std::chrono::duration_cast<std::chrono::milliseconds>(endExtract - startExtract);

            std::string title = extractTitle(webpage);
            std::string contentSummary = extractContentSummary(webpage);

            {
                std::unique_lock<std::mutex> lock(visitedMutex);
                visitedWebpages.insert(url);
                ++pagesVisited;
                websiteIndex.push_back({url, title, contentSummary});
            }

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

            // Check if the link has already been visited or added to the queue
            if (!visitedWebpages.contains(link) && urlVisitedSet.insert(link).second) {
                urlQueue.push(link);
                queueCondVar.notify_one();
            }
        }
    }
            auto endQueue = std::chrono::steady_clock::now();
            performanceMetrics["update_queue"] += std::chrono::duration_cast<std::chrono::milliseconds>(endQueue - startQueue);

        } catch (const std::exception& e) {
            std::cerr << "Error crawling webpage: " << url << " - " << e.what() << std::endl;
        }

        activeThreads--;
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