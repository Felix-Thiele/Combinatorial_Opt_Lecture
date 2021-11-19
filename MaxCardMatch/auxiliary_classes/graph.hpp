#ifndef GRAPH_HPP
#define GRAPH_HPP

/**
   @file graph.hpp

   @brief This file provides a simple class @c Graph to model undirected graphs, written specifically for the purpose of
   implementing Edmonds' Cardinality Matching Algorithm.
**/

#include "node.hpp"
#include "partition.hpp"




class Graph
{
public:


   Graph (size_type num_nodes);

   size_type get_num_nodes ();
   size_type get_num_edges ();
   size_type get_num_tree_edges ();
   size_type get_num_match_edges ();


   Node & get_node_from_id (NodeId id);


   void add_edge (NodeId node1_id, NodeId node2_id);
   void add_tree_edge (NodeId node1_id, NodeId node2_id);
   void add_match_edge (NodeId node1_id, NodeId node2_id);

   void remove_tree_edge (NodeId node1_id, NodeId node2_id);
   void remove_match_edge (NodeId node1_id, NodeId node2_id);

   bool contains_match_edge (NodeId node1_id, NodeId node2_id);


   int  odd_even (NodeId node1_id); // this function checks if a node is odd-0, even-1, or neither-2


   bool init_tree (); //return false if we already have a matching


   //combine two nodes
   void combine(NodeId node1_id, NodeId node2_id, int evenodd);

   // check if combined
   bool check_combined(NodeId node1_id, NodeId node2_id);


private:

   std::vector<Node> _nodes;
   size_type _num_edges;

   Partition<NodeId> _partition;

   NodeId _root;
   std::map<NodeId, std::vector<std::pair<NodeId, NodeId>>> _tree_paths;

   std::vector<std::pair<NodeId, NodeId>> _match_edges;
   size_type _num_match_edges;

}; // class Graph


#endif /* GRAPH_HPP */
