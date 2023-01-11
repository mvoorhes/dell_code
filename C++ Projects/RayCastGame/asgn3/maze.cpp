#include <iostream>
#include <fstream>
#include <getopt.h>
#include <queue>
#include <vector>
#include <algorithm>
#include <functional>
#include <set>

#include "grid.h"
#include "util.h"

std::string usage();

#define OPTS "i:o:p:h"
#define MAX_INT 4294967295

// Initialize MST, weight, and Vertices vectors in O(n^2) for loop
// Get random node, set MST value to true, add to priority queue
// While there is stuff in priority queue
//  add top element to MST
//  Run through top elements adjacency list
//  If we find edge between top element and adjacent point that is smaller than current weight, 
//  update edge, push adjacent point to priority queue
// Go through list of vertices, check if down and right edges are in small_weights list

// struct Edge {
//     Point vertex;               // main vertex
//     Point edge;                 // vertex providing cheapest edge
//     Grid::weight_t weight;      // The cheapest edge
//     std::vector<Edge> adj;    // adjacency list of other edges
// };

struct Edge {
    Point main;
    Grid::weight_t weight;
    Point neighbor;
};

bool compare_weights(Edge a, Edge b) {
    return a.weight > b.weight;
}

// void setAdjList(Edge *p, Grid g) {
//     for (unsigned k = 0; k < 4; k++) {
//         Edge ad;
//         if (k < 2) {            // y edges (up & down)
//             ad.vertex.x = (*p).vertex.x;
//         } else {                // x edges (left & right)
//             ad.vertex.y = (*p).vertex.y;
//         }
//         if (k == 0) {           // up edge
//             ad.vertex.y = (*p).vertex.y - 1;
//         } else if (k == 1) {    // down edge
//             ad.vertex.y = (*p).vertex.y + 1;
//         } else if (k == 2) {    // left edge
//             ad.vertex.x = (*p).vertex.x - 1;
//         } else {                // right edge
//             ad.vertex.x = (*p).vertex.x + 1;
//         }
//         ad.weight = MAX_INT;
//         if ((ad.vertex.x < g.getWidth() && ad.vertex.y < g.getHeight())) {
//             // filter out of bounds vertices out of list
//             (*p).adj.push_back(ad);
//         }
//     }
// }

int main(int const argc, char *argv[]) {
    exec::execname(argv[0]);

    // Options
    std::string infile = "-";
    std::string outfile = "-";
    std::string imgfile;

    // Getopt
    int opt;
    while ((opt = getopt(argc, argv, OPTS)) != -1) {
        switch (opt) {
            case 'i': infile = optarg; break;
            case 'o': outfile = optarg; break;
            case 'p': imgfile = optarg; break;
            case 'h': std::cout << usage(); return 0;
            default: std::cerr << usage(); return 1;
        }
    }

    Grid g(1, 1);

    if (infile == "-") {
        g = Grid::load(std::cin);
    } else {
        std::ifstream in {infile};
        g = Grid::load(in);
    }

    std::vector<Point> V;       // List of vertices we can access
    std::vector<std::vector<bool>> inMST;                   // Checks if node is in MST
    std::vector<std::vector<Grid::weight_t>> small_weight;  // vector gets the smallest weight of each node

    // Prims Algorithm
    inMST.resize(g.getHeight());
    small_weight.resize(g.getHeight());
    for (unsigned i = 0; i < g.getHeight(); i++) {
        inMST[i].resize(g.getWidth());
        small_weight[i].resize(g.getWidth());
        for (unsigned j = 0; j < g.getWidth(); j++) {
            inMST[i][j] = false;
            small_weight[i][j] = MAX_INT;
            Point e;
            e.x = j;
            e.y = i;
            V.push_back(e);
        }
    }

    std::priority_queue<Edge, std::vector<Edge>, std::function<bool(Edge, Edge)>> Q(compare_weights);
    Edge current;     // edge u in pseudocode
    Point po;

    current.main.x = rand() % g.getWidth();
    current.main.y = rand() % g.getHeight();
    Q.push(current);

    while (Q.size() > 0) {
        current = Q.top();
        Q.pop();

        if (inMST[current.main.y][current.main.x] == false) {
            // add neighbors to priority queue
            current.neighbor.x = current.main.x + 1;
            current.neighbor.y = current.main.y;
            current.weight = g.getEdge(current.main, current.neighbor);
            if (current.weight != Grid::NOEDGE) {
                Q.push(current);
            }
            current.neighbor.x = current.main.x;
            current.neighbor.y = current.main.y + 1;
            current.weight = g.getEdge(current.main, current.neighbor);
            if (current.weight != Grid::NOEDGE) {
                Q.push(current);
            }
            current.neighbor.x = current.main.x - 1;
            current.neighbor.y = current.main.y;
            current.weight = g.getEdge(current.main, current.neighbor);
            if (current.weight != Grid::NOEDGE) {
                Q.push(current);
            }
            current.neighbor.x = current.main.x;
            current.neighbor.y = current.main.y - 1;
            current.weight = g.getEdge(current.main, current.neighbor);
            if (current.weight != Grid::NOEDGE) {
                Q.push(current);
            }
            inMST[current.main.y][current.main.x] = true;
        }
    }

    // current.vertex.x = rand() % g.getWidth();
    // current.vertex.y = rand() % g.getHeight();

    // current.weight = MAX_INT;
    // current.edge.x = -1;
    // current.edge.y = -1;
    // inMST[current.vertex.y][current.vertex.x] = true;
    // // small_weight[current.vertex.y][current.vertex.x] = MAX_INT;
    // setAdjList(&current, g);
    // Q.push(current);

    // while (Q.size() > 0) {
    //     current = Q.top();
    //     Q.pop();
    //     if (current.weight > small_weight[current.vertex.y][current.vertex.x]) {
    //         continue;
    //     }
    //     inMST[current.vertex.y][current.vertex.x] = true;
    //     for (unsigned i = 0; i < current.adj.size(); i++) {
    //         Edge temp = current.adj[i];
    //         Grid::weight_t temp_w = g.getEdge(current.vertex, temp.vertex);
    //         // If adjacent vertex isn't in the MST and 
    //         // the edge is lower than the currently recognized edge,
    //         // push to priority queue 
    //         if (inMST[temp.vertex.y][temp.vertex.x] == false && 
    //             temp_w < temp.weight && 
    //             temp_w < small_weight[temp.vertex.y][temp.vertex.x]) {
    //             current.adj[i].edge = current.vertex;
    //             current.adj[i].weight = temp_w;
    //             small_weight[temp.vertex.y][temp.vertex.x] = temp_w;
    //             setAdjList(&current.adj[i], g);
    //             Q.push(current.adj[i]);
    //         }
    //     }
    // }

    Point q;
    Point d;    // Down node
    Point r;    // Right node
    bool found_right = false;
    bool found_down = false;

    for (unsigned i = 0; i < V.size(); i++) {
        d.x = V[i].x;
        d.y = V[i].y + 1;
        r.x = V[i].x + 1;
        r.y = V[i].y;

        Grid::weight_t down_w = g.getEdge(V[i], d);
        Grid::weight_t right_w = g.getEdge(V[i], r);

        if (down_w == Grid::NOEDGE && right_w == Grid::NOEDGE) {
            continue;
        }

        if ((small_weight[V[i].y][V[i].x] == right_w) ||
            (r.x < g.getWidth() && small_weight[r.y][r.x] == right_w)) {
            found_right = true;
        } 
        if ((small_weight[V[i].y][V[i].x] == down_w) || 
            (d.y < g.getHeight() && small_weight[d.y][d.x] == down_w)) {
            found_down = true;
        }

        if (!found_right) {
            g.setEdge(V[i], r, Grid::NOEDGE);
        }
        if (!found_down) {
            g.setEdge(V[i], d, Grid::NOEDGE);
        }
        found_right = false;
        found_down = false;
    }

    // Test to see if I'm good at opening files
    if (outfile == "-") {
        g.serialize(std::cout);
    } else {
        std::ofstream out {outfile};
        g.serialize(out);
    }

    if (imgfile == "-") {
        g.draw(std::cout);
    } else {
        std::ofstream out {imgfile};
        g.draw(out);
    }

    // Fin
    return exec::status();
}

std::string usage() {
    return std::string{}
        + "SYNOPSIS\n"
        + "     Given a randomized \"grid-graph\", generate a maze by creating a\n"
        + " minimum spanning tree\n"
        + "\n"
        + "USAGE\n"
        + "     " + exec::execname() + " [-" + OPTS + "]\n"
        + "\n"
        + "OPTIONS\n"
        + "     -i infile           input file with serialized grid data [default: stdin]\n"
        + "     -o outfile          maze output in serialized form [default: stdout]\n"
        + "     -p img.ppm          generate ppm image of maze\n"
        + "     -h                  Print out helpful information\n"
        ;
}
