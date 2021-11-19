#include "Perf_Matching.h"


void find_edge(Graph Graph, NodeId *node_x, NodeId *node_y, bool found){
	for(NodeId even_node = 0; i < _nodes.size(); ++even_node) {
		if _nodes.even_node.is_even(){
			for (auto neighbor: Graph._nodes[even_node].neighbors()){
				if (not Graph.check_combined(even_node,neighbor) and not Graph._nodes[neighbor].is_odd()){
					node_x = even_node;
					node_y = neighbor;
					found = true;
					return;
				}
			}
		}
	}
	found=false;
	return;
}


void perfect_matching(ED::Graph Graph) {
	// this calculates a perfect matching if it exists

	bool found = true;
	NodeId x;
	NodeId y;


	while (found){

		find_edge(Graph, x, y, found);

		if (not found){
			// has no matching
			return;
		}


		Node node_x = Graph._nodes[x];
		Node node_y = Graph._nodes[y];


		if (not node_y.is_matched() and not node_y.is_tree()){
			//augment
			Graph.add_match_edge(x,y);
			bool parity_count = true;
			for (auto tree_edge:Graph.tree_paths[x]){
  				if (parity_count){
  					Graph.add_match_edge(tree_edge[0], tree_edge[1]);
  				} else {
  					Graph.remove_match_edge(tree_edge[0], tree_edge[1]);
  				}
			}

			if (2*Graph.get_num_match_edges() == num_nodes){
				//found matching
				return;
			} else {
				Graph.init_tree();
			};



		}else if (not node_y.is_tree() and node_y.is_matched()){
			//extend
			Graph.add_tree_edge(x, y);




		} else {
			//shrink
			int counter = -1; // counter is the last same vertex in the paths from r to x and r to y
			std::vector<std::pair<NodeId, NodeId>> path1 = Graph.tree_paths[x];
			std::vector<std::pair<NodeId, NodeId>> path2 = Graph.tree_paths[y];
			while (path1[counter+1]==path2[counter+1]){
				counter++;
			}

			for (int i = counter; i < path1.size(); ++i){
				Graph.remove_tree_edge(path1[i][0], path1[i][1]);
				int parity;
				if (Graph._nodes[path1[i][0]].is_odd()){
					parity = 0;
				} else if (Graph._nodes[path1[i][0]].is_even()){
					parity = 1;
				} else{
					parity = 2;
				}
				Graph.partition.combine(path1[i][0], path1[i][1], parity);
			}
			for (int i = counter; i < path2.size(); ++i){
				Graph.remove_tree_edge(path2[i][0], path2[i][1]);
				int parity;
				if (Graph._nodes[path2[i][0]].is_odd()){
					parity = 0;
				} else if (Graph._nodes[path2[i][0]].is_even()){
					parity = 1;
				} else{
					parity = 2;
				}
				Graph.partition.combine(path2[i][0], path2[i][1], parity);
			}
		}
	}
}