#ifndef SQUARE_H
#define SQUARE_H

#include <math.h>
#include <random>
#include <iomanip>
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <queue>

struct Position
{
    float x;
    float y;
};

struct Direction
{
    float x;
    float y;
};

class Square
{
private:
    float size = 1;

    float DimBoard = 1;

    float vertexCoords[8] = { // Coordinates for the vertices of a cube.
        size, size, -size, size, -size, -size, size, -size};

    float vertexColors[12] = { // An RGB color value for each vertex
        1, 0, 0, 1, 0, 0,
        1, 0, 0, 1, 0, 0};

    int elementArray[4] = { // Vertex number for the six faces.
        0, 1, 2, 3};

    float Color[3] = {1, 1, 1};

    std::vector<std::vector<int>> nodes;

    std::queue<int> sequence;

    std::vector<std::vector<int>> adjacency_list;

public:

    float vel = 0;

    int nextNode = 0;

    int currentNode = 0;

    int steps = 0;

    Position position;

    Direction direction;

    std::vector<Position> nodes_postiions;

    Square(float, float, float);
    Square(float, std::vector<std::vector<int>>, std::vector<Position>, int, float);
    ~Square();
    void set_direction();
    void random_update();
    void draw();
    void update();
    void switch_node();
};

#endif