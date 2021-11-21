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

void perfect_matching(Graph& graph) {   // this calculates a perfect matching if it exists

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
				return;
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
}