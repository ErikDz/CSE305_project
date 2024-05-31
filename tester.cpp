#include "webpage_downloader.h"
#include "link_extractor.h"
#include "setlist.h"
#include "crawler.h"
#include <iostream>
#include <cassert>


//RECORDATORIO: HACER TESTS DE TODAS LAS NUEVA FUNCIONES
void testWebpageDownloader() {
    std::string url = "https://en.wikipedia.org/wiki/Web_crawler";
    std::string webpage = downloadWebpage(url);
    assert(!webpage.empty());
    std::cout << "Webpage Downloader: Passed" << std::endl;
}

void testSetList() {
    SetList setlist;
    std::string url1 = "https://www.example.com";
    std::string url2 = "https://www.example.org";
    setlist.insert(url1);
    assert(setlist.contains(url1));
    assert(!setlist.contains(url2));
    std::cout << "SetList: Passed" << std::endl;
}

void testCrawler() {
    //std::string startUrl = "https://en.wikipedia.org/wiki/Web_crawler";
    std::string startUrl = "https://blog.adacta.ai/";
    int numThreads = 2;
    Crawler crawler(startUrl, numThreads);
    crawler.run();
    int pagesVisited = crawler.getPagesVisited();
    assert(pagesVisited > 0);
    std::cout << "Crawler: Passed" << std::endl;
}

int main() {
    testWebpageDownloader();
    testSetList();
    testCrawler();

    std::cout << "All tests passed!" << std::endl;

    return 0;
}
