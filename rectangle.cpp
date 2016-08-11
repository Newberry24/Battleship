#include <GL/glew.h>
#include "rectangle.h"

#define BUFFER_OFFSET(x) ((const void*) (x))

//  Constructors
rectangle::rectangle()
{
    red = 1;
    green = 1;
    blue = 1;
}

rectangle::rectangle(GLfloat r, GLfloat g, GLfloat b, GLint x, GLint y, GLint w, GLint h)
{
    red = r;
    green = g;
    blue = b;
    ulx=x;
    uly=y;
    width=w;
    height=h;
}

//  Destructor
rectangle::~rectangle()
{
}

//  Accessor Functions

void rectangle::setColor(GLfloat r, GLfloat g, GLfloat b)
{
    red = r;
    green = g;
    blue = b;
    reloadData();
}
void rectangle::setUpperLeft(GLint posx, GLint posy){
    ulx=posx;
    uly=posy;
    reloadData();
}
void rectangle::setMaxTexCoord(GLfloat mtc)
{
    MaxTexCoord = mtc;
    reloadData();
}

bool rectangle::isRed(){
    if(red==1 && green==0 && blue==0)
        return true;
    else
        return false;
}

bool rectangle::isBlue(){
    if(red==0 && green==0 && blue==1)
        return true;
    else
        return false;
}
void rectangle::reloadData()
{
    reload = GL_TRUE;
    load(vPosition, vColor, vNormal, vTex);
}

void rectangle::load(GLint v, GLint c, GLint n, GLint t)
{
    vPosition = v;
    vColor = c;
    vNormal = n;
    vTex = t;

    GLushort indices[] = {0, 1, 2,
                          2, 3, 0,
                         };

    GLfloat points[] =
    {
        ulx, uly,
        ulx, uly-height,
        ulx+width, uly-height,
        ulx+width, uly
    };

    GLfloat normals[] =
    {
        0, 1, 0,
        0, 1, 0,
        0, 1, 0,
        0, 1, 0,
    };

    GLfloat tex[] =
    {
        0, MaxTexCoord,
        0, 0,
        MaxTexCoord, 0,
        MaxTexCoord, MaxTexCoord,
    };

    GLfloat colors[4*4];
    for (int i = 0; i < 4; i++)
    {
        colors[4*i] = red;
        colors[4*i+1] = green;
        colors[4*i+2] = blue;
        colors[4*i+3] = 1;
    }

    if (!reload)
        glGenVertexArrays(1, &vboptr);

    glBindVertexArray(vboptr);

    if (!reload)
    {
        GLuint eboptr;
        glGenBuffers(1, &eboptr);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptr);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);
    }

    if (!reload)
        glGenBuffers(1, &bufptr);
    glBindBuffer(GL_ARRAY_BUFFER, bufptr);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors) + sizeof(normals) + sizeof(tex), NULL, GL_DYNAMIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), sizeof(normals), normals);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors) + sizeof(normals), sizeof(tex), tex);

    glVertexAttribPointer(vTex, 2, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(sizeof(points) + sizeof(colors) + sizeof(normals)));
    glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(sizeof(points) + sizeof(colors)));
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(sizeof(points)));
    glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vColor);
    glEnableVertexAttribArray(vNormal);
    glEnableVertexAttribArray(vTex);

    reload = GL_FALSE;
}

void rectangle::draw()
{
    glBindVertexArray(vboptr);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);
}
