#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ve/headers/engine.h"
#include "ve/headers/shader.h"
#include "ve/headers/shaderProgramm.h"
#include "ve/headers/model2d.h"
#include "ve/headers/model3d.h"
#include "ve/headers/defines.h"
#include "ve/headers/scene.h"

void loop();


ve::ShaderProgramm prog;
ve::Engine game;
ve::Model3D model;

int main() {
    game = ve::Engine(800.0f, 600.0f, "**!!TEST IT!!**", VE_MODE_3D);
    // game.setClearColor(0.0f, 0.0f, 0.9f, 1.0f);
    ve::Shader vert      = ve::Shader("./shaders/vertexShader.shad", GL_VERTEX_SHADER);
    ve::Shader frag      = ve::Shader("./shaders/fragShader.shad", GL_FRAGMENT_SHADER);
    ve::Shader shaders[] = { vert, frag };
    prog = ve::ShaderProgramm(2, shaders);

    model = ve::Model3D(prog, "./3dFiles/cube/CubeFinal.obj");
    model.loadTexture("./3dFiles/cube/cube_texture.png", 1024, 1024);

    game.addModel(model);

    // TODO  model2d sprite uv generation model3d .obj uploading
    // then screen working options: fullscreen etc
    // collisions of model2d and model3d

    game.loop(loop);
    game.Engine_Terminate();
    return 0;
} // main

void loop() {
    model.draw(game.getScene().getView(), game.getScene().getProjection());
    if (glfwGetKey(game.getWindow(), GLFW_KEY_A) == GLFW_PRESS) {
        model.translate(-0.1f, 0.0f, 0.0f);
    } else if (glfwGetKey(game.getWindow(), GLFW_KEY_D) == GLFW_PRESS) {
        model.translate(0.1f, 0.0f, 0.0f);
    }
}
