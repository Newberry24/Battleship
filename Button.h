//Button header file for the battleship program
//Creator: James Newberry
//Created: April 16, 2014
//Last Modified: April 16, 2014
#ifndef BUTTON_H
#define BUTTON_H

#include "rectangle.h"
#include <GL/glew.h>
#include <GL/freeglut.h>

using namespace std;

class Button{
private:
    bool isDisabled;
    bool isSelected;

    GLint ulx;
    GLint uly;
    GLint width=60;
    GLint height=30;

    GLint vPosition = 0;
    GLint vColor = 1;
    GLint vTexture = 3;
    int buttonIndex;

    rectangle imageHolder;

public:
    Button();  //Dummy Default Constructor
    Button(GLint upperLeftx, GLint upperLefty, int index);

    void setDisabled(bool d);
    void setSelected(bool s);

    bool getDisabled();
    bool getSelected();

    void drawImage(GLint texLocation);
};

#endif
