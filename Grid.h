//Grid header file for the Battleship program
//Creator: James Newberry
//Created: March 22, 2014
//Last Modified: April 16, 2014

#ifndef GRID_H
#define GRID_H

#include <GL/glew.h>
#include <GL/freeglut.h>
using namespace std;

class Grid{
private:
    GLint SQUARE_LENGTH=19;
    bool isHit[10][10];
    bool squarePicked[10][10];
    bool isPlayerGrid;

    GLint ulPosx;
    GLint ulPosy;

    GLuint vboptr;
    GLuint bufptr;
    GLint vPosition = 0;
    GLint vColor = 1;

public:
    Grid();
    Grid(bool forPlayer, GLint upperLeftx, GLint upperLefty);

    bool getHitStatus(int x, int y);
    bool getPicked(int x, int y);
    bool isPlayers();
    void setHit(int x, int y);
    void setMiss(int x, int y);
    void setPicked(int x, int y);

    void loadGrid();
    void drawGrid();
};

#endif // GRID_H
