//Ship C++ file for battleship program.
//Creator: James Newberry
//Created: March 22, 2014
//Last Modified: April 16, 2014

#include "Ship.h"
#include "Grid.h"
#include "rectangle.h"
#include "Image_File_IO.h"
//#include "vec_mat.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>
#include <iostream>
using namespace std;



bool Ship::validateShipPosition(int xStart, int yStart, Grid holder){
    if(allPositionsOnGrid(xStart, yStart, holder)){
        if(notOverlapping(xStart, yStart, holder))
            return true;
    }
    return false;
}

bool Ship::allPositionsOnGrid(int xStart, int yStart, Grid holder){
    if(isHorizontal){
			if(xStart+len<=10)//Check to see if the last position is on the grid.
				return true;
			else
				return false;

    }
    else{
			if(yStart+len<=10)
				return true;
			else
				return false;
    }
}

bool Ship::notOverlapping(int xStart, int yStart, Grid holder){
    for(int i=0; i<len; i++){
        if(isHorizontal){
            if(holder.getHitStatus(xStart+i, yStart))
                return false;
        }
        else{
            if(holder.getHitStatus(xStart, yStart+i))
                return false;
        }
    }
    return true;
}

Ship::Ship(){}

Ship::Ship(int lenOfShip, Grid holder, string shipName, char* file, char* filev, GLint xStart, GLint yStart, int shipNumber, GLuint programmer){
    len=lenOfShip;
    height=widthv=19;
    width=heightv=20*len-1;
    hits=0;
    isPlaced=false;
    isHorizontal=false;
    origx=xStart;
    ulx=xStart;
    origy=yStart;
    uly=yStart;
    name=shipName;
    shipIndex=shipNumber;
    if(holder.isPlayers()){//Images are only shown for the player on their grid, not the computer grid.
        imageHolder=rectangle(0, 1, 1, ulx, uly, width, height);
        imageHolderv=rectangle(0, 1, 1, ulx, uly, widthv, heightv);
        imageHolder.load(vPosition, vColor, 2, vTexture);
        imageHolderv.load(vPosition, vColor, 2, vTexture);
        image=LoadBMP(file, &imageWidth, &imageHeight);
        if(image ==0){
            cout<<"Texture error:  Horizontal image not available"<<endl;
        }
        imagev=LoadBMP(filev, &imageWidthv, &imageHeightv);
        if(imagev ==0){
            cout<<"Texture error:  Vertical image not available"<<endl;
        }

        textureLocation=glGetUniformLocation(programmer, "tex1");
        glUniform1i(textureLocation, 0);

        glActiveTexture(GL_TEXTURE0+shipIndex*2);
        glGenTextures(1, &texptr);
        glBindTexture(GL_TEXTURE_2D, texptr);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glActiveTexture(GL_TEXTURE0+shipIndex*2+1);
        glGenTextures(1, &texptrv);
        glBindTexture(GL_TEXTURE_2D, texptrv);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidthv, imageHeightv, 0, GL_RGB, GL_UNSIGNED_BYTE, imagev);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
}

bool Ship::placeShip(int xStart, int yStart, Grid &holder){
    if(validateShipPosition(xStart, yStart, holder)){
        setHitPositions(xStart, yStart, holder);
        isPlaced=true;
        if(holder.isPlayers()){
            setUpperLeft(-240+20*xStart, 100-20*yStart);
        }
        return true;
    }
    return false;
}

void Ship::removeShip(Grid &holder){
    if(isPlaced){
        for(int i=0; i<len; i++){
            if(isHorizontal){
                holder.setMiss(firstPositionX+i, firstPositionY);
            }
            else{
                holder.setMiss(firstPositionX, firstPositionY+i);
            }
        }
        isPlaced=false;
    }
}

void Ship::setHitPositions(int xStart, int yStart, Grid &holder){
    firstPositionX=xStart;
    firstPositionY=yStart;
    for (int i=0; i<len; i++){
        if(isHorizontal){
            holder.setHit(firstPositionX+i, firstPositionY);
        }
        else{
            holder.setHit(firstPositionX, firstPositionY+i);
        }
    }

}

void Ship::rotateShip(){
    if(isHorizontal){
        isHorizontal=false;
    }
    else{
        isHorizontal=true;
    }
//    reloadImage();
}

bool Ship::isItPlaced(){
    return isPlaced;
}

bool Ship::isItHorizontal(){
    return isHorizontal;
}

bool Ship::gotHit(int x, int y){
    for(int i=0; i<len; i++){
        if(isHorizontal){
            if(x==firstPositionX+i && y==firstPositionY){
                hits+=1;
                return true;
            }
        }
        else{
            if(x==firstPositionX && y==firstPositionY+i){
                hits+=1;
                return true;
            }
        }
    }
    return false;
}

bool Ship::gotSunk(){
    if(hits==len){
        return true;
    }
    else
        return false;
}

GLint Ship::getUpperLeftX(){
    return ulx;
}

GLint Ship::getUpperLeftY(){
    return uly;
}

void Ship::setUpperLeft(GLint x, GLint y){
    ulx=x;
    uly=y;
    imageHolder.setUpperLeft(ulx, uly);
    imageHolderv.setUpperLeft(ulx, uly);
}

GLint Ship::getOriginX(){
    return origx;
}

GLint Ship::getOriginY(){
    return origy;
}

GLint Ship::getWidth(){
    if(isHorizontal)
        return width;
    else
        return widthv;
}

GLint Ship::getHeight(){
    if(isHorizontal)
        return height;
    else
        return heightv;
}

int Ship::getFirstPositionX(){
    return firstPositionX;
}

int Ship::getFirstPositionY(){
    return firstPositionY;
}

int Ship::getLength(){
    return len;
}

void Ship::drawImage(){
    if(isHorizontal){
        //glActiveTexture(GL_TEXTURE0+shipIndex*2);
        glUniform1i(textureLocation, shipIndex*2);
        imageHolder.draw();
    }
    else{
       // glActiveTexture(GL_TEXTURE0+shipIndex*2+1);
        glUniform1i(textureLocation, shipIndex*2+1);
        imageHolderv.draw();
    }


}
