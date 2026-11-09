#include "Utilities.h"
#include <iostream>

using namespace std;

namespace utilities
{

    // remove spaces from both ends
    string trim(const string& s) {
        const char* ws = " \t\r\n\v\f";
        size_t b = s.find_first_not_of(ws);
        if (b == string::npos) return "";
        size_t e = s.find_last_not_of(ws);
        return s.substr(b, e - b + 1);
    }

    // make letters uppercase
    string toUpper(const string& s) {
        string r = s;
        for (size_t i = 0; i < r.size(); ++i) {
            if (r[i] >= 'a' && r[i] <= 'z') {
                r[i] = static_cast<char>(r[i] - 'a' + 'A');
            }
        }
        return r;
    }

    // make letters lowercase
    string toLower(const string& s) {
        string r = s;
        for (size_t i = 0; i < r.size(); ++i) {
            if (r[i] >= 'A' && r[i] <= 'Z') {
                r[i] = static_cast<char>(r[i] - 'A' + 'a');
            }
        }
        return r;
    }

    // check if two strings match, ignoring case
    bool equalsIgnoreCase(const string& a, const string& b) {
        return toLower(a) == toLower(b);
    }

    // split a line by the delimiter (keeps empty fields)
    vector<string> splitFields(const string& line, char delim)
    {
        vector<string> out;
        string cur;
        for (size_t i = 0; i < line.size(); ++i)
            {
            if (line[i] == delim)
                {
                out.push_back(cur);
                cur.clear();
            }
            else
                {
                cur += line[i];
            }
        }
        out.push_back(cur);
        return out;
    }

    // ask the user and read a whole line, false if EOF
    bool promptLine(const string& prompt, string& out)
    {
        cout << prompt << endl;
        string line;
        if (!getline(cin, line)) return false;
        out = trim(line);
        return true;
    }

}
