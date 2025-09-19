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


    GLuint CreateCompiledShaderObject(const GLenum shaderType, const std::string shaderSource, GLint* isShaderCompiled)
    {
        *isShaderCompiled = 0;
        GLuint shaderObjectId = glCreateShader(shaderType);
        if (shaderObjectId == 0)
            return 0;

        const GLchar* source = shaderSource.c_str();
        const GLint sourceLength = shaderSource.length();
        glShaderSource(shaderObjectId, 1, &source, &sourceLength);

        glCompileShader(shaderObjectId);
        glGetShaderiv(shaderObjectId, GL_COMPILE_STATUS, isShaderCompiled);
        return shaderObjectId;
    }

    GLuint CreateCompiledShaderProgramObject(const size_t shadersCount, const GLuint* shaders, GLint* isShaderProgramCompiled)
    {
        *isShaderProgramCompiled = 0;
        GLuint shaderProgramObjectId = glCreateProgram();
        if (shaderProgramObjectId == 0)
            return 0;

        for (size_t i = 0; i < shadersCount; i++)
            glAttachShader(shaderProgramObjectId, shaders[i]);

        glLinkProgram(shaderProgramObjectId);
        glGetProgramiv(shaderProgramObjectId, GL_LINK_STATUS, isShaderProgramCompiled);

        for (size_t i = 0; i < shadersCount; i++)
            glDetachShader(shaderProgramObjectId, shaders[i]);

        return shaderProgramObjectId;
    }

}