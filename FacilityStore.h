#ifndef FACILITYSTORE_H
#define FACILITYSTORE_H
#include <string>
#include <vector>
#include "Facility.h"
#include "HashTable.h"

class FacilityStore
{
public:

    bool add(const Facility& f);
    bool contains(const string& id) const
    {
        return table_.find(id) != nullptr;
    }
    const HashTable& table() const
    {
        return table_;
    }
    std::size_t size() const
    {
        return list_.size();
    }

    void displayAll() const;
    void searchById(const string& rawId) const;
    void searchByName(const string& name) const;
    void searchByCategory(const string& cat) const;
    void categoryReport() const;

private:
    static void printFacility(const Facility& f);
    vector<Facility> list_;
    HashTable table_;
};
#endif
