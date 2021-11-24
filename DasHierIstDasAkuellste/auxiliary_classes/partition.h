#ifndef PARTITION_HPP
#define PARTITION_HPP


#include <vector>
#include <map>
#include <algorithm>

template <typename T>
class Partition{
public:
	// this implements a union find structure
	Partition(std::vector<T> element_vec);
	Partition(T num_nodes);

	//combine two nodes
    void combine(T x, T y);
    void combine(std::vector<T>& vec);

    std::vector<T> get_partition_class(T x);
    std::vector<T> get_below_partition_class(T x);

    void unshrink(std::vector<T>& vec);

	// check if combined
	bool check_combined(T x, T y);

private:
    std::map<T,std::vector<T>> _partition_class;
    T _num_nodes;
};

template<class T>
std::vector<T> Partition<T>::get_partition_class(T x) {
    std::vector<T> vec;
    auto part_class = _partition_class[x].back();
    for(int i = 0; i < _num_nodes; i++){
        if(_partition_class[i].back() == part_class){
            vec.push_back(i);
        }
    }
    return vec;
}
/*
template<class T>
std::vector<T> Partition<T>::get_below_partition_class(T x) {
    std::vector<T> vec;
    auto part_class = _partition_class[x][_partition_class[x].size()-2];
    for(int i = 0; i < _num_nodes; i++){
        if(_partition_class[i].back() == part_class){
            vec.push_back(i);
        }
    }
    return vec;
}*/

template<class T>
void Partition<T>::combine(T x, T y) {
    _partition_class[y].push_back(_partition_class[x].back());
    _partition_class[x].push_back(_partition_class[x].back());
}
template<class T>
void Partition<T>::combine(std::vector<T>& vec) {
    auto n = _partition_class[vec[0]].back();
    std::vector<T> to_combine;
    for(auto x : vec){
        auto x_vec = get_partition_class(x);
        for(auto y : x_vec){
            to_combine.push_back(y);
        }
    }
    std::sort( to_combine.begin(), to_combine.end() );
    to_combine.erase( unique( to_combine.begin(), to_combine.end() ), to_combine.end() );
    for(auto m : to_combine){
        _partition_class[m].push_back(n);
    }
}

template<class T>
void Partition<T>::unshrink(std::vector<T>& vec) {
    auto part_class = _partition_class[vec[0]].back();
    for(int i = 0; i < _num_nodes; i++){
        if(_partition_class[i].back() == part_class){
            _partition_class[i].pop_back();
        }
        ///popback für alle Klassen in der selben Klasse, geht weil immer letzter kreis geshrinkt
    }
}

template <class T>
bool Partition<T>::check_combined(T x, T y) {
    return _partition_class[x].back() == _partition_class[y].back();
}

template <typename T>
Partition<T>::Partition(std::vector<T> element_vec):_num_nodes(element_vec.size()){
    for (auto element:element_vec) {
        _partition_class[element].push_back(element);
    }
}
template <typename T>
Partition<T>::Partition(T num_nodes):_num_nodes(num_nodes) {
    std::vector<T> element_vec(num_nodes);
    for(int i = 0; i < num_nodes; i++){
        element_vec[i] = i;
    }
    for (auto element:element_vec) {
        _partition_class[element].push_back(element);
    }
}
/*
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

}*/
/*
template <typename T>
int Partition<T>::find_rep(T x) {
    if (x == _parent[x]) {
        return x;

    } else {
        _parent[x] = find_rep(_parent[x]);
        return _parent[x];

    }
}*/
/*
template <typename T>
bool Partition<T>::check_combined(T x, T y) {
    return find_rep(x) == find_rep(y);
}*/

#endif /* PARTITION_HPP */
