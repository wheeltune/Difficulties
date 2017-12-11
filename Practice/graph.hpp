#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct Graph {
    Graph(int num_vertexes, int edge_chance = 0.5) {
        graph_.clear();
        graph_.resize(num_vertexes);

        for (int v1 = 0; v1 < num_vertexes; ++v1) {
            for (int v2 = v1 + 1; v2 < num_vertexes; ++v2) {
                bool is_edge_exists = ((double)rand() / RAND_MAX) < edge_chance;
                if (is_edge_exists) {
                    graph_[v1].append(v2);
                    graph_[v2].append(v1);
                }
            }
        }
    }

    Graph(std::string file_name) {
        std::ifstream input_stream(file_name);
        int n, m;
        input_stream >> n >> m;

        graph_.clear();
        graph_.resize(n);
        for (int i = 0; i < n; ++i) {
            int v1, v2;
            std::cin >> v1 >> v2;
            graph_[v1].append(v2);
            graph_[v2].append(v2);
        }
    }

    std::vector<std::vector<int>> graph_;
}