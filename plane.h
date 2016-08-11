#ifndef PLANE_H_INCLUDED
#define PLANE_H_INCLUDED

#include "vec_mat.h"

class plane
{
public:
    plane();
    plane(GLfloat r, GLfloat g, GLfloat b);
    ~plane();

    void setColor(GLfloat r, GLfloat g, GLfloat b);
    void setMaxTexCoord(GLfloat mtc);

    void load(GLint v, GLint c, GLint n, GLint t);
    void reloadData();
    void draw();

private:
    GLfloat red = 1;
    GLfloat green = 1;
    GLfloat blue = 1;

    GLfloat MaxTexCoord = 1;

    GLuint vboptr;
    GLuint bufptr;

    GLint vPosition = 0;
    GLint vColor = 1;
    GLint vNormal = 2;
    GLint vTex = 3;

    GLboolean reload = GL_FALSE;
};

#endif // PLANE_H_INCLUDED
