#include "auxiliary_classes/graph.h"
#include <iostream>

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
        auto x = graph.get_last_cycle();
        auto cycle_vec = x.first;
        auto cycle_edges_vec = x.second;
        if(graph.get_node_from_id(cycle_vec[0]).is_removed()){
            continue;
        }
        ///first we find the outside neighbor
        NodeId outside_neighbor = graph.get_num_nodes()+1;    ///for debugging
        NodeId matched_to_outside_id = graph.get_num_nodes()+1;    ///for debugging
        for(auto node_id : cycle_vec){
            if(not graph.get_node_from_id(node_id).is_matched()){ continue; }
            NodeId neighbor = graph.get_node_from_id(node_id).get_match_neighbors()[0];
            if((graph.get_node_from_id(node_id).get_match_neighbors().size() > 1)){
                throw "Too many matching edges!";
            };
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
            }
        }
        if(outside_neighbor == graph.get_num_nodes()+1){
            std::cout << "Haven't found the outside neighbor!!!\n";
        }
        int matched_to_outside_index = 0;
        for(int i = 0; i < cycle_vec.size(); i++){
            if(cycle_vec[i] == matched_to_outside_id){
                matched_to_outside_index = i;
                break;
            }
        }
        int matched_to_outside_edge = 0;
        for(int i = 0; i < cycle_edges_vec.size(); i++){
            if(cycle_edges_vec[i].first == matched_to_outside_id){
                matched_to_outside_edge = i;
                break;
            }
        }
        ///now we adjust the matching; traverse the cycle once, add and remove edges from M alternatingly
        int size_edges = cycle_edges_vec.size();
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
/*
void unshrink(Graph& graph){
	while (graph.has_circle()){
		NodeId outNode_id;
		std::vector<NodeId> circ_nodes;
		auto [circ_0, circ_1, circ_2] = graph.last_added_circle();

		for (auto nodepair : circ_0){
			circ_nodes.push_back(nodepair.first);
			circ_nodes.push_back(nodepair.second);
		}
		for (auto nodepair : circ_1){
			circ_nodes.push_back(nodepair.first);
			circ_nodes.push_back(nodepair.second);
		}
		circ_nodes.push_back(circ_2.first);
		circ_nodes.push_back(circ_2.second);

		for (auto node : circ_nodes){
			 for (neighbor : graph.get_node_from_id(node).get_match_neighbors()){
			 	if(not std::find(circ_nodes.begin(), circ_nodes.end(), neighbor) != circ_nodes.end()){
                    outNode_id = node;
			 	}
			 }
		}

		int found_node = 0; // go through the circle edges twice
		// after having found the out node add or remove the next edges from the matching.
		// Variable is 0 until found and the switch between 1 and 2 counting the parity, until we find the node again.
		// then it is 3 and we are done.
		full_path = path1;
		full_path.push_back(edge);
		full_path.insert(full_path.end(), path2.rbegin(), path2.rend()); // funktioniert der reverse iterator... ist das generell hubsch wie ich das gemacht habe?
		full_path.insert(full_path.end(), full_path.begin(), full_path.end()); // klappt das?
		for (auto edge : full_path){
			if (found_node == 0){
				if (edge[0]==outNode_id or edge[1]==outNode_id){
					found_node = 1;
				} else {
					if (found_node == 1){ // dont add edge to matching
						graph.add_match_edge(edge);
						found_node = 2;
					} else { //found_node ==2, add edge to matching
						found_node = 1;
						graph.remove_match_edge(edge);
					}
					if (edge[0]==outNode_id or edge[1]==outNode_id){
						found_node = 3;
				}
			}
		}
	}
}
*/

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
		Node& node_x = graph.get_node_from_id(x);
		Node& node_y = graph.get_node_from_id(y);


		if (not graph.is_matched(y) and not graph.is_in_tree(y)){
            //augment
		    //std::cout << "We augment the matching!\n";
            graph.add_match_edge(x,y);
			bool parity_count = true;
			auto path = graph.get_tree_path(x);
			for (auto tree_edge : path){
  				if (parity_count){
                    graph.add_match_edge(tree_edge.first, tree_edge.second);
  				} else {
                    graph.remove_match_edge(tree_edge.first, tree_edge.second);
  				}
  				parity_count = not parity_count;
			}
            unshrink(graph);
			if (2 * graph.get_num_match_edges() == graph.get_num_nodes() - graph.get_num_removed()){
				//found perfect matching (or near perfect, which is fine too)
				return true;
			} else {
                graph.init_tree();
			}
		} else if (not graph.is_in_tree(y) and graph.is_matched(y)){
			//extend
			//std::cout << "We extend the tree!\n";
            graph.add_tree_edge(x, y);
            graph.add_tree_edge(y, node_y.get_match_neighbors()[0]);

		} else {
			//shrink
			//std::cout << "We shrink an odd circuit!\n";
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

std::vector<std::pair<NodeId, NodeId>> the_matching;
int mm = 0;
void delete_tree(Graph& graph){
    for(int nodeid = 0; nodeid < graph.get_num_nodes(); nodeid++){
        auto node = graph.get_node_from_id(nodeid);
        if(graph.is_in_tree(nodeid)){
            for(int node2id = 0; node2id < graph.get_num_nodes(); node2id++){
                auto node2 = graph.get_node_from_id(node2id);
                if(graph.partition().check_combined(nodeid, node2id)){
                    graph.remove_node(node2id);
                    if(node2.is_matched() and node2.get_match_neighbors()[0] < node2id){
                        the_matching.push_back(std::make_pair(node2.get_match_neighbors()[0]+1, node2id));
                        mm = the_matching.size();
                        //std::cout << "e " << node2.get_match_neighbors()[0] << " " << node2id << "\n";
                        graph.remove_match_edge(node2id, node2.get_match_neighbors()[0]);
                    }
                }
            }
        }
    }
    graph.init_tree();
}

void print_rest(Graph& graph){
    for(int i = 0; i < graph.get_num_nodes(); i++){
        auto node = graph.get_node_from_id(i);
        if(not node.is_removed()){
            if(node.is_matched() and node.get_match_neighbors()[0] < i){
                if(graph.get_node_from_id(node.get_match_neighbors()[0]).is_removed()){
                    continue;
                }
                the_matching.push_back( std::make_pair(node.get_match_neighbors()[0]+1, i+1) );
            }
        }
    }
    for(int i = 0; i < the_matching.size(); i++){
        auto pair = the_matching[i];
        bool nnew = true;
        for(auto pair2: the_matching){
            if(pair.first == pair2.first and pair.second == pair2.second){
                continue;
            }
            if(pair.first == pair2.first or
               pair.first == pair2.second or
               pair.second == pair2.first or
               pair.second == pair2.second) {
                nnew = false;
                std::cout << "e " << pair.first << " " << pair.second << "\n";
                std::cout << "e " << pair2.first << " " << pair2.second << "\n\n";
                //the_matching.erase(the_matching.begin()+i);
                break;
            }
        }
        if (not nnew){ continue; }
        //std::cout << "e " << pair.first << " " << pair.second << "\n";
    }
    std::cout << "Size of the matching: " << the_matching.size() << "\n";
}

void matching(Graph& graph){

	while(graph.has_nodes()){
	    if(not graph.has_exposed_node()){
            print_rest(graph);
            return;
	    }
		bool pm = perfect_matching(graph);
	    if(pm){
            print_rest(graph);
	        return;
	    }
        delete_tree(graph);
	}
}