#include "input.h"
#include "Perf_Matching.h"
#include <iostream>


int main() {
    try {
        Graph G = read_graph();
        std::cout << G.get_num_nodes() << "\n";
        std::cout << G.get_num_edges() << "\n";
        std::cout << G.get_node_from_id(0).degree() << "\n";
        std::cout << " Hello\n";

        perfect_matching(G);

        G.print_matching();
    }
    catch(std::string& msg){
        std::cout << msg;
    }
}