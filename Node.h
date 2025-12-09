#ifndef __NODE_H__
#define __NODE_H__

#include <stdint.h>
#include "BTree.h"

#include <vector>
/*
struct Node {
    private:
        uint8_t block_id;
        uint8_t parent_block_id;
        uint8_t num_keys;

        std::vector<uint8_t> keys;
        std::vector<uint8_t> values;
        std::vector<uint8_t> child_block_ids;

        bool is_leaf;
        
    public:
        Node() {
            block_id = 0;
            is_leaf = true;
        }
        Node(uint8_t _block_id, bool _is_leaf) {
            block_id = _block_id;
            is_leaf = _is_leaf;
        }

        std::vector<uint8_t> get_child_block_ids() {
            return this->child_block_ids;
        }

        bool get_is_leaf() {
            return this->is_leaf;
        }

        bool is_root() {
            return block_id = 0;
        }
};
*/
#endif