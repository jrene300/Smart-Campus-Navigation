#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <cstddef>
#include <string>
#include <vector>
#include "Facility.h"

class HashTable
{
public:

    static constexpr std::size_t DEFAULT_CAPACITY = 11;

    explicit HashTable(std::size_t capacity = DEFAULT_CAPACITY);

    bool insert(const Facility& f);

    const Facility* find(const std::string& id) const;

    std::size_t hashIndex(const std::string& key) const;

    std::size_t capacity() const
    {
        return buckets_.size();
    }

    std::size_t size() const
    {
        return entries_;
    }

    std::size_t collisions() const
    {
        return collisions_;
    }

private:
    std::vector<std::vector<Facility> > buckets_;
    std::size_t entries_;
    std::size_t collisions_;
};
#endif
