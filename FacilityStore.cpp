#include "FacilityStore.h"
#include <iomanip>
#include <iostream>
#include <map>
#include "Utilities.h"

using namespace std;

// adds a facility, false if the ID is already there
bool FacilityStore::add(const Facility& f)
{
    if (!table_.insert(f))
    {
        return false;
    }
    Facility c = f;
    c.id = utilities::toUpper(utilities::trim(f.id));
    list_.push_back(c);
    return true;
}

// prints one facility on one line
void FacilityStore::printFacility(const Facility& f)
{
    cout << f.id << " | " << f.name << " | " << f.building << " | "
         << f.category << endl;
}

// prints all facilities and the total
void FacilityStore::displayAll() const
{
    if (list_.empty())
    {
        cout << "No facilities are loaded." << endl;
        return;
    }
    for (size_t i = 0; i < list_.size(); ++i)
    {
        printFacility(list_[i]);
    }
    cout << "Total facilities: " << list_.size() << endl;
}

// looks up an ID in the hash table
void FacilityStore::searchById(const string& rawId) const
{
    string id = utilities::toUpper(utilities::trim(rawId));
    if (!isValidFacilityId(id))
    {
        cout << "Invalid format: an ID is F followed by exactly three digits." << endl;
        return;
    }
    const Facility* f = table_.find(id);
    if (!f)
    {
        cout << "Not found: no facility with ID " << id << "." << endl;
        return;
    }
    cout << "Found: ";
    printFacility(*f);
}

// finds facilities with the same name (ignores case)
void FacilityStore::searchByName(const string& name) const
{
    string key = utilities::trim(name);
    int n = 0;
    for (size_t i = 0; i < list_.size(); ++i)
    {
        if (utilities::equalsIgnoreCase(list_[i].name, key))
        {
            printFacility(list_[i]);
            ++n;
        }
    }
    if (n == 0)
    {
        cout << "No facilities match that name." << endl;
    }
    cout << "Matches: " << n << endl;
}

// finds facilities with the same category (ignores case)
void FacilityStore::searchByCategory(const string& cat) const
{
    string key = utilities::trim(cat);
    int n = 0;
    for (size_t i = 0; i < list_.size(); ++i)
    {
        if (utilities::equalsIgnoreCase(list_[i].category, key))
        {
            printFacility(list_[i]);
            ++n;
        }
    }
    if (n == 0)
    {
        cout << "No facilities match that category." << endl;
    }
    cout << "Matches: " << n << endl;
}

// counts each category and shows the percent
void FacilityStore::categoryReport() const
{
    if (list_.empty())
    {
        cout << "No facilities are available." << endl;
        return;
    }

    // count how many facilities are in each category
    map<string, int> counts;
    for (size_t i = 0; i < list_.size(); ++i)
    {
        counts[utilities::toUpper(list_[i].category)]++;
    }

    // print each category with its percent
    cout << fixed << setprecision(1);
    for (map<string, int>::const_iterator it = counts.begin(); it != counts.end(); ++it)
    {
        double percent = 100.0 * it->second / list_.size();
        cout << it->first << ": " << it->second << " (" << percent << "%)" << endl;
    }
}