#include "HashTable.h"
#include "Utilities.h"

using namespace std;

// makes an empty table with the given number of buckets (at least 1)
HashTable::HashTable(size_t capacity)
    : buckets_(capacity == 0 ? 1 : capacity), entries_(0), collisions_(0)
{
}

// turns a key into a bucket number: h = (h * 31 + char) mod capacity
size_t HashTable::hashIndex(const string& key) const
{
    unsigned long long h = 0;
    for (size_t i = 0; i < key.size(); ++i)
        h = (h * 31ULL + static_cast<unsigned char>(key[i])) % buckets_.size();
    return static_cast<size_t>(h);
}

// adds a facility, returns false if the ID is already there
bool HashTable::insert(const Facility& f)
{
    Facility copy = f;
    copy.id = utilities::toUpper(utilities::trim(f.id));
    vector<Facility>& chain = buckets_[hashIndex(copy.id)];
    for (size_t i = 0; i < chain.size(); ++i)
        if (chain[i].id == copy.id) return false;  // duplicate, no collision counted
    if (!chain.empty()) ++collisions_;              // bucket already had something
    chain.push_back(copy);
    ++entries_;
    return true;
}

// looks only in the one bucket the key goes to
const Facility* HashTable::find(const string& id) const
{
    string key = utilities::toUpper(utilities::trim(id));
    const vector<Facility>& chain = buckets_[hashIndex(key)];
    for (size_t i = 0; i < chain.size(); ++i)
        if (chain[i].id == key) return &chain[i];
    return nullptr;
}
