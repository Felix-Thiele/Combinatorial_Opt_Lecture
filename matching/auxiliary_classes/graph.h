#ifndef GRAPH_HPP
#define GRAPH_HPP

/**
   @file graph.hpp

   @brief This file provides a simple class @c Graph to model undirected graphs, written specifically for the purpose of
   implementing Edmonds' Cardinality Matching Algorithm.
**/

#include "node.h"
#include "partition.h"
#include <iostream>

typedef unsigned LargeUnsigned;
typedef LargeUnsigned size_type;
typedef size_type NodeId;

class Graph
{
public:
   Graph (size_type num_nodes);

   size_type get_num_nodes();
   size_type get_num_edges();
   size_type get_num_tree_edges();
   size_type get_num_match_edges();
   size_type get_num_removed();

   bool is_matched(NodeId n);
   bool is_in_tree(NodeId n);
   bool has_exposed_node();

   Partition<NodeId>& partition();

   std::vector<std::pair<NodeId, NodeId>> get_tree_path(NodeId x);

   Node & get_node_from_id (NodeId id);

   void add_edge (NodeId node1_id, NodeId node2_id);
   void add_tree_edge (NodeId node1_id, NodeId node2_id);
   void add_match_edge (NodeId node1_id, NodeId node2_id);

   bool has_nodes();

   bool has_cycle();
   void add_cycle(std::vector<std::pair<NodeId, NodeId>>& path1,
                  std::vector<std::pair<NodeId, NodeId>>& path2,
                  std::pair<NodeId, NodeId> last_edge,
                  int last_common_index);
   std::pair< std::vector<NodeId>, std::vector<std::pair<NodeId, NodeId>> > get_last_cycle();

   void remove_tree_edge (NodeId node1_id, NodeId node2_id);
   void remove_match_edge (NodeId node1_id, NodeId node2_id);

   bool contains_match_edge (NodeId node1_id, NodeId node2_id);

   int  odd_even (NodeId node1_id); // this function checks if a node is odd-0, even-1, or neither-2

   bool init_tree (); //return false if we already have a matching

   //combine two nodes
   void combine(NodeId node1_id, NodeId node2_id, int evenodd);

   void combine(std::vector<NodeId> vec, int evenodd);

   // check if combined
   bool check_combined(NodeId node1_id, NodeId node2_id);

   auto get_nodes() -> std::vector<Node>&;
   auto get_matching() -> std::vector<std::pair<NodeId, NodeId>>&;

   void print_matching();

   void set_last_matching();
   std::vector<std::pair<NodeId, NodeId>>& get_last_matching();

   void add_final_matching_edge(std::pair<NodeId, NodeId> p);


   void remove_node(NodeId n){
       std::cout << n << " ";
       _nodes[n].remove();
       _removed_count++;
   }


private:

   NodeId _removed_count = 0;

   std::vector<Node> _nodes;
   size_type _num_edges = 0;

   size_type _num_non_deleted_nodes;
   std::vector<NodeId> _deleted_nodes;

   Partition<NodeId> _partition;

   NodeId _root;
   std::map<NodeId, std::vector<std::pair<NodeId, NodeId>>> _tree_paths;

   std::vector<std::pair<NodeId, NodeId>> _match_edges;
   size_type _num_match_edges = 0;

    std::vector<std::pair<NodeId, NodeId>> _matching_before_building_last_tree;

   std::vector<std::vector<NodeId>> _cycles;
   std::vector<std::vector<std::pair<NodeId,NodeId>>> _cycle_edges;

   std::vector<std::pair<NodeId, NodeId>> _final_matching_edges;

}; // class Graph


#endif /* GRAPH_HPP */