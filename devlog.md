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