#ifndef __BTREE_H__
#define __BTREE_H__

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <stdint.h>

#include "Node.h"

const size_t BLOCK_SIZE = 512;
const char* MAGIC_NUMBER = "4348PRJ3";
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

            int root_id = 0;

            // define header
           // long header = std::stoi(magic_number) + root_id + next_block_id;
        }
};

#endif