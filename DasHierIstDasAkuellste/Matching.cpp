#include "auxiliary_classes/graph.h"
#include <iostream>

bool debug2 = false;

bool find_edge(Graph& graph, NodeId& node_x, NodeId& node_y){
	for(NodeId even_node = 0; even_node < graph.get_num_nodes(); ++even_node) {
		if (not graph.get_node_from_id(even_node).is_removed() and graph.get_node_from_id(even_node).is_even()){
			for (auto neighbor: graph.get_node_from_id(even_node).get_neighbors()){
				if (not graph.get_node_from_id(neighbor).is_removed() and
				(not graph.check_combined(even_node,neighbor) and not graph.get_node_from_id(neighbor).is_odd())){
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
    while(graph.has_cycle()){
        /**
         * take the last shrunken cycle and unshrink it
         */
        auto x = graph.get_last_cycle();
        auto cycle_vec = x.first;
        auto cycle_edges_vec = x.second;
        if(graph.get_node_from_id(cycle_vec[0]).is_removed()){
            continue;
        }
        if(debug2) {
            std::cout << "U: ";
            for (auto pair : cycle_edges_vec) {
                std::cout << "(" << pair.first << " " << pair.second << ") ";
            }
            std::cout << ")\n";
        }
        graph.partition().unshrink(cycle_vec);
        /**
         * find the unique node on this cycle that is currently matched
         * and remember one adjacent edge in the cycle
         */
        NodeId outside_neighbor = graph.get_num_nodes()+1;
        NodeId matched_to_outside_id = graph.get_num_nodes()+1;
        for(auto node_id : cycle_vec){
            if(not graph.is_matched(node_id)){ continue; }
            auto part_class = graph.partition().get_partition_class(node_id);
            NodeId neighbor;
            for(auto node2_id : part_class){
                if(graph.get_node_from_id(node2_id).is_matched()){
                    neighbor = graph.get_node_from_id(node2_id).get_match_neighbors()[0];
                    break;
                }
            }
            /*
            bool is_outside = true;
            for(auto node2_id : cycle_vec){
                if( neighbor == node2_id){
                    is_outside = false;
                    break;
                }
            }
            if(is_outside and not(graph.partition().check_combined(neighbor, node_id))){
                outside_neighbor = neighbor;
                matched_to_outside_id = node_id;
                break;
            }*/
            outside_neighbor = neighbor;
            matched_to_outside_id = graph.get_node_from_id(outside_neighbor).get_match_neighbors()[0];
        }
        if(outside_neighbor == graph.get_num_nodes()+1){
            std::cout << "Haven't found the outside neighbor!!!\n";
        }
        int matched_to_outside_edge = 0;
        for(int i = 0; i < cycle_edges_vec.size(); i++){
            if( graph.partition().check_combined(cycle_edges_vec[i].first, matched_to_outside_id) ){
                matched_to_outside_edge = i;
                break;
            }
        }
        /**
         * adjust the matching: we remove and add matching edges,
         * starting at the edge adjacent to the matching and alternating
         */
        int size_edges = cycle_edges_vec.size();
        if(debug2) {
            std::cout << matched_to_outside_id << " " << matched_to_outside_edge << "\n";
        }
        for(int j = 0; j < size_edges; j++){
            if(j%2 == 0){
                graph.remove_match_edge(cycle_edges_vec[(j+matched_to_outside_edge)%size_edges].first,
                                        cycle_edges_vec[(j+matched_to_outside_edge)%size_edges].second);
            } else{
                graph.add_match_edge(cycle_edges_vec[(j+matched_to_outside_edge)%size_edges].first,
                                     cycle_edges_vec[(j+matched_to_outside_edge)%size_edges].second);
            }
        }
    }
}
/**
 * finds the last common edge of two paths, we consider paths in the alternating trees
 * @param path1
 * @param path2
 * @return
 */
int find_split(std::vector<std::pair<NodeId, NodeId>>& path1, std::vector<std::pair<NodeId, NodeId>>& path2){
    if(path1.empty() or path2.empty()){
        return 0;
    }
    int i = 0;
    while (path1[i].second == path2[i].second){
        if(i == path1.size() or i == path2.size()){
            break;
        }
        i++;
    }
    return i;
}

void find_last_matching_edges(Graph& graph){
    std::vector<std::pair<NodeId, NodeId>> m;
    if(2*graph.get_num_match_edges() >= graph.get_num_nodes() - graph.get_num_removed() - 1){
        m = graph.get_matching();
    } else {
        m = graph.get_last_matching();
    }
    for(int i = 0; i < m.size(); i++){
        graph.add_final_matching_edge( std::make_pair(m[i].first, m[i].second) );
    }
}

/**
 * computes a perfect matching or ends with a frustrated tree
 */
bool perfect_matching(Graph& graph) {

	NodeId x = 0;
	NodeId y = 0;

	while (find_edge(graph, x, y)){
        //std::cout << "Found the edge " << x << " " << y << "\n";
		Node& node_x = graph.get_node_from_id(x);
		Node& node_y = graph.get_node_from_id(y);
		if(debug2){
            std::cout << "(" << x << " " << y << "): ";
		}
		if (not graph.is_matched(y) and not graph.is_in_tree(y)){
            //augment
		    //std::cout << "M: augment ";
			bool parity_count = false;
			auto path = graph.get_tree_path(x);
			for (int i = path.size()-1; i >= 0; i--){
			    auto tree_edge = path[i];
                if (parity_count){
                    //graph.add_match_edge(tree_edge.first, tree_edge.second);
  				} else {
                    graph.remove_match_edge(tree_edge.first, tree_edge.second);
  				}
  				parity_count = not parity_count;
			}
			parity_count = false;
            for (int i = path.size()-1; i >= 0; i--){
                auto tree_edge = path[i];
                if(debug2) {
                    std::cout << "(" << tree_edge.second << " " << tree_edge.first << ") ";
                }
                if (parity_count){
                    graph.add_match_edge(tree_edge.first, tree_edge.second);
                } else {
                    //graph.remove_match_edge(tree_edge.first, tree_edge.second);
                }
                parity_count = not parity_count;
            }
            graph.add_match_edge(x,y);
            if(debug2) {
                std::cout << "\n";
            }
            unshrink(graph);
			if (2 * graph.get_num_match_edges() >= graph.get_num_nodes() - graph.get_num_removed() - 1){
                find_last_matching_edges(graph);
				return true;
			} else {
                graph.init_tree();
			}
		} else if (not graph.is_in_tree(y) and graph.is_matched(y)){
			//extend
			//std::cout << "T: (" << x << " " << y << ") (" << y << " " << node_y.get_match_neighbors()[0] << ")\n";
            graph.add_tree_edge(x, y);
            graph.add_tree_edge(y, node_y.get_match_neighbors()[0]);

		} else {
			std::vector<std::pair<NodeId, NodeId>> path1 = graph.get_tree_path(x);
			std::vector<std::pair<NodeId, NodeId>> path2 = graph.get_tree_path(y);


			int last_shared_nodeid = find_split(path1, path2);
			std::pair<NodeId, NodeId> con_edge = {x,y};
			graph.add_cycle(path1, path2, con_edge, last_shared_nodeid);

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
			}
		}
	}
	/**
	 * Problem: right now, when the edge search while loop stops, everything is shrunken, so we dont have any matching edges any more
	 */
	return false;
}

/**
 * deletes the currently stored tree (which will be frustrated),
 * stores and deletes the matching edges inside,
 * then resets the tree
 * @param graph
 */
void delete_tree(Graph& graph){
    std::cout << "!!! D: ";
    for(int nodeid = 0; nodeid < graph.get_num_nodes(); nodeid++){
        auto node = graph.get_node_from_id(nodeid);
        if(graph.is_in_tree(nodeid)){
            for(int node2id = 0; node2id < graph.get_num_nodes(); node2id++){
                auto node2 = graph.get_node_from_id(node2id);
                if(graph.partition().check_combined(nodeid, node2id)){
                    graph.remove_node(node2id);
                    if(node2.is_matched() and node2.get_match_neighbors()[0] < node2id){
                        graph.add_final_matching_edge(std::make_pair(node2.get_match_neighbors()[0]+1, node2id) );
                        graph.remove_match_edge(node2id, node2.get_match_neighbors()[0]);
                    }
                }
            }
        }
    }
    std::cout << "!!!\n";
    graph.init_tree();
}

void matching(Graph& graph){

    /**
     * we continue while not all nodes were at some point removed from the graph,
     * and there are still exposed nodes left
     * We try to compute a PM, if it doesn't exist we delete the M-frustrating tree and store all its matching edges
     * once we find a PM or break out of the loop, we print the matching
     */
	while(graph.has_nodes()){
	    if(not graph.has_exposed_node()){
            find_last_matching_edges(graph);
            break;
	    }
		bool pm = perfect_matching(graph);
	    if(pm){
	        break;
	    }
        delete_tree(graph);
	}
	graph.print_matching();
}