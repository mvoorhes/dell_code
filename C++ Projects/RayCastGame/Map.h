#include <vector>
#include <cstdint>

class Map {
    private:
        int size;
        std::vector<std::vector<int>> wallGrid;
    public:
        Map(int size);
        void addWall(std::pair<int, int> position);
        void printMap();
};