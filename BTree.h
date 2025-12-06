#ifndef __BTREE_H__
#define __BTREE_H__

#include <string>
#include "Node.h"

#define BLOCK_SIZE 512
#define MAGIC_NUMBER "48348PRJ3"
#define MIN_DEGREE 10
#define MAX_KEYS (2 * MIN_DEGREE - 1)

class BTree {
    private:
        std::string filename;
        Node root;
        int next_block_id = 1;
     
    public:
        BTree() {
            filename = "";
            root = Node(0, true);
        }

        BTree(std::string _filename) {
            filename = _filename;
        }

        void create() {
            std::string magic_number = MAGIC_NUMBER;

            int root_id = 0;

            // define header
            long header = std::stoi(magic_number) + root_id + next_block_id;
        }
};

#endif