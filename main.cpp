#include "crawler.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <thread>
#include <cmath>

double calculateSpeedup(int newPagesVisited, int oldPagesVisited, int newThreads, int oldThreads) {
    return static_cast<double>(newPagesVisited) / oldPagesVisited / (newThreads / static_cast<double>(oldThreads));
}

int main() {
    std::string startUrl;
    std::cout << "Enter the starting URL: ";
    std::getline(std::cin, startUrl);

    int initialThreads = std::thread::hardware_concurrency();
    int maxThreads = 4 * initialThreads;
    int step = initialThreads;
    int bestThreads = initialThreads;
    double bestPerformance = 0;

    int previousPagesVisited = 0;
    int previousThreads = initialThreads;

    for (int numThreads = initialThreads; numThreads <= maxThreads; numThreads += step) {
        std::cout << "Testing with " << numThreads << " threads..." << std::endl;
        Crawler crawler(startUrl, numThreads);

        auto startTime = std::chrono::steady_clock::now();

        try {
            crawler.run();
            crawler.saveWebsiteIndexToFile("website_index_" + std::to_string(numThreads) + ".json");
            crawler.performanceAnalysis();
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return 1;
        }

        auto endTime = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

        int pagesVisited = crawler.getPagesVisited();
        double performance = static_cast<double>(pagesVisited) / duration.count() * 1000; // Pages per millisecond

        std::cout << "Crawling completed with " << numThreads << " threads." << std::endl;
        std::cout << "Number of pages visited: " << pagesVisited << std::endl;
        std::cout << "Time elapsed: " << duration.count() << " milliseconds" << std::endl;
        std::cout << "Performance: " << performance << " pages/ms" << std::endl << std::endl;

        if (performance > bestPerformance) {
            bestPerformance = performance;
            bestThreads = numThreads;
        } else {
            double speedup = calculateSpeedup(pagesVisited, previousPagesVisited, numThreads, previousThreads);
            if (speedup < 1.1) { // Diminishing returns threshold
                break;
            }
        }

        previousPagesVisited = pagesVisited;
        previousThreads = numThreads;

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::cout << "Optimal number of threads: " << bestThreads << std::endl;
    std::cout << "Best performance: " << bestPerformance << " pages/s" << std::endl;

    return 0;
}
