#include "../headers/scene.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

namespace ve {
void Scene::addModel(Model3D model) {
    if (mode != VE_MODE_2D) {
        d3Models.push_back(model);
    } else {
        std::cout << "Can`t add 3d model int 2d mode scene" << std::endl;
        exit(-1);
    }
}

void Scene::addModel(Model2D model) {
    d2Models.push_back(model);
}

void Scene::drawAll() {
    for (size_t i = 0; i < d3Models.size(); ++i) {
        d3Models[i].draw(ViewMatrix, ProjectionMatrix);
    }
    for (size_t i = 0; i < d2Models.size(); ++i) {
        d2Models[i].draw();
    }
}

void Scene::cameraTranslate(float xt, float yt, float zt) {
    ViewMatrix *= glm::translate(glm::mat4(), glm::vec3(xt, yt, zt));
    x += xt;
    y += yt;
    z += zt;
}

glm::mat4 Scene::getView() {
    return ViewMatrix;
}

glm::mat4 Scene::getProjection() {
    return ProjectionMatrix;
}

void Scene::cameraRotate(float angle, float x, float y, float z) {
    ViewMatrix *= glm::rotate(glm::mat4(), angle, glm::vec3(x, y, z));
}

void Scene::cameraLookAt(float xd, float yd, float zd) {
    ViewMatrix = glm::lookAt(
        glm::vec3(x, y, z),
        glm::vec3(xd, yd, zd),
        glm::vec3(0, 1, 0)
      );
}
}
