#ifndef PERF_MATCHING_H
#define PERF_MATCHING_H

#include <vector>
#include <iostream>
#include <fstream>
#include "auxiliary_classes/graph.h"

void find_edge(Graph& graph, NodeId& node_x, NodeId& node_y, bool found);

bool perfect_matching(Graph& graph);

#endif