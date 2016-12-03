#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "scene.h"
#include <vector>
#include "defines.h"
#include "model2d.h"
#include "model3d.h"

namespace ve {
struct CLEAR_COLOR {
    GLfloat r;
    GLfloat g;
    GLfloat b;
    GLfloat alpha;
};

class Engine {
private:
    float width;
    float height;
    GLFWwindow * window;
    CLEAR_COLOR clearColor;
    int workMode;
    Scene mainScene;
    GLuint VAO;

public:
    Engine(float, float, const char *, int);
    Engine() { };
    void loop(void (* loopFunc)());
    int getWindowWidth();
    int getWindowHeight();
    void Engine_Terminate();
    Scene getScene();

    GLFWwindow * getWindow();

    void addModel(Model3D);
    void addModel(Model2D);

    void setClearColor(GLfloat, GLfloat, GLfloat, GLfloat);
};
}
