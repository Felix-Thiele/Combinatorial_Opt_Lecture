#include "node.hpp"


Node::size_type Node::degree ()
{
   return _neighbors.size();
}

Node::size_type Node::tree_degree ()
{
   return _tree_neighbors.size();
}

Node::size_type Node::matching_degree ()
{
   return _match_neighbors.size();
}

std::vector<NodeId> & Node::get_neighbors ()
{
   return _neighbors;
}

std::vector<NodeId> & Node::get_tree_neighbors ()
{
   return _tree_neighbors;
}

std::vector<NodeId> & Node::get_match_neighbors ()
{
   return _match_neighbors;
}

void Node::add_neighbor (NodeId id)
{
   _neighbors.push_back(id);
}

void Node::add_tree_neighbor (NodeId id)
{
   _tree_neighbors.push_back(id);
}

void Node::add_match_neighbor (NodeId id)
{
   _match_neighbors.push_back(id);
}

void Node::remove_neighbor (NodeId id)
{
	if(std::find(_neighbors.begin(), _neighbors.end(), id) != _neighbors.end()){
		_neighbors.erase (std::find(_neighbors.begin(), _neighbors.end(), id));
	}
}

void Node::remove_tree_neighbor (NodeId id)
{
	if(std::find(_tree_neighbors.begin(), _tree_neighbors.end(), id) != _tree_neighbors.end()){
		_tree_neighbors.erase (std::find(_tree_neighbors.begin(), _tree_neighbors.end(), id));
	}
}

void Node::remove_match_neighbor (NodeId id)
{
	if(std::find(_match_neighbors.begin(), _match_neighbors.end(), id) != _match_neighbors.end()){
		_match_neighbors.erase (std::find(_match_neighbors.begin(), _match_neighbors.end(), id));
	}
}


bool Node::is_matched(){
	return _is_matched;
}

bool Node::is_even(){
	return _is_even;
}

bool Node::is_odd(){
	return _is_odd;
}


void Node::set_is_matched(bool to){
	_is_matched = to;
	return;
}

void Node::set_even(bool to){
	_is_even = to;
	_is_odd = not to;
	return;
}

void Node::set_odd(bool to){
	_is_odd = to;
	_is_even = not to;
	return;
}
