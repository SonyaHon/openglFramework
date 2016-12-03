#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "defines.h"
#include <vector>
#include "model2d.h"
#include "model3d.h"

/*
 *   Special obj for storing and operating with model2d/3d
 */

namespace ve {
class Scene {
private:
    int mode;
    glm::mat4 ProjectionMatrix;
    glm::mat4 ViewMatrix;

    std::vector<Model3D> d3Models;
    std::vector<Model2D> d2Models;

    float x;
    float y;
    float z;

    int lel;

public:
    Scene(int mod, float width, float height) {
        mode = mod;
        x    = 0.0f;
        y    = 4.0f;
        z    = 10.0f;

        ProjectionMatrix = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 100.0f);
        ViewMatrix       = glm::lookAt(glm::vec3(x, y, z), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    };
    Scene() { };

    void addModel(Model2D);
    void addModel(Model3D);
    void drawAll();
    void cameraTranslate(float, float, float);
    void cameraRotate(float, float, float, float);
    void cameraLookAt(float, float, float);
    glm::mat4 getView();
    glm::mat4 getProjection();
};
}
