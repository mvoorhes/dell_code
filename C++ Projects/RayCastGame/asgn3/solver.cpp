#include <iostream>
#include <fstream>
#include <getopt.h>
#include <queue>
#include <functional>

#include "grid.h"
#include "util.h"

std::string usage();

#define OPTS "i:o:s:e:p:h"
#define MAX_INT 4294967295

struct newPoint {
    Point vertex;   // Main point
    Point parent;   // Parent node of vertex
    bool explore;   // True if we've seen this point before, false otherwise
    std::vector<newPoint> adj; // Adjacency list of that point
};

void setAdjList(newPoint *p, Grid g) {
    for (unsigned k = 0; k < 4; k++) {
        newPoint q;
        q.parent = (*p).vertex;
        if (k < 2) {            // up & down widths = p widths
            q.vertex.x = (*p).vertex.x;
        } else {                // left & right
            q.vertex.y = (*p).vertex.y;
        }
        if (k == 0) {           // up edge
            q.vertex.y = (*p).vertex.y - 1;
        } else if (k == 1) {    // down edge
            q.vertex.y = (*p).vertex.y + 1;
        } else if (k == 2) {    // left edge
            q.vertex.x = (*p).vertex.x - 1;
        } else if (k == 3) {    // right edge
            q.vertex.x = (*p).vertex.x + 1;
        }
        if (g.getEdge((*p).vertex, q.vertex) != Grid::NOEDGE) {
            // If the edge isn't a wall, we can add it to the adjacency list
            (*p).adj.push_back(q);
        }
    }

    // for (unsigned k = 0; k < 4; k++) {
    //     newPoint q;
    //     q.parent = (*p).vertex;
    //     if (k < 2) {            // up & down widths = p widths
    //         q.vertex.x = (*p).vertex.x;
    //     } else {                // left & right
    //         q.vertex.y = (*p).vertex.y;
    //     }
    //     if (k == 0) {           // up edge
    //         q.vertex.y = (*p).vertex.y - 1;
    //     } else if (k == 1) {    // down edge
    //         q.vertex.y = (*p).vertex.y + 1;
    //     } else if (k == 2) {    // left edge
    //         q.vertex.x = (*p).vertex.x - 1;
    //     } else if (k == 3) {    // right edge
    //         q.vertex.x = (*p).vertex.x + 1;
    //     }
    //     if (g.getEdge((*p).vertex, q.vertex) != Grid::NOEDGE) {
    //         // If the edge isn't a wall, we can add it to the adjacency list
    //         (*p).adj.push_back(&q);
    //     }
    // }
}

int main (int const argc, char *argv[]) {
    exec::execname(argv[0]);

    // Options
    std::string infile = "-";
    std::string outfile = "-";
    std::string imgfile;
    std::string start_string = "-";
    std::string end_string = "-";

    // Getopt
    int opt;
    while ((opt = getopt(argc, argv, OPTS)) != -1) {
        switch(opt) {
            case 'i': infile = optarg; break;
            case 'o': outfile = optarg; break;
            case 's': start_string = optarg; break;
            case 'e': end_string = optarg; break;
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

    // Get starting and ending nodes
    Point start;
    Point end;

    if (start_string == "-") {
        start.x = 0;
        start.y = 0;
    } else {
        const char * start_copy = start_string.c_str();
        sscanf(start_copy, "%u%*c%u", &start.x, &start.y);
    }

    if (end_string == "-") {
        end.x = g.getWidth() - 1;
        end.y = g.getHeight() - 1;
    } else {
        const char * end_copy = end_string.c_str();
        sscanf(end_copy, "%u%*c%u", &end.x, &end.y);
    }

    // Solver algorithm (DFS) - This gets all of the nodes that have been explored
    std::vector<newPoint> explored;     // Nodes that have been explored
    std::vector<newPoint> frontier;     // List of nodes to explore

    // std::vector<std::vector<newPoint>> E;
    // E.resize(g.getHeight());
    // for (unsigned i = 0; i < g.getHeight(); i++) {
    //     E[i].resize(g.getWidth());
    // }

    newPoint s;
    s.vertex.x = start.x;
    s.vertex.y = start.y;
    s.parent.x = MAX_INT;
    s.parent.y = MAX_INT;
    setAdjList(&s, g);

    newPoint e;
    e.vertex.x = end.x;
    e.vertex.y = end.y;

    frontier = s.adj;
    // for (unsigned i = 0; i < s.adj.size(); i++) {
    //     frontier.push_back(*s.adj[i]);
    // }
    explored.push_back(s);
    s.explore = true;
    // E[start.y][start.x] = s;

    while (frontier.size() > 0) {
        newPoint b = frontier[0];
        frontier.erase(frontier.begin());
        setAdjList(&b, g);
        bool found = false;
        for (unsigned i = 0; i < explored.size(); i++) {
            if (b.vertex.x == explored[i].vertex.x && b.vertex.y == explored[i].vertex.y) {
                found = true;
                break;
            }
        }
        if (found) {
            found = false;
            continue;
        }
        // if (E[b.vertex.y][b.vertex.x].explore == true) {
        //     continue;
        // }
        b.explore = true;
        explored.push_back(b);
        // E[b.vertex.y][b.vertex.x] = b;
        if (b.vertex.x == e.vertex.x && b.vertex.y == e.vertex.y) {
            // If we've found the last vertex
            break;
        } else {
            for (unsigned i = 0; i < b.adj.size(); i++) {
                frontier.insert(frontier.begin(), b.adj[i]);
            }
        }
    }

    // Get actual path  - This gets the only path from those nodes
    newPoint temp;
    temp = explored.back();
    // temp = E[end.y][end.x];
    while (temp.parent.x != MAX_INT) {
        g.setEdge(temp.vertex, temp.parent, Grid::PATH);
        // temp = E[temp.parent.y][temp.parent.x];
        for (unsigned i = 0; i < explored.size(); i++) {
            if (temp.parent.x == explored[i].vertex.x && 
                temp.parent.y == explored[i].vertex.y) {
                temp = explored[i];
                break;
            }
        }
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
        + "     Find the path between two points in a maze\n"
        + "\n"
        + "USAGE\n"
        + "     " + exec::execname() + " [-" + OPTS + "]\n"
        + "\n"
        + "OPTIONS\n"
        + "     -i input.dat        input file with serialized data [default: stdin]\n"
        + "     -o outfile.dat      output file in serialized form [default: stdout]\n"
        + "     -s x:y              starting point for finding path [default: 0:0]\n"
        + "     -e x:y              ending point for finding path [default: bottom right]\n"
        + "     -p img.ppm          generate ppm image of solved maze\n"
        + "     -h                  Print out helpful information\n"
        ;
}
