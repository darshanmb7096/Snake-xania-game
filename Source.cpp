#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <gl/glut.h>

#define UP 1
#define Down 2
#define LEFT 3
#define RIGHT 4

GLint   lvl = 1;
GLint   points = 0;
GLint   size = 0;
GLbyte  gameOver = false;
static int  life = 3;
GLbyte lifeOver = false;
GLbyte  EnableLight = true;


GLint   bodyPos[2][100] = { {} };
GLint   _x = 5;
GLint   _z = 10;
GLint   _oldX[2] = {};
GLint   _oldZ[2] = {};
GLbyte  direction = 0;


GLint   _bx = 0;
GLint   _bz = 0;


GLint   _w = 800;
GLint   _h = 550;
GLint   _Giw = 0;
GLint   _Gih = 0;
GLint   _Gfw = 150;
GLint   _Gfh = 150;


static GLfloat view_rotx = 45.0F;
static GLfloat view_roty = 0.0F;
static GLfloat view_rotz = 0.0F;

static GLfloat headRotation = 90.0F;
static GLfloat zoom = -300.0f;

void initLight()
{

    GLfloat ambientColor[] = { 0.3f, 0.4f, 0.8f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

    GLfloat lightColor0[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat lightPos0[] = { 4.0f, 0.0f, 8.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

    GLfloat lightColor1[] = { 0.5f, 0.2f, 0.2f, 1.0f };

    GLfloat lightPos1[] = { -1.0f, 0.5f, 0.5f, 0.0f };
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
}

void Initialize(void)
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.8f, 0.8f, 2.5f, 2.0f); //Change the background to sky blue
    if (EnableLight) {
        glEnable(GL_COLOR_MATERIAL);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
        glEnable(GL_NORMALIZE);
    }
}

void resize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1, 800.0);
}
void Write(char* string) {
    while (*string)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *string++);
}

void ManipulateViewAngle() {
    glRotatef(view_rotx, 1.0, 0.0, 0.0);
    glRotatef(view_roty, 0.0, 1.0, 0.0);
    glRotatef(view_rotz, 0.0, 0.0, 1.0);
}

void Reset() {
    _x = 5;
    _z = 10;
    direction = 0;
    lvl = 1;
    points = 0;
    size = 0;
    gameOver = false;
    lifeOver = false;
    life = 3;
    view_rotx = 45.0F;
    view_roty = 0.0F;
    view_rotz = 0.0F;
    headRotation = 90.0F;
}

void WelcomeScreen() {
    char tmp_str[40];

    glRasterPos2f(-10, 60);
    char text1[] = "SIMPLE SNAKE GAME";
    for (int i = 0; i < strlen(text1); i++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text1[i]);
    glRasterPos2f(-10, 50);
    char text2[] = "-Developed by Darshan and Omkar";
    for (int i = 0; i < strlen(text2); i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text2[i]);
}
void DrawSnake() {
    int  i;
    glPushMatrix();
    ManipulateViewAngle();
    glPushMatrix();
    glColor3f(0.0, 0.6, 0.2);
    glTranslatef(75.0, -16.0, 75.0);
    glScalef(155, 5.0, 155);
    glutSolidCube(1);
    glPopMatrix();
    glColor3f(1, 0, 0);
    glTranslatef(_x, -10.0, _z);
    glScalef(0.5, 0.5, 0.5);
    glutSolidSphere(10, 20, 20);
    glRotatef(headRotation, 0.0, 1.0, 0.0);
    glColor3f(1, 0, 0);
    glTranslatef(0, 0.0, 6.0);
    glScalef(0.8, 1.0, 1.0);
    glutSolidCone(10, 10, 20, 20);
    glColor3f(1, 1, 1);
    glTranslatef(-4.0, 10.0, 0.0);
    glScalef(0.3, 0.3, 0.3);
    glutSolidSphere(10, 20, 20);
    glTranslatef(26.0, 0.0, 0.0);
    glutSolidSphere(10, 20, 20);
    glPopMatrix();


    for (i = 0; i < size; i++) {
        glPushMatrix();
        ManipulateViewAngle();
        glTranslatef(bodyPos[0][i], -10.0, bodyPos[1][i]);
        glColor3f(0, 1, 1);
        glScalef(0.5, 0.5, 0.5);
        glutSolidSphere(7, 20, 20);
        glPopMatrix();
    }
}
void DrawFood()
{

    glPushMatrix();
    ManipulateViewAngle();
    glTranslatef(_bx, -10.0, _bz);
    glColor3f(0, 0, 0);
    glScalef(0.5, 0.5, 0.5);
    glutSolidSphere(7, 20, 20);
    glPopMatrix();
}
void GameStatus() {
    char tmp_str[40];


    glColor3f(0, 0, 0);
    glRasterPos2f(-4, 10);
    sprintf_s(tmp_str, "Level: %d Points: %d Life: %d", lvl, points, life);
    Write(tmp_str);
}
void GameOver() {
    char tmp_str[40];
    glRasterPos2f(-40, 60);
    char text1[] = "GAME OVER!! PRESS S TO RESTART ";
    for (int i = 0; i < strlen(text1); i++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text1[i]);
    glColor3f(0, 0, 0);
    glRasterPos2f(-4, 40);
    char text2[] = "Press Esc To Exit";
    for (int i = 0; i < strlen(text2); i++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text2[i]);
    glRasterPos2f(-4, 10);
    sprintf_s(tmp_str, "Level: %d Points: %d Life: %d", lvl, points, life);
    Write(tmp_str);

}
void LifeOver() {
    // life--;

    char tmp_str[40];
    glRasterPos2f(-10, 60);
    char text1[] = "Life lost:  Press c to continue";
    for (int i = 0; i < strlen(text1); i++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text1[i]);

}


int RandomNumber(int high, int low)
{
    return (rand() % (high - low)) + low;
}

void newFood() {
    time_t seconds;
    time(&seconds);
    srand((unsigned int)seconds);
    _bx = RandomNumber(_Gfw - _Giw, _Giw + 10);
    _bz = RandomNumber(_Gfh - _Gih, _Gih + 10);
}
bool collision() {
    int i;
    for (i = 0; i < size; i++) {
        if ((bodyPos[0][i] == _x && bodyPos[1][i] == _z) ||

            ((bodyPos[0][i] >= _x) && (bodyPos[0][i] <= _x + 5) && (bodyPos[1][i] <= _z) && (bodyPos[1][i] >= _z - 5)))
            return true;
    }
    return false;
}

void Run(int value) {
    int i;
    _oldX[1] = _x;
    _oldZ[1] = _z;
    switch (direction) {
    case RIGHT:
        headRotation = 90;
        _x += 6;
        if (_x > _Gfw - 2) _x = _Giw - 1;
        break;
    case LEFT:
        headRotation = -90;
        _x -= 6;
        if (_x < 0) _x = _Gfw - 2;
        break;
    case UP:
        headRotation = 0;
        _z += 6;
        if (_z > _Gfh - 2) _z = _Gih - 1;
        break;
    case Down:
        headRotation = 180;
        _z -= 6;
        if (_z < 2) _z = _Gfh - 2;
        break;
    }

    if (collision()) {
        if (life >= 1) {
            life--;
            lifeOver = true;

        }
        else if (life == 0) {
            gameOver = true;
        }

    }


    if ((_x == _bx && _z == _bz) ||
        ((_x >= _bx) && (_x <= _bx + 4) && (_z >= _bz) && (_z <= _bz + 4)) ||
        ((_x <= _bx) && (_x >= _bx - 4) && (_z <= _bz) && (_z >= _bz - 4)) ||
        ((_x <= _bx) && (_x >= _bx - 4) && (_z >= _bz) && (_z <= _bz + 4)) ||
        ((_x >= _bx) && (_x <= _bx + 4) && (_z <= _bz) && (_z >= _bz - 4))) {
        points++;
        if (points < 100) size++;
        if (points % 5 == 0 && lvl < 15) lvl++;
        newFood();
    }

    for (i = 0; i < size; i++) {
        _oldX[0] = _oldX[1];
        _oldZ[0] = _oldZ[1];
        _oldX[1] = bodyPos[0][i];
        _oldZ[1] = bodyPos[1][i];
        bodyPos[0][i] = _oldX[0];
        bodyPos[1][i] = _oldZ[0];
    }


    glutTimerFunc(130, Run, 0);
}

void Display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if (EnableLight) initLight();

    glTranslatef(-60.0, 40.0, zoom);


    if (gameOver)
        GameOver();
    else if (!lifeOver) {
        GameStatus();
        DrawFood();
        DrawSnake();
        WelcomeScreen();
    }
    else if (lifeOver)
        LifeOver();
    GameStatus();
    glutPostRedisplay();
    glutSwapBuffers();
}


void Special(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_RIGHT:
        if (direction != LEFT)
            direction = RIGHT;
        break;
    case GLUT_KEY_LEFT:
        if (direction != RIGHT)
            direction = LEFT;
        break;
    case GLUT_KEY_UP:
        if (direction != UP)
            direction = Down;
        break;
    case GLUT_KEY_DOWN:
        if (direction != Down)
            direction = UP;
        break;
    }
}
void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 'S': Reset();
        glutPostRedisplay();
        break;
    case 's': Reset();
        glutPostRedisplay();
        break;
    case 'c':lifeOver = false;
        Display();
        break;
    case 'C':lifeOver = false;
        Display();
        break;

    case 27:
        exit(0);
        break;
    default:
        break;
    }
}
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(_w, _h);
    glutInitWindowPosition(80, 80);
    glutCreateWindow("Snake Game");
    Initialize();
    glutSpecialFunc(Special);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(Display);
    glutReshapeFunc(resize);
    newFood();
    Run(0);
    glutMainLoop();
}
