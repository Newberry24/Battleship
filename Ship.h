//Ship header file for the battleship program
//Creator: James Newberry
//Created: March 22, 2014
//Last Updated: April 13, 2014

#ifndef SHIP_H
#define SHIP_H

#include<string>
#include "Grid.h"
#include "rectangle.h"
//#include "vec_mat.h"
#include <GL/glew.h>
#include <GL/freeglut.h>

using namespace std;
class Ship{
private:
    int len; //length of the ship
    int hits; //number of hits made to the ship
    GLint origx, origy; //Origin variables
    GLint ulx, uly;  //Upper left corner position variables.

    int firstPositionX=-1;  //Default values are sentinel data.
    int firstPositionY=-1;

    bool isPlaced;
    bool isHorizontal;

    GLint width;
    GLint height;
    GLint widthv;
    GLint heightv;

     //Variables for storing image data
    GLbyte* image;
    GLbyte* imagev;
    GLuint texptr;
    GLuint texptrv;
    GLint imageHeight;
    GLint imageHeightv;
    GLint imageWidth;
    GLint imageWidthv;
    GLuint textureLocation;
    string name;

    GLint vPosition = 0;
    GLint vColor = 1;
    GLint vTexture = 3;
    int shipIndex;

    rectangle imageHolder;
    rectangle imageHolderv;

    bool validateShipPosition(int xStart, int yStart, Grid holder); //Validation Functions
    bool allPositionsOnGrid(int xStart, int yStart, Grid holder);
    bool notOverlapping(int xStart, int yStart, Grid holder);
public:
    Ship();  //Dummy Default Constructor
    Ship(int lenOfShip, Grid holder, string shipName, char* file=NULL, char* filev=NULL, GLint xStart=0, GLint yStart=0, int shipNumber=0, GLuint programmer=0);
    //~Ship();
    bool placeShip(int xStart, int yStart, Grid &holder);
    void removeShip(Grid &holder);
    void setHitPositions(int xStart, int yStart, Grid &holder);
    void rotateShip();
    bool isItPlaced();
    bool isItHorizontal();
    bool gotHit(int x, int y);
    bool gotSunk();
    GLint getUpperLeftX();
    GLint getUpperLeftY();
    void setUpperLeft(GLint x, GLint y);
    GLint getOriginX();
    GLint getOriginY();
    GLint getWidth();
    GLint getHeight();

    int getFirstPositionX();
    int getFirstPositionY();
    int getLength();

    void drawImage();
};
#endif // SHIP_H
