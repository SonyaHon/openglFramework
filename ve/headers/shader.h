#pragma once

/*
 *  Loads and compiles shaders
 *  Than u can use it to create programms with ve::ShaderProgramm
 */
 #include <GL/glew.h>
 #include <GLFW/glfw3.h>
 #include <glm/glm.hpp>
 #include <glm/gtc/matrix_transform.hpp>
 #include <vector>
 #include <fstream>
 #include <cstring>
 #include <iostream>
 #include <stdio.h>
 #include <stdlib.h>
 #include "defines.h"

namespace ve {
class Shader {
private:
    GLuint shader;
public:
    Shader(std::string, GLenum);
    GLuint getShader();
};
}
