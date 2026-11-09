#include <iostream>
#include <string>
#include "CampusGraph.h"
#include "DataLoader.h"
#include "DepartmentTree.h"
#include "FacilityStore.h"
#include "Utilities.h"

using namespace std;

namespace {

enum Action { A_EXIT, A_VIEW_FAC, A_ID, A_NAME, A_CAT, A_GRAPH, A_COUNTS, A_BFS, A_PATH,
              A_ADD_DEPT, A_SEARCH_DEPT, A_HIER, A_PRE, A_POST, A_LEVEL, A_STATS,
              A_CAT_REPORT, A_CONN_REPORT };

struct Item { const char* label; Action action; };

#ifdef MILESTONE1
const Item MENU[] = {
    {"View all facilities", A_VIEW_FAC},
    {"Search facility by ID", A_ID},
    {"View campus graph", A_GRAPH},
    {"Display graph counts and degrees", A_COUNTS},
    {"Find reachable buildings (BFS)", A_BFS},
    {"Add department", A_ADD_DEPT},
    {"Search department", A_SEARCH_DEPT},
    {"Display department hierarchy", A_HIER},
    {"Run preorder traversal", A_PRE},
    {"Exit", A_EXIT}};
#else
const Item MENU[] = {
    {"View all facilities", A_VIEW_FAC},
    {"Search facility by ID", A_ID},
    {"Search facility by name", A_NAME},
    {"Search facility by category", A_CAT},
    {"View campus graph", A_GRAPH},
    {"Display graph counts and degrees", A_COUNTS},
    {"Find reachable buildings (BFS)", A_BFS},
    {"Find shortest path (BFS)", A_PATH},
    {"Add department", A_ADD_DEPT},
    {"Search department", A_SEARCH_DEPT},
    {"Display department hierarchy", A_HIER},
    {"Preorder traversal", A_PRE},
    {"Postorder traversal", A_POST},
    {"Level-order traversal", A_LEVEL},
    {"Department statistics", A_STATS},
    {"Facility category report", A_CAT_REPORT},
    {"Graph connectivity report", A_CONN_REPORT},
    {"Exit", A_EXIT}};
#endif
const int MENU_SIZE = static_cast<int>(sizeof(MENU) / sizeof(MENU[0]));

// prints the numbered menu
void showMenu()
{
    cout << endl << "  Campus Menu " << endl;
    for (int i = 0; i < MENU_SIZE; ++i) cout << (i + 1) << ". " << MENU[i].label << endl;
}

// turns the typed text into a number, -1 if it is not a whole number
int parseChoice(const string& s)
{
    if (s.empty() || s.size() > 6) return -1;
    for (size_t i = 0; i < s.size(); ++i)
        if (s[i] < '0' || s[i] > '9') return -1;
    return stoi(s);
}

// asks for a name, a blank name gives an error
bool askName(const string& prompt, string& out, bool& eof)
{
    if (!utilities::promptLine(prompt, out))
        {
        eof = true; return false;
    }
    if (out.empty())
        {
        cout << "Error: name is empty." << endl; return false;
    }
    return true;
}

// asks for a department name and parent, then adds it (memory only)
void addDepartment(DepartmentTree& tree, bool& eof)
{
    string name, parent;
    if (!askName("Department name: ", name, eof)) return;
    if (tree.find(name))
        {
        cout << "Error!!! " << name << "  exists." << endl; return;
    }
    if (!tree.empty() && !askName("Parent department: ", parent, eof)) return;
    DepartmentTree::AddResult r = tree.add(name, parent);
    if (r == DepartmentTree::ADD_UNKNOWN_PARENT)
        cout << "Error!!! " << parent << " is unknown." << endl;

    else if (r == DepartmentTree::ADD_DUPLICATE)

        cout << "Error!!! duplicated department." << endl;
    else cout << (parent.empty() ? "Added root dep " : "Added dep ") << name << " ." << endl;
}

}  // namespace

int main(int argc, char* argv[])
{
    string dir = (argc > 1) ? argv[1] : "data";
    CampusGraph graph;
    FacilityStore facilities;
    DepartmentTree tree;
    loadAllData(dir, graph, facilities, tree);

    bool eof = false;
    string line, a, b;
    while (!eof)
        {
        showMenu();
        if (!utilities::promptLine("Choice: ", line)) break;
        int c = parseChoice(line);
        if (c == 0) break;
        if (c < 1 || c > MENU_SIZE)
            {
            cout << "Enter number from 1 to " << MENU_SIZE << "." << endl;
            continue;
        }
        switch (MENU[c - 1].action)
            {
            case A_EXIT: cout << "Goodbye!!!" << endl; return 0;
            case A_VIEW_FAC: facilities.displayAll(); break;
            case A_ID:
                if (askName("Facility ID: ", a, eof)) facilities.searchById(a);
                break;
            case A_NAME:
                if (askName("Facility name: ", a, eof)) facilities.searchByName(a);
                break;
            case A_CAT:
                if (askName("Category: ", a, eof)) facilities.searchByCategory(a);
                break;
            case A_GRAPH: graph.display(); break;
            case A_COUNTS: graph.displayCountsAndDegrees(); break;
            case A_BFS:
                if (askName("Start building: ", a, eof)) graph.bfsReachable(a);
                break;
            case A_PATH:
                if (askName("Start building: ", a, eof) && askName("Destination building: ", b, eof))
                    graph.shortestPath(a, b);
                break;
            case A_ADD_DEPT: addDepartment(tree, eof); break;
            case A_SEARCH_DEPT:
                if (askName("Department name: ", a, eof)) tree.search(a);
                break;
            case A_HIER: tree.displayHierarchy(); break;
            case A_PRE: tree.preorder(); break;
            case A_POST: tree.postorder(); break;
            case A_LEVEL: tree.levelOrder(); break;
            case A_STATS: tree.statistics(); break;
            case A_CAT_REPORT: facilities.categoryReport(); break;
            case A_CONN_REPORT: graph.connectivityReport(); break;
        }
    }
    return 0;
}
