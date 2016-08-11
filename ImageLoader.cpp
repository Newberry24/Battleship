//Image loader header file for the battleship program
//Creator: James Newberry
//Created: April 16, 2014
//Last Modified: April 16, 2014
#include "Image_File_IO.h"
#include <GL/glew.h>
#include <GL/freeglut.h>


ImageLoader::ImageLoader(GLbyte* image, char* file, GLint* width, GLint* height){
    image=LoadBMP(file, width, height);
}


