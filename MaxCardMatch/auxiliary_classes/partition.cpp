#include "partition.hpp"

template <typename T> 
Partition<T>::Partition(std::vector<T> element_vec){ 
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