//Button C++ file for the battleship program
//Creator: James Newberry
//Created: April 16, 2014
//Last Modified: April 16, 2013
#include "rectangle.h"
#include "Button.h"
#include "LoadShaders.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
using namespace std;

Button::Button(){}

Button::Button(GLint upperLeftx, GLint upperLefty, int index){
    ulx=upperLeftx;
    uly=upperLefty;
    buttonIndex=index;
    isDisabled=false;
    isSelected=false;
    imageHolder=rectangle(0, 0, 0, ulx, uly, width, height);
    imageHolder.load(vPosition, vColor, 2, vTexture);
}

void Button::setDisabled(bool d){
    isDisabled=d;
}

void Button::setSelected(bool s){
    isSelected=s;
}

bool Button::getDisabled(){
    return isDisabled;
}

bool Button::getSelected(){
    return isSelected;
}

void Button::drawImage(GLint textureLocation){
    if(isDisabled)
        glUniform1i(textureLocation, buttonIndex*3+11);
    else if(isSelected)
        glUniform1i(textureLocation, buttonIndex*3+12);
    else
        glUniform1i(textureLocation, buttonIndex*3+10);
    imageHolder.draw();
}
