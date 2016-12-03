#include "../headers/shaderProgramm.h"
namespace ve {
    ShaderProgramm::ShaderProgramm(int a, Shader shaders[]) {

        programm = glCreateProgram();

        GLint Result = GL_FALSE;
        int InfoLogLength;

        // Attaching shaders if there are any
        if(a > 0) {
            for(int sh = 0; sh < a; ++sh) {
                glAttachShader(programm, shaders[sh].getShader());
            }
            glLinkProgram(programm);
        }

        //Check programm for Error
        glGetProgramiv(programm, GL_LINK_STATUS, &Result);
        glGetProgramiv(programm, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if(InfoLogLength > 1) {
            std::vector<char> programmErrorMsg(InfoLogLength + 1);
            glGetProgramInfoLog(programm, InfoLogLength, NULL, &programmErrorMsg[0]);
            fprintf(stdout, "%s", &programmErrorMsg[0]);
        }
    }

    GLuint ShaderProgramm::getProg() {
        return programm;
    }
}
