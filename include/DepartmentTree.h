#ifndef DEPARTMENTTREE_H
#define DEPARTMENTTREE_H
#include <string>
#include <vector>

struct DepartmentNode
{
    std::string name;
    DepartmentNode* parent;
    std::vector<DepartmentNode*> children;
    explicit DepartmentNode(const std::string& n, DepartmentNode* p = nullptr)
        : name(n), parent(p) {}
};

class DepartmentTree
{
public:
    enum AddResult { ADD_OK, ADD_DUPLICATE, ADD_UNKNOWN_PARENT };

    DepartmentTree() : root_(nullptr), count_(0)
    {

    }
    ~DepartmentTree();
    DepartmentTree(const DepartmentTree&) = delete;
    DepartmentTree& operator=(const DepartmentTree&) = delete;

    bool empty() const
    {
        return root_ == nullptr;
    }

    std::size_t size() const
    {
        return count_;
    }

    DepartmentNode* find(const std::string& name) const;

    AddResult add(const std::string& name, const std::string& parentName);

    void search(const std::string& name) const;
    void displayHierarchy() const;
    void preorder() const;
    void postorder() const;
    void levelOrder() const;
    void statistics() const;

private:
    static void destroy(DepartmentNode* n);
    static void printHierarchy(const DepartmentNode* n, int depth);
    static void collectPre(const DepartmentNode* n, std::vector<std::string>& out);
    static void collectPost(const DepartmentNode* n, std::vector<std::string>& out);
    static void stats(const DepartmentNode* n, int depth, int& leaves, int& height);
    static void printSequence(const std::vector<std::string>& seq);

    DepartmentNode* root_;
    std::size_t count_;
};
#endif
