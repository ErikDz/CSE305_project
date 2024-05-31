#ifndef CRAWLER_H
#define CRAWLER_H

#include <string>
#include <queue>
#include <thread>
#include <mutex>
#include "setlist.h"
#include "stripedhashset.h"
#include <chrono>
#include <map>

class Crawler {
public:
    Crawler(const std::string& startUrl, int numThreads);
    void run();
    int getPagesVisited() const;
    struct WebpageData {
        std::string url;
        std::string title;
        std::string contentSummary;
    };
    void saveWebsiteIndexToFile(const std::string& filename);
    void performanceAnalysis();
    void setThreadCount(int count);  // New method to set the number of threads

private:
    void crawlLoop();
    std::string getAbsoluteUrl(const std::string& baseUrl, const std::string& relativeUrl);
    std::string extractDomain(const std::string& url);
    bool urlQueueContains(const std::string& url);

    std::string startUrl;
    int numThreads;
    std::queue<std::string> urlQueue;
    std::mutex queueMutex;
    std::condition_variable queueCondVar;
    std::mutex visitedMutex;
    int pagesVisited;
    std::vector<WebpageData> websiteIndex;
    StripedHashSet visitedWebpages;

    // Performance metrics
    std::map<std::string, std::chrono::milliseconds> performanceMetrics;
};

#endif
