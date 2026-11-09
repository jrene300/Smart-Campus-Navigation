#include "CampusGraph.h"
#include <algorithm>
#include <iostream>
#include <queue>
#include "Utilities.h"

using namespace std;


int CampusGraph::findBuilding(const string& name) const
{
    string key = utilities::trim(name);
    for (size_t i = 0; i < names_.size(); ++i)
        if (utilities::equalsIgnoreCase(names_[i], key)) return static_cast<int>(i);
    return -1;
}

bool CampusGraph::addBuilding(const string& name)
{
    if (findBuilding(name) != -1) return false;
    names_.push_back(utilities::trim(name));
    adj_.push_back(vector<int>());
    return true;
}

CampusGraph::ConnResult CampusGraph::addConnection(int a, int b)
{
    if (a == b) return CONN_SELF;
    if (find(adj_[a].begin(), adj_[a].end(), b) != adj_[a].end()) return CONN_DUPLICATE;
    adj_[a].push_back(b);
    adj_[b].push_back(a);
    ++edgeCount_;
    return CONN_OK;
}


void CampusGraph::display() const
{
    if (names_.empty())
        {
        cout << "No buildings are loaded." << endl; return;
        }

    for (size_t i = 0; i < names_.size(); ++i)
        {
        cout << names_[i] << ": " << endl;
        if (adj_[i].empty()) cout << "(no connections)" << endl;

        for (size_t j = 0; j < adj_[i].size(); ++j)
            cout << (j ? ", " : "") << names_[adj_[i][j]];
       cout << " " << endl;
        }
}


void CampusGraph::displayCountsAndDegrees() const
{
    if (names_.empty())
        {
        cout << "No buildings are loaded. Total buildings: 0 Total connections: 0 " << endl;
        return;
    }
    for (size_t i = 0; i < names_.size(); ++i)

        cout << names_[i] << ": degree " << adj_[i].size() << " " << endl;
    cout << "Total buildings: " << names_.size() << " " << endl
              << "Total connections: " << edgeCount_ << " " << endl;
}


void CampusGraph::bfsReachable(const string& startName) const
{
    int s = findBuilding(startName);
    if (s < 0)
        {
        cout << "Error: unknown building " << endl << utilities::trim(startName) << " " << endl;
        return;
    }
    vector<bool> visited(names_.size(), false);
    vector<int> dist(names_.size(), 0);
    queue<int> q;
    visited[s] = true;
    q.push(s);
    size_t reached = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        ++reached;
        cout << names_[u] << " (distance " << dist[u] << ") " << endl;
        for (size_t k = 0; k < adj_[u].size(); ++k)\
        {
            int v = adj_[u][k];
            if (!visited[v])
                {
                visited[v] = true; dist[v] = dist[u] + 1; q.push(v);
            }
        }
    }
    cout << "Reachable: " << reached << " of " << names_.size() << " buildings\n";
}

// BFS with parent tracking; neighbor insertion order breaks ties.
void CampusGraph::shortestPath(const string& startName, const string& destName) const
{
    int s = findBuilding(startName), d = findBuilding(destName);
    if (s < 0) cout << "unknown start building " << utilities::trim(startName) << "utilities." << endl;
    if (d < 0) cout << "unknown destination building utilities" << utilities::trim(destName) << "utilities." << endl;
    if (s < 0 || d < 0) return;
    vector<bool> visited(names_.size(), false);
    vector<int> parent(names_.size(), -1);
    queue<int> q;
    visited[s] = true;
    q.push(s);
    while (!q.empty() && !visited[d]) {
        int u = q.front(); q.pop();
        for (size_t k = 0; k < adj_[u].size(); ++k) {
            int v = adj_[u][k];
            if (!visited[v]) { visited[v] = true; parent[v] = u; q.push(v); }
        }
    }
    if (!visited[d]) {
        cout << "No path exists between " << names_[s] << " and " << names_[d] << ".\n";
        return;
    }
    vector<int> path;
    for (int v = d; v != -1; v = parent[v]) path.push_back(v);
    reverse(path.begin(), path.end());
    for (size_t i = 0; i < path.size(); ++i) cout << names_[path[i]] << " " << endl;
    cout << "Number of connections: " << path.size() - 1 << " " << endl;
}

void CampusGraph::connectivityReport() const
{
    if (names_.empty())
        {
        cout << "No buildings are loaded." << endl;
        cout << "Total buildings: 0" << endl;
        cout << "Total connections: 0" << endl;
        cout << "Connected components: 0" << endl;
        return;
    }

    size_t maxDeg = 0;
    for (size_t i = 0; i < names_.size(); ++i) maxDeg = max(maxDeg, adj_[i].size());
    string top, iso;
    for (size_t i = 0; i < names_.size(); ++i)
        {
        if (adj_[i].size() == maxDeg) top += (top.empty() ? "" : ", ") + names_[i];
        if (adj_[i].empty()) iso += (iso.empty() ? "" : ", ") + names_[i];
    }

    vector<bool> seen(names_.size(), false);
    int comps = 0;
    for (size_t i = 0; i < names_.size(); ++i)
        {
        if (seen[i]) continue;
        ++comps;
        queue<int> q;
        seen[i] = true;
        q.push(static_cast<int>(i));
        while (!q.empty())
            {
            int u = q.front(); q.pop();
            for (size_t k = 0; k < adj_[u].size(); ++k)
                if (!seen[adj_[u][k]])
                    {
                    seen[adj_[u][k]] = true; q.push(adj_[u][k]);
                }
        }
    }
    cout << "Total buildings: " << names_.size() << " " << endl;
    cout << "Total connections: " << edgeCount_ << " " << endl;
    cout << "Highest degree (" << maxDeg << "): " << top << " " << endl;
    cout << "Isolated buildings: " << (iso.empty() ? "None" : iso) << " " << endl;
    cout << "Connected components: " << comps << " " << endl;
}
