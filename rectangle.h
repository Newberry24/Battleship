 #include <GL/glew.h>
 #include <GL/freeglut.h>
 #include <GL/gl.h>

 #ifndef RECTANGLE_H
 #define RECTANGLE_H

class rectangle
 {
 public:
    rectangle();
    rectangle(GLfloat r, GLfloat g, GLfloat b, GLint x, GLint y, GLint w, GLint h);
    ~rectangle();

    void setColor(GLfloat r, GLfloat g, GLfloat b);
    void setUpperLeft(GLint posx, GLint posy);
    void setMaxTexCoord(GLfloat mtc);

    bool isRed();
    bool isBlue();

    void load(GLint v, GLint c, GLint n, GLint t);
    void reloadData();
    void draw();

private:
    GLfloat red = 1;
    GLfloat green = 1;
    GLfloat blue = 1;

    GLint ulx;
    GLint uly;
    GLint width;
    GLint height;

    GLfloat MaxTexCoord = 1;

    GLuint vboptr;
    GLuint bufptr;

    GLint vPosition = 0;
    GLint vColor = 1;
    GLint vNormal = 2;
    GLint vTex = 3;

    GLboolean reload = GL_FALSE;
};

#endif // RECTANGLE_H_INCLUDED

