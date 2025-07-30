#include <fstream>
#include <string>
#include <glew.h>
#include "tutorial_glutils.h"


namespace tutorial
{
    const std::string ReadFile(const std::string pathToFile)
    {
        std::ifstream fileStream(pathToFile);
        if (!fileStream.is_open())
            return "";

        std::string content = "";
        std::string line = "";
        while (!fileStream.eof()) {
            std::getline(fileStream, line);
            content.append(line + "\n");
        }

        fileStream.close();
        return content;
    }


    GLuint CreateCompiledShader(const GLenum shaderType, const std::string shaderSource, GLint* isShaderCompiled)
    {
        *isShaderCompiled = 0;
        GLuint shaderId = glCreateShader(shaderType);
        if (shaderId == 0)
            return 0;

        const GLchar* shaderSourceChar = shaderSource.c_str();
        const GLint shaderSourceLength = shaderSource.length();
        glShaderSource(shaderId, 1, &shaderSourceChar, &shaderSourceLength);

        glCompileShader(shaderId);
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, isShaderCompiled);
        return shaderId;
    }

    GLuint CreateCompiledShaderProgram(const size_t shadersLength, const GLuint* shaders, GLint* isShaderProgramCompiled)
    {
        *isShaderProgramCompiled = 0;
        GLuint shaderProgramId = glCreateProgram();
        if (shaderProgramId == 0)
            return 0;

        for (size_t i = 0; i < shadersLength; i++)
            glAttachShader(shaderProgramId, shaders[i]);

        glLinkProgram(shaderProgramId);
        glGetProgramiv(shaderProgramId, GL_LINK_STATUS, isShaderProgramCompiled);
        
        for (size_t i = 0; i < shadersLength; i++)
            glDetachShader(shaderProgramId, shaders[i]);

        return shaderProgramId;
    }

}