#pragma once

#include <memory>
#include <string>
#include <../include/glad/glad.h>
#include <glm/glm.hpp>

class Shader {
public:
    Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    GLuint GetID() const { return m_RendererID; }
    void SetMat4(const std::string &name, glm::mat4 &data);
    void SetVec2(const std::string &name, float data[2]);
    void SetFloat(const std::string &name, float data);
    void SetUInt(const std::string &name, uint32_t data);
    void SetInt(const std::string &name, int data);
    static std::shared_ptr<Shader> FromFile(const std::string& vertexSrc, const std::string& fragmentSrc);

private:
    GLuint m_RendererID;

    GLuint CompileShader(GLenum type, const std::string& source);
    GLuint CreateProgram(const std::string& vertexSrc, const std::string& fragmentSrc);

};

