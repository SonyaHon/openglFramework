#include "../headers/shader.h"
namespace ve{

Shader::Shader(std::string filename, GLenum shaderType) {

    shader = glCreateShader(shaderType);


    // Reading shader code from file
    std::string shaderCode;
    std::ifstream shaderStream(filename.c_str(), std::ios::in);

    if(shaderStream.is_open()) {
        std::string Line = "";
        while(getline(shaderStream, Line)) {
            shaderCode += "\n" + Line;
        }
        shaderStream.close();
    } else {
        std::cout << "Error opening shader " + filename << std::endl;
    }

    GLint Result = GL_FALSE;
    int InfoLogLenth;

    // Compile shader
    char const * sourcePointer = shaderCode.c_str();
    glShaderSource(shader, 1, &sourcePointer, NULL);
    glCompileShader(shader);

    //Test shader

    glGetShaderiv(shader, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &InfoLogLenth);
    if(InfoLogLenth > 1) {
        std::vector<char> shaderErrorMsg(InfoLogLenth + 1);
        glGetShaderInfoLog(shader, InfoLogLenth, NULL, &shaderErrorMsg[0]);
        std::cout << filename + " info: ";
        fprintf(stdout, "%s", &shaderErrorMsg[0]);
    }
}
GLuint Shader::getShader() {
    return shader;
}

}
