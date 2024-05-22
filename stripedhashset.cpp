#include "stripedhashset.h"

StripedHashSet::StripedHashSet(int numStripes) : stripes(numStripes), locks(numStripes) {}

bool StripedHashSet::contains(const std::string& webpage) {
    size_t stripeIndex = std::hash<std::string>{}(webpage) % stripes.size();
    std::shared_lock<std::shared_mutex> lock(locks[stripeIndex]);
    return stripes[stripeIndex].find(webpage) != stripes[stripeIndex].end();
}

void StripedHashSet::insert(const std::string& webpage) {
    size_t stripeIndex = std::hash<std::string>{}(webpage) % stripes.size();
    std::unique_lock<std::shared_mutex> lock(locks[stripeIndex]);
    stripes[stripeIndex].insert(webpage);
}