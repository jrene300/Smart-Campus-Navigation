#ifndef UTILITIES_H
#define UTILITIES_H
#include <string>
#include <vector>

namespace utilities {
    std::string trim(const std::string& s);

    std::string toUpper(const std::string& s);

    std::string toLower(const std::string& s);

    bool equalsIgnoreCase(const std::string& a, const std::string& b);

    std::vector<std::string> splitFields(const std::string& line, char delim = '|');

    bool promptLine(const std::string& prompt, std::string& out);
}

#endif
