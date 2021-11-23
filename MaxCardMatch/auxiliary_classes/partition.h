#ifndef PARTITION_HPP
#define PARTITION_HPP


#include <vector>
#include <map>

template <typename T>
class Partition{
public:
	// this implements a union find structure
	Partition(std::vector<T> element_vec);
	Partition(T num_nodes);

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


template <typename T>
Partition<T>::Partition(std::vector<T> element_vec){
    for (auto element:element_vec) {
        _parent[element] = element;
    }
}
template <typename T>
Partition<T>::Partition(T num_nodes) {
    std::vector<T> element_vec(num_nodes);
    for(T i = 0; i < num_nodes; i++){
        element_vec[i] = i;
    }
    for (auto element:element_vec) {
        _parent[element] = element;
    }
}

template <typename T>
void Partition<T>::combine(T x, T y) {

    T rootX = find_rep(x);
    T rootY = find_rep(y);

    if (_depth[rootX] < _depth[rootY]) {

        T temp = rootX;
        rootX = rootY;
        rootY = temp;

    } else if (_depth[rootX] == _depth[rootY]) {

        _depth[rootX]++;

    }

    _parent[rootY] = rootX;

}

template <typename T>
int Partition<T>::find_rep(T x) {
    if (x == _parent[x]) {
        return x;

    } else {
        _parent[x] = find_rep(_parent[x]);
        return _parent[x];

    }
}

template <typename T>
bool Partition<T>::check_combined(T x, T y) {
    return find_rep(x) == find_rep(y);
}

#endif /* PARTITION_HPP */
