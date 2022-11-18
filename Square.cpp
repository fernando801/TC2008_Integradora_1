#include "Square.h"

Square::Square(float dim, std::vector<std::vector<int>> nodes_, std::vector<Position> nodes_postiions_, float x, float y, float vel_)
    : DimBoard(dim), nodes(nodes_), nodes_postiions(nodes_postiions_), size(dim / nodes_.size() * 0.8), vel(vel_), vertexCoords{size, size, -size, size, -size, -size, size, -size}
{

    position.x = x;
    position.y = y;

    Color[0] = 1;
    Color[1] = 0;
    Color[2] = 1;

    for (int i = 0; i < 12; i++)
    {
        vertexColors[i] = Color[(i + 3) % 3];
    }

    int n = nodes.size();


    for(int i = 0; i<n; i++){
        if(i%2 == 0){
            for(int j = 0; j<n; j++){
                sequence.push(nodes[i][j]);
            }
        }else{
            for(int j = n-1; j>=0; j--){
                sequence.push(nodes[i][j]);
            }
        }
    }

    adjacency_list = std::vector<std::vector<int>>(n*n);

    for(int i = 0; i<n; i++){
        for(int j = 0; j<n; j++){
            if(i>0 && j>0){
                adjacency_list[nodes[i][j]].push_back(nodes[i-1][j-1]);
            }
            if(i>0){
                adjacency_list[nodes[i][j]].push_back(nodes[i-1][j]);
            }
            if(i>0 && j<n-1){
                adjacency_list[nodes[i][j]].push_back(nodes[i-1][j+1]);
            }
            if(j>0){
                adjacency_list[nodes[i][j]].push_back(nodes[i][j-1]);
            }
            if(j<n-1){
                adjacency_list[nodes[i][j]].push_back(nodes[i][j+1]);
            }
            if(i<n-1 && j>0){
                adjacency_list[nodes[i][j]].push_back(nodes[i+1][j-1]);
            }
            if(i<n-1){
                adjacency_list[nodes[i][j]].push_back(nodes[i+1][j]);
            }
            if(i<n-1 && j<n-1){
                adjacency_list[nodes[i][j]].push_back(nodes[i+1][j+1]);
            }
        }
    }
}

Square::Square(float size_, float x, float y)
    : size(size_)
{
    position.x = x;
    position.y = y;

    Color[0] = 0;
    Color[1] = 0;
    Color[2] = 0;
}

Square::~Square()
{
}

void Square::set_direction(){
    direction.x = nodes_postiions[nextNode].x - position.x;
    direction.y = nodes_postiions[nextNode].y - position.y;
    // std::cout<<nodes_postiions[6].x<<" "<<position.x<<std::endl;
    // Se normaliza el vector de direccion
    float m = sqrt(direction.x * direction.x + direction.y * direction.y);
    if(m != 0){
        direction.x /= m;
        direction.y /= m;
    }else{
        direction.x = 0;
        direction.y = 0;
    }
    // se multiplica el vector de direccion por la magnitud de la velocidad
    direction.x *= vel;
    direction.y *= vel;
}

void Square::draw()
{
    glPushMatrix();
    // Se dibuja el cubo
    glTranslatef(position.x, position.y, 0.0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, vertexCoords);
    glColorPointer(3, GL_FLOAT, 0, vertexColors);
    glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, elementArray);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    glPopMatrix();
}

void Square::update()
{
    int new_x = position.x + direction.x;
    int new_y = position.y + direction.y;

    if(sqrt(pow(nodes_postiions[nextNode].x - new_x, 2) + pow(nodes_postiions[nextNode].y - new_y, 2)) < vel){
        steps += 1;
        sequence.push(nextNode);
        nextNode = sequence.front();
        sequence.pop();
        // std::cout<<steps<<std::endl;
    }

    position.x = new_x;
    position.y = new_y;

    set_direction();
}

void Square::random_update()
{
    int new_x = position.x + direction.x;
    int new_y = position.y + direction.y;

    if(sqrt(pow(nodes_postiions[nextNode].x - new_x, 2) + pow(nodes_postiions[nextNode].y - new_y, 2)) < vel){
        steps += 1;
        nextNode = adjacency_list[nextNode][rand() % adjacency_list[nextNode].size()];
        // std::cout<<steps<<std::endl;
    }

    position.x = new_x;
    position.y = new_y;

    set_direction();
}