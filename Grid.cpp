//Grid C++ file for the Battleship program
//Creator: James Newberry
//Created: April 13, 2014
//Last Updated: April 13, 2014

#include "Grid.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "LoadShaders.h"

#define BUFFER_OFFSET(x) ((const void*) (x))

Grid::Grid(){}

Grid::Grid(bool forPlayer, GLint upperLeftx, GLint upperLefty){
    isPlayerGrid=forPlayer;
    for(int x=0; x<10; x++){
        for(int y=0; y<10; y++){
            isHit[x][y]=false;
            squarePicked[x][y]=false;
        }
    }
    ulPosx=upperLeftx;
    ulPosy=upperLefty;

    loadGrid();
}

bool Grid::getHitStatus(int x, int y){
    return isHit[x][y];
}

bool Grid::getPicked(int x, int y){
    return squarePicked[x][y];
}

bool Grid::isPlayers(){
    return isPlayerGrid;
}

void Grid::setHit(int x, int y){
    isHit[x][y]=true;
}

void Grid::setMiss(int x, int y){
    isHit[x][y]=false;
}

void Grid::setPicked(int x, int y){
    squarePicked[x][y]=true;
}

struct VertexData{
    GLfloat color[3];
    GLfloat position[2];
};

void Grid::loadGrid(){
    VertexData gridVertices[600];
    for(int y=0; y<10; y++){
        GLfloat uly=ulPosy-y*(SQUARE_LENGTH+1);
        for(int x=0; x<10; x++){
            GLfloat ulx=ulPosx+x*(SQUARE_LENGTH+1);
            gridVertices[6*(x+10*y)]={{0,0,1},{ulx,uly}};
            gridVertices[6*(x+10*y)+1]={{0,0,1},{ulx+SQUARE_LENGTH,uly}};
            gridVertices[6*(x+10*y)+2]={{0,0,1},{ulx,uly-SQUARE_LENGTH}};
            gridVertices[6*(x+10*y)+3]={{0,0,1},{ulx+SQUARE_LENGTH,uly}};
            gridVertices[6*(x+10*y)+4]={{0,0,1},{ulx,uly-SQUARE_LENGTH}};
            gridVertices[6*(x+10*y)+5]={{0,0,1},{ulx+SQUARE_LENGTH,uly-SQUARE_LENGTH}};
        }
    }
    glGenVertexArrays(1, &vboptr);
    glBindVertexArray(vboptr);

    glGenBuffers(1, &bufptr);
    glBindBuffer(GL_ARRAY_BUFFER, bufptr);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gridVertices),
                 gridVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(vColor, 3, GL_FLOAT,
                          GL_FALSE, sizeof(VertexData), BUFFER_OFFSET(0));

    glVertexAttribPointer(vPosition, 2, GL_FLOAT,
                          GL_FALSE, sizeof(VertexData),
                          BUFFER_OFFSET(sizeof(gridVertices[0].color)));

    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vColor);
}

void Grid::drawGrid(){
    glBindVertexArray(vboptr);
    glDrawArrays(GL_TRIANGLES, 0, 600);
}
