// setlist.h
#ifndef SETLIST_H
#define SETLIST_H

#include <string>
#include <unordered_set>

class SetList {
public:
    bool contains(const std::string& webpage);
    void insert(const std::string& webpage);

private:
    std::unordered_set<std::string> visitedWebpages;
};

#endif
