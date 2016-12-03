#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstring>
#include "shaderProgramm.h"
#include "defines.h"


/*
*   plain model with texture on it
*   can be used in 3d mode and 2d mode
*   in 2d mode camera stays on top and
*   can be moved only in x n z directions
*   so 2d model is a sprite in 3d mode
*   its just a simple plain model for
*   a very low poly mesh like grass etc.
*
*/

namespace ve{
class Model2D {
private:
    float x;
    float y;

public:
    Model2D();
    void draw();
};
}
