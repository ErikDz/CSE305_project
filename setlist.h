#ifndef SETLIST_H
#define SETLIST_H

#include <string>
#include <unordered_set>
#include <shared_mutex>

class SetList {
public:
    bool contains(const std::string& webpage);
    void insert(const std::string& webpage);

private:
    std::unordered_set<std::string> visitedWebpages;
    std::shared_mutex setMutex;
};

#endif
