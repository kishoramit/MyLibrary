#ifndef __BST_TEST_H_INCL__
#define __BST_TEST_H_INCL__

#include <iostream>
#include <mutex>
#include <vector>
#include <algorithm>
#include <cmath>
#include <math.h>
#include <ctime>
#include "../bst/bst.h"

class BSTTest
{
public:
    static void test1();
};

void
BSTTest::test1()
{
    const uint32_t max_keys = 100; // 100 + 2
    const uint32_t max_key_value = 1000;

    BinarySearchTree bst;
    srand(clock());
    uint32_t insert_key_count = rand() % max_keys + 2;
    uint32_t remove_key_count = rand() % max_key_value + 2;

    std::cout << "Creates a BST with " << insert_key_count << " random keys, prints "
              << "the BST keys in the ascending/descending order, Tries to remove "
              << remove_key_count << " keys randomly, and then print the BST again."
              << std::endl;

    uint32_t i = insert_key_count;
    while (i--) { bst.insert(rand() % max_key_value); }
    std::cout << "BST height after inserting " << insert_key_count << " key is "
              << bst.height() << ". Minimum possible BST height is "
              << std::floor(std::log2(insert_key_count)) << std::endl;
    bst.print_ascending();
    bst.print_descending();
    bst.print_preorder();

    std::cout << std::endl;
    uint32_t delete_count = 0;
    while (remove_key_count)
    {
        if (bst.remove(rand() % max_key_value)) { delete_count++; }
        remove_key_count--;
    }
    std::cout << "BST height after deleting " <<  delete_count << " keys is "
              << bst.height() << ". Minimum possible BST height is "
              << std::floor(std::log2(insert_key_count - delete_count))
              << std::endl;
    bst.print_ascending();
    bst.print_descending();
    bst.print_preorder();

    return;
};

#endif // __BST_TEST_H_INCL__

