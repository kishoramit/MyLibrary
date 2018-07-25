#ifndef __BST_H_INCL__
#define __BST_H_INCL__

class BinarySearchTree
{
public:
    BinarySearchTree()
    : root_(nullptr), count_(0) {}
    ~BinarySearchTree() { delete_all(); }

    void insert(uint64_t key);
    bool remove(uint64_t key);
    uint64_t height() { return root_->height; }
    void print_ascending();
    void print_descending();
    void print_preorder();

private:
    struct Node
    {
    public:
        Node(uint64_t key, Node *p)
        : key(key), p(p), l(nullptr), r(nullptr), height(0) { }

        uint64_t key;
        Node *p;
        Node *l;
        Node *r;
        uint64_t height;
    };

    Node *root_;
    uint64_t count_;
    void delete_all();
    void print_node(Node *n)
    {
        std::cout << "k(" << n->key << ") p(" << n->p << ") r(" << n->r << ") l("
                  << n->l << ")" << std::endl;
    }
    Node* minimum(Node* node);
    Node* maximum(Node* node);
    Node* successor(Node* node);
    Node* predeccessor(Node* node);
    Node* search(uint64_t key);
    Node* merge(Node *lst, Node *rst);
    void adjust_height(Node *from, Node *to);
    bool lcop(const Node *n)
    {
        if (n->p == nullptr) { return false; }
        if (n == n->p->l) { return true; }
        return false;
    }

    bool rcop(const Node *n)
    {
        if (n->p == nullptr) { return false; }
        if (n == n->p->r) { return true; }
        return false;
    }
};

#endif // __BST_H_INCL__

