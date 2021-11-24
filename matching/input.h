#ifndef INPUT_H
#define INPUT_H

#include <vector>
#include <iostream>
#include <fstream>
#include "auxiliary_classes/graph.h"

Graph read_file();

Graph read_graph(char const* filename);

#endif