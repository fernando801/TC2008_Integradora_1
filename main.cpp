#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <random>
#include <iomanip>
#include <vector>
#include <chrono>
#include <ctime>

#include "Square.h"

auto start = std::chrono::system_clock::now();

bool move_randomly = false;

// Variables dimensiones de la pantalla
int WIDTH = 800;
int HEIGTH = 800;

int n = 1;
int m = 5;

std::vector<std::vector<int>> board;

// Variables para dibujar los ejes del sistema
float X_MIN = -500;
float X_MAX = 500;
float Y_MIN = -500;
float Y_MAX = 500;
float Z_MIN = -500;
float Z_MAX = 500;
// Size del tablero
int DimBoard = (1000/(2*n)) * 2 * n;

std::vector<std::vector<int>> nodes;
std::vector<Position> nodes_postions;

std::vector<Square> trash;
Square* robot;

void drawAxis()
{
    glLineWidth(3.0);
    // X axis in red
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(0.0, 0.0);
    glVertex2f(0.1, 0.0);
    glEnd();
    // Y axis in green
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex2f(0.0, Y_MIN);
    glVertex2f(0.0, Y_MAX);
    glEnd();
    glLineWidth(1.0);
}

void drawString(int x, int y, int z, std::string text) {
  //glEnable(GL_TEXTURE_3D);
  glColor3f(1.0f, 1.0f, 1.0f);
  // Render the text
  glRasterPos3i(x, y, z);
  std::string s = text;
  void * font = GLUT_BITMAP_9_BY_15;
  for (std::string::iterator i = s.begin(); i != s.end(); ++i){
    char c = *i;
    glutBitmapCharacter(font, c);
  }
}

void drawGrid(){
    glLineWidth(5.0);
    for(int i = 0; i < n; i++){
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_LINES);
        glVertex2f((DimBoard/n) * 2 * i - DimBoard, -DimBoard);
        glVertex2f((DimBoard/n) * 2 * i - DimBoard, DimBoard);
        glEnd();
    }

    for(int i = 0; i < n; i++){
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_LINES);
        glVertex2f(-DimBoard, (DimBoard/n) * 2 * i - DimBoard);
        glVertex2f(DimBoard, (DimBoard/n) * 2 * i - DimBoard);
        glEnd();
    }
    
    glLineWidth(1.0);
}

void init()
{
    glClearColor(0, 0, 0, 0);
    glViewport(0, 0, WIDTH, HEIGTH);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(- DimBoard, DimBoard, -DimBoard, DimBoard);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void display()
{
    int node = 0;
    int clean = true;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(0.3, 0.3, 0.3);
    // El piso es dibujado
    glBegin(GL_QUADS);
    glVertex2d(-DimBoard, -DimBoard);
    glVertex2d(-DimBoard, DimBoard);
    glVertex2d(DimBoard, DimBoard);
    glVertex2d(DimBoard, -DimBoard);
    drawGrid();
    for(int i = 0; i<n; i++){
        for(int j = 0; j<n; j++){
            if(board[i][j] == 1){
                node = nodes[i][j];
                if(sqrt(pow(nodes_postions[node].x - robot->position.x, 2) + pow(nodes_postions[node].y - robot->position.y, 2)) < 10){
                    board[i][j] = 0;
                }else{
                    Square((DimBoard / nodes.size() * 0.4), nodes_postions[node].x, nodes_postions[node].y).draw();
                    clean = false;
                }
            }
        }
    }

    if(clean){
        std::cout<<"Pasos dados: "<<robot->steps<<std::endl;
        auto end = std::chrono::system_clock::now();
 
        std::chrono::duration<double> elapsed_seconds = end-start;
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    
        std::cout << "Elapsed time: " << elapsed_seconds.count() << "s" << std::endl;
        exit(0);
    }

    // drawAxis();
    if(move_randomly){
        robot->random_update();
    }else{
        robot->update();
    }
    robot->draw();
    glEnd();

    // for(int k = 0; k<n*n; k++){
    //     drawString(nodes_postions[k].x, nodes_postions[k].y, 0, std::to_string(k));
    // }

    glutSwapBuffers();
}

void idle()
{
    display();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    { // SOLID
    case 's':
    case 'S':
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glShadeModel(GL_FLAT);
        break;
    // INTERPOL
    case 'i':
    case 'I':
        glShadeModel(GL_SMOOTH);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;

    case 'w':
    case 'W':
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;

    case 27: // escape
        exit(0);
        break;
    }
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    std::cout<<"Indique el tamaño del tablero: ";
    std::cin>>n;

    std::cout<<"Indique el número de basuras a recoger: ";
    std::cin>>m;

    std::cout<<"Indique si el movimiento será aleatorio [1 para si 0 para no]: ";
    std::cin>>move_randomly;

    DimBoard = (1000/(2*n)) * 2 * n;

    board = std::vector<std::vector<int>>(n,std::vector<int>(n,0));
    nodes = std::vector<std::vector<int>>(n,std::vector<int>(n,0));
    nodes_postions = std::vector<Position>(n*n);

    int i = 0;
    int j = 0;


    for(int k = 0; k<m; k++){
        j = rand() % n;
        i = rand() % n;
        
        if(board[i][j] != 1){
           board[i][j] = 1;
        }else{
            k--;
        }
    }

    int k = 0;

    for(int i = 0; i<n; i++){
        for(int j = 0; j<n; j++){
            nodes[i][j] = k;
            nodes_postions[k].y = (((DimBoard*2)/n) * i - DimBoard + DimBoard/n);
            nodes_postions[k].x = (((DimBoard*2)/n) * j - DimBoard + DimBoard/n);
            k++;
        }
    }

    robot = new Square(DimBoard, nodes, nodes_postions, (float)(DimBoard/n - DimBoard), (float)(DimBoard/n - DimBoard), 10);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(WIDTH, HEIGTH);
    glutCreateWindow("Cubo 1");
    init();
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    
    return 0;
}
