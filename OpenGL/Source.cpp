/*Merve Sahin*/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

//library to use rand function
#include <stdlib.h> 

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

#define TIMER_PERIOD  10 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

bool preStart = false, start = false, timer = false; //booleans to start the game and the timer
float distance; //the distance between the firing object and the icecream
float xW = 0, yW = 0; //x and y coordinates of the weapon
double counter = 60.00; //initializing the timer to 1 min
int angle = 90, angle1; //they are integer because we need to check if they are equal
int hitsPC = 0; //the global variable for the game
bool question = false; //boolean for answering question
int guess = 0; //Guess of the player

typedef struct {
    float x;
    float y;
    bool active;
} str_t;

str_t   fire = { 0, 0, false };
str_t icecream = { 0, 0, false };

//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void print(int x, int y, const char* string, void* font)
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}

void drawBackground()
{
    glBegin(GL_QUADS);
    glColor3f(0.7, 0.3, 0.5);
    glVertex2f(-400, 300);
    glVertex2f(-400 + 800, 300);
    glColor3f(0.5 + 0.4, 0.5 + 0.4, 0.9 + 0.4);
    glVertex2f(-400 + 800, 300 - 600);
    glVertex2f(-400, 300 - 600);
    glEnd();

    glColor3f(0, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(-400, 300);
    glVertex2f(400, 300);
    glVertex2f(400, 230);
    glVertex2f(-400, 230);
    glVertex2f(-400, -300);
    glVertex2f(400, -300);
    glVertex2f(400, -230);
    glVertex2f(-400, -230);
    glEnd();
}


void drawIcecream()
{

    if (!icecream.active)
    {

        icecream.x = rand() % 801 - 400;
        icecream.y = rand() % 601 - 300;

        icecream.active = true;
    }

    glColor3f(1, 0.5, 0);
    glBegin(GL_LINES);
    glVertex2f(icecream.x, icecream.y);
    glVertex2f(icecream.x + 10, icecream.y + 35);
    glEnd();

    glColor3f(0, 0, 1);
    circle(icecream.x, icecream.y + 11, 11);
    glColor3f(0, 1, 0);
    circle(icecream.x + 7, icecream.y + 5, 9);
    glColor3f(1, 0, 0);
    circle(icecream.x - 5, icecream.y + 3, 10);

    glColor3f(0.8, 0.5, 0.2);
    glBegin(GL_TRIANGLES);
    glVertex2f(icecream.x - 15, icecream.y);
    glVertex2f(icecream.x + 15, icecream.y);
    glVertex2f(icecream.x, icecream.y - 45);
    glEnd();

    glColor3f(0.6, 0.1, 0.1);
    circle(icecream.x - 9, icecream.y - 6, 1);
    circle(icecream.x + 10, icecream.y - 8, 1);
    circle(icecream.x, icecream.y - 12, 1);
    circle(icecream.x - 5, icecream.y - 20, 1);
    circle(icecream.x + 5, icecream.y - 25, 1);
    circle(icecream.x, icecream.y - 35, 1);
}

void drawWeapon()
{
    glColor3f(0.6, 0.1, 0.1);
    glBegin(GL_LINES);
    glVertex2f(0, 0);
    glVertex2f(xW + 30 * cos(angle * D2R), yW + 30 * sin(angle * D2R));
    glEnd();

    angle1 = atan2(icecream.y, icecream.x) / D2R;
    if (angle1 > 360)
        angle1 -= 360;
    if (angle1 < 0)
        angle1 += 360;


    if (angle > 360)
        angle -= 360;
    if (angle < 0)
        angle += 360;

    if ((angle1 - angle >= -180 && angle1 - angle < 0) || (angle1 - angle > 180 && angle1 - angle < 360))
        angle--;
    else if ((angle1 - angle <= 180 && angle1 - angle > 0) || (angle1 - angle < -180 && angle1 - angle > -360))
        angle++;
    else
    {
        fire.active = true;
    }

    vprint(-5, -15, GLUT_BITMAP_8_BY_13, "%d", angle);
}

void resetFiring()
{
    fire.x = 0;
    fire.y = 0;
}


void firing()
{
    glColor3f(1, 0, 0);
    circle(fire.x, fire.y, 11);
    glColor3f(1.0f, 0.5f, 0.0f);
    circle(fire.x, fire.y, 8);
    glColor3f(1.0f, 1.0f, 0.0f);
    circle(fire.x, fire.y, 5);
    glColor3f(1, 0, 0);

    glBegin(GL_TRIANGLES);
    glVertex2f(fire.x - 10, fire.y - 1);
    glVertex2f(fire.x, fire.y);
    glVertex2f(fire.x - 10, fire.y + 20);

    glVertex2f(fire.x + 10, fire.y - 1);
    glVertex2f(fire.x, fire.y);
    glVertex2f(fire.x + 10, fire.y + 20);

    glVertex2f(fire.x + 6, fire.y);
    glVertex2f(fire.x - 6, fire.y);
    glVertex2f(fire.x, fire.y + 25);

    glColor3f(1.0f, 0.5f, 0.0f);

    glVertex2f(fire.x - 8.5, fire.y - 1);
    glVertex2f(fire.x, fire.y);
    glVertex2f(fire.x - 8.5, fire.y + 18);

    glVertex2f(fire.x + 8.5, fire.y - 1);
    glVertex2f(fire.x, fire.y);
    glVertex2f(fire.x + 8.5, fire.y + 18);

    glVertex2f(fire.x + 3, fire.y);
    glVertex2f(fire.x - 3, fire.y);
    glVertex2f(fire.x, fire.y + 22);

    glColor3f(1.0f, 1.0f, 0.0f);

    glVertex2f(fire.x - 5.5, fire.y - 1);
    glVertex2f(fire.x, fire.y);
    glVertex2f(fire.x - 5.5, fire.y + 15);

    glVertex2f(fire.x + 5.5, fire.y - 1);
    glVertex2f(fire.x, fire.y);
    glVertex2f(fire.x + 5.5, fire.y + 15);

    glVertex2f(fire.x + 2, fire.y);
    glVertex2f(fire.x - 2, fire.y);
    glVertex2f(fire.x, fire.y + 16);

    glEnd();

    distance = sqrtf(powf((fire.x - icecream.x), 2) + powf((fire.y - icecream.y), 2));

    if (distance > 20)
    {
        fire.x += 5 * (cos(angle * D2R));
        fire.y += 5 * (sin(angle * D2R));

    }

    else
    {
        resetFiring();
        icecream.active = false;
        fire.active = false;
        question = false;
        hitsPC++;
    }
}

//
// To display onto window using OpenGL commands
//
void display() {
    //
    // clear window to black
    //
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    drawBackground();

    glColor3f(1, 1, 0);

    if (!start && !preStart)
    {
        vprint(-396, -270, GLUT_BITMAP_8_BY_13, "Use up and down arrow keys to increment and decrement your guess and press enter when you are ready");
        vprint(-30, 260, GLUT_BITMAP_9_BY_15, "MERVE SAHIN");
        glColor3f(0, 0, 0);
        vprint(-260, 0, GLUT_BITMAP_9_BY_15, "How many ice creams do you think the sun can melt in a minute?");
        vprint(0, -50, GLUT_BITMAP_9_BY_15, "%d", guess);
    }

    if (preStart && start)
    {
        vprint(300, -270, GLUT_BITMAP_9_BY_15, "SUN: %d", hitsPC);
        vprint(150, -270, GLUT_BITMAP_9_BY_15, "YOUR GUESS: %d", guess);

        circle(0, 0, 35);
        glLineWidth(3);
        glBegin(GL_LINES);
        glVertex2f(0, 70);
        glVertex2f(0, -70);
        glVertex2f(70, 0);
        glVertex2f(-70, 0);
        glVertex2f(-40, -40);
        glVertex2f(40, 40);
        glVertex2f(-40, 40);
        glVertex2f(40, -40);
        glEnd();

        drawIcecream();
        drawWeapon();

        if (timer)
        {
            glColor3f(1, 1, 0);
            vprint(-350, -270, GLUT_BITMAP_9_BY_15, "Time: %.2f", counter);


            if (fire.active)
            {
                firing();
            }
        }

        else
        {
            glColor3f(1, 1, 0);
            vprint(-350, -270, GLUT_BITMAP_9_BY_15, "GAME OVER");

            if (guess == hitsPC)
                vprint(-230, 270, GLUT_BITMAP_9_BY_15, "CONGRATULATIONS! Your guess was correct!!");
            else if (abs(hitsPC - guess < 1))
                vprint(-320, 270, GLUT_BITMAP_9_BY_15, "You unfortunately couldn't guess it correctly, but you were close :)");
            else
                vprint(-260, 270, GLUT_BITMAP_9_BY_15, "You couldn't guess it correctly, maybe next time :(");
        }

    }


    glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    if (key == 13) //when enter key is pressed
        preStart = true;

    if (preStart) {
        // exit when ESC is pressed.
        if (key == 27)
            exit(0);

        else
        {
            start = true;
            timer = true;
        }

    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
    // Write your codes here.

    if (!preStart) {
        if (key == GLUT_KEY_UP)
            guess++;
        else if (key == GLUT_KEY_DOWN && guess > 0) //No negative values
            guess--;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = false; break;
    case GLUT_KEY_DOWN: down = false; break;
    case GLUT_KEY_LEFT: left = false; break;
    case GLUT_KEY_RIGHT: right = false; break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function   
    glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.

    if (timer && counter >= 0)
        counter -= 1 / 60.0; //we are decreasing 1/60th of a second to make the timer correct
    else
        timer = false;

    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(300, 100);
    glutCreateWindow("Fireball Against Icecream");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //
    // mouse registration
    //
    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();

    glutMainLoop();
}