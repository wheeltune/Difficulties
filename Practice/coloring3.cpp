#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "coloring3.hpp"

Graph::Graph(std::string filename) {
    std::ifstream is(filename);
    int vertex_count, edges_count;
    is >> vertex_count >> edges_count;

    adjacency_list_.resize(vertex_count);
    for (int i = 0; i < edges_count; ++i) {
        int from, to;
        is >> from >> to;

        adjacency_list_[from].push_back(  to);
        adjacency_list_[  to].push_back(from);
    }
}

Graph::Graph(int vertex_count, double edge_chance) {
    srand(time(NULL));

    adjacency_list_.resize(vertex_count);

    for (int from = 0; from < vertex_count; ++from) {
        for (int to = from + 1; to < vertex_count; ++to) {
            if ((double) rand() / RAND_MAX < edge_chance) {
                adjacency_list_[from].push_back(  to);
                adjacency_list_[  to].push_back(from);
            }
        }
    }
}

Graph::~Graph() {
    for (int v = 0; v < adjacency_list_.size(); v++) {
        adjacency_list_[v].clear();
    }
    adjacency_list_.clear();
}

bool Graph::is3Colorable() {
    return Colorability3(&adjacency_list_).check();
}

bool Graph::is3ColorableSimple() {
    return Colorability3Simple(&adjacency_list_).check();
}

void Graph::store(std::string filename) {
    std::ofstream of(filename);
    of << adjacency_list_.size();

    int edges_count = 0;
    for (int v = 0; v < adjacency_list_.size(); v++) {
        edges_count += adjacency_list_[v].size();
    }

    of << edges_count << std::endl;

    for (int v = 0; v < adjacency_list_.size(); v++) {
        for (int i = 0; i < adjacency_list_[v].size(); i++) {
            of << v << " " << adjacency_list_[v][i];
        }
    }
}

Graph::Colorability3Simple::Colorability3Simple(const std::vector<std::vector<int>>* adjacency_list) {
    adjacency_list_ = adjacency_list;

    vertex_count_ = adjacency_list_->size();
}

Graph::Colorability3Simple::~Colorability3Simple() {
    colors_.clear();
}

bool Graph::Colorability3Simple::check() {
    colors_.resize(vertex_count_, 0);

    while (generateNext()) {
        if (isCorrect()) {
            return true;
        }
    }
    return false;
}

bool Graph::Colorability3Simple::generateNext() {
    colors_[0]++;
    int i = 0;
    while (colors_[i] > 2 && i < vertex_count_ - 1) {
        colors_[i] = 0;
        colors_[i + 1]++;
        i++;
    }
    if (colors_[i] > 2) {
        colors_[i] = 0;
        return false;
    }
    return true;
}

bool Graph::Colorability3Simple::isCorrect() {
    for (int from = 0; from < vertex_count_; ++from) {
        for (auto to_it = (*adjacency_list_)[from].begin();
             to_it != (*adjacency_list_)[from].end();
             to_it++)
        {
            if (colors_[from] == colors_[*to_it]) {
                return false;
            }
        }
    }
    return true;
}

Graph::Colorability3::Colorability3(const std::vector<std::vector<int>>* adjacency_list) {
    adjacency_list_ = adjacency_list;
    vertex_count_ = adjacency_list_->size();

    used_.resize(vertex_count_, false);
    used_count_ = 0;
    independent_.resize(vertex_count_, false);
    vertexes_degree_.resize(vertex_count_);
    for (int v = 0; v < vertex_count_; v++) 
    {
        vertexes_degree_[v] = (*adjacency_list_)[v].size();
    }
}

Graph::Colorability3::~Colorability3() {
    vertexes_degree_.clear();
    used_.clear();
    independent_.clear();
}

bool Graph::Colorability3::check() {
    return MIS((vertex_count_ + 2) / 3);
}

void Graph::Colorability3::printDegree() {
    for (int i = 0; i < vertex_count_; ++i) {
        std::cout << vertexes_degree_[i] << " ";
    }
    std::cout << std::endl;
}

void Graph::Colorability3::printUsed() {
    for (int i = 0; i < vertex_count_; ++i) {
        std::cout << used_[i] << " ";
    }
    std::cout << std::endl;
}

void Graph::Colorability3::printIndependent() {
    for (int i = 0; i < vertex_count_; ++i) {
        std::cout << independent_[i] << " ";
    }
    std::cout << std::endl;
}

int Graph::Colorability3::argmaxVertexDegree() {
    int ans = -1;
    for (int v = 0; v < vertex_count_; ++v) {
        if (used_[v] == 0) {
            if (ans == -1 || 
                vertexes_degree_[ans] < vertexes_degree_[v]) 
            {
               ans = v;
            }
        }
    }
    return ans;
}

int Graph::Colorability3::argminVertexDegree() {
    int ans = -1;
    for (int v = 0; v < vertex_count_; ++v) {
        if (used_[v] == 0) {
            if (ans == -1 || 
                vertexes_degree_[ans] > vertexes_degree_[v])  
            {
               ans = v;
            }
        }
    }
    return ans;
}

bool Graph::Colorability3::isRest2Colorable() {
    std::vector<char> colors(vertex_count_, 0);

    std::vector<int> vertices_stack;
    for (int v = 0; v < vertex_count_; v++) {
        if (!independent_[v] && colors[v] == 0) {
            colors[v] = 1;
            vertices_stack.push_back(v);

            while (vertices_stack.size() != 0) {
                int from = vertices_stack.back();
                vertices_stack.pop_back();

                for (auto to_it = (*adjacency_list_)[from].begin(); 
                     to_it < (*adjacency_list_)[from].end(); 
                     ++to_it) 
                {
                    int to = *to_it;
                    if (!independent_[to]) {
                        if (colors[to] != 0) {
                            if (colors[to] == colors[from]) {
                                return false;
                            }
                        } else {
                            colors[to] = (colors[from] == 1 ? 2 : 1);
                            vertices_stack.push_back(to);
                        }
                    }
                }
            }
        }
    }
    return true;
}

void Graph::Colorability3::eraseVertexesDegree(int v) {
    for (auto to_it = (*adjacency_list_)[v].begin();
         to_it != (*adjacency_list_)[v].end();
         to_it++)
    {
        vertexes_degree_[*to_it]--;
    }
}

void Graph::Colorability3::insertVertexesDegree(int v) {
    for (auto to_it = (*adjacency_list_)[v].begin();
         to_it != (*adjacency_list_)[v].end();
         to_it++)
    {
        vertexes_degree_[*to_it]++;
    }
}

void Graph::Colorability3::use(int v) {
    if (used_[v] == 0) {
        used_count_++;
        eraseVertexesDegree(v);
    }
    used_[v]++;
}

void Graph::Colorability3::unuse(int v) {
    used_[v]--;
    if (used_[v] == 0) {
        used_count_--;
        insertVertexesDegree(v);
    }
}

void Graph::Colorability3::useNeighbors(int v) {
    for (auto to_it = (*adjacency_list_)[v].begin();
         to_it != (*adjacency_list_)[v].end(); 
         to_it++)
    {
        use(*to_it);
    }
}

void Graph::Colorability3::unuseNeighbors(int v) {
    for (auto to_it = (*adjacency_list_)[v].begin();
         to_it != (*adjacency_list_)[v].end(); 
         to_it++)
    {
        unuse(*to_it);
    }
}

void Graph::Colorability3::useWithNeighbors(int v) {
    use(v);
    useNeighbors(v);
}

void Graph::Colorability3::unuseWithNeighbors(int v) {
    unuse(v);
    unuseNeighbors(v);
}

bool Graph::Colorability3::MIS(int k) {
    if (used_count_ == vertex_count_ || k == 0) {
        if (isRest2Colorable()) {
            return true;
        }
    } else if (k > 0) {
        int v = argmaxVertexDegree();

        if (vertexes_degree_[v] >= 3) {
            useWithNeighbors(v);    

            independent_[v] = true;
            if (MIS(k - 1)) {
                return true;
            }
            independent_[v] = false;

            unuseNeighbors(v);

            if (MIS(k)) {
                return true;
            }
            
            unuse(v);
        } else {
            v = argminVertexDegree();

            useWithNeighbors(v);

            independent_[v] = true;
            if (MIS(k - 1)) {
                return true;
            }
            independent_[v] = false;

            unuseWithNeighbors(v);

            for (auto to_it = (*adjacency_list_)[v].begin();
                 to_it != (*adjacency_list_)[v].end();
                 to_it++) 
            {
                int to = *to_it;
                if (!used_[to]) {
                    useWithNeighbors(to);

                    independent_[to] = true;
                    if (MIS(k - 1)) {
                        return true;
                    }
                    independent_[to] = false;

                    unuseWithNeighbors(to);
                }
            }
        }
    }
    return false;
}