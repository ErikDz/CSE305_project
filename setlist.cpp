#include "setlist.h"

bool SetList::contains(const std::string& webpage) {
    std::shared_lock lock(setMutex);
    return visitedWebpages.find(webpage) != visitedWebpages.end();
}

void SetList::insert(const std::string& webpage) {
    std::unique_lock lock(setMutex);
    visitedWebpages.insert(webpage);
}
