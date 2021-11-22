#include "Perf_Matching.h"


bool find_edge(Graph& graph, NodeId& node_x, NodeId& node_y){
	for(NodeId even_node = 0; even_node < graph.get_num_nodes(); ++even_node) {
		if (graph.get_node_from_id(even_node).is_even()){
			for (auto neighbor: graph.get_node_from_id(even_node).get_neighbors()){
				if (not graph.check_combined(even_node,neighbor) and not graph.get_node_from_id(neighbor).is_odd()){
					node_x = even_node;
					node_y = neighbor;
					return true;
				}
			}
		}
	}
	return false;
}


void unshrink(Graph& graph){
	while (graph.has_circle()){
		std::pair<std::vector<std::pair<NodeId, NodeId>>, std::vector<std::pair<NodeId, NodeId>>, std::pair<NodeId, NodeId>> circ = graph.last_added_circle();
		
		NodeId outNode;
		std::vector<NodeId> circ_nodes;

		for (auto nodepair : circ[0]){
			circ_nodes.push_back(nodepair[0]);
			circ_nodes.push_back(nodepair[1]);
		}
		for (auto nodepair : circ[1]){
			circ_nodes.push_back(nodepair[0]);
			circ_nodes.push_back(nodepair[1]);
		}
		circ_nodes.push_back(circ[2][0]);
		circ_nodes.push_back(circ[2][1]);

		for (auto node : circ_nodes){
			 for (neighbor : graph.get_node_from_id(node).get_match_neighbors()){
			 	if(not std::find(circ_nodes.begin(), circ_nodes.end(), neighbor) != circ_nodes.end()){
			 		outNode = node;
			 	}
			 }
		}

		int found_node = 0; // go through the circle edges twice and after having found the out node add or remove the next edges from the matching . Variable is 0 until found and the switch between 1 and 2 counting the parity, until we find the node again. then it is 3 and we are done
		full_path = path1;
		full_path.push_back(edge);
		full_path.insert(full_path.end(), path2.rbegin(), path2.rend()); // funktioniert der reverse iterator... ist das generell hubsch wie ich das gemacht habe?
		full_path.insert(full_path.end(), full_path.begin(), full_path.end()); // klappt das?
		for (auto edge : full_path){
			if (found_node == 0){
				if (edge[0]==outNode or edge[1]==outNode){
					found_node = 1;
				} else {
					if (found_node == 1){ // dont add edge to matching
						graph.add_match_edge(edge);
						found_node = 2;
					} else { //found_node ==2, add edge to matching
						found_node = 1;
						graph.remove_match_edge(edge);
					}
					if (edge[0]==outNode or edge[1]==outNode){
						found_node = 3;
				}
			}
		}
	}
}


int find_split(std::vector<std::pair<NodeId, NodeId>>& path1, std::vector<std::pair<NodeId, NodeId>>& path2){
    if(path1.empty() or path2.empty()){
        return 0;
    }
    int i = 0;
    while (path1[i].second == path2[i].second){
        i++;
    }
    return i;
}

bool perfect_matching(Graph& graph) {   // this calculates a perfect matching if it exists

	NodeId x = 0;
	NodeId y = 0;

	while (find_edge(graph, x, y)){
        std::cout << "Found the edge " << x << " " << y << "\n";
		Node& node_x = graph.get_node_from_id(x);
		Node& node_y = graph.get_node_from_id(y);


		if (not node_y.is_matched() and not node_y.is_in_tree()){
			//augment
            graph.add_match_edge(x,y);
			bool parity_count = true;
			for (auto tree_edge : graph.get_tree_path(x)){
  				if (parity_count){
                    graph.add_match_edge(tree_edge.first, tree_edge.second);
  				} else {
                    graph.remove_match_edge(tree_edge.first, tree_edge.second);
  				}
  				parity_count = not parity_count;
			}
            ///hier müssen wir erst einmal unshrinken, danach schauen, ob es perfekt ist
			if (2 * graph.get_num_match_edges() == graph.get_num_nodes()){
				//found perfect matching
				std::cout << "We found a p.m.!" << std::endl;
				return true;
			} else {
                graph.init_tree();
			}



		}else if (not node_y.is_in_tree() and node_y.is_matched()){
			//extend
            graph.add_tree_edge(x, y);

		} else {
			//shrink
			std::vector<std::pair<NodeId, NodeId>> path1 = graph.get_tree_path(x);
			std::vector<std::pair<NodeId, NodeId>> path2 = graph.get_tree_path(y);


			int last_shared_nodeid = find_split(path1, path2);
			std::pair<NodeId, NodeId> con_edge = {x,y};
			graph.add_circle(path1, path2, con_edge);

			for (int i = last_shared_nodeid; i < path1.size(); ++i){
                graph.remove_tree_edge(path1[i].first, path1[i].second);
				int parity;
				if (graph.get_node_from_id(path1[i].first).is_odd()){
					parity = 0;
				} else if (graph.get_node_from_id(path1[i].first).is_even()){
					parity = 1;
				} else{
					parity = 2;
				}
                graph.combine(path1[i].first, path1[i].second, parity);
			}
			for (int i = last_shared_nodeid; i < path2.size(); ++i){
                graph.remove_tree_edge(path2[i].first, path2[i].second);
				int parity;
				if (graph.get_node_from_id(path2[i].first).is_odd()){
					parity = 0;
				} else if (graph.get_node_from_id(path2[i].first).is_even()){
					parity = 1;
				} else{
				    std::cout << "Some node of the circuit is not in the tree!\n";
					//parity = 2;
				}
				graph.combine(path2[i].first, path2[i].second, parity);
			}
		}
	}
	return false;
}

void matching(Graph& graph){ // was genau macht das &? brauch ich ein * in perf_mathcing damit die graph instance auch veraewndert wird?
	bool not_done = true;
	while(not_done){
		perfect_matching(graph);
		for(NodeId id = 0; id < _nodes.size(); ++id) {
			if (graph.get_node_from_id(id).is_in_tree()){
				//add to deleted node :: todo implement deleted nodes and in all functions only return node if not deleted.... FUCKFUCKFUCK egal morgen ;D
			}
			// und matching und tree edges resetten....
		}
	}
}