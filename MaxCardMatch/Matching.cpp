#include "Matching.h"


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
    std::cout << "was here" << "\n";
	while (graph.has_cycle()){

    	std::cout << "unshrink" << "\n";
        std::pair< std::vector<NodeId>, std::vector<std::pair<NodeId, NodeId>>> cyc_pair = graph.get_last_cycle();
        std::vector<NodeId> cycle_nodes = cyc_pair.first;
        std::vector<std::pair<NodeId, NodeId>> cycle_edges = cyc_pair.second;


		NodeId matched_to_outside_id; // the node in the cycle matched to an outside node

		// find the node in the cycle that has a matching neighbor that is not in th circle. Note that some nodes are partitioned together:
		for (auto node : cycle_nodes){
			 for (auto neighbor : graph.get_node_from_id(node).get_match_neighbors()){
			 	bool is_outside = true;
			 	for (auto cycle_node : cycle_nodes){
			 		if (graph.check_combined(cycle_node, neighbor)){
			 			is_outside=false;
			 		}
			 	}

	            if(is_outside){
	                matched_to_outside_id = node;
	                break;
	            }
			 }
		}


		int found_node = 0; // go through the circle edges twice
		// after having found the out node add or remove the next edges from the matching.
		// Variable is 0 until found and the switch between 1 and 2 counting the parity, until we find the node again.
		// then it is 3 and we are done.

		auto iterate_edges = [&](){
			for (auto edge : cycle_edges){
				if (found_node == 0){
					if (edge.first==matched_to_outside_id or edge.second==matched_to_outside_id){
						found_node = 1;
					} else {
						if (found_node == 1){ // dont add edge to matching
							graph.add_match_edge(edge.first, edge.second);
							found_node = 2;
						} else { //found_node ==2, add edge to matching
							found_node = 1;
							graph.remove_match_edge(edge.first, edge.second);
						}
						if (edge.first==matched_to_outside_id or edge.second==matched_to_outside_id){
							found_node = 3;
						}
					}
				}
			}
			return;
		};
		iterate_edges();
		iterate_edges();	
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
        //std::cout << "Found the edge " << x << " " << y << "\n";

		if (not graph.get_node_from_id(y).is_matched() and not graph.get_node_from_id(y).is_in_tree()){
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
			unshrink(graph);
            ///hier müssen wir erst einmal unshrinken, danach schauen, ob es perfekt ist
			if (2 * graph.get_num_match_edges() == graph.get_num_nodes()){
				//found perfect matching
				std::cout << "We found a p.m.!" << std::endl;
				return true;
			} else {
                graph.init_tree();
			}



		}else if (not graph.get_node_from_id(y).is_in_tree() and graph.get_node_from_id(y).is_matched()){
			//extend
            graph.add_tree_edge(x, y);

		} else {
			//shrink
			std::vector<std::pair<NodeId, NodeId>> path1 = graph.get_tree_path(x);
			std::vector<std::pair<NodeId, NodeId>> path2 = graph.get_tree_path(y);


			NodeId last_shared_nodeid = find_split(path1, path2);
			std::pair<NodeId, NodeId> con_edge = {x,y};
			std::cout << "added cycles";
			graph.add_cycle(path1, path2, con_edge, last_shared_nodeid);

			int parity;
			if (graph.get_node_from_id(path1[last_shared_nodeid].first).is_odd()){
					parity = 0;
				} else if (graph.get_node_from_id(path1[last_shared_nodeid].first).is_even()){
					parity = 1;
				} else{
					parity = 2;
			}	

			for (NodeId i = last_shared_nodeid; i < path1.size(); ++i){
                graph.remove_tree_edge(path1[i].first, path1[i].second);
				
                graph.combine(path1[i].first, path1[i].second, parity);
			}
			for (NodeId i = last_shared_nodeid; i < path2.size(); ++i){
                graph.remove_tree_edge(path2[i].first, path2[i].second);
				
				graph.combine(path2[i].first, path2[i].second, parity);
			}
		}
	}
	return false;
}
/*
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
}*/