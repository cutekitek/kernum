#pragma once

#include <string>
#include <../include/glad/glad.h>

class Shader {
public:
    Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    GLuint GetID() const { return m_RendererID; }
    void SetMat4(const std::string &name, float data[16]);
    void SetVec2(const std::string &name, float data[2]);
    void SetFloat(const std::string &name, float data);
    void SetUInt(const std::string &name, uint32_t data);
    void SetInt(const std::string &name, int data);


private:
    GLuint m_RendererID;

    GLuint CompileShader(GLenum type, const std::string& source);
    GLuint CreateProgram(const std::string& vertexSrc, const std::string& fragmentSrc);

};

