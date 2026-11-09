#include "DepartmentTree.h"
#include <iostream>
#include <queue>
#include "Utilities.h"

using namespace std;


DepartmentTree::~DepartmentTree()
{ destroy(root_); }


void DepartmentTree::destroy(DepartmentNode* n)
{
    if (!n) return;
    for (size_t i = 0; i < n->children.size(); ++i) destroy(n->children[i]);
    delete n;
}

DepartmentNode* DepartmentTree::find(const string& name) const
{
    if (!root_) return nullptr;
    string key = utilities::trim(name);
    queue<DepartmentNode*> q;
    q.push(root_);
    while (!q.empty())
        {
        DepartmentNode* n = q.front(); q.pop();
        if (utilities::equalsIgnoreCase(n->name, key)) return n;
        for (size_t i = 0; i < n->children.size(); ++i) q.push(n->children[i]);
    }
    return nullptr;
}


DepartmentTree::AddResult DepartmentTree::add(const string& name, const string& parentName) {
    string nm = utilities::trim(name);
    if (find(nm)) return ADD_DUPLICATE;
    if (!root_) { root_ = new DepartmentNode(nm); ++count_; return ADD_OK; }
    DepartmentNode* p = find(parentName);
    if (!p) return ADD_UNKNOWN_PARENT;
    p->children.push_back(new DepartmentNode(nm, p));
    ++count_;
    return ADD_OK;
}


void DepartmentTree::search(const string& name) const
{
    DepartmentNode* n = find(name);
    if (!n)
        {
        cout << "Department not found: " << utilities::trim(name) << endl; return;
    }
    int depth = 0;
    for (DepartmentNode* p = n->parent; p; p = p->parent) ++depth;
    cout << "Name: " << n->name << endl
              << "Parent: " << (n->parent ? n->parent->name : "None") << endl
              << "Direct children: " << n->children.size() << endl
              << "Depth: " << depth << endl;
}


void DepartmentTree::printHierarchy(const DepartmentNode* n, int depth)
{
    cout << string(depth * 4, ' ') << n->name << endl;
    for (size_t i = 0; i < n->children.size(); ++i) printHierarchy(n->children[i], depth + 1);
}

void DepartmentTree::displayHierarchy() const
{
    if (!root_)
        {
        cout << "No departments are loaded." << endl; return;
    }
    printHierarchy(root_, 0);
}

void DepartmentTree::collectPre(const DepartmentNode* n, vector<string>& out)
{
    out.push_back(n->name);
    for (size_t i = 0; i < n->children.size(); ++i) collectPre(n->children[i], out);
}

void DepartmentTree::collectPost(const DepartmentNode* n, vector<string>& out) {
    for (size_t i = 0; i < n->children.size(); ++i) collectPost(n->children[i], out);
    out.push_back(n->name);
}


void DepartmentTree::printSequence(const vector<string>& seq)
{
    for (size_t i = 0; i < seq.size(); ++i) cout << (i ? " -> " : "") << seq[i];
    cout << endl;
}

void DepartmentTree::preorder() const
{
    if (!root_)
        {
        cout << "No departments are loaded." << endl; return;
    }
    vector<string> seq;
    collectPre(root_, seq);
    printSequence(seq);
}

void DepartmentTree::postorder() const
{
    if (!root_)
        {
        cout << "No departments are loaded." << endl; return;
    }
    vector<string> seq;
    collectPost(root_, seq);
    printSequence(seq);
}


void DepartmentTree::levelOrder() const
{
    if (!root_)
        {
        cout << "No departments are loaded." << endl; return;
    }
    vector<string> seq;
    queue<const DepartmentNode*> q;
    q.push(root_);
    while (!q.empty())
        {
        const DepartmentNode* n = q.front(); q.pop();
        seq.push_back(n->name);
        for (size_t i = 0; i < n->children.size(); ++i) q.push(n->children[i]);
    }
    printSequence(seq);
}

void DepartmentTree::stats(const DepartmentNode* n, int depth, int& leaves, int& height)
{
    if (depth > height) height = depth;
    if (n->children.empty()) ++leaves;
    for (size_t i = 0; i < n->children.size(); ++i) stats(n->children[i], depth + 1, leaves, height);
}

void DepartmentTree::statistics() const
{
    if (!root_)
        {
        cout << "No departments are loaded." << endl << "Total departments: 0" << endl
                     << "Leaf departments: 0" << endl << "Tree height: N/A" << endl;
        return;
    }
    int leaves = 0, height = 0;
    stats(root_, 0, leaves, height);
    cout << "Total departments: " << count_ << endl << "Leaf departments: " << leaves
              << endl << "Tree height: " << height << endl;
}