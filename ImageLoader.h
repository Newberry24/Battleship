//Image loader header file for the battleship program
//Creator: James Newberry
//Created: April 16, 2014
//Last Modified: April 16, 2014
#ifndef IMAGE_LOADER_H
#define IMAGE_LOADER_H

#include <GL/glew.h>
#include <GL/freeglut.h>

class ImageLoader{
public:
    ImageLoader(GLbyte* image, char* file, GLint* width, GLint* height);
};
#endif
