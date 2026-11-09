#ifndef CAMPUSGRAPH_H
#define CAMPUSGRAPH_H
#include <string>
#include <vector>

class CampusGraph {
public:
    enum ConnResult { CONN_OK, CONN_SELF, CONN_DUPLICATE };

    int findBuilding(const std::string& name) const;

    bool addBuilding(const std::string& name);

    ConnResult addConnection(int a, int b);

    std::size_t buildingCount() const
    {
        return names_.size();
    }
    std::size_t connectionCount() const
    {
        return edgeCount_;
    }
    const std::string& name(int i) const
    {
        return names_[i];
    }

    void display() const;
    void displayCountsAndDegrees() const;
    void bfsReachable(const std::string& start) const;
    void shortestPath(const std::string& start, const std::string& dest) const;
    void connectivityReport() const;

private:
    std::vector<std::string> names_;
    std::vector<std::vector<int> > adj_;
    std::size_t edgeCount_ = 0;
};
#endif
