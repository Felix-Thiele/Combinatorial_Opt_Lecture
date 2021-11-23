#include "graph.h"
#include <string>
#include <iostream>
#include <stdexcept>
#include <algorithm>


void xcp_check (bool condition, std::string msg)
{ if (not condition) { throw msg; } }



Graph::Graph (size_type num_nodes):_nodes(num_nodes),_num_non_deleted_nodes(num_nodes),_partition(Partition<NodeId>(num_nodes))
{
    _num_edges = 0;
    init_tree();
}




size_type Graph::get_num_nodes ()
{
   return _nodes.size();
}

size_type Graph::get_num_edges ()
{
   return _num_edges;
}

/*size_type Graph::get_num_tree_edges ()
{
   return _num_tree_edges;
}*/

size_type Graph::get_num_match_edges ()
{
   return _num_match_edges;
}




Node & Graph::get_node_from_id (NodeId id)
{
   return _nodes[id];
}





void Graph::add_edge (NodeId node1_id, NodeId node2_id)
{
   xcp_check(node1_id != node2_id, "Graph class does not support loops!");

   Node & node1 = _nodes[node1_id];
   node1.add_neighbor(node2_id);

   Node & node2 = _nodes[node2_id];
   node2.add_neighbor(node1_id);

   _num_edges += 1;
}


void Graph::add_tree_edge (NodeId node1_id, NodeId node2_id)
{
   Node & node1 = _nodes[node1_id];
   Node & node2 = _nodes[node2_id];

   // node 1 should already be in the tree
   // node 2 should not yet be in the tree
	xcp_check((node1.is_even()==true or node1.is_odd()==true) and node2.is_in_tree()==false, "adding an edge to a tree must have the first node in the edge be in the tree");

   node1.add_tree_neighbor(node2_id);
   node2.add_tree_neighbor(node1_id);


   // set path of node2
   std::vector<std::pair<NodeId, NodeId>> path(_tree_paths[node1_id]);
   std::pair<NodeId, NodeId> nodepair = {node1_id, node2_id};
   path.push_back(nodepair);
   _tree_paths[node2_id] = path;

   	// set parity of node2
	if(node1.is_odd()){
		node2.set_even(true);
	} else if (node1.is_even()){
		node2.set_odd(true);
	}
}


void Graph::add_match_edge (NodeId node1_id, NodeId node2_id)
{
   
   Node & node1 = _nodes[node1_id];
   Node & node2 = _nodes[node2_id];
	xcp_check(not node1.is_matched() and not node2.is_matched(), "adding an edge to the matching must fulfill both nodes not being matched already");
    xcp_check(not node1.is_removed() and not node2.is_removed(), "Adding an edge to a removed neighbor");
   node1.add_match_neighbor(node2_id);
   node2.add_match_neighbor(node1_id);
   node1.set_is_matched(true);
   node2.set_is_matched(true);

   _num_match_edges += 1;

   std::pair<NodeId, NodeId> edge = {node1_id, node2_id};
   _match_edges.push_back(edge);
}

/*
void Graph::add_circle (std::vector<std::pair<NodeId, NodeId>> path1, std::vector<std::pair<NodeId, NodeId>> path2, std::pair<NodeId, NodeId> edge){
	std::tuple<std::vector<std::pair<NodeId, NodeId>>, std::vector<std::pair<NodeId, NodeId>>, std::pair<NodeId, NodeId>> circ = {path1, path2, edge};
	_circles.push_back(circ);
}

std::tuple<std::vector<std::pair<NodeId, NodeId>>, std::vector<std::pair<NodeId, NodeId>>, std::pair<NodeId, NodeId>> Graph::last_added_circle (){
	std::tuple<std::vector<std::pair<NodeId, NodeId>>, std::vector<std::pair<NodeId, NodeId>>, std::pair<NodeId, NodeId>> val = _circles.back();
	_circles.pop_back();
	return val;
}

bool Graph::has_circle(){
	if (_circles.size()>0){
		return true;
	}
	return false;
}*/

bool Graph::has_cycle(){
    return (not _cycles.empty());
}
void Graph::add_cycle(std::vector<std::pair<NodeId, NodeId>>& path1,
                      std::vector<std::pair<NodeId, NodeId>>& path2,
                      std::pair<NodeId, NodeId> last_edge, int last_common_index){
    std::vector<NodeId> cycle;
    std::vector<std::pair<NodeId, NodeId>> cycle_edges;
    for(int i = last_common_index; i < path1.size(); i++){
        cycle.push_back(path1[i].first);
        cycle.push_back(path1[i].second);
        cycle_edges.push_back(path1[i]);
    }
    cycle.push_back(last_edge.first);
    cycle.push_back(last_edge.second);
    cycle_edges.push_back(last_edge);
    for(int i = path2.size()-1; i >= last_common_index; i--){
        cycle.push_back(path2[i].second);
        cycle.push_back(path2[i].first);
        cycle_edges.push_back(std::make_pair(path2[i].second, path2[i].first));
    }
    _cycles.push_back(cycle);
    _cycle_edges.push_back(cycle_edges);
}

std::pair< std::vector<NodeId>, std::vector<std::pair<NodeId, NodeId>> > Graph::get_last_cycle(){
    auto pair = std::make_pair(_cycles.back(), _cycle_edges.back());
    _cycles.pop_back();
    _cycle_edges.pop_back();
    return pair;
}


void Graph::remove_tree_edge (NodeId node1_id, NodeId node2_id)
{
	Node & node1 = _nodes[node1_id];
	Node & node2 = _nodes[node2_id];

	_nodes[node1_id].get_tree_neighbors().erase (std::find(_nodes[node1_id].get_tree_neighbors().begin(), _nodes[node1_id].get_tree_neighbors().end(), node2_id));
	_nodes[node2_id].get_tree_neighbors().erase (std::find(_nodes[node2_id].get_tree_neighbors().begin(), _nodes[node2_id].get_tree_neighbors().end(), node1_id));

	std::pair<NodeId, NodeId> edge12 = {node1_id, node2_id};
	std::pair<NodeId, NodeId> edge21 = {node2_id, node1_id};

	// if node2 is on the path from node1 to the root:
	if(std::find(_tree_paths[node1_id].begin(), _tree_paths[node1_id].end(), edge12) != _tree_paths[node1_id].end() or std::find(_tree_paths[node1_id].begin(), _tree_paths[node1_id].end(), edge21) != _tree_paths[node1_id].end()){
		_tree_paths[node1_id] = {};
		node1.set_even(false);
		node1.set_odd(false);
	} else {
		_tree_paths[node2_id] = {};
		node2.set_even(false);
		node2.set_odd(false);
	}
}



void Graph::remove_match_edge (NodeId node1_id, NodeId node2_id)
{
	Node & node1 = _nodes[node1_id];
	Node & node2 = _nodes[node2_id];

	_nodes[node1_id].get_match_neighbors().erase (std::find(_nodes[node1_id].get_match_neighbors().begin(), _nodes[node1_id].get_match_neighbors().end(), node2_id));
	_nodes[node2_id].get_match_neighbors().erase (std::find(_nodes[node2_id].get_match_neighbors().begin(), _nodes[node2_id].get_match_neighbors().end(), node1_id));

	std::pair<NodeId, NodeId> edge12 = {node1_id, node2_id};
	std::pair<NodeId, NodeId> edge21 = {node2_id, node1_id};

	if(std::find(_match_edges.begin(), _match_edges.end(), edge12) != _match_edges.end()){
		_match_edges.erase (std::find(_match_edges.begin(),_match_edges.end(), edge12));
	} else if (std::find(_match_edges.begin(), _match_edges.end(), edge21) != _match_edges.end()){
		_match_edges.erase (std::find(_match_edges.begin(), _match_edges.end(), edge21));
	}

	_num_match_edges -= 1;

	node1.set_is_matched(false);
	node2.set_is_matched(false);
}

bool Graph::contains_match_edge (NodeId node1_id, NodeId node2_id){
	std::pair<NodeId, NodeId> edge12 = {node1_id, node2_id};
	std::pair<NodeId, NodeId> edge21 = {node2_id, node1_id};
	if(std::find(_match_edges.begin(), _match_edges.end(), edge12) != _match_edges.end()){
		return true;
	}
	if(std::find(_match_edges.begin(), _match_edges.end(), edge21) != _match_edges.end()){
		return true;
	}
	return false;
}

int Graph::odd_even (NodeId node1_id)
{
	Node & node1 = _nodes[node1_id];
	if(node1.is_odd()){
		return 0;
	} else if (node1.is_even()){
		return 1;
	} 
	return 2;
}



bool Graph::init_tree ()
{
    _tree_paths.clear();
    for(auto node : _nodes){
        node.get_tree_neighbors().clear();
    }
	bool found_root=false;
	for(NodeId i = 0; i < _nodes.size(); ++i) {
		_nodes[i].set_even(false);
		_nodes[i].set_odd(false);
        if (not found_root) {
            if (not _nodes[i].is_removed() and not _nodes[i].is_matched()) {
                _root = i;
                found_root = true;
                _nodes[i].set_even(true);
            }
        }

	}
	return found_root;
}

//combine two nodes
void Graph::combine(NodeId node1_id, NodeId node2_id, int evenodd){
	_partition.combine(node1_id, node2_id);
	if (evenodd==0){
		_nodes[node1_id].set_odd(true);
		_nodes[node2_id].set_odd(true);
	} else if (evenodd==1){
		_nodes[node1_id].set_even(true);
		_nodes[node2_id].set_even(true);
	} else {    ///passiert nicht
	/*
		_nodes[node1_id].set_even(false);
		_nodes[node2_id].set_even(false);
		_nodes[node1_id].set_odd(false);
		_nodes[node2_id].set_odd(false);
	 */
	std::cout << "Trying to shrink a node not in the tree!\n";

	}
}


// check if combined
bool Graph::check_combined(NodeId node1_id, NodeId node2_id)
{
	return _partition.check_combined(node1_id, node2_id);
}

auto Graph::get_nodes() -> std::vector<Node>&{
    return _nodes;
}
auto Graph::get_matching() -> std::vector<std::pair<NodeId, NodeId>>&{
    return _match_edges;
}

Partition<NodeId>& Graph::partition(){
    return _partition;
}

std::vector<std::pair<NodeId, NodeId>> Graph::get_tree_path(NodeId x){
    return _tree_paths[x];
}

void Graph::print_matching()
{
    std::cout << "\n";
    std::cout << "p edge " << get_num_nodes() << " " << get_num_match_edges() << "\n";
    for (auto me : _match_edges){
        std::cout << "e " << me.first+1 << " " << me.second+1 << "\n";
    }
    std::cout << "\n";
}
