#ifndef COLORING3_HPP
#define COLORING3_HPP

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

class Graph {
public:
    Graph(std::string filename);
    Graph(int vertex_count, double edge_chance);
    ~Graph();

    bool is3Colorable();
    bool is3ColorableSimple();

    void store(std::string filename);
private:
    std::vector<std::vector<int>> adjacency_list_;

    class Colorability3;
    class Colorability3Simple;
};

class Graph::Colorability3Simple {
public:
    Colorability3Simple(const std::vector<std::vector<int>>* adjacency_list);
    ~Colorability3Simple();

    bool check();
private:
    int vertex_count_;
    const std::vector<std::vector<int>>* adjacency_list_;
    std::vector<char> colors_;

    bool generateNext();
    bool isCorrect();
};

class Graph::Colorability3 {
public:
    Colorability3(const std::vector<std::vector<int>>* adjacency_list);
    ~Colorability3();

    bool check();
private:
    int vertex_count_;
    std::vector<int> vertexes_degree_;

    const std::vector<std::vector<int>>* adjacency_list_;
    std::vector<int> used_; 
    int used_count_;
    std::vector<bool> independent_;

    void printDegree();
    void printUsed();
    void printIndependent();

    int argmaxVertexDegree();
    int argminVertexDegree();

    bool isRest2Colorable();

    void eraseVertexesDegree(int v);
    void insertVertexesDegree(int v);

    void use(int v);
    void unuse(int v);
    void useNeighbors(int v);
    void unuseNeighbors(int v);
    void useWithNeighbors(int v);
    void unuseWithNeighbors(int v);

    bool MIS(int k);
};

#endif // COLORING3_HPP