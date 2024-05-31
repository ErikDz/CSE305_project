#ifndef STRIPEDHASHSET_H
#define STRIPEDHASHSET_H

#include <string>
#include <vector>
#include <unordered_set>
#include <shared_mutex>

class StripedHashSet {
public:
    StripedHashSet(int numStripes);
    bool contains(const std::string& webpage);
    void insert(const std::string& webpage);

private:
    std::vector<std::unordered_set<std::string>> stripes;
    std::vector<std::shared_mutex> locks;
};

#endif