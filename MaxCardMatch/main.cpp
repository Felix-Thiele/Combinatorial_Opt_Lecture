#include "input.h"
#include <vector>
#include <iostream>


int main() {
	Graph G = read_file();
	std::cout << G.num_nodes();
	std::cout << G.num_edges();
	std::cout << G.node(0).degree();
	std::cout << G.root;
	std::cout << " Hello";
}