#include "input.h"
#include "Matching.h"
#include "auxiliary_classes/graph.h"
#include <iostream>


int main() {
    try {
        std::cout << "Calculate some m.f. Matchings "<< "\n";
        Graph G = read_graph();
        std::cout << "Calculate some m.f. Matchings "<< "\n";
        std::cout << G.get_num_nodes() << "\n";
        std::cout << G.get_num_edges() << "\n";
        std::cout << G.get_node_from_id(0).degree() << "\n"<< "\n";

        perfect_matching(G);
        std::cout << "Calculate some m.f. Matchings "<< "\n";

        G.print_matching();
        std::cout << "Calculate some m.f. Matchings "<< "\n";
    }
    catch(std::string& msg){
        std::cout << msg;
    }
}