#include <vector>
#include <iostream>

#include "Map.h"

Map::Map(int s) {
    size = s;
    wallGrid.resize(size);
    for (int i = 0; i < size; i++) {
        wallGrid[i].resize(size);
        for (int j = 0; j < size; j++) {
            // Creates top and bottom walls of map
            if (i == 0 || i == size - 1) {
                wallGrid[i][j] = 1;
            }
            // Creates left and right walls of map
            else if (j == 0 || j == size - 1) {
                wallGrid[i][j] = 1;
            }
            else {
                wallGrid[i][j] = 0;
            }
        }
    }
    for (auto &row : wallGrid) {
        row.resize(size);
    }
}

void Map::addWall(std::pair<int, int> position) {
    if (position.first > size || 
        position.first < 0 || 
        position.second > size || 
        position.second < 0) {
            return;
    }
    wallGrid[position.first][position.second] = 1;
}

void Map::printMap() {
    for (auto row : wallGrid) {
        for (auto col : row) {
            std::cout << col << " ";
        }
        std::cout << std::endl;
    }
}