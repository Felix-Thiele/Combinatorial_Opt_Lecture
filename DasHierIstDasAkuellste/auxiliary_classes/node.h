#ifndef NODE_HPP
#define NODE_HPP


#include <vector>
#include <limits>
#include <map>
#include <stdint.h>
#include <string>
#include <stdexcept>
#include <algorithm>

typedef unsigned size_type;
typedef size_type NodeId;

class Node
{
public:

   /** @brief Create an isolated node (you can add neighbors later). **/
   Node () = default;

   /** @return The number of neighbors of this node. **/
   size_type degree ();
   size_type tree_degree ();
   size_type matching_degree ();

   /** @return The array of ids of the neighbors of this node. **/
   std::vector<NodeId> & get_neighbors ();
   std::vector<NodeId> & get_tree_neighbors ();
   std::vector<NodeId> & get_match_neighbors ();
   
   void add_neighbor (NodeId id);
   void add_tree_neighbor (NodeId id);
   void add_match_neighbor (NodeId id);
   
   void remove_neighbor (NodeId id);
   void remove_tree_neighbor (NodeId id);
   void remove_match_neighbor (NodeId id);

   bool is_matched();
   bool is_in_tree();
   bool is_even();
   bool is_odd();
   void set_is_matched(bool to);
   void set_even(bool to);
   void set_odd(bool to);
   void remove(){
       _is_removed = true;
   }
   bool is_removed(){
       return _is_removed;
   }

private:

   std::vector<NodeId> _neighbors = {};
   std::vector<NodeId> _tree_neighbors = {};
   std::vector<NodeId> _match_neighbors = {};

   bool _is_matched;
   bool _is_even = false;
   bool _is_odd = false;
   bool _is_removed = false;

}; // class Node


#endif /* NODE_HPP */