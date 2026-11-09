#include "DataLoader.h"
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include "Utilities.h"

using namespace std;

namespace {

// the character that separates fields in the files
const char DELIM = ',';

// keeps count of good and bad lines
struct Stats
{
    int accepted;
    int rejected;
};

// function that checks one line's fields
typedef function<bool(const vector<string>&, string&)> Handler;

// reads a file line by line and sends each line to the handler
Stats processFile(const string& dir, const string& fname,
                  size_t expected, const Handler& handle)
{
    Stats st = {0, 0};

    // build the full path
    string path = dir;
    if (!path.empty() && path[path.size() - 1] != '/')
    {
        path += '/';
    }
    path += fname;

    ifstream in(path.c_str());
    if (!in)
    {
        cout << "Error: cannot open file " << fname << " (" << path
             << "); treating it as empty." << endl;
        return st;
    }

    string line;
    int lineNo = 0;
    while (getline(in, line))
    {
        ++lineNo;
        string t = utilities::trim(line);

        // skip blank lines and comments
        if (t.empty() || t[0] == '#')
        {
            continue;
        }

        vector<string> f = utilities::splitFields(t, DELIM);
        string reason;
        bool ok = true;

        // check the number of fields
        if (f.size() != expected)
        {
            reason = "wrong number of fields (expected " + to_string(expected) +
                     ", found " + to_string(f.size()) + ")";
            ok = false;
        }
        else
        {
            // trim every field
            for (size_t i = 0; i < f.size(); ++i)
            {
                f[i] = utilities::trim(f[i]);
            }

            // no field can be empty
            for (size_t i = 0; i < f.size() && ok; ++i)
            {
                if (f[i].empty())
                {
                    reason = "empty required field";
                    ok = false;
                }
            }

            // let the handler do its own checks
            if (ok)
            {
                ok = handle(f, reason);
            }
        }

        if (ok)
        {
            ++st.accepted;
        }
        else
        {
            ++st.rejected;
            cout << "Warning: " << fname << " line " << lineNo << ": " << reason << endl;
        }
    }
    return st;
}

}

// loads all four data files
void loadAllData(const string& dir, CampusGraph& graph,
                 FacilityStore& facilities, DepartmentTree& tree)
{
    // buildings
    Stats s = processFile(dir, "buildings.txt", 1,
        [&](const vector<string>& f, string& why)
        {
            if (!graph.addBuilding(f[0]))
            {
                why = "duplicate building \"" + f[0] + "\"";
                return false;
            }
            return true;
        });
    cout << "buildings.txt: " << s.accepted << " accepted, " << s.rejected << " rejected" << endl;

    // connections between buildings
    s = processFile(dir, "connections.txt", 2,
        [&](const vector<string>& f, string& why)
        {
            int a = graph.findBuilding(f[0]);
            int b = graph.findBuilding(f[1]);
            if (a < 0)
            {
                why = "unknown building \"" + f[0] + "\"";
                return false;
            }
            if (b < 0)
            {
                why = "unknown building \"" + f[1] + "\"";
                return false;
            }
            CampusGraph::ConnResult r = graph.addConnection(a, b);
            if (r == CampusGraph::CONN_SELF)
            {
                why = "self-connection";
                return false;
            }
            if (r == CampusGraph::CONN_DUPLICATE)
            {
                why = "duplicate connection";
                return false;
            }
            return true;
        });
    cout << "connections.txt: " << s.accepted << " accepted, " << s.rejected << " rejected" << endl;

    // facilities
    s = processFile(dir, "facilities.txt", 4,
        [&](const vector<string>& f, string& why)
        {
            Facility fac;
            fac.id = utilities::toUpper(f[0]);

            // ID must be F + 3 digits
            if (!isValidFacilityId(fac.id))
            {
                why = "invalid ID \"" + f[0] + "\" (must be F plus exactly three digits)";
                return false;
            }

            // building has to exist
            int b = graph.findBuilding(f[2]);
            if (b < 0)
            {
                why = "unknown building \"" + f[2] + "\"";
                return false;
            }

            fac.name = f[1];
            fac.building = graph.name(b);
            fac.category = f[3];

            if (!facilities.add(fac))
            {
                why = "duplicate facility ID " + fac.id;
                return false;
            }
            return true;
        });
    const HashTable& h = facilities.table();
    cout << "facilities.txt: " << s.accepted << " accepted, " << s.rejected
         << " rejected (hash capacity " << h.capacity() << ", entries " << h.size()
         << ", collisions " << h.collisions() << ")" << endl;

    // departments
    s = processFile(dir, "departments.txt", 2,
        [&](const vector<string>& f, string& why)
        {
            bool none = utilities::equalsIgnoreCase(f[1], "NONE");

            // the first one is the root
            if (tree.empty())
            {
                if (!none)
                {
                    why = "first department (root) must use NONE as parent";
                    return false;
                }
                tree.add(f[0], "");
                return true;
            }

            if (tree.find(f[0]))
            {
                why = "duplicate department \"" + f[0] + "\"";
                return false;
            }

            // parent has to already be in the tree
            if (none || !tree.find(f[1]))
            {
                why = "unknown parent \"" + f[1] + "\"";
                return false;
            }

            tree.add(f[0], f[1]);
            return true;
        });
    cout << "departments.txt: " << s.accepted << " accepted, " << s.rejected << " rejected" << endl;
}
