2025-11-17 19:13

For this project, I need to write an interactive program that creates and manages index files which contain a b-tree. There will be a command menu where the user can create, insert, search, load, print, and extract these files. The index file will also be divided into 512 byte blocks, with each btree node fitting into a single 512 byte block. The file header takes up the entire first block. New nodes will be appended to the end of the file.

Numbers will be stored as 8-byte integers in big endian format. 

The header contains 3 8-byte fields, with the remainder of the 512 byte block being left unused.

The b-tree has at least degree 10 (i.e. 19 key/value pairs and 20 child pointers). Each node is stored in a single block with header info.

All arguments will be passed via the command line. For this project, I am planning to develop it on Ubuntu using WSL. I will also test it on the cs1 servers using MobaXterm. 

Since this program does not require the use of pipes or threads, I will probably take a functional programming approach with this project. I may separate the program logic based on the program driver, menu driver, and communication logic, but I'm not 100% sure how I will divide it yet. I will probably just start off putting everything in a main driver file.

2025-11-17 19:56

I am planning to start representing the blocks in std::cout, instead of worrying about creating a separate file. I have created the initial driver.cpp file, where I will begin to implement this logic. I won't implement the commands menu yet, I just want to worry about getting the blocks output properly. Later, I will need to add arguments to main() to accept command line arguments.

I am having a difficult time visualizing how the index file output is supposed to look, so I am going to come back to this later.

2025-12-5 21:44

I need to change course a little bit. Instead of focusing on the output first, I am going to focus on getting the B-tree logic down. 

This session, I will start by making main take command line arguments. I will also make separate classes for the Node and the BTree itself, making sure they all contain the proper parameters to store the values, keys, and block IDs. I have implemented an outline for the Node and BTree classes, and set up the header logic for the index file within the BTree class. I also declared some literals. I am not sure what data type to use for hexadecimal values, so I will need to toy around with that later.

2025-12-8 17:46

This session, I will work on refining the B-Tree. I am going to implement the create() function so that I can make sure I am populating the fields correctly. The header file needs to contain the magic number, and the root id and next block id, so 3 8-bit numbers total. I am planning to use a vector implementation and utilize the bigendian conversion functions provided in the project document to make sure the 8-bit numbers are properly formatted.

I changed the magic number to be a const* char, as it needs to be an 8-bit sequence of ASCII values. The create function was also partially implemented. I created a format_bytes() function to handle the block padding and populate each node with 512 0x00's (initially), as each block consists of 512 bytes.

2025-12-8 20:55

I need to finalize the B-Tree logic. I will probably store the Node data within std::vectors to keep things consistent. Sometimes I think using Python would make this way easier but... I like writing C++. 

I temporarily moved Node into BTree.h for simplification. I'm not sure if I will keep it this way or not. This will avoid me needing to rely on getters and setters in the testing phase. I accomplished some more reformatting. I am encountering a lot of difficulty implementing the B-Tree.

2025-12-9 20:00

Because I am struggling to implement the B-Tree, I am going to consolidate everything into a single file (driver.cpp) to minimize class inheritance issues. I think for this session, I am going to create a real skeleton for every function from the menu, as well as for all of the B-tree functions.

I changed Node to be a struct instead of a class, and removed all unused functions. I simplified the Node structure by simply giving it all of its parameters plus a constructor, with no extraneous functions. I also changed the data types to uint64_t to maintain consistency throughout the program. I also added all of the functions and helpers to the BTree class.

I put the logic for creating the index file into distinct functions. I converted each number to bigendian and inserted that into a buffer, which is then written to an ofstream. This should provide more consistent file writing throughout my entire project, minimizing errors in the idx files.

I also added some error handling, to validate input and to check if a file exists, and also to check the magic number. I partially implemented the menu within main() (i.e. our program driver), and implemented the header logic.

2025-12-10 9:13

For this session, I need to implement the insertion and shift logic for the B-Tree. Once the nodes are inserted properly, I can also work on printing them / extracting them from a file. Now that I have shifted to arrays instead of vectors, making the adjustments within the tree is significantly easier.

I implemented the insert_non_full function, which will serve as a helper for insert(). It traverses the tree and inserts nodes where needed, and loads nodes as well.

Since dumping everything into a single file, and partially starting over the B-Tree, I feel like I have a greater grasp of this project.

2025-12-10 12:57

I need to finalize the insertion method. I will also incorporate the menu in the program driver to support this insertion. The insertion function creates a header if the root doesn't exist and saves it to the file. Otherwise, it checks for duplicate keys and alters the subroots and splits children until the B-Tree order is maintained.

I finished the insertion logic, and also incorporated the fixed split_child function. I am continuing to update the menu. All of the input error handling has been finished. The menu is also done.

Save node and load node functionality has been added. This was a relatively simple addition since they are basically the same function with a few tweaks. The buffers are properly formatted to support the 8-bit wide number format in the hex file. To finalize this project, I need to finish the print and extract functions, and then do another quick pass of everything before I submit.