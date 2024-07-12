#include <GL/glut.h>
#include <vector>
#include <cstdlib>
#include <ctime>

float butterflyScale = 1.0f;

// Struct to represent a flower
struct Flower {
    float x, y;
};

// Struct to represent a butterfly
struct Butterfly {
    float x, y;
    float dx, dy;
    float wingAngle;
};

// Struct to represent a star
struct Star {
    float x, y;
};

// Vectors to store flowers, butterflies, and stars
std::vector<Flower> flowers;
std::vector<Butterfly> butterflies;
std::vector<Star> stars;

// Variables to control cloud movement
float cloudX = 0.0f;
float cloudSpeed = 0.001f;

// Variables for windmill rotation
float windmillAngle = 0.0f;
bool windmillRotating = false;

// Variables to control the sky
bool isDay = true;
bool showStars = false;

// Function to initialize butterflies
void initButterflies(int count) {
    srand(static_cast<unsigned>(time(0)));
    for (int i = 0; i < count; i++) {
        Butterfly butterfly = { static_cast<float>(rand() % 800), static_cast<float>(rand() % 600), (rand() % 3 - 1) / 500.0f, (rand() % 3 - 1) / 500.0f, 0.0f };
        butterflies.push_back(butterfly);
    }
}

// Function to initialize stars
void initStars(int count) {
    srand(static_cast<unsigned>(time(0)));
    for (int i = 0; i < count; i++) {
        Star star = { static_cast<float>(rand() % 800), 300.0f + static_cast<float>(rand() % 300) };
        stars.push_back(star);
    }
}

// Function to draw a circle
void drawCircle(float cx, float cy, float r, int num_segments) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < num_segments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        glVertex2f(x + cx, y + cy);
    }
    glEnd();
}

// Function to draw a flower
void drawFlower(float x, float y) {
    // Draw the stem
    glColor3f(0.0f, 1.0f, 0.0f);
    glRectf(x - 2, y, x + 2, y + 15);

    // Draw the petals
    glColor3f(1.0f, 0.0f, 1.0f);
    drawCircle(x, y + 20, 5, 20);
    drawCircle(x + 5, y + 15, 5, 20);
    drawCircle(x - 5, y + 15, 5, 20);
    drawCircle(x + 5, y + 25, 5, 20);
    drawCircle(x - 5, y + 25, 5, 20);

    // Draw the center
    glColor3f(1.0f, 1.0f, 0.0f);
    drawCircle(x, y + 20, 3, 20);
}

// Function to draw a tree
void drawTree(float x, float y) {
    // Draw the trunk
    glColor3f(0.55f, 0.27f, 0.07f);
    glRectf(x - 5, y, x + 5, y + 50);

    // Draw the leaves with a darker shade of green
    glColor3f(0.0f, 0.39f, 0.0f);
    drawCircle(x, y + 60, 20, 100);
    drawCircle(x - 15, y + 50, 15, 100);
    drawCircle(x + 15, y + 50, 15, 100);

    // Draw fruits
    glColor3f(1.0f, 0.0f, 0.0f);
    drawCircle(x - 10, y + 60, 3, 20);
    drawCircle(x + 10, y + 55, 3, 20);

    // Draw exactly two flowers next to the tree
    drawFlower(x - 20, y);
    drawFlower(x + 20, y);
}

// Function to draw a butterfly
void drawButterfly(Butterfly& butterfly, float butterflyScale) {
    glPushMatrix();
    glTranslatef(butterfly.x, butterfly.y, 0);
    glScalef(butterflyScale, butterflyScale, 1.0f); // Apply scaling transformation

    butterfly.wingAngle += 0.02f;
    float wingDisplacement = sin(butterfly.wingAngle) * 5;

    // Draw the body
    glColor3f(0.0f, 0.0f, 0.0f);
    glRectf(-2, -5, 2, 5);

    // Draw the wings
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(2, 0);
    glVertex2f(20 + wingDisplacement, 10);
    glVertex2f(20 + wingDisplacement, -10);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(-2, 0);
    glVertex2f(-20 - wingDisplacement, 10);
    glVertex2f(-20 - wingDisplacement, -10);
    glEnd();

    // Draw antennas
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.5);
    glBegin(GL_LINES);
    glVertex2f(0, 5);   // Starting point of the left antenna
    glVertex2f(-5, 10); // End point of the left antenna
    glVertex2f(0, 5);   // Starting point of the right antenna
    glVertex2f(5, 10);  // End point of the right antenna
    glEnd();

    glPopMatrix();
}


// Function to draw the windmill
void drawWindmill(float x, float y) {
    // Draw the frustum cone shaped tower
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_POLYGON);
    glVertex2f(x - 50, y - 50); // bottom left
    glVertex2f(x + 50, y - 50); // bottom right
    glVertex2f(x + 30, y + 100); // top right
    glVertex2f(x - 30, y + 100); // top left
    glEnd();

    // Draw the black roof (isosceles triangle)
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(x, y + 170); // apex of the triangle
    glVertex2f(x + 45, y + 100); // right corner
    glVertex2f(x - 45, y + 100); // left corner
    glEnd();

    // Draw the rotating blades
    glPushMatrix();
    glTranslatef(x, y + 100, 0);
    glRotatef(windmillAngle, 0.0f, 0.0f, 1.0f);

    glColor3f(0.8f, 0.8f, 0.8f);
    glBegin(GL_TRIANGLES);
    glVertex2f(0, 0);
    glVertex2f(110, 22);
    glVertex2f(130, -22);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(0, 0);
    glVertex2f(-130, 22);
    glVertex2f(-110, -22);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(0, 0);
    glVertex2f(22, 130);
    glVertex2f(-22, 110);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(0, 0);
    glVertex2f(22, -110);
    glVertex2f(-22, -130);
    glEnd();

    glPopMatrix();
}


// Function to draw the garden scene
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the sky
    if (isDay) {
        glColor3f(0.53f, 0.81f, 0.98f);
    }
    else {
        glColor3f(0.0f, 0.0f, 0.0f);
    }
    glRectf(0, 300, 800, 600);

    // Draw the sun or moon
    if (isDay) {
        glColor3f(1.0f, 1.0f, 0.0f);
    }
    else {
        glColor3f(1.0f, 1.0f, 1.0f);
    }
    drawCircle(700, 500, 30, 100);

    // Draw the grass
    glColor3f(0.0f, 0.5f, 0.0f);
    glRectf(0, 0, 800, 300);

    // Draw trees
    drawTree(100, 200);
    drawTree(300, 200);
    drawTree(500, 200);
    drawTree(700, 200);

    // Draw flowers
    for (const auto& flower : flowers) {
        drawFlower(flower.x, flower.y);
    }

    // Draw clouds
    if (isDay) {
        glColor3f(1.0f, 1.0f, 1.0f);
    }
    else {
        glColor3f(0.5f, 0.5f, 0.5f);
    }
    drawCircle(100 + cloudX, 500, 30, 100);
    drawCircle(130 + cloudX, 500, 30, 100);
    drawCircle(160 + cloudX, 500, 30, 100);
    drawCircle(600 + cloudX, 450, 40, 100);
    drawCircle(640 + cloudX, 450, 40, 100);
    drawCircle(680 + cloudX, 450, 40, 100);

    // Update cloud position
    cloudX += cloudSpeed;
    if (cloudX > 800) cloudX = -200;

    // Rotate the windmill if it's rotating
    if (windmillRotating) {
        windmillAngle += 0.1f;
        if (windmillAngle > 360.0f) {
            windmillAngle -= 360.0f;
        }
    }

    // Draw the windmill
    drawWindmill(400, 100);

    // Draw stars if it's night
    if (showStars) {
        glColor3f(1.0f, 1.0f, 1.0f);
        for (const auto& star : stars) {
            drawCircle(star.x, star.y, 2, 10);
        }
    }

    // Draw butterflies if it's day, ensuring they are drawn last
    if (isDay) {
        for (auto& butterfly : butterflies) {
            drawButterfly(butterfly, butterflyScale);
            butterfly.x += butterfly.dx;
            butterfly.y += butterfly.dy;

            // Keep butterflies within the window bounds
            if (butterfly.x < 0 || butterfly.x > 800) butterfly.dx = -butterfly.dx;
            if (butterfly.y < 0 || butterfly.y > 600) butterfly.dy = -butterfly.dy;
        }
    }

    glutSwapBuffers();
}

// Function to handle mouse clicks for planting flowers
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        Flower flower = { (float)x, 600 - (float)y };
        flowers.push_back(flower);
    }
}

// Menu callback function
void menu(int option) {
    switch (option) {
    case 1:
        windmillRotating = true;
        break;
    case 2:
        windmillRotating = false;
        break;
    case 3:
        isDay = true;
        showStars = false;
        initButterflies(10); // Reinitialize butterflies
        break;
    case 4:
        isDay = false;
        showStars = true;
        butterflies.clear(); // Clear butterflies when it's night
        break;
    }
}

// Function to create the menu
void createMenu() {
    int windmillSubmenu = glutCreateMenu(menu);
    glutAddMenuEntry("Start Rotation", 1);
    glutAddMenuEntry("Stop Rotation", 2);

    int skySubmenu = glutCreateMenu(menu);
    glutAddMenuEntry("Day", 3);
    glutAddMenuEntry("Night", 4);

    glutCreateMenu(menu);
    glutAddSubMenu("Windmill", windmillSubmenu);
    glutAddSubMenu("Sky", skySubmenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Function to handle keyboard input for cloud movement
void keyboard(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_RIGHT:
        cloudX+=10;  // Move clouds to the right
        break;
    case GLUT_KEY_LEFT:
        cloudX -= 10;  // Move clouds to the left
        break;
    case GLUT_KEY_UP:
        butterflyScale += 0.1f; // Increase scaling factor
        //if (butterflyScale > 5.0f) butterflyScale = 5.0f;
        break;
    case GLUT_KEY_DOWN:
        butterflyScale -= 0.1f; // Decrease scaling factor
        if (butterflyScale < 0.1f) butterflyScale = 0.1f; // Minimum scaling factor
        break;
    }
}

void init() {
    glClearColor(0.53f, 0.81f, 0.98f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);
    initButterflies(10);
    initStars(50);

    // Create the menu
    createMenu();
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("2D Garden View");

    init();

    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutMouseFunc(mouse);
    glutSpecialFunc(keyboard);  // Register the keyboard function

    glutMainLoop();
    return 0;
}
