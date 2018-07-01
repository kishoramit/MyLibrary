#ifndef __BST_H_INCL__
#define __BST_H_INCL__

class BinarySearchTree
{
public:
    BinarySearchTree()
    : root_(nullptr), count_(0) {}
    ~BinarySearchTree() { delete_all(); }

    void insert(uint64_t key);
    uint64_t height() { return root_->height; }
    void print_ascending();

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
    std::mutex mtx_;
    void delete_all();
    void print_node(Node *n)
    {
        std::cout << "k(" << n->key << ") p(" << n->p << ") r(" << n->r << ") l("
                  << n->l << ")" << std::endl;
    }
    Node* minimum(Node* node);
    Node* maximum(Node* node);
    Node* successor(Node* node);
};

#endif // __BST_H_INCL__

