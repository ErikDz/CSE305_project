// setlist.cpp
#include "setlist.h"
#include <string>
#include <unordered_set>

bool SetList::contains(const std::string& webpage) {
    return visitedWebpages.find(webpage) != visitedWebpages.end();
}

void SetList::insert(const std::string& webpage) {
    visitedWebpages.insert(webpage);
}
