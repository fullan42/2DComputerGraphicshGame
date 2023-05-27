#include <cmath>
#include <GL/glut.h>
#include <iostream>
#include <string>
#include <GL/glu.h>
#include <GL/gl.h>

using namespace std;

// Game state variables
int score = 0;
int level = 1;
int lives = 3;

// Major character variables
float majorCharX = 0.0;
float majorCharY = -1;
float majorCharSpeed = 0.05;

float secondaryCharX = 0.0;
float secondaryCharY = 0.0;
float secondaryCharSpeed = 0.0;

float secondaryObjX[24];
float secondaryObjY[24];
float trecondaryObjX[24];
float trecondaryObjY[24];
float trecondaryObjspeed[24];
int timerInterval = 32; // ~60 FPS

// Initialize OpenGL graphics
void init() {
    srand(time(NULL)); // Initialize random number generator
    for (int i = 0; i < 24; i++) {
        secondaryObjX[i] = ((float)rand() / (float)RAND_MAX) * 2.0 - 1.0;
        secondaryObjY[i] = ((float)rand() / (float)RAND_MAX) * 2.0 - 1.0;
    }
    for (int i = 0; i < 24; i++) {
        trecondaryObjX[i] = ((float)rand() / (float)RAND_MAX) * 2.0 - 1.0;
        trecondaryObjY[i] = ((float)rand() / (float)RAND_MAX) * 2.0 - 1.0;
        if (i < 12) {
            trecondaryObjspeed[i] = 0.5;
        }
        else {
            trecondaryObjspeed[i] = -0.5;
        }

    }

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();


}
void resetGame() {
    majorCharX = 0.0;
    majorCharY = -1;
    for (int i = 0; i < 24; i++) {
        trecondaryObjX[i] = ((float)rand() / (float)RAND_MAX) * 2.0 - 1.0;
        trecondaryObjY[i] = ((float)rand() / (float)RAND_MAX) * 2.0 - 1.0;
    }
}

void collision() {
    float radius1 = 0.01; // radius of major
    float radius2 = 0.025; // radius of green
    float radius3 = 0.01; // radius of blue

    for (int i = 0; i < 24; i++) {
        float dx = majorCharX - secondaryObjX[i];
        float dy = majorCharY - secondaryObjY[i];
        float distance = sqrt(dx * dx + dy * dy);
        if (distance < radius1 + radius2) {
            // Collision occurred with green object,

            lives--;
            if (lives == 0) {
                // End the game if no lives left
               MessageBoxW(NULL, L"Game Over", L"Info", MB_OK | MB_ICONEXCLAMATION);

                exit(0);
            }
            resetGame();
            MessageBoxW(NULL, L"collision detected", L"Info", MB_OK | MB_ICONEXCLAMATION);


            return;
        }
    }

    for (int i = 0; i < 24; i++) {
        float dx = majorCharX - trecondaryObjX[i];
        float dy = majorCharY - trecondaryObjY[i];
        float distance = sqrt(dx * dx + dy * dy);
        if (distance < radius1 + radius3) {
            // Collision occurred with blue object, decrement lives
            lives--;
            resetGame();
           MessageBoxW(NULL, L"collision detected", L"Info", MB_OK | MB_ICONEXCLAMATION);
            trecondaryObjX[i] = 0.0;
            trecondaryObjY[i] = 0.0;
            if (lives == 0) {
                // End the game if no lives left
              MessageBoxW(NULL, L"Game Over", L"Info", MB_OK | MB_ICONEXCLAMATION);
                exit(0);
            }
        }
    }
}



// Update game state and render graphics
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Show lives
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRasterPos2f(-0.9, 0.9);
    string livesStr = "Lives: " + to_string(lives);
    for (int i = 0; i < livesStr.length(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, livesStr[i]);
    }
    // Render major character
    glColor3f(1.0, 1.0, 1.0);
    glPointSize(10);
    glBegin(GL_POINTS);
    glVertex2f(majorCharX, majorCharY);
    glEnd();

    // Render secondary objects
    glColor3f(0.0, 1.0, 0.0); // Set color to green
    for (int i = 0; i < 24; i++) {
        glPushMatrix();
        glTranslatef(secondaryObjX[i], secondaryObjY[i], 0.0); // Translate to object position
        glScalef(0.025, 0.025, 1.0); // Scale to desired radius
        glutSolidSphere(1.0, 16, 16); // Draw sphere with 16 slices and 16 stacks
        glPopMatrix();
        //rendering
        glPointSize(5);
        glBegin(GL_POINTS);
        glVertex2f(secondaryObjX[i], secondaryObjY[i]);
        glEnd();
    }
    glColor3f(0.0, 0.0, 1.0); // Set color to blue
    for (int i = 0; i < 24; i++) {
        if (trecondaryObjX[i] != 0.0 || trecondaryObjY[i] != 0.0) { // Only draw if object exists
            glPushMatrix();
            glTranslatef(trecondaryObjX[i], trecondaryObjY[i], 0.0); // Translate to object position
            glScalef(0.01, 0.01, 1.0); // Scale to desired radius
            glutSolidSphere(1.0, 16, 16); // Draw sphere with 16 slices and 16 stacks
            glPopMatrix();
            //rendering
            glPointSize(5);
            glBegin(GL_POINTS);
            glVertex2f(trecondaryObjX[i], trecondaryObjY[i]);
            glEnd();
        }
    }
    collision(); // Check for collision
    glutSwapBuffers();
}


// Handle keyboard input
void specialKeyboard(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:
        majorCharY += majorCharSpeed;
        break;
    case GLUT_KEY_DOWN:
        majorCharY -= majorCharSpeed;
        break;
    case GLUT_KEY_LEFT:
        majorCharX -= majorCharSpeed;
        break;
    case GLUT_KEY_RIGHT:
        majorCharX += majorCharSpeed;
        break;
    default:
        break;
    }
}

// Timer function for updating game state and rendering graphics
void timer(int) {
    static int counter = 0;
    static int timeElapsed = 0;
    counter++;
    timeElapsed += timerInterval;

    // Move the objects to the right at a speed of 1 unit per second
    for (int i = 0; i < 24; i++) {
        secondaryObjX[i] += secondaryCharSpeed * timerInterval / 1000.0;
        trecondaryObjX[i] += trecondaryObjspeed[i] * timerInterval / 1000.0;
    }

    // Change direction of trecondary objects after 5 seconds
    if (counter % 150 == 0) {
        for (int i = 0; i < 24; i++) {
            if (trecondaryObjX[i] != 0.0 || trecondaryObjY[i] != 0.0) { // Only change direction if object exists
                trecondaryObjspeed[i] = -trecondaryObjspeed[i];
            }
        }
    }

    // Keep the objects inside the screen
    for (int i = 0; i < 24; i++) {
        if (trecondaryObjX[i] > 1.0) {
            trecondaryObjspeed[i] = -0.5; // Change speed direction
        }
        if (trecondaryObjX[i] < -1.0) {
            trecondaryObjspeed[i] = 0.5; // Change speed direction
        }
    }
    if (majorCharY >= 1) {
        // Display a message box
        MessageBoxW(NULL, L"you are the challanger and winner", L"Info", MB_OK | MB_ICONEXCLAMATION);

        // Close the window
        glutDestroyWindow(glutGetWindow());
        return;
    }

    // Redraw graphics
    glutPostRedisplay();

    // Set timer for next frame
    glutTimerFunc(timerInterval, timer, 0);
}


// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1080, 768);
    glutCreateWindow("2D Game Engine");

    init();

    glutDisplayFunc(display);
    glutSpecialFunc(specialKeyboard);

    // Start timer
    glutTimerFunc(timerInterval, timer, 0);

    // Enable double buffering
    glutSwapBuffers();

    glutMainLoop();

    return 0;
}
