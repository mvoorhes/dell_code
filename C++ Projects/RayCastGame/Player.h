#include <iostream>
#include <vector>
#include <math.h>

class Player {
    private:
        float x;
        float y;
        float angle;
    public:
        Player(float x, float y, float a);
        void move(char key);
        void draw();      
};
