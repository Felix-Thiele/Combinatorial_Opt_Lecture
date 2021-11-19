#ifndef PARTITION_HPP
#define PARTITION_HPP


#include <vector>
#include <map>


template <typename T> 

class Partition{
public:
	// this implements a union find structure
	Partition(std::vector<T> element_vec);

	//combine two nodes
	void combine(T x, T y);

	//finds a nodes representant
	int find_rep(T x);

	// check if combined
	bool check_combined(T x, T y);

private:

	std::map<T,T> _parent; //This stores the index of each nodes parent
	std::map<T, int> _depth; //This stores upper bounds for the actual depth of the arborescence rooted in each node.

};

#endif /* PARTITION_HPP */
