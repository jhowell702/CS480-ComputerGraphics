#ifndef SHADER_H
#define SHADER_H

#include <vector>

#include "graphics_headers.h"

class Shader
{
  public:
    Shader();
    ~Shader();
    bool Initialize();
    void Enable(int shader);
    bool AddShader(GLenum ShaderType, std::string shaderFileName);
    bool AddShader2(GLenum ShaderType, std::string shaderFileName);
    std::string LoadShaderFile(std::string);
    bool Finalize();
    GLint GetUniformLocation(const char* pUniformName, int shader);

  private:
    GLuint m_shaderProg;
    GLuint m_shaderProg2;
    std::vector<GLuint> m_shaderObjList;
};

#endif  /* SHADER_H */
