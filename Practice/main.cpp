#include <iostream>
#include "coloring3.hpp"

void runTest(int n, double p) {
    Graph g = Graph(n, p);
    if (g.is3Colorable() != g.is3ColorableSimple()) {
        std::cout << "Wrong\n";
        g.store("wrong_test.txt");
    }
}

void run(int n, double p) {
    Graph g = Graph(n, p);

    clock_t elapsed = clock();;
    
    g.is3Colorable();

    elapsed = clock() - t;

    std::ofstream of("results.txt", std::ofstream::out | std::ofstream::app);
    of << n << " " << p << " " << elapsed << std::endl;
}

int main(int argc, char* argv[]) {
    int n = rand() % 8 + 3;
    double p = (double) rand() / RAND_MAX;

    if (argc > 1) {
        n = atoi(argv[1]);
    }
    if (argc > 2) {
        p = atof(argv[2]);
    }

    run(n, p);
    return EXIT_SUCCESS;
}