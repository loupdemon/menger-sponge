#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

const float ROTATION_SPEED = 3.0f;
const float INITIAL_LENGTH = 10.0f;
const float ROTATE_BTN_LENGTH = 0.1f;

enum {
  MENU_INCREASE_DEPTH = 1,
  MENU_DECREASE_DEPTH,
  MENU_EXIT
};

// Rotation controls
int leftPressed = false;
int rightPressed = false;

int rotateRight = false;
int rotateLeft = false;
int rotateUp = false;
int rotateDown = false;

float rotateLeftX[2];
float rotateLeftY[2];
float rotateRightX[2];
float rotateRightY[2];
float rotateUpX[2];
float rotateUpY[2];
float rotateDownX[2];
float rotateDownY[2];

/* Point class defines a point in 3d space */
class Point
{
    public:
        Point(GLfloat x, GLfloat y, GLfloat z);
        GLfloat x,y,z;
};

/* Point constructor */
Point::Point(GLfloat _x, GLfloat _y, GLfloat _z)
{
    x = _x;
    y = _y;
    z = _z;
}

/* Cube class defines a cube */
class Cube
{
    public:
        Cube(Point* center, GLfloat eL, GLfloat depth);
        void DisplayCube();
     
    private:
        void DivideCube(Point* center, GLfloat e, GLfloat d);
        void DefineCube(Point* center, GLfloat e);
        void DrawCubeFace(Point** vertices);
     
        Point** vertexPoints;
        Point** topVertices;
        Point** bottomVertices;
        Point** frontVertices;
        Point** backVertices;
        Point** rightVertices;
        Point** leftVertices;
        Cube** newCubes;
};

/* Cube constructor, called recursively from DivideCube function
 * paramters: center point of cube, edge length, depth */
Cube::Cube(Point* center, GLfloat eL, GLfloat depth)
{
    if(depth <= 1)
        DefineCube(center, eL);
    else
        DivideCube(center, eL, depth);
}

void Cube::DisplayCube()
{
    if(vertexPoints)
    {

        glBegin(GL_QUADS);
            // Color each of 6 faces a different color RGBCYM
            glColor3f(1.0,0.0,0.0);
           
            DrawCubeFace(topVertices);

            glColor3f(0.0,1.0,1.0);
            DrawCubeFace(leftVertices);
             
            glColor3f(0.0,1.0,0.0);
            DrawCubeFace(backVertices);

            glColor3f(1.0,0.0,1.0);
            DrawCubeFace(bottomVertices);
             
            glColor3f(0.0,0.0,1.0);
            DrawCubeFace(rightVertices);

            glColor3f(1.0,1.0,0.0);
            DrawCubeFace(frontVertices);
        glEnd();
    }
 
    if(newCubes)
        for(int i = 0; i < 20; i++)
            newCubes[i]->DisplayCube();
}

/* Define the cube by its eight vertices */
void Cube::DefineCube(Point* center, GLfloat eL)
{
    newCubes = NULL;

    // Compute new 8 new vertex coordinates (x, y, z) for smaller cube
    GLfloat nL = eL / 2.0;
 
    GLfloat x = nL+center->x;
    GLfloat y = nL+center->y;
    GLfloat z = nL+center->z;

    GLfloat x2 = -nL+center->x;
    GLfloat y2 = -nL+center->y;
    GLfloat z2 = -nL+center->z;

    vertexPoints = new Point*[8];

    // top vertices, clockwise starting from top right
    vertexPoints[0] = new Point( x,  y, z);
    vertexPoints[1] = new Point( x, y2, z);
    vertexPoints[2] = new Point(x2, y2,  z);
    vertexPoints[3] = new Point(x2,  y,  z);
    
    // bottom vertices, clockwise starting from top right
    vertexPoints[4] = new Point( x,  y, z2);
    vertexPoints[5] = new Point( x, y2, z2);
    vertexPoints[6] = new Point(x2, y2, z2);
    vertexPoints[7] = new Point(x2,  y, z2);

    // Arrays to hold vertices for each individual face
    topVertices     = new Point*[4];
    bottomVertices  = new Point*[4];
    frontVertices   = new Point*[4];
    backVertices    = new Point*[4];
    rightVertices   = new Point*[4];
    leftVertices    = new Point*[4];

    // Define top face vertices
    topVertices[0] = vertexPoints[0];
    topVertices[1] = vertexPoints[1];
    topVertices[2] = vertexPoints[2];
    topVertices[3] = vertexPoints[3];

    // Define bottom face vertices
    bottomVertices[0] = vertexPoints[4];
    bottomVertices[1] = vertexPoints[5];
    bottomVertices[2] = vertexPoints[6];
    bottomVertices[3] = vertexPoints[7];

    // Define front face vertices
    frontVertices[0] = vertexPoints[1];
    frontVertices[1] = vertexPoints[5];
    frontVertices[2] = vertexPoints[6];
    frontVertices[3] = vertexPoints[2];

    // Define back facevertices
    backVertices[0] = vertexPoints[0];
    backVertices[1] = vertexPoints[4];
    backVertices[2] = vertexPoints[7];
    backVertices[3] = vertexPoints[3];

    // Define right face vertices
    rightVertices[0] = vertexPoints[0];
    rightVertices[1] = vertexPoints[4];
    rightVertices[2] = vertexPoints[5];
    rightVertices[3] = vertexPoints[1];

    // Define left face vertices
    leftVertices[0] = vertexPoints[3];
    leftVertices[1] = vertexPoints[7];
    leftVertices[2] = vertexPoints[6];
    leftVertices[3] = vertexPoints[2];
}

/* Sub divide cube into 20 new, smaller cubes
 * paramters: cube center point, edge length, depth */
void Cube::DivideCube(Point* center, GLfloat eL, GLfloat d)
{
    // New length and new depth
    GLfloat nL = eL / 3.0;
    d -= 1;
 
    /* Each cube is divided into 27 smaller cubes
     * 7 of these cubes are removed (1 from each of 6 faces + center)
     * This results in 20 solid cubes remaining */
    newCubes = new Cube*[20];
    vertexPoints = NULL;
 
    // Compute relative center points for each new sub cube
    GLfloat x = nL+center->x;
    GLfloat y = nL+center->y;
    GLfloat z = nL+center->z;

    GLfloat x2 = -nL+center->x;
    GLfloat y2 = -nL+center->y;
    GLfloat z2 = -nL+center->z;

    GLfloat x0 = center->x;
    GLfloat y0 = center->y;
    GLfloat z0 = center->z;

    newCubes[0] = new Cube(new Point(x2,  y,  z), nL, d);
    newCubes[1] = new Cube(new Point(x0,  y,  z), nL, d);
    newCubes[2] = new Cube(new Point( x,  y,  z), nL, d);
    newCubes[3] = new Cube(new Point( x, y0,  z), nL, d);
    newCubes[4] = new Cube(new Point( x, y2,  z), nL, d);
    newCubes[5] = new Cube(new Point(x0, y2,  z), nL, d);
    newCubes[6] = new Cube(new Point(x2, y2,  z), nL, d);
    newCubes[7] = new Cube(new Point(x2, y0,  z), nL, d);
    newCubes[8] = new Cube(new Point( x,  y,  z0), nL, d);
    newCubes[9] = new Cube(new Point( x, y2,  z0), nL, d);
    newCubes[10] = new Cube(new Point(x2, y,  z0), nL, d);
    newCubes[11] = new Cube(new Point(x2, y2, z0), nL, d);
    newCubes[12] = new Cube(new Point(x2,  y, z2), nL, d);
    newCubes[13] = new Cube(new Point(x0,  y, z2), nL, d);
    newCubes[14] = new Cube(new Point( x,  y, z2), nL, d);
    newCubes[15] = new Cube(new Point( x, y0, z2), nL, d);
    newCubes[16] = new Cube(new Point( x, y2, z2), nL, d);
    newCubes[17] = new Cube(new Point(x0, y2, z2), nL, d);
    newCubes[18] = new Cube(new Point(x2, y2, z2), nL, d);
    newCubes[19] = new Cube(new Point(x2, y0, z2), nL, d);
}

/* Draw a cube face given 4 face vertices */
void Cube::DrawCubeFace(Point** vertices)
{
    for(int i = 0; i < 4; i++)
    {
        glVertex3f(vertices[i]->x, vertices[i]->y, vertices[i]->z);
    }
}

Cube* MengerSponge;

/* called when events are not being received */
void idle()
{
    // only redisplay if cube is rotating
    if (rotateRight || rotateLeft || rotateUp || rotateDown)
        glutPostRedisplay();
}

int WINDOW_WIDTH;
int WINDOW_HEIGHT;
void drawRotateLeftBtn()
{
    rotateLeftX[0] = 0.0f;
    rotateLeftX[1] = (float) WINDOW_WIDTH * ROTATE_BTN_LENGTH;
    rotateLeftY[0] = WINDOW_HEIGHT/2.0f - (float)WINDOW_HEIGHT*ROTATE_BTN_LENGTH/2.0f;
    rotateLeftY[1] = WINDOW_HEIGHT/2.0f + (float)WINDOW_HEIGHT*ROTATE_BTN_LENGTH/2.0f;

    GLfloat buttonLength = (float)GLUT_WINDOW_HEIGHT*ROTATE_BTN_LENGTH;
    GLfloat yStart  = (GLUT_WINDOW_HEIGHT/2.0f)-buttonLength/2.0f;
    GLfloat yEnd    = yStart + buttonLength;
    GLfloat xStart  = 0.0f;
    GLfloat xEnd    = buttonLength;

    glLineWidth(8.0f);
    glColor3f(0.9, 0.0, 0.9);
    glBegin(GL_LINES);
        glVertex3f(xStart + buttonLength*0.2f, yStart+buttonLength/2.0f, 0.0f);
        glVertex3f(xStart + buttonLength*0.8f, yEnd-buttonLength*0.2f, 0);
    glEnd();

    glBegin(GL_LINES);
        glVertex3f(xStart + buttonLength*0.2f, yStart+buttonLength/2.0f, 0);
        glVertex3f(xStart + buttonLength*0.8f, yStart+buttonLength*0.2f, 0.0f);
    glEnd();

    glColor3f(0.9, 0.9, 0.9);
    glBegin(GL_POLYGON);
        glVertex3f(xStart, yStart, 0.0f);
        glVertex3f(xStart, yEnd, 0.0f);
        glVertex3f(xEnd, yEnd, 0.0f);
        glVertex3f(xEnd, yStart, 0.0f);
    glEnd();
}

void drawRotateRightBtn()
{
    rotateRightX[0] = WINDOW_WIDTH - (float)WINDOW_WIDTH*ROTATE_BTN_LENGTH;
    rotateRightX[1] = (float)WINDOW_WIDTH;
    rotateRightY[0] = WINDOW_HEIGHT/2.0f - (float)WINDOW_HEIGHT*ROTATE_BTN_LENGTH/2.0f;
    rotateRightY[1] = WINDOW_HEIGHT/2.0f + (float)WINDOW_HEIGHT*ROTATE_BTN_LENGTH/2.0f;

    //printf("%f %f\n", rotateLeftY[0], rotateLeftY[1]);

    GLfloat buttonLength = (float)GLUT_WINDOW_HEIGHT*ROTATE_BTN_LENGTH;
    GLfloat yStart  = (GLUT_WINDOW_HEIGHT/2.0f)-buttonLength/2.0f;
    GLfloat yEnd    = yStart + buttonLength;
    GLfloat xStart  = GLUT_WINDOW_WIDTH - buttonLength;
    GLfloat xEnd    = GLUT_WINDOW_WIDTH;

    glLineWidth(8.0f);
    glColor3f(1.0, 0.2, 0.2);
    glBegin(GL_LINES);
        glVertex3f(xEnd-buttonLength*0.2f, yStart+buttonLength/2.0f, 0.0f);
        glVertex3f(xEnd-buttonLength*0.8f, yEnd-buttonLength*0.2f, 0);
    glEnd();

    glBegin(GL_LINES);
        glVertex3f(xEnd-buttonLength*0.2f, yStart+buttonLength/2.0f, 0);
        glVertex3f(xEnd-buttonLength*0.8f, yEnd-buttonLength+buttonLength*0.2f, 0.0f);
    glEnd();

    glColor3f(0.9, 0.9, 0.9);
    glBegin(GL_POLYGON);
        glVertex3f(xStart, yStart, 0.0f);
        glVertex3f(xStart, yEnd, 0.0f);
        glVertex3f(xEnd, yEnd, 0.0f);
        glVertex3f(xEnd, yStart, 0.0f);
    glEnd();
}

void drawRotateUpBtn()
{
    rotateUpX[0] = WINDOW_WIDTH/2.0f - (float)WINDOW_WIDTH*ROTATE_BTN_LENGTH/2.0f;
    rotateUpX[1] = WINDOW_WIDTH/2.0f + (float)WINDOW_WIDTH*ROTATE_BTN_LENGTH/2.0f;
    rotateUpY[0] = 0.0f;
    rotateUpY[1] = (float)WINDOW_HEIGHT*ROTATE_BTN_LENGTH;

    GLfloat buttonLength = (float)GLUT_WINDOW_HEIGHT*ROTATE_BTN_LENGTH;
    GLfloat yStart  = GLUT_WINDOW_HEIGHT;
    GLfloat yEnd    = yStart-buttonLength;
    GLfloat xStart  = GLUT_WINDOW_WIDTH/2.0f-buttonLength/2.0f;
    GLfloat xEnd    = xStart + buttonLength;

    glLineWidth(8.0f);
    glColor3f(0.0, 0.7, 0.0);
    glBegin(GL_LINES);
        glVertex3f(xStart+buttonLength/2.0f, yStart-buttonLength*0.2f, 0.0f);
        glVertex3f(xStart+buttonLength*0.2f, yEnd+buttonLength*0.2f, 0);
    glEnd();

    glBegin(GL_LINES);
        glVertex3f(xStart+buttonLength/2.0f, yStart-buttonLength*0.2f, 0);
        glVertex3f(xEnd-buttonLength*0.2f, yEnd+buttonLength*0.2f, 0.0f);
    glEnd();

    glColor3f(0.9, 0.9, 0.9);
    glBegin(GL_POLYGON);
        glVertex3f(xStart, yStart, 0.0f);
        glVertex3f(xStart, yEnd, 0.0f);
        glVertex3f(xEnd, yEnd, 0.0f);
        glVertex3f(xEnd, yStart, 0.0f);
    glEnd();
}

void drawRotateDownBtn()
{
    rotateDownX[0] = WINDOW_WIDTH/2.0f - (float)WINDOW_WIDTH*ROTATE_BTN_LENGTH/2.0f;
    rotateDownX[1] = WINDOW_WIDTH/2.0f + (float)WINDOW_WIDTH*ROTATE_BTN_LENGTH/2.0f;
    rotateDownY[0] = WINDOW_HEIGHT - (float)WINDOW_HEIGHT*ROTATE_BTN_LENGTH;
    rotateDownY[1] = WINDOW_HEIGHT;

    GLfloat buttonLength = (float)GLUT_WINDOW_HEIGHT*ROTATE_BTN_LENGTH;
    GLfloat yStart  = 0.0f + buttonLength;
    GLfloat yEnd    = 0.0f;
    GLfloat xStart  = GLUT_WINDOW_WIDTH/2.0f-buttonLength/2.0f;
    GLfloat xEnd    = xStart + buttonLength;

    glLineWidth(8.0f);
    glColor3f(0.3, 0.5, 1.0);
    glBegin(GL_LINES);
        glVertex3f(xStart + buttonLength*0.2f, buttonLength-buttonLength*0.2f, 0.0f);
        glVertex3f(xStart + buttonLength/2.0f, buttonLength*0.2f, 0);
    glEnd();

    glBegin(GL_LINES);
        glVertex3f(xStart + buttonLength/2.0f, buttonLength*0.2f, 0);
        glVertex3f(xEnd - buttonLength*0.2f, buttonLength-buttonLength*0.2f, 0.0f);
    glEnd();

    glColor3f(0.9, 0.9, 0.9);
    glBegin(GL_POLYGON);
        glVertex3f(xStart, yStart, 0.0f);
        glVertex3f(xStart, yEnd, 0.0f);
        glVertex3f(xEnd, yEnd, 0.0f);
        glVertex3f(xEnd, yStart, 0.0f);
    glEnd();
}

bool clickedLeftRotate(int x, int y)
{
    return (x >= rotateLeftX[0] && x <= rotateLeftX[1]
            && y >= rotateLeftY[0] && y <= rotateLeftY[1]);
}

bool clickedRightRotate(int x, int y)
{
    return (x >= rotateRightX[0] && x <= rotateRightX[1]
            && y >= rotateRightY[0] && y <= rotateRightY[1]);
}

bool clickedUpRotate(int x, int y)
{
    return (x >= rotateUpX[0] && x <= rotateUpX[1]
            && y >= rotateUpY[0] && y <= rotateUpY[1]);
}

bool clickedDownRotate(int x, int y)
{
    return (x >= rotateDownX[0] && x <= rotateDownX[1]
            && y >= rotateDownY[0] && y <= rotateDownY[1]);
}

float rotX = 0.0f;
float rotY = 0.0f;
float rotZ = 90.0f;
/* main display function */
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float radX = (M_PI * rotX)/180.0f;
    float radY = (M_PI * rotY)/180.0f;
    float radZ = (M_PI * rotZ)/180.0f;

    /* Determine which direction to rotate in mouse buttons are pressed */
    if (rotateLeft)
    {
        glRotatef(ROTATION_SPEED, -0.5, 0.0, 0.0);
    }
    else if (rotateRight)
    {
        glRotatef(ROTATION_SPEED, 0.5, 0.0, 0.0);
    }
    else if (rotateUp)
    {
        glRotatef(ROTATION_SPEED, 0.0, 0.5, 0.0);
    }
    else if (rotateDown)
    {
        glRotatef(ROTATION_SPEED, 0.0, -0.5, 0.0);
    }

    // draw 3d
    MengerSponge->DisplayCube();
    // End draw 3d

    // draw 2d
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    gluOrtho2D(0.0f, GLUT_WINDOW_WIDTH, 0.0f, GLUT_WINDOW_HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

        /* draw stuff */
    drawRotateLeftBtn();
    drawRotateRightBtn();
    drawRotateUpBtn();
    drawRotateDownBtn();

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    // End draw2d

    glutSwapBuffers();
}

 /* called upon resizing window */
void onReshape(int w, int h)
{
    WINDOW_HEIGHT = h;
    WINDOW_WIDTH = w;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(70.0f, (1.0f*w/h), 1, 2000); // FoV, aspect ratio, distance to near clipping plane, distance to far flipping plane
    gluLookAt(
        0.0,12.0,20.0,  // camera/eye point
        0.0,0.0,0.0,    // position of reference point
        0.0,1.0,0.0);   // direction of the up vector

    glRotatef(90.0f, 0.0, 0.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
}

/* mouse button click callback
 * determine direction of cube rotation*/
void onMouse(int button, int state, int x, int y) {
    float x1 = x /(float) GLUT_WINDOW_WIDTH;
    float y1 = y /(float) GLUT_WINDOW_HEIGHT;

    // printf("Clicked: %d %d\n", x, y);

    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_UP)
        {
            rotateRight = false;
            rotateLeft = false;
            rotateUp = false;
            rotateDown = false;
        }
        else
        {
            rotateLeft  = clickedLeftRotate(x, y);
            rotateRight = clickedRightRotate(x, y);
            rotateUp    = clickedUpRotate(x, y);
            rotateDown  = clickedDownRotate(x, y);
        }
    }
}

/* called when mouse cursor is moved */
void onMotion(int x, int y)
{

}

int depth = 3;
void SelectFromMenu(int idCommand)
{
    switch (idCommand)
    {
        case MENU_INCREASE_DEPTH:
            depth += 1;
            if (depth > 5) depth = 5;
            else 
            {
                Point center(0.0, 0.0, 0.0);
                MengerSponge = new Cube(&center, INITIAL_LENGTH, depth);
            }
            break;
        case MENU_DECREASE_DEPTH:
            depth -= 1;
            if (depth < 1) depth = 1;
            else 
            {
                Point center(0.0, 0.0, 0.0);
                MengerSponge = new Cube(&center, INITIAL_LENGTH, depth);
            }
            break; 
        case MENU_EXIT:
            exit (0);
            break;
    }
    glutPostRedisplay();
}

int BuildPopupMenu (void)
{
  int menu;
  menu = glutCreateMenu (SelectFromMenu);
  glutAddMenuEntry ("Increase depth", MENU_INCREASE_DEPTH);
  glutAddMenuEntry ("Decrease depth", MENU_DECREASE_DEPTH);
  glutAddMenuEntry ("Exit demo", MENU_EXIT);
  return menu;
}

void initLight()
{
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat mat_shininess[] = { 50.0 };
   GLfloat light_position[] = { 1.0, 1.0, 8.0, 0.0 };
   glShadeModel (GL_SMOOTH);

   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
}

int main(int argc, char* argv[])
{
    WINDOW_HEIGHT = 500;
    WINDOW_WIDTH = 500;
    depth = (argc > 1) ? atoi(argv[1]) : 3;
    if (depth > 5) depth = 5;
 
    // Initialize the sponge cube at origin
    Point center(0.0, 0.0, 0.0);
    MengerSponge = new Cube(&center, INITIAL_LENGTH, depth);
 
    // Initialize window and display
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500,500);
    glutInitWindowPosition(0,0);
    glutCreateWindow("Menger Sponge");

    // Bind callbacks
    glutReshapeFunc(onReshape);
    glutDisplayFunc(display);
    glutMouseFunc(onMouse);
    glutMotionFunc(onMotion);
    glutIdleFunc(idle);

    //initLight();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 0.0);

    BuildPopupMenu ();
    glutAttachMenu (GLUT_RIGHT_BUTTON);

    glutMainLoop();
}