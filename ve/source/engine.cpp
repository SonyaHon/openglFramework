#include "../headers/engine.h"

#include <iostream>
#include <stdlib.h>


namespace ve {
Engine::Engine(float width, float height, const char * name, int mode) {
    if (!glfwInit()) {
        std::cout << "Error with glfwInit()" << std::endl;
        glfwTerminate();
        exit(-1);
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    window = glfwCreateWindow(width, height, name, NULL, NULL);
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "Error with glewInit()" << std::endl;
        exit(-1);
    }


    clearColor.r     = 0.0f;
    clearColor.g     = 0.0f;
    clearColor.b     = 0.0f;
    clearColor.alpha = 1.0f;
    workMode         = mode;

    mainScene = Scene(mode, width, height);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
}

Scene Engine::getScene() {
    return mainScene;
}

void Engine::addModel(Model2D model) {
    mainScene.addModel(model);
}

void Engine::addModel(Model3D model) {
    mainScene.addModel(model);
}

void Engine::loop(void (* loopFunc)()) {
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    do {
        glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.alpha);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        loopFunc();

        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (!glfwWindowShouldClose(window));
}

int Engine::getWindowHeight() {
    return height;
}

int Engine::getWindowWidth() {
    return width;
}

void Engine::Engine_Terminate() {
    glfwTerminate();
}

void Engine::setClearColor(GLfloat r, GLfloat g, GLfloat b, GLfloat alpha) {
    clearColor.r     = r;
    clearColor.g     = g;
    clearColor.b     = b;
    clearColor.alpha = alpha;
}

GLFWwindow * Engine::getWindow() {
    return window;
}
}
