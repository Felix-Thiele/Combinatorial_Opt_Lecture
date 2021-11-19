#include "input.h"


std::vector<std::string> split(std::string str){
	// This splits a string into the components seperated by spaces
	std::vector<std::string> result = {};
	std::string temp = "";
	for (std::string::size_type i=0; i < str.size(); i++){
		if (str[i] == ' '){
			if (temp != ""){
				result.push_back(temp);
			}
			temp = "";
		}else{
			temp.push_back(str[i]);
		}
	}
	result.push_back(temp);
	return result;
}


ED::Graph read_file() {
	//this reads the input file and returns the corresponding graph
	ED::Graph G = ED::Graph(0);
	std::ifstream file("input.txt");
	
	if (file.is_open()) {
		std::string line;
		std::getline(file, line);
		// the third index of the first line gives the number of nodes in our graph.
		G = ED::Graph(std::stoi(split(line.c_str()).at(2)));

		while (std::getline(file, line)) {
			std::vector<std::string> split_line = split(line.c_str());
			// the second and third index of the latter lines are the nodes that are connected. In the file format node names start with 1 instead of 0.
			G.add_edge(std::stoi(split_line.at(1).c_str())-1, std::stoi(split_line.at(2).c_str())-1);
			//std::cout << "i was here";
		}
		file.close();
	}
	return G;
}