#ifndef PERF_MATCHING_H
#define PERF_MATCHING_H

#include <vector>
#include <iostream>
#include <fstream>
#include "auxiliary_classes/graph.h"

bool find_edge(Graph& graph, NodeId& node_x, NodeId& node_y);

void unshrink(Graph& graph);

int find_split(std::vector<std::pair<NodeId, NodeId>>& path1, std::vector<std::pair<NodeId, NodeId>>& path2);

void perfect_matching(Graph& graph);

void matching(Graph& graph);

#endif