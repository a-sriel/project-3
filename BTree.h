#ifndef __BTREE_H__
#define __BTREE_H__

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <stdint.h>

//#include "Node.h"

const size_t BLOCK_SIZE = 512;
const char* MAGIC_NUMBER = "4348PRJ3";
#define MIN_DEGREE 10
#define MAX_KEYS (2 * MIN_DEGREE - 1)


class BTree {
    private:
        std::string filename;

        struct Node {
            private:
                uint8_t block_id;
                uint8_t parent_block_id;
                uint8_t num_keys;

                std::vector<uint8_t> keys;
                std::vector<uint8_t> values;
                std::vector<uint8_t> child_block_ids;

                bool is_leaf;
                Node* next;
                
            public:
                Node() {
                    block_id = 0;
                    is_leaf = true;
                }

                Node(bool _is_leaf = false) {
                    is_leaf = _is_leaf;
                    next = nullptr;
                }
                std::vector<uint8_t> get_child_block_ids() {
                    return this->child_block_ids;
                }

                uint8_t get_key_at(int i) {
                    return this->keys.at(i);
                }

                void set_key_at(int i) {
                    
                }

                bool get_is_leaf() {
                    return this->is_leaf;
                }

                bool get_num_keys() {
                    return this->num_keys;
                }
        };
        
     
    public:
        Node* root;

        size_t degree = MIN_DEGREE;

        void split_child(Node* parent, size_t idx, Node* child);

        void insert_non_full(Node* node, uint8_t key);
        void borrow_from_prev(Node* node, size_t idx);
        void borrow_from_next(Node* node, size_t idx);

        void merge(Node* node, size_t idx);

        BTree(size_t _degree) {
            root = nullptr;
            degree = _degree;
        }

        void insert(uint8_t key);
        bool search(uint8_t key);
        std::vector<uint8_t> range_query(uint8_t lower, uint8_t upper);

        int is_bigendian() {
            int x = 1;
            return ((uint8_t *)&x)[0] != 1;
        }

        uint64_t reverse_bytes(uint64_t x) {
            uint8_t dest[sizeof(uint64_t)];
            uint8_t *source = (uint8_t*)&x;

            for (int c = 0; c < sizeof(uint64_t); c++)
                dest[c] = source[sizeof(uint64_t)-c-1];

            return *((uint64_t *)dest);
        }

        uint8_t to_bigendian(uint8_t x) {
            if (is_bigendian()) return x;

            return (uint8_t)(reverse_bytes((uint64_t)x) >> 32);
        }

        void format_bytes(std::ofstream& file, const std::vector<uint8_t>& header_bytes) {
            size_t padding_size = BLOCK_SIZE - header_bytes.size();

            file.write(reinterpret_cast<const char*>(header_bytes.data()), header_bytes.size());

            if (padding_size > 0) {
                std::vector<uint8_t> padding_bytes(padding_size, 0x00);
                file.write(reinterpret_cast<const char*>(padding_bytes.data()), padding_size);
            }

        }

        void create() {
            std::ofstream file("test.idx", std::ios::binary);

            if (file.is_open()) {
                //std::vector<uint8_t> buffer = {0x34, 0x33, 0x34, 0x38, 0x50, 0x52, 0x4A, 0x33};

                uint8_t root_block_id = to_bigendian(0x00);
                uint8_t next_block_id = to_bigendian(0x01);
    
                std::vector<uint8_t> seq(MAGIC_NUMBER, MAGIC_NUMBER + 8);
                seq.push_back(root_block_id);
                seq.push_back(next_block_id);

                format_bytes(file, seq);
                
                file.close();
            }

        }

        /*
        void split_child(Node x, int i) {
            std::vector<uint8_t> y = x.get_child_block_ids();
            Node z = Node(y->is_leaf);
        }
        */

};


#endif