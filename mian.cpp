/*************************************************************************************/
// Najprostszy program wykorzystujący funkcje biblioteki OpenGL
/*************************************************************************************/
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <chrono>
#include <thread>
#include <iostream>
#include <string>
#include <vector>

int iterations = 2;
float distortionLevel = 0.0;
int height = 900;
int width = 900;

bool colorized = true;


void setColor()
{
    int red = 255;
    int green = 255;
    int blue = 255;
    if (colorized)
    {
        red = rand() % 256;
        green = rand() % 256;
        blue = rand() % 256;
    }
    glColor3ub(red, green, blue);
}

float randomFloat(float min, float max)
{
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

void setDistortion(GLfloat& x, GLfloat& y, GLfloat width)
{
    x += randomFloat(-width * distortionLevel, width * distortionLevel);
    y += randomFloat(-width * distortionLevel, width * distortionLevel);
}

void draw(GLfloat x, GLfloat y, GLfloat width)
{

    if (distortionLevel > 0.0) {
        setDistortion(x, y, width);
    }
    glBegin(GL_POLYGON);
    setColor();
    glVertex2f(x + width, y + width);
    setColor();
    glVertex2f(x + width, y - width);
    setColor();
    glVertex2f(x - width, y - width);
    setColor();
    glVertex2f(x - width, y + width);
    glEnd();
}

GLfloat modifyXCoord(int j, GLfloat x, GLfloat width)
{
    switch (j) {
    case 0: x = x - 2 * (width); return x;
    case 1: return x; break;
    case 2: x = x + 2 * (width); return x;
    }
}

GLfloat modifyYCoord(int i, GLfloat y, GLfloat width)
{
    switch (i) {
    case 0: y = y - 2 * (width); return y;
    case 1: return y;
    case 2: y = y + 2 * (width); return y;
    }
}

void sierpinskiCarpet(GLfloat x, GLfloat y, GLfloat width, int currentIter, int iterations)
{
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            GLfloat newX = modifyXCoord(j, x, width);
            GLfloat newY = modifyYCoord(i, y, width);
            if (i == 1 && j == 1) continue;
            if (iterations == currentIter)
            {
                draw(newX, newY, width);
                glFlush();
            }
            else
            {
                sierpinskiCarpet(newX, newY, width / 3, currentIter + 1, iterations);
            }
        }
    }

}


void RenderScene(void) {
    int saved_iter = iterations;
    for (int i = 1; i <= iterations; i++) {
        sierpinskiCarpet(0, 0, 0.333333, 1, i);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        glClear(GL_COLOR_BUFFER_BIT);
    }
    if (saved_iter <= 5) {
        std::this_thread::sleep_for(std::chrono::milliseconds(saved_iter*1000));
    }
    if (saved_iter <= 6) {
        std::this_thread::sleep_for(std::chrono::milliseconds(saved_iter * 5200));
    }
    exit(0);
}


bool parseFlags(const std::vector<std::string> in) {
    for (size_t pos = 0; pos < in.size(); pos++)
    {
        if (in.at(pos) == "--colorization")
        {
            colorized = false;
            continue;
        }

        if (in.at(pos) == "--winH" )
        {
            height = std::stoi(in.at(pos+1));
        }

        if (in.at(pos) == "--winW")
        {
            width = std::stoi(in.at(pos + 1));
        }

        if (in.at(pos) == "--steps")
        {
            iterations = std::stoi(in.at(pos+1));
            std::cout << iterations << std::endl;

            if (iterations < 1)
            {
                std::cerr << "Liczba krokow ma byc wieksza od zera";
                return 1;
            }
        }

        if (in.at(pos) == "--distortion")
        {
            distortionLevel = std::stoi(in.at(pos + 1));

            if (distortionLevel < 0.0 | distortionLevel> 1.0)
            {
                std::cerr << "Deformation value must be between 0.0 and 1.0";
                return 1;
            }
        }

        if (in.at(pos) == "--help")
        {
            std::cout << "========================= Manual ==========================" << std::endl;
            std::cout << "--colorization - black and while output  Default in color" << std::endl;
            std::cout << "--winH         - window heihght          Default is 900" << std::endl;
            std::cout << "--winW         - window width            Default is 900" << std::endl;
            std::cout << "--steps        - number of iterations    Default is 2" << std::endl;
            std::cout << "--distortion   - distortion level        Default in 0.0" << std::endl;

            return false;
        }
    }
    return true;
}

int main(int argc, char** argv) {
    std::vector<std::string> arguments(argv + 1, argv + argc);
    
    if (parseFlags(arguments)) {
        glutInit(&argc, argv);

        glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
        glutInitWindowSize(width, height);
        glutCreateWindow("Dywan Sierpinskiego");

        glutDisplayFunc(RenderScene);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        glutMainLoop();  
    }
    return (0);
}
