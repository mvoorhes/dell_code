#include "grid.h"

#include <cassert>
#include <iostream>

#define MAX_POINT 3
#define MAX_EDGE 10

// Constants ---------------------------------------------------------
Grid::weight_t const Grid::NOEDGE = -1;
Grid::weight_t const Grid::PATH = 1;
std::string const P3_NODE = "255 255 255\n";
std::string const P3_EDGE = "255 255 255\n";
std::string const P3_WALL = "0 0 0\n";
std::string const P3_PATH = "255 0 0\n";

// Definitions -------------------------------------------------------
std::ostream& operator<<(std::ostream &out, Point const &p) {
    return out << '(' << p.x << ',' << p.y << ')';
}

Grid::Node::Node() {
    for (auto i = 0; i < NUM_DIRECTIONS; i++) {
        weights[i] = Grid::NOEDGE;
    }
}

Grid::Grid(unsigned height, unsigned width)
{
    nodes_.resize(height);
    for (unsigned i = 0; i < height; i++) {
        nodes_[i].resize(width);
        for (unsigned j = 0; j < width; j++) {
            nodes_[i][j] = Node();
        }
    }
}

unsigned Grid::getHeight() const {
    return nodes_.size();
}

unsigned Grid::getWidth() const {
    return nodes_[0].size();
}

Grid::weight_t Grid::getEdge(Point const &a, Point const &b) const {    
    if (a.x == b.x && a.y == b.y) {
        // if nodes are the same
        return 0;
    } else if (a.x != b.x && a.y != b.y) {
        // if nodes are not neighbors & not on same row or column
        return Grid::NOEDGE;
    } else if (a.x >= getWidth() || b.x >= getWidth() || a.y >= getHeight() || b.y >= getHeight()) {
        // nodes are out of bounds
        return Grid::NOEDGE;
    } else if ((b.x > a.x + 1) || (b.y > a.y + 1) || (a.x > b.x + 1) || (a.y > b.y + 1)) {
        // nodes aren't neighbors but they could potentially on the same row or column
        return Grid::NOEDGE;
    }

    if (a.x < b.x) {
        return nodes_[a.y][a.x].weights[Node::RIGHT];
    } else if (a.y < b.y) {
        return nodes_[a.y][a.x].weights[Node::DOWN];
    } else if (a.x > b.x) {
        return nodes_[b.y][b.x].weights[Node::RIGHT];
    } else if (a.y > b.y) {
        return nodes_[b.y][b.x].weights[Node::DOWN];
    }
    return Grid::NOEDGE;
}

bool Grid::setEdge(Point const &a, Point const &b, weight_t weight) {
    if (a.x != b.x && a.y != b.y) {
        // nodes are not neighbors
        return false;
    } else if (a.x == b.x && a.y == b.y) {
        // nodes are equal to eachother
        return false;
    } else if (a.x >= getWidth() || b.x >= getWidth() || a.y >= getHeight() || b.y >= getHeight()) {
        // nodes are out of bounds
        return false;
    } else if ((b.x > a.x + 1) || (b.y > a.y + 1) || (a.x > b.x + 1) || (a.y > b.y + 1)) {
        // nodes aren't neighbors but they could potentially on the same row or column
        return false;
    }

    if (a.x < b.x) {
        nodes_[a.y][a.x].weights[Node::RIGHT] = weight;
    } else if (a.y < b.y) {
        nodes_[a.y][a.x].weights[Node::DOWN] = weight;
    } else if (a.x > b.x) {
        nodes_[b.y][b.x].weights[Node::RIGHT] = weight;
    } else if (a.y > b.y) {
        nodes_[b.y][b.x].weights[Node::DOWN] = weight;
    }
    return true;
}


// Worry only about top and left
void Grid::draw(std::ostream &out) const {
    auto new_height = (Grid::getHeight() * 2) + 1;
    auto new_width = (Grid::getWidth() * 2) + 1;
    // print out first 3 lines of ppm file
    out << "P3\n" << new_width << ' ' << new_height << '\n' << 255 << '\n';
    unsigned x = 0;
    unsigned y = -1;
    weight_t edge = Grid::NOEDGE;
    Point p;
    Point q;

    // Creating top border
    for (unsigned width = 0; width < new_width; width++) {
        out << P3_WALL;
    }

    for (unsigned height = 1; height < new_height; height++) {
        if (height % 2 != 0) {
            // Times we reinitialize y
            y += 1;
        }
        x = 0;  // reset x
        for (unsigned width = 0; width < new_width; width++) {
            if (width == 0) {
                // Create the border
                out << P3_WALL;
            } else if (height % 2 == 0 && width % 2 == 0) {
                // This is the case where we absolutely know there are no edges
                out << P3_WALL;
                x += 1;
            } else if (width % 2 != 0) {
                // This is the case where we either print a node & an edge or just an edge 
                // depending on the row we are on.
                p.x = x;
                p.y = y;
                if (height % 2 != 0) {
                    // get right node & print node
                    q.x = p.x + 1;  // right node
                    q.y = p.y;
                    Point r;        // down node
                    r.x = p.x;
                    r.y = p.y + 1;
                    Point s;        // left node
                    s.x = p.x - 1;
                    s.y = p.y;
                    Point t;        // up node
                    t.x = p.x;
                    t.y = p.y - 1;
                    if (getEdge(p, q) == Grid::PATH || 
                        getEdge(p, r) == Grid::PATH || 
                        getEdge(s, p) == Grid::PATH || 
                        getEdge(t, p) == Grid::PATH) {
                        out << P3_PATH;
                    } else {
                        out << P3_NODE;
                    }
                } else {
                    // get down node
                    q.x = p.x;
                    q.y = p.y + 1;
                }
                edge = getEdge(p, q);
                if (edge != Grid::NOEDGE && edge != Grid::PATH) {
                    out << P3_EDGE;
                } else if (edge == Grid::PATH) {
                    out << P3_PATH;
                } else {
                    out << P3_WALL;
                }
            } 
            else {
                // don't print anything, just increment x.
                x += 1;
            }
        }
    }
}

void Grid::serialize(std::ostream &out) const {
    out << getWidth() << " " << getHeight() << std::endl;
    Point p;
    Point q;
    for (unsigned i = 0; i < getHeight(); i++) {
        p.y = i;
        for (unsigned j = 0; j < getWidth(); j++) {
            p.x = j;
            // print right first
            q.x = j + 1;
            q.y = i;
            if (getEdge(p, q) != Grid::NOEDGE) {
                out << p << getEdge(p, q) << q << std::endl;
            }
            // print down next
            q.x = j;
            q.y = i + 1;
            if (getEdge(p, q) != Grid::NOEDGE) {
                out << p << getEdge(p, q) << q << std::endl;
            }
        }
    }
}

Grid Grid::load(std::istream &in) {
    std::string line;
    
    int height;
    int width;

    // get width and height
    in >> width;
    in >> height;

    Grid g = Grid(height, width);

    Point p;
    Point q;
    weight_t w = 0;

    while (in) {
        std::getline(in, line);
        const char * line_copy = line.c_str();
        sscanf(line_copy, "%*c%u%*c%u%*c%u%*c%u%*c%u%*c", &p.x, &p.y, &w, &q.x, &q.y);
        g.setEdge(p, q, w);
    }
    return g;
}
