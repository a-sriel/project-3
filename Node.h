#ifndef __NODE_H__
#define __NODE_H__

#include <vector>

class Node {
    private:
        int block_id;
        bool is_leaf;

        std::vector<int> keys;
        std::vector<int> values;

        std::vector<int> child_block_ids;
        
    public:
        Node() {
            block_id = 0;
            is_leaf = true;
        }
        Node(int _block_id, bool _is_leaf) {
            block_id = _block_id;
            is_leaf = _is_leaf;
        }
};

#endif