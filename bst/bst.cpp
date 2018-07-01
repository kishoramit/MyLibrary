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
    std::unique_lock<std::mutex> lock(mtx_);

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
    while (p != nullptr)
    {
        uint64_t p_lh = p->l ? p->l->height : 0;
        uint64_t p_rh = p->r ? p->r->height : 0;

        uint64_t ph = std::max(p_lh, p_rh) + 1;
        if (p->height != ph)
        {
            p->height = ph;
            p = p->p;
        }
        else
        {
            break;
        }
    }

    //print_node(node);
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

    // 1. If this node has a right child, successor is a node with minimum
    // value in the subtree rooted by its right child.
    if (node->r)
    {
        //cout << " RST " << std::flush;
        return minimum(node->r);
    }

    // No right child, We have to move up now. Three possibilities -
    // 1. This node is the left child. So, parent is the successor.
    // 2. This node is the right child. So, there is no successor in the subtree
    // rooted by its parent. Go one more level up.
    // 3. There is no parent. i.e. node is the root, so, there is no successor
    //  of this node.

    while (node->p != nullptr)
    {
        if (node->p->l == node)
        {
            //cout << " LC " << std::flush;
            return node->p;
        }
        else
        {
            // This has to be the right child.
            //cout << " RC "<< std::flush;
            node = node->p;
        }
    }
    return nullptr;
}

BinarySearchTree::Node*
BinarySearchTree::minimum(Node* node)
{
    while (node->l != nullptr) { node = node->l; }
    return node;
}

BinarySearchTree::Node*
BinarySearchTree::maximum(Node* node)
{
    while (node->r != nullptr) { node = node->r; }
    return node;
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
