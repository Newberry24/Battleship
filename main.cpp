//Main C++ code for the Battleship program.
//Creator: James Newberry
//Created: April 14, 2014
//Last Modified: April 16, 2014
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Grid.h"
#include "Ship.h"
//#include "Button.h"
#include "LoadShaders.h"
#include "vec_mat.h"
#include <time.h>
#include <cstdlib>
#include <stdio.h>
#include <iostream>
//#include "Image_File_IO.h"
using namespace std;
//Button confirmer;
//Button autoPlace;
Ship yourShips[6];
Ship computerShips[6];
Grid yourGrid;
Grid computerGrid;
bool gamePhase=false;

GLint numGridVAOs=2;
GLuint gridVAOs[2];
GLenum mode=GL_FILL;

//GLuint texID[5];

GLuint program;

GLfloat clickx, clicky;
//GLfloat posx, posy;
//GLfloat ulx, uly;

GLfloat xMin=-300;
GLfloat xMax=300;
GLfloat yMin=-100;
GLfloat yMax=100;

bool mouseClicked=false;
bool mouseClickedOnShip=false;
Ship* mouseClickedOn;

bool yourTurn=true;
bool youChose=false;
int youChosex;
int youChosey;

bool computerFoundHit=false;
bool computerDetectsDirection=false;
bool directionHorizontal=false;
bool missWhenHit;
int shipFoundX;
int shipFoundY;
int lastHitX;
int lastHitY;

clock_t start;

rectangle yourSquaresPicked[11][11];
rectangle computerSquaresPicked[11][11];

bool winnerDecided=false;

void init(){
    srand(time(NULL));
    ShaderInfo  shaders[] =
    {
        {GL_VERTEX_SHADER, "vertexShader.vert"},
        {GL_FRAGMENT_SHADER, "SimpleTexture.frag"},
        {GL_NONE, NULL}
    };

    program=LoadShaders(shaders);
    glUseProgram(program);

    yourGrid=Grid(true, -240, 100);
    computerGrid=Grid(false, 100, 100);

//    confirmer=Button(0, 65, 0);
  //  autoPlace=Button(0, -35, 1);


    yourShips[1]=Ship(5, yourGrid, "aircraft carrier", "carrier.bmp", "carrierv.bmp", -300, 0, 0, program);
    yourShips[2]=Ship(4, yourGrid, "battleship", "battleship.bmp", "battleshipv.bmp", -300, 100, 1, program);
    yourShips[3]=Ship(3, yourGrid, "frigate", "frigate.bmp", "frigatev.bmp", -270, 100, 2, program);
    yourShips[4]=Ship(3, yourGrid, "submarine", "submarine.bmp", "submarinev.bmp", -270, 20, 3, program);
    yourShips[5]=Ship(2, yourGrid, "minesweeper", "minesweeper.bmp", "minesweeperv.bmp", -270, -60, 4, program);

    computerShips[1]=Ship(5, computerGrid, "aircraft carrier");
    computerShips[2]=Ship(4, computerGrid, "battleship");
    computerShips[3]=Ship(3, computerGrid, "frigate");
    computerShips[4]=Ship(3, computerGrid, "submarine");
    computerShips[5]=Ship(2, computerGrid, "minesweeper");

    for(int x=1; x<=10; x++){
        for(int y=1; y<=10; y++){
            yourSquaresPicked[x][y]=rectangle(0, 0, 1, -260+x*20, 120-y*20, 19, 19);
            computerSquaresPicked[x][y]=rectangle(0, 0, 1, 80+x*20, 120-y*20, 19, 19);
            yourSquaresPicked[x][y].load(0, 1, 2, 3);
            computerSquaresPicked[x][y].load(0, 1, 2, 3);
        }
    }

}

void printOpenGLErrors()
{
    GLenum errCode;
    const GLubyte *errString;

    while ((errCode = glGetError()) != GL_NO_ERROR)
    {
        errString = gluErrorString(errCode);
        fprintf(stderr, "OpenGL Error: %s\n", errString);
    }
}
void display(){
    glClear(GL_COLOR_BUFFER_BIT);

    GLboolean usingTexture = glGetUniformLocation(program, "useTexture");
    glUniform1i(usingTexture, 0);

    yourGrid.drawGrid();
    computerGrid.drawGrid();

    glUniform1i(usingTexture, 1);

    for(int i=1; i<=5; i++){
       yourShips[i].drawImage();
    }

    glUniform1i(usingTexture, 0);
    for(int x=1; x<=10; x++){
        for(int y=1; y<=10; y++){
            if(yourGrid.getPicked(x-1, y-1)){
                yourSquaresPicked[x][y].draw();
            }
            if(computerGrid.getPicked(x-1, y-1))
                computerSquaresPicked[x][y].draw();
        }
    }
    printOpenGLErrors();
    glutSwapBuffers();
}

void reshape(GLint width, GLint height)
{
    glViewport(0, 0, width, height);

    mat4 projection;

    if (height == 0 || width == 0){
        xMin=-100;
        xMax=100;
        yMin=-300;
        yMax=300;
    }
    else if (width > 3*height){
        xMin=-100*width/height;
        xMax=100*width/height;
        yMin=-100;
        yMax=-100;
    }
    else{
        xMin=-300;
        xMax=300;
        yMin=-300*height/width;
        yMax=300*height/width;
    }


    //std::cout << projection << std::endl;
    projection = Ortho2D(xMin, xMax, yMin, yMax);
    GLuint projloc = glGetUniformLocation(program, "Projection");
    glUniformMatrix4fv(projloc, 1, GL_TRUE, projection);
}

void autoPlaceShips(Grid &targetGrid, Ship targetShips[]){
    for(int i=1; i<=5; i++){
       if(targetShips[i].isItPlaced()){
            targetShips[i].removeShip(targetGrid);
       }
    }
    for(int i=1; i<=5; i++){
        do{
            bool rotator=rand()%2;
            int xPos=rand()%10;
            int yPos=rand()%10;
            if(rotator)
                targetShips[i].rotateShip();
            targetShips[i].placeShip(xPos, yPos, targetGrid);
        }while(!targetShips[i].isItPlaced());
    }
}

void keyboard(unsigned char key, int x, int y){
    switch(key){
    case 033:  // ASCII Escape Key Code
        exit(EXIT_SUCCESS);
        break;
    case 'a':
        if(!gamePhase){
            autoPlaceShips(yourGrid, yourShips);
        }
        break;
    case 'c':
        if(yourShips[1].isItPlaced() && yourShips[2].isItPlaced() && yourShips[3].isItPlaced() && yourShips[4].isItPlaced() && yourShips[5].isItPlaced() && !gamePhase){
            autoPlaceShips(computerGrid, computerShips);
            gamePhase=true;
            glutSetWindowTitle("Your Turn");

        }
        break;
    }
    glutPostRedisplay();
}

bool mouseOnShip(Ship focus){
    if(clickx>=focus.getUpperLeftX()
       && clickx<=focus.getUpperLeftX()+focus.getWidth()
       && clicky<=focus.getUpperLeftY()
       && clicky>=focus.getUpperLeftY()-focus.getHeight()){
        return true;
       }
    else{
        return false;
       }

}

void mouse(int button, int state, int x, int y){
    GLint h = glutGet(GLUT_WINDOW_HEIGHT);
    GLint w = glutGet(GLUT_WINDOW_WIDTH);

    if(!gamePhase){
        if((button==GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)){
            if(!mouseClicked){
                mouseClicked=true;
                clickx = xMin+(xMax-xMin)*x/w;
                clicky = yMin+(yMax-yMin)*(h-y)/h;
                for(int i=1; i<=5; i++){
                    if(mouseOnShip(yourShips[i])){
                        mouseClickedOnShip=true;
                        mouseClickedOn=&yourShips[i];
                        mouseClickedOn->setUpperLeft(clickx, clicky);
                        if(mouseClickedOn->isItPlaced()){
                            mouseClickedOn->removeShip(yourGrid);
                        }
                    }
                }
                if(!mouseClickedOnShip)
                    mouseClicked=false;
            }else{
                cout<<mouseClickedOn->getUpperLeftX()<<",  "<<mouseClickedOn->getUpperLeftY()<<endl;
                for(int x=0; x<10; x++){
                    for(int y=0; y<10; y++){
                        if(mouseClickedOn->getUpperLeftX()>=20*x-240 && mouseClickedOn->getUpperLeftX()<=20*(x+1)-240 &&
                                mouseClickedOn->getUpperLeftY()<=(-20*y)+100 && mouseClickedOn->getUpperLeftY()>=(-20*(y+1))+100){

                            mouseClickedOn->placeShip(x,y, yourGrid);
                            break;
                        }
                    }
                    if(mouseClickedOn->isItPlaced()){
                        break;
                    }
                }
                //System.out.print(6);
                if(!mouseClickedOn->isItPlaced()){
                    mouseClickedOn->setUpperLeft(mouseClickedOn->getOriginX(), mouseClickedOn->getOriginY());
                    if(mouseClickedOn->isItHorizontal()){
                        mouseClickedOn->rotateShip();
                    }
                }
                mouseClicked=false;
                mouseClickedOnShip=false;
                mouseClickedOn=NULL;
            }
        }
        else if(button==GLUT_RIGHT_BUTTON && state==GLUT_DOWN){
            if(mouseClickedOnShip){
                mouseClickedOn->rotateShip();
            }
        }
    }
    else if(yourTurn){
        if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
            clickx = xMin+(xMax-xMin)*x/w;
            clicky = yMin+(yMax-yMin)*(h-y)/h;
            for(int x=0; x<10; x++){
                for(int y=0; y<10; y++){
                    if(clickx>=20*x+100 && clickx<=20*(x+1)+100 &&  //if mouse was clicked on a grid square.
                            clicky<=(-20*y)+100 && clicky>=(-20*(y+1))+100){
                        if(!computerGrid.getPicked(x, y)){
                            //yourTurn=false;
                            youChose=true;
                            youChosex=x;
                            youChosey=y;
                            start=clock();
                        }
                    }
                }
            }
        }
    }
}

void mouseMotion(int x, int y){
    GLint h = glutGet(GLUT_WINDOW_HEIGHT);
    GLint w = glutGet(GLUT_WINDOW_WIDTH);
    clickx = xMin+(xMax-xMin)*x/w;
    clicky = yMin+(yMax-yMin)*(h-y)/h;
    if(mouseClickedOnShip){
        mouseClickedOn->setUpperLeft(clickx, clicky);
    }
    glutPostRedisplay();
}

void winnerIs(){
    winnerDecided=true;
    if(yourTurn){
        glutSetWindowTitle("You win!");
    }
    else{
        glutSetWindowTitle("You lose!");
    }
}

void fireMissile(int x, int y, Grid target, rectangle squares[11][11], Ship ships[], bool computerMove){
    int startX;
    int startY;
    if(target.getHitStatus(x, y)){
        squares[x+1][y+1].setColor(1, 0, 0);
        for(int i=1; i<=5; i++){
            if(ships[i].gotHit(x, y)){
                 if(computerMove){
                    if(!computerFoundHit){
                        shipFoundX=x;
                        shipFoundY=y;
                        computerFoundHit=true;
                    }else if(!computerDetectsDirection){
                        lastHitX=x;
                        lastHitY=y;
                        computerDetectsDirection=true;
                        if(x-1==ships[i].getFirstPositionX() || x+1==ships[i].getFirstPositionY()){
                            directionHorizontal=true;
                        }else{
                            directionHorizontal=false;
                        }
                    }else{
                        lastHitX=x;
                        lastHitY=y;
                    }
                }
                if(ships[i].gotSunk()){
                    startX=ships[i].getFirstPositionX();
                    startY=ships[i].getFirstPositionY();
                    for(int j=0; j<ships[i].getLength(); j++){
                        if(ships[i].isItHorizontal()){
                            squares[startX+j+1][startY+1].setColor(0, 0, 0);
                        }
                        else{
                            squares[startX+1][startY+j+1].setColor(0, 0, 0);
                        }
                    }
                    if(computerMove){
                        computerFoundHit=false;
                        computerDetectsDirection=false;
                        directionHorizontal=false;
                        for(int j=1; j<=10; j++){
                            for(int k=1; k<=10; k++){
                                if(squares[j][k].isRed()){
                                    computerFoundHit=true;
                                    shipFoundX=j-1;
                                    shipFoundY=k-1;
                                    if(j!=10){
                                        if(squares[j+1][k].isRed()){
                                            computerDetectsDirection=true;
                                            directionHorizontal=true;
                                        }
                                    }
                                    if(k!=10){
                                        if(squares[j][k+1].isRed()){
                                            computerDetectsDirection=true;
                                            directionHorizontal=false;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    if(ships[1].gotSunk() && ships[2].gotSunk() && ships[3].gotSunk() && ships[4].gotSunk() && ships[5]. gotSunk()){
                        winnerIs();
                    }
                }
            }
        }
    }else{
        squares[x+1][y+1].setColor(1, 1, 1);
        if (computerMove && computerFoundHit){  //When a miss occurs if a ship was hit.
            missWhenHit=true;
        }
    }
}

void oneTurn(){
    int computerChoseX;
    int computerChoseY;
    bool computerChose=false;
    static int directionChoice;
    clock_t timer=clock();
    if(!gamePhase && yourShips[1].isItPlaced() && yourShips[2].isItPlaced() && yourShips[3].isItPlaced() && yourShips[4].isItPlaced() && yourShips[5].isItPlaced()){
        glutSetWindowTitle("Press C to start the game.");
    }
    if(gamePhase && youChose && !winnerDecided){
        int numClocks=(int)(timer-start);
        if(yourTurn){
            if(numClocks%2000==1000){
                fireMissile(youChosex, youChosey, computerGrid, computerSquaresPicked, computerShips, false);
                computerGrid.setPicked(youChosex, youChosey);
                yourTurn=false;
                if(!winnerDecided)
                    glutSetWindowTitle("Computer turn");
                glutPostRedisplay();
            }
        }
        if(!yourTurn){
            if(numClocks%2000==0){
                if(!computerFoundHit){
                    do{
                        computerChoseX=rand()%10;
                        computerChoseY=rand()%10;
                    }while(yourGrid.getPicked(computerChoseX, computerChoseY));
                }else if(!computerDetectsDirection){
                    do{
                        directionChoice=rand()%4;
                        switch(directionChoice){
                        case 0:
                            if(shipFoundX!=0){
                                if(yourSquaresPicked[shipFoundX][shipFoundY+1].isBlue()){
                                    computerChoseX=shipFoundX-1;
                                    computerChoseY=shipFoundY;
                                    computerChose=true;
                                }
                            }
                            break;

                        case 1:
                            if(shipFoundY!=0){ switch(directionChoice){
                        case 0:
                        if(lastHitX!=0 && yourSquaresPicked[lastHitX][lastHitY+1].isBlue()){
                                computerChoseX=lastHitX-1;
                                computerChoseY=lastHitY;
                                computerChose=true;
                        }else if(shipFoundX!=9 && yourSquaresPicked[shipFoundX+2][shipFoundY+1].isBlue()){
                                directionChoice=2;
                                computerChoseX=shipFoundX+1;
                                computerChoseY=shipFoundY;
                                computerChose=true;
                        }else if(shipFoundY!=0 && yourSquaresPicked[shipFoundX+1][shipFoundY].isBlue()){// in case of two adjacent ships
                                computerDetectsDirection=false;
                                directionChoice=1;
                                computerChoseX=shipFoundX;
                                computerChoseY=shipFoundY-1;
                                computerChose=true;
                        }else if(shipFoundY!=9 && yourSquaresPicked[shipFoundX+1][shipFoundY+2].isBlue()){
                                computerDetectsDirection=false;
                                directionChoice=3;
                                computerChoseX=shipFoundX;
                                computerChoseY=shipFoundY+1;
                                computerChose=true;
                        }
                        break;

                        case 1:
                            if(lastHitY!=0 && yourSquaresPicked[lastHitX+1][lastHitY].isBlue()){
                                    computerChoseX=lastHitX;
                                    computerChoseY=lastHitY-1;
                                    computerChose=true;
                            }else if(shipFoundY!=9 && yourSquaresPicked[shipFoundX+1][shipFoundY+2].isBlue()){
                                    directionChoice=3;
                                    computerChoseX=shipFoundX;
                                    computerChoseY=shipFoundY+1;
                                    computerChose=true;
                            }else if(shipFoundX!=0 && yourSquaresPicked[shipFoundX][shipFoundY+1].isBlue()){
                                    computerDetectsDirection=false;
                                    directionChoice=0;
                                    computerChoseX=shipFoundX-1;
                                    computerChoseY=shipFoundY;
                                    computerChose=true;
                            }else  if(shipFoundX!=9 && yourSquaresPicked[shipFoundX+2][shipFoundY+1].isBlue()){
                                    computerDetectsDirection=false;
                                    directionChoice=2;
                                    computerChoseX=shipFoundX+1;
                                    computerChoseY=shipFoundY;
                                    computerChose=true;
                            }
                            break;
                        case 2:
                            if(lastHitX!=9 && yourSquaresPicked[lastHitX+2][lastHitY+1].isBlue()){
                                computerChoseX=lastHitX+1;
                                computerChoseY=lastHitY;
                                computerChose=true;
                            }else if(shipFoundX!=0 && yourSquaresPicked[shipFoundX][shipFoundY+1].isBlue()){
                                directionChoice=0;
                                computerChoseX=shipFoundX-1;
                                computerChoseY=shipFoundY;
                                computerChose=true;
                            }else if(shipFoundY!=0 && yourSquaresPicked[shipFoundX+1][shipFoundY].isBlue()){
                                computerDetectsDirection=false;
                                directionChoice=1;
                                computerChoseX=shipFoundX;
                                computerChoseY=shipFoundY-1;
                                computerChose=true;
                            }else if(shipFoundY!=9 && yourSquaresPicked[shipFoundX+1][shipFoundY+2].isBlue()){
                                computerDetectsDirection=false;
                                directionChoice=3;
                                computerChoseX=shipFoundX;
                                computerChoseY=shipFoundY+1;
                                computerChose=true;
                            }
                            break;
                        case 3:
                            if(lastHitY!=9 && yourSquaresPicked[lastHitX+1][lastHitY+2].isBlue()){
                                computerChoseX=lastHitX;
                                computerChoseY=lastHitY+1;
                                computerChose=true;
                            }else if(shipFoundY!=0 && yourSquaresPicked[shipFoundX+1][shipFoundY].isBlue()){
                                directionChoice=1;
                                computerChoseX=shipFoundX;
                                computerChoseY=shipFoundY-1;
                                computerChose=true;
                            }else if(shipFoundX!=0 && yourSquaresPicked[shipFoundX][shipFoundY+1].isBlue()){
                                computerDetectsDirection=false;
                                directionChoice=0;
                                computerChoseX=shipFoundX-1;
                                computerChoseY=shipFoundY;
                                computerChose=true;
                            }else if(shipFoundX!=9 && yourSquaresPicked[shipFoundX+2][shipFoundY+1].isBlue()){
                                computerDetectsDirection=false;
                                directionChoice=2;
                                computerChoseX=shipFoundX+1;
                                computerChoseY=shipFoundY;
                                computerChose=true;
                            }
                            break;
                        }
                                if(yourSquaresPicked[shipFoundX+1][shipFoundY].isBlue()){
                                    computerChoseX=shipFoundX;
                                    computerChoseY=shipFoundY-1;
                                    computerChose=true;
                                }
                            }
                            break;
                        case 2:
                            if(shipFoundX!=9){
                                if(yourSquaresPicked[shipFoundX+2][shipFoundY+1].isBlue()){
                                    computerChoseX=shipFoundX+1;
                                    computerChoseY=shipFoundY;
                                    computerChose=true;
                                }
                            }
                            break;
                        case 3:
                            if(shipFoundY!=9){
                                if(yourSquaresPicked[shipFoundX+1][shipFoundY+2].isBlue()){
                                    computerChoseX=shipFoundX;
                                    computerChoseY=shipFoundY+1;
                                    computerChose=true;
                                }
                            }
                            break;
                        }
                    }while(!computerChose);
                }else{
                    switch(directionChoice){
                        case 0:
                        if(lastHitX!=0 && yourSquaresPicked[lastHitX][lastHitY+1].isBlue()){
                                computerChoseX=lastHitX-1;
                                computerChoseY=lastHitY;
                                computerChose=true;
                        }else if(shipFoundX!=9 && yourSquaresPicked[shipFoundX+2][shipFoundY+1].isBlue()){
                                directionChoice=2;
                                computerChoseX=shipFoundX+1;
                                computerChoseY=shipFoundY;
                                computerChose=true;
                        }else if(shipFoundY!=0 && yourSquaresPicked[shipFoundX+1][shipFoundY].isBlue()){// in case of two adjacent ships
                                computerDetectsDirection=false;
                                directionChoice=1;
                                computerChoseX=shipFoundX;
                                computerChoseY=shipFoundY-1;
                                computerChose=true;
                        }else if(shipFoundY!=9 && yourSquaresPicked[shipFoundX+1][shipFoundY+2].isBlue()){
                                computerDetectsDirection=false;
                                directionChoice=3;
                                computerChoseX=shipFoundX;
                                computerChoseY=shipFoundY+1;
                                computerChose=true;
                        }
                        break;

                        case 1:
                            if(lastHitY!=0 && yourSquaresPicked[lastHitX+1][lastHitY].isBlue()){
                                    computerChoseX=lastHitX;
                                    computerChoseY=lastHitY-1;
                                    computerChose=true;
                            }else if(shipFoundY!=9 && yourSquaresPicked[shipFoundX+1][shipFoundY+2].isBlue()){
                                    directionChoice=3;
                                    computerChoseX=shipFoundX;
                                    computerChoseY=shipFoundY+1;
                                    computerChose=true;
                            }else if(shipFoundX!=0 && yourSquaresPicked[shipFoundX][shipFoundY+1].isBlue()){
                                    computerDetectsDirection=false;
                                    directionChoice=0;
                                    computerChoseX=shipFoundX-1;
                                    computerChoseY=shipFoundY;
                                    computerChose=true;
                            }else  if(shipFoundX!=9 && yourSquaresPicked[shipFoundX+2][shipFoundY+1].isBlue()){
                                    computerDetectsDirection=false;
                                    directionChoice=2;
                                    computerChoseX=shipFoundX+1;
                                    computerChoseY=shipFoundY;
                                    computerChose=true;
                            }
                            break;
                        case 2:
                            if(lastHitX!=9 && yourSquaresPicked[lastHitX+2][lastHitY+1].isBlue()){
                                computerChoseX=lastHitX+1;
                                computerChoseY=lastHitY;
                                computerChose=true;
                            }else if(shipFoundX!=0 && yourSquaresPicked[shipFoundX][shipFoundY+1].isBlue()){
                                directionChoice=0;
                                computerChoseX=shipFoundX-1;
                                computerChoseY=shipFoundY;
                                computerChose=true;
                            }else if(shipFoundY!=0 && yourSquaresPicked[shipFoundX+1][shipFoundY].isBlue()){
                                computerDetectsDirection=false;
                                directionChoice=1;
                                computerChoseX=shipFoundX;
                                computerChoseY=shipFoundY-1;
                                computerChose=true;
                            }else if(shipFoundY!=9 && yourSquaresPicked[shipFoundX+1][shipFoundY+2].isBlue()){
                                computerDetectsDirection=false;
                                directionChoice=3;
                                computerChoseX=shipFoundX;
                                computerChoseY=shipFoundY+1;
                                computerChose=true;
                            }
                            break;
                        case 3:
                            if(lastHitY!=9 && yourSquaresPicked[lastHitX+1][lastHitY+2].isBlue()){
                                computerChoseX=lastHitX;
                                computerChoseY=lastHitY+1;
                                computerChose=true;
                            }else if(shipFoundY!=0 && yourSquaresPicked[shipFoundX+1][shipFoundY].isBlue()){
                                directionChoice=1;
                                computerChoseX=shipFoundX;
                                computerChoseY=shipFoundY-1;
                                computerChose=true;
                            }else if(shipFoundX!=0 && yourSquaresPicked[shipFoundX][shipFoundY+1].isBlue()){
                                computerDetectsDirection=false;
                                directionChoice=0;
                                computerChoseX=shipFoundX-1;
                                computerChoseY=shipFoundY;
                                computerChose=true;
                            }else if(shipFoundX!=9 && yourSquaresPicked[shipFoundX+2][shipFoundY+1].isBlue()){
                                computerDetectsDirection=false;
                                directionChoice=2;
                                computerChoseX=shipFoundX+1;
                                computerChoseY=shipFoundY;
                                computerChose=true;
                            }
                            break;
                        }
                }
                fireMissile(computerChoseX, computerChoseY, yourGrid, yourSquaresPicked, yourShips, true);
                yourGrid.setPicked(computerChoseX, computerChoseY);
                yourTurn=true;
                youChose=false;
                if(!winnerDecided)
                    glutSetWindowTitle("Your Turn");
                glutPostRedisplay();
            }
        }
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(1536, 512);
    glutInitContextVersion(4, 0);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutCreateWindow("Welcome to Battleship: Place your ships or press A");

    if (glewInit())
    {
        std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
        exit(EXIT_FAILURE);
    }

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(mouseMotion);
    glutIdleFunc(oneTurn);
    glutReshapeFunc(reshape);

    glutMainLoop();
}

