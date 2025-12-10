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

        void insert_non_full(Node* x, uint64_t key, uint64_t value) {
            int i = x->num_keys - 1;
            
            if (x->is_leaf) {
                while (i >= 0 && key < x->keys[i]) {
                    x->keys[i + 1] = x->keys[i];
                    x->values[i + 1] = x->values[i];
                    i--;
                }

                if (i >= 0 && x->keys[i] == key) {
                    std::cout << "Key already exists." << std::endl;
                    return;
                }

                // update next keys and values
                x->keys[i+1] = key;
                x->values[i+1] = value;
                x->num_keys++;
                
                save_node(x);
            }
            else {
                while (i >= 0 && key < x->keys[i]) {
                    i--;
                }

                i++;

                if (i > 0 && x->keys[i-1] == key) {
                    std::cout << "Key already exists." << std::endl;
                    return;
                }

                Node* child = load_node(x->child_block_ids[i]);
                if (child->num_keys == MAX_KEYS) {
                    split_child(x, i, child);
                    if (key > x->keys[i]) {
                        i++;
                        delete child;
                        child = load_node(x->child_block_ids[i]);
                    } else {
                        delete child;
                        child = load_node(x->child_block_ids[i]);
                    }
                }
                insert_non_full(child, key, value);
                delete child;
            }
        }

        void insert(uint64_t key, uint64_t value) {
            if (!root_node_) {
                root_node_ = new Node();

                root_node_->block_id = next_block_id_;
                root_node_->is_leaf = true;
                root_node_->num_keys = 1;
                root_node_->keys[0] = key;
                root_node_->values[0] = value;
                next_block_id_++;
                
                save_node(root_node_);
                save_header();
                return;
            }

            // duplicate keys check
            for (int i=0; i < root_node_->num_keys; ++i) {
                if (root_node_->keys[i] == key) {
                    std::cout << "Key already exists." << std::endl;
                    return;
                }
            }

            if (root_node_->num_keys == MAX_KEYS) {
                Node* new_root = new Node();
                new_root->block_id = next_block_id_++;
                new_root->is_leaf = false;
                new_root->num_keys = 0;
                new_root->child_block_ids[0] = root_node_->block_id;

                // change parent of old root
                root_node_->parent_id = new_root->block_id;
                save_node(root_node_);

                Node* old_root = root_node_;
                root_node_ = new_root;
                save_header();  // update rood id

                split_child(new_root, 0, old_root);
                insert_non_full(new_root, key, value);
            } else {
                insert_non_full(root_node_, key, value);
            }
        }

        void split_child(Node* x, int i, Node* y) {
            Node* z = new Node(); 
            z->block_id = next_block_id_++;
            z->is_leaf = y->is_leaf;
            z->parent_id = x->block_id;
            z->num_keys = MIN_DEGREE - 1;

            for (int j = 0; j < MIN_DEGREE - 1; j++) {
                z->keys[j] = y->keys[j + MIN_DEGREE];
                z->values[j] = y->values[j + MIN_DEGREE];
                y->keys[j + MIN_DEGREE] = 0;
                y->values[j + MIN_DEGREE] = 0;
            }

            if (!y->is_leaf) {
                for (int j = 0; j < MIN_DEGREE; j++) {
                    z->child_block_ids[j] = y->child_block_ids[j + MIN_DEGREE];
                    y->child_block_ids[j + MIN_DEGREE] = 0;
                }
            }

            y->num_keys = MIN_DEGREE - 1;

            for (int j = x->num_keys; j >= i + 1; j--) {
                x->child_block_ids[j + 1] = x->child_block_ids[j];
            }

            x->child_block_ids[i + 1] = z->block_id;

            for (int j = x->num_keys-1; j >= i; j--) {
                x->keys[j+1] = x->keys[j];
                x->values[j+1] = x->values[j];
            }

            x->keys[i] = y->keys[MIN_DEGREE-1];
            x->values[i] = y->values[MIN_DEGREE-1];
            y->keys[MIN_DEGREE-1] = 0;
            y->values[MIN_DEGREE - 1] = 0;
            x->num_keys++;

            save_node(y);
            save_node(z);
            save_node(x);
            save_header();

            // check parents for z's children, update as needed
            if (!z->is_leaf) {
                for (int j = 0; j <= z->num_keys; j++) {
                    if (z->child_block_ids[j] != 0) {
                        Node* child= load_node(z->child_block_ids[j]);
                        child->parent_id = z->block_id;
                        save_node(child);
                        delete child;
                    }
                }
            }

            delete z;
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
        if (argc < 5) return 1;
        if (!file_exists(argv[2])) {
            std::cerr << "File does not exist." << std::endl;
            return 1;
        }

        BTree btree(argv[2]);
        btree.load_header();
        btree.insert(std::stoull(argv[3]), std::stoull(argv[4]));

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