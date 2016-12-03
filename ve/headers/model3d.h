#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "defines.h"
#include "shaderProgramm.h"
#include <png.h>

/*
 *   3d model is a complex model
 *   can be moved in 3d space
 *   can be used only in 3d engine mode
 */


namespace ve {
class Model3D {
private:
    glm::mat4 ModelMatrix; // For glsl

    float posX; // Position
    float posY;
    float posZ;

    float scaX; // Scale
    float scaY;
    float scaZ;

    float rotX; // Rotation
    float rotY;
    float rotZ;

    ShaderProgramm shProg;

    GLuint vertexbuffer;
    GLuint uvbuffer;

    GLuint texture;
    GLuint png_texture_load(const char *, int *, int *);
    bool loadOBJ(const char *, std::vector<glm::vec3> &, std::vector<glm::vec2> &, std::vector<glm::vec3> &);

public:
    Model3D() { };
    Model3D(ShaderProgramm p, std::string filename) {
        posX = 0.0f; // Position
        posY = 0.0f;
        posZ = 0.0f;

        scaX = 0.0f; // Scale
        scaY = 0.0f;
        scaZ = 0.0f;

        rotX = 0.0f; // Rotation
        rotY = 0.0f;
        rotZ = 0.0f;

        ModelMatrix = glm::mat4(1.0f);
        shProg      = p;

        std::vector<glm::vec3> verts;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> normals;

        loadOBJ(filename.c_str(), verts, uvs, normals);

        glGenBuffers(1, &vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(glm::vec3), &verts[0], GL_STATIC_DRAW);

        glGenBuffers(1, &uvbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec3), &uvs[0], GL_STATIC_DRAW);
    };

    void draw(glm::mat4, glm::mat4);
    void translate(float, float, float);

    void loadTexture(std::string, int, int);

    void scale(float, float, float);
    void scale(float);

    void rotate(float, float, float, float);
};
}
