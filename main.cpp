#include "crawler.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <thread>

int main() {
    std::string startUrl;
    std::cout << "Enter the starting URL: ";
    std::getline(std::cin, startUrl);

    int initialThreads = std::thread::hardware_concurrency();
    int maxThreads = 4 * initialThreads;  // Adjust based on system specs and testing
    int step = initialThreads;

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
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime);

        std::cout << "Crawling completed with " << numThreads << " threads." << std::endl;
        std::cout << "Number of pages visited: " << crawler.getPagesVisited() << std::endl;
        std::cout << "Time elapsed: " << duration.count() << " seconds" << std::endl << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
