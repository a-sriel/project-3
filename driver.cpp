#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include <cstring>
#include <algorithm>
#include <sstream>

#include "Node.h"
#include "BTree.h"
#include "Btree.cpp"

const int BLOCK_SIZE = 512;
const std::string MAGIC_NUMBER = "4348PRJ3";
const int MIN_DEGREE = 10;
const int MAX_KEYS = (2 * MIN_DEGREE - 1);

struct Node {
    uint64_t block_id;
    uint64_t parent_id;
    uint64_t num_keys;

    uint64_t keys[MAX_KEYS];
    uint64_t values[MAX_KEYS];
    uint64_t child_block_ids[MAX_KEYS + 1];

    bool is_leaf;
    
    Node() {
        block_id = 0;
        parent_id = 0;
        num_keys = 0;
        is_leaf = false;

        // set memory
        std::memset(keys, 0, sizeof(keys));
        std::memset(values, 0, sizeof(values));
        std::memset(child_block_ids, 0, sizeof(child_block_ids));
    }
};

class BTree {
    private:
        std::string file_path_;
        Node* root_node_;
        int next_block_id_;

    public:
        BTree(const std::string& file_path) : file_path_(file_path), root_node_(nullptr), next_block_id_(1) {}
        
        void create_index_file() {
            std::ofstream file(file_path_, std::ios::binary);
            if (!file.is_open()) {
                std::cerr << "File write failed. Unable to open file." << std::endl;
                return;
            }

            // write header file
            std::vector<uint8_t> buffer;
            buffer.insert(buffer.end(), MAGIC_NUMBER.begin(), MAGIC_NUMBER.end());

            uint64_t root_block_id = 0;
            uint64_t next_block_id = 1;

            uint64_t root_be = to_bigendian(root_block_id);
            uint64_t next_be = to_bigendian(next_block_id);
            
            buffer.insert(buffer.end(), (uint8_t*)&root_be, (uint8_t*)&root_be + sizeof(root_be));
            buffer.insert(buffer.end(), (uint8_t*)&next_be, (uint8_t*)&next_be + sizeof(next_be));

            // pad block with 0s
            format_bytes(file, buffer);
            
            file.close();
        }

        // load header from index file
        void load_header() {
            std::ifstream file(file_path_, std::ios::binary);
            if (!file.is_open()) {
                std::cerr << "File read failed. Unable to open file." << std::endl;
                return;
            }

            // read header file
            char header[BLOCK_SIZE];
            file.read(header, BLOCK_SIZE);

            if (std::string(header, MAGIC_NUMBER.size()) != MAGIC_NUMBER) {
                std::cerr << "Invalid index file." << std::endl;
                exit(1);
            }

            // read block ids
            uint64_t root_block_id;
            uint64_t next_block_id;
            std::memcpy(&root_block_id, header + MAGIC_NUMBER.size(), sizeof(root_block_id));
            std::memcpy(&next_block_id, header + MAGIC_NUMBER.size() + sizeof(root_block_id), sizeof(next_block_id));

            root_block_id = to_bigendian(root_block_id);
            next_block_id = to_bigendian(next_block_id);

            next_block_id_ = next_block_id;

            if (root_block_id != 0) {
                root_node_ = load_node(root_block_id);
            }

            file.close();

        }

        void save_header() {
            std::ofstream file(file_path_, std::ios::binary);
            if (!file.is_open()) {
                std::cerr << "File write failed. Unable to open file." << std::endl;
                return;
            }
            
            // write header
            file.seekp(0);

            std::vector<uint8_t> buffer;
            buffer.insert(buffer.end(), MAGIC_NUMBER.begin(), MAGIC_NUMBER.end());

            uint64_t root_block_id = root_node_ ? root_node_->block_id : 0;
            uint64_t next_block_id = next_block_id_;

            uint64_t root_be = to_bigendian(root_block_id);
            uint64_t next_be = to_bigendian(next_block_id);
            
            buffer.insert(buffer.end(), (uint8_t*)&root_be, (uint8_t*)&root_be + sizeof(root_be));
            buffer.insert(buffer.end(), (uint8_t*)&next_be, (uint8_t*)&next_be + sizeof(next_be));

            // pad block with 0s
            format_bytes(file, buffer);

            file.close();
        }

        // Bigendian formatting stuff
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

        // *************

        void print_helper(uint64_t block_id) {}

        void print() {}

        void extract_helper(uint64_t block_id, std::ofstream& out) {}

        void extract(const std::string& filename) {}

        Node* load_node(uint64_t block_id) {}

        // save node to idx file
        void save_node(Node* node) {}

        void insert(uint64_t key, uint64_t value) {}

        void insert_non_full(Node* x, uint64_t key, uint64_t value) {}

        void split_child(Node* x, Node* y, int i) {
            Node* z = new Node(); 
        }

        int search(uint64_t key) {}
};

// file checking
bool file_exists(const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good();
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: project3 <command> ..." << std::endl;
        return 1;
    }

    std::string command = argv[1];

    if (command == "create") {
        if (argc < 3) {
            std::cerr << "Usage" << std::endl;
            return 1;
        }
        if (file_exists(argv[2])) {
            std::cerr << "File exists." << std::endl;
            return 1;
        }

        BTree btree(argv[2]);
        btree.create_index_file();
    }
    else if (command == "insert") {

    }
    else if (command == "search") {

    }
    else if (command == "load") {

    }
    else if (command == "print") {

    }
    else if (command == "extract") {
        
    }
}