#include "input.h"
#include "Matching.h"
#include <iostream>


int main(int argc, char* argv[]) {
    try {
    	if (argc>0){
        	Graph G = read_graph(argv[1]);
        	matching(G);
    	}


        //G.print_matching();
    }
    catch(std::string& msg){
        std::cout << msg;
    }
}