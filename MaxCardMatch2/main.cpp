#include "input.h"
#include "Matching.h"
#include <iostream>


int main() {
    try {
        Graph G = read_graph();

        matching(G);

        //G.print_matching();
    }
    catch(std::string& msg){
        std::cout << msg;
    }
}