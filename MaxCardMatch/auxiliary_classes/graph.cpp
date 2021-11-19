#include "graph.hpp"

#include <string>
#include <stdexcept>
#include <algorithm>


void xcp_check (bool condition, std::string msg)
{ if (not condition) { throw std::runtime_error(msg); } }



Graph::Graph (size_type num_nodes):partition(Partition(num_nodes))
{
    std::vector<Node> _nodes(num_nodes);
    _num_edges = 0;
    init_tree();
}




Graph::size_type Graph::get_num_nodes ()
{
   return _nodes.size();
}

Graph::size_type Graph::get_num_edges ()
{
   return _num_edges;
}

Graph::size_type Graph::get_num_tree_edges ()
{
   return _num_tree_edges;
}

Graph::size_type Graph::get_num_match_edges ()
{
   return _num_match_edges;
}




Node & Graph::get_node_from_id (NodeId id)
{
   return _nodes[id];
}





void Graph::add_edge (NodeId node1_id, NodeId node2_id)
{
   xcp_check(node1_id != node2_id, "ED::Graph class does not support loops!");

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

	return;
}


void Graph::add_match_edge (NodeId node1_id, NodeId node2_id)
{
   
   Node & node1 = _nodes[node1_id];
   Node & node2 = _nodes[node2_id];
	xcp_check(node1_id.is_matched()==false and node2_id.is_matched()==false, "adding an edge to the matching must fulfull both nodes not being matched already");

   node1.add_match_neighbor(node2_id);
   node2.add_match_neighbor(node1_id);

   _num__match_edges += 1;

   std::pair<NodeId, NodeId> edge = {node1_id, node2_id};
   _match_edges.push_back(edge);

	return;
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

	return;
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
	} else (std::find(_match_edges.begin(), _match_edges.end(), edge21) != _match_edges.end()){
		_match_edges.erase (std::find(_match_edges.begin(), _match_edges.end(), edge21));
	}

	_num_match_edges-=1;

	node1.set_is_matched(false);
	node2.set_is_matched(false);

	return;
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
	_tree_paths = {};
	bool found_root=false;
	for(NodeId i = 0; i < _nodes.size(); ++i) {
		_nodes[i].set_even(true);
		_nodes[i].set_odd(true);
		if (not _nodes[i].is_matched()){
			if (not found_root){
				_root=i;
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
	} else {
		_nodes[node1_id].set_even(false);
		_nodes[node2_id].set_even(false);
		_nodes[node1_id].set_odd(false);
		_nodes[node2_id].set_odd(false);

	}
}


// check if combined
bool Graph::check_combined(NodeId node1_id, NodeId node2_id)
{
	return _partition.check_combined(node1_id, node2_id);
}
