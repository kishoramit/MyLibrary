#include <iostream>
#include <mutex>
#include <vector>
#include <algorithm>
#include <cmath>
#include <math.h>
#include <ctime>
#include "bst.h"

using namespace std;

void
BinarySearchTree::insert(uint64_t key)
{
    Node *n, *p = nullptr;

    if (root_ == nullptr)
    {
        root_ = new Node(key, nullptr);
        //print_node(root_);
        count_++;
        return;
    }
    n = root_;

    // Find the parent node which will adopt this key as left/right child.
    while (n != nullptr)
    {
        if (key < n->key)
        {
            p = n;
            n = n->l;
        }
        else
        {
            p = n;
            n = n->r;
        }
    }

    // Create a new node for the received key.
    Node *node = new Node(key, p);

    // Have its parent adopt the node now.
    assert(p != nullptr);
    if (key < p->key)
    {
        assert(p->l == nullptr);
        p->l = node;
    }
    else
    {
        assert(p->r == nullptr);
        p->r = node;
    }
    count_++;

    // Trace your steps back to the root now, and adjust height for each node
    // encountered in path.
    adjust_height(p, root_);

    //print_node(node);
}

bool
BinarySearchTree::remove(uint64_t key)
{
    Node *node = search(key);
    if (node == nullptr) { return false; }

#if 0
    ///XXX: This is also a valid method to delete a node. However, it is not very
    //AVL/RedBlock tree friendly because of the height change.

    // Three possibilities -
    // 1. A root node is being deleted.
    // 2. An intermediate node is being deleted.
    // 3. A leaf node is being deleted.

    Node *merged_subtree = merge(node->l, node->r);
    if (merged_subtree != nullptr) { merged_subtree->p = node->p; }

    // 1
    if (node->p == nullptr)
    {
        root_ = merged_subtree;
    }
    else
    {
        if(node == node->p->l) { node->p->l = merged_subtree; }
        else { node->p->r = merged_subtree; }
        adjust_height(node->p, root_);
    }

    delete node;
    count_--;
    return true;
#endif

    // Three possibilities -
    // 1. No subtree is present. i.e. root or leaf node
    // 2. Only one subtree is present.
    // 3. Both subrees are present.

    Node *promoted_node = nullptr;

    if ((node->l == nullptr) && (node->r == nullptr))
    {
        // 1
        promoted_node = nullptr;
    }
    else if ((node->l == nullptr) || (node->r == nullptr))
    {
        // 2
        if (node->l) { promoted_node = node->l; }
        else { promoted_node = node->r; }
        assert(promoted_node != nullptr);
    }
    else
    {
        // 3
        // Have the successor node take the place of the deleting node.
        Node *s = successor(node);
        assert(s != nullptr);
        assert(s->l == nullptr);
        node->key = s->key;

        // Delete the redundant successor node.
        node = s;
        promoted_node = s->r;
    }

    if (promoted_node != nullptr) { promoted_node->p = node->p; }

    if (node->p == nullptr) { root_ = promoted_node; }
    else
    {
        if(lcop(node)) { node->p->l = promoted_node; }
        else { node->p->r = promoted_node; }
    }

    adjust_height(node->p, root_);
    delete node;
    count_--;
    return true;
}

void
BinarySearchTree::delete_all()
{
    std::function<void(Node*)> deleter;
    deleter = [this, &deleter](Node *node)
    {
        if (node == nullptr) { return; }
        deleter(node->l);
        deleter(node->r);
        delete node;
        count_--;
    };
    deleter(root_);
    root_ = nullptr;
    assert(!count_);
}

BinarySearchTree::Node*
BinarySearchTree::successor(Node* node)
{
    if (node == nullptr ) { return nullptr; }

    // 1. If the node's right child present, its successor is the node which have
    // minimum value in the sub-tree rooted by its right child.
    // 2. If the node is the left child of its parent, its parent node is the next
    // successor.
    // 3. If the node is the right child of its parent, next successor doesn't exist
    // in the sub-tree rooted by its parent. So, keep moving up until this node
    // is in a sub-tree rooted by its ancestor's left child.

    // 1
    if (node->r) { return minimum(node->r); }

    // 2, 3
    while (node->p != nullptr)
    {
        if (node->p->l == node) { return node->p; }
        else { node = node->p; }
    }
    return nullptr;
}

BinarySearchTree::Node*
BinarySearchTree::predeccessor(Node* node)
{
    if (node == nullptr ) { return nullptr; }

    // 1. If the node's left child present, its predecessor is the node which have
    // maximum value in the sub-tree rooted by its left child.
    // 2. If the node is the right child of its parent, its parent node is the next
    // predecessor.
    // 3. If the node is the left child of its parent, next predecessor doesn't exist
    // in the sub-tree rooted by its parent. So, keep moving up until this node
    // is in a sub-tree rooted by its ancestor's right child.

    // 1
    if (node->l) { return maximum(node->l); }

    // 2, 3
    while (node->p)
    {
        if (node->p->r == node) { return node->p; }
        else { node = node->p; }
    }
    return nullptr;
}

BinarySearchTree::Node*
BinarySearchTree::minimum(Node* node)
{
    if (node == nullptr) { return node; }
    while (node->l != nullptr)
    {node = node->l; }
    return node;
}

BinarySearchTree::Node*
BinarySearchTree::maximum(Node* node)
{
    if (node == nullptr) { return node; }
    while (node->r != nullptr) { node = node->r; }
    return node;
}

BinarySearchTree::Node*
BinarySearchTree::search(uint64_t key)
{
    std::function<Node*(Node*, uint64_t)> find;
    find = [&find](Node *n, uint64_t key)
    {
        if ((n == nullptr) || (key == n->key)) { return n; }
        if (key < n->key) { return find(n->l, key); }
        else { return find(n->r, key); }
    };
    return find(root_, key);
}

BinarySearchTree::Node*
BinarySearchTree::merge(Node *lst, Node *rst)
{
    if ((lst == nullptr) && (rst == nullptr)) { return nullptr; }
    if (lst == nullptr) { return rst; }
    if (rst == nullptr) { return lst; }

    // Largest key value in lst has to be less than the lowest key value in rst.
    assert(maximum(lst)->key < minimum(rst)->key);

    // Left-sub-tree (aka lst) will be folded into the right-sub-tree.
    Node *m_rst = minimum(rst);
    assert(m_rst != nullptr);

    m_rst->l = lst;
    lst->p = m_rst;

    // Now fix the height, nodes' height in lst is not affected.
    adjust_height(m_rst, rst);

    return rst;
}

void
BinarySearchTree::adjust_height(Node *from, Node *to)
{
    // Inclusive of both from and to.
    if ((from == nullptr) || (to == nullptr)) { return; }

    while (from != to->p)
    {
        int32_t l_h = from->l? from->l->height: -1;
        int32_t r_h = from->r? from->r->height: -1;
        int32_t h = max(l_h, r_h) + 1;
        if (h != from->height)
        {
            from->height = h;
            from = from->p;
        }
        else
        {
            // No need to keep going now as no change is needed from this node onward.
            break;
        }
    }
    return;
}

void
BinarySearchTree::print_ascending()
{
    cout << "{ ";
    if (0)
    {
        // 1 : use recursive lambda function.
        std::function<void(Node *node)> inorder;
        inorder = [&inorder](Node *node) -> void
        {
            if (node == nullptr) { return; }
            inorder(node->l);
            cout << node->key << " ";
            inorder(node->r);
        };
        inorder(root_);
    }
    else
    {
        // Use successor to get to the next node.
        Node *node = minimum(root_);
        while (node != nullptr)
        {
            cout << node->key << " ";
            node = successor(node);
        }
    }
    cout << "}" << endl;
}

void
BinarySearchTree::print_descending()
{
    cout << "{ ";
    Node *node = maximum(root_);
    while (node != nullptr)
    {
        cout << node->key << " ";
        node = predeccessor(node);
    }

    cout << "}" << endl;
}

void
BinarySearchTree::print_preorder()
{
    cout << "{ ";

    function<void(Node*)> preorder;
    preorder = [&preorder](Node *n)
    {
        if (n == nullptr) { return; }
        cout << n->key << " ";
        preorder(n->l);
        preorder(n->r);
    };
    preorder(root_);
    cout << "}" << endl;
}
