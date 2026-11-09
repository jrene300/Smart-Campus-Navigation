#ifndef FACILITY_H
#define FACILITY_H
#include <string>

using namespace std;

struct Facility
{
    string id;
    string name;
    string building;
    string category;
};

inline bool isValidFacilityId(const string& id)
{
    if (id.size() != 4 || id[0] != 'F') return false;
    for (int i = 1; i < 4; ++i)
        if (id[i] < '0' || id[i] > '9') return false;
    return true;
}
#endif
