#pragma once

/*
*   Create shader programs from an array of
*   ve::Shaders and use them like usual
*   GLSL shader programs
*/


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include "defines.h"

namespace ve {
class ShaderProgramm {
private:
    GLuint programm;
public:
    ShaderProgramm(int a, Shader shaders[]);
    ShaderProgramm(){};
    GLuint getProg();
};
}
