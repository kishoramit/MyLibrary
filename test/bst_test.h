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
    BinarySearchTree bst;
    srand(clock());
    uint32_t key_count = rand() % 100 + 2;
    std::vector<uint64_t> keys;

    std::cout << "Creates a BST with " << key_count << " keys, then prints them in the"
                 " ascending/descending order." << std::endl;

    for (uint64_t i = 0; i < key_count; i++)
    {
        uint64_t k = rand() % 1000;
        bst.insert(k);
    }

    bst.print_ascending();

    return;
};

#endif // __BST_TEST_H_INCL__

