#include "input.h"
#include <sstream>


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

Graph read_graph(){
    std::ifstream file("C:\\Users\\maxmu\\CLionProjects\\untitled1\\instances\\lu980.dmx");
    std::string line;
    std::getline(file, line);
    line.erase(line.begin(), line.begin()+7);
    std::stringstream ss(line);
    ///lets assume the first line will have correct format
    unsigned n, m;
    ss >> n;
    ss >> m;
    Graph g(n);
    while(std::getline(file, line)){
        std::stringstream sss(line);
        unsigned a, b;
        char x;
        sss >> x;
        if(x == 'c'){ continue; }
        else if (x == 'e'){
            sss >> a;
            sss >> b;
            g.add_edge(a-1,b-1);
        }
    }
    file.clear();
    file.seekg(0);
    file.close();
    return g;

}