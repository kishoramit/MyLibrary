#ifndef __BSTRES_H_INCL__
#define __BSTRES_H_INCL__

#include <iostream>
#include <mutex>
#include <vector>
#include <algorithm>
#include <cmath>
#include <math.h>
#include <ctime>
#include "../bst/bst.h"

class BSTRes
{
public:
    static void res1();
};

void
BSTRes::res1()
{
    const uint64_t key_count = 7;
    const uint64_t start_key = 1;

    std::vector<uint64_t> keys;
    std::vector<uint64_t> bst_height(key_count, 0);
    const clock_t begin_time = clock();

    // Generate a sequential set of keys and sort them if needed.
    for (uint64_t i = start_key; i <= key_count; i++)
    {
        keys.push_back(i);
    }
    std::sort(keys.begin(), keys.end());

    // Create a binary search key for each permutation of keys and then measure its
    // height.
    do
    {
        BinarySearchTree bst;
        for (auto key : keys)
        {
            bst.insert(key);
        }
        uint64_t h = bst.height();
        assert(h <= key_count);
        bst_height[h] = bst_height[h] + 1;

    } while(std::next_permutation(keys.begin(), keys.end()));

    // Print the output
    uint64_t index = 0;
    uint64_t sum = 0;
    for (auto c : bst_height)
    {
        std::cout << "Height " << index << " : " << c << " BSTs" << std::endl;
        index++;
        sum += c;
    }
    uint64_t balance_tree_height = floor(log2(key_count));
    assert(balance_tree_height <= key_count);
    uint64_t percentage = (bst_height[balance_tree_height] * 100) / sum;

    std::cout << "A balance tree height of size " << key_count << " is "
         << balance_tree_height << "."<< std::endl;

    std::cout << "Total " << sum << "(=" << key_count<< "!) BSTs were constructed." << std::endl;

    std::cout << "Only " << percentage << "% key insert order resulted in a balanced BST."
         << std::endl;

    std::cout << "Took " << ceil((clock() - begin_time) / CLOCKS_PER_SEC)
         << " secs to compute the result." << std::endl;
}

#endif // __BSTRES_H_INCL__

