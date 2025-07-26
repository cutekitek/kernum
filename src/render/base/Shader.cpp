#include "Shader.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

#include "glm/gtc/type_ptr.hpp"


Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc) {
    m_RendererID = CreateProgram(vertexSrc, fragmentSrc);
}

Shader::~Shader() {
    glDeleteProgram(m_RendererID);
}

void Shader::Bind() const {
    glUseProgram(m_RendererID);
}

void Shader::Unbind() const {
    glUseProgram(0);
}

void Shader::SetMat4(const std::string &name, glm::mat4 &data) {
    glUniformMatrix4fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, GL_FALSE, glm::value_ptr(data));
}

void Shader::SetVec2(const std::string &name, float data[2]) {
    glUniform2fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, data);
}
void Shader::SetFloat(const std::string &name, float data) {
    glUniform1f(glGetUniformLocation(m_RendererID, name.c_str()), data);
}
void Shader::SetUInt(const std::string &name, uint32_t data) {
    glUniform1ui(glGetUniformLocation(m_RendererID, name.c_str()), data);
}

void Shader::SetInt(const std::string &name, int data) {
    glUniform1i(glGetUniformLocation(m_RendererID, name.c_str()), data);
}

std::shared_ptr<Shader> Shader::FromFile(const std::string &vertexSrc, const std::string &fragmentSrc) {
    std::ifstream vertFile(vertexSrc);
    std::stringstream vert;
    vert << vertFile.rdbuf();
    std::ifstream fragFile(fragmentSrc);
    std::stringstream frag;
    frag << fragFile.rdbuf();
    auto vertStr = vert.str();
    auto fragStr = frag.str();
    return std::make_shared<Shader>(vertStr, fragStr);
}


GLuint Shader::CompileShader(GLenum type, const std::string& source) {
    GLuint id = glCreateShader(type);
    const char* src = source.c_str();

    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // Error handling
    GLint result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        GLint length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        std::string message(length, ' ');
        glGetShaderInfoLog(id, length, &length, &message[0]);

        std::cerr << "Failed to compile "
                  << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
                  << " shader:\n" << message << std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}

GLuint Shader::CreateProgram(const std::string& vertexSrc, const std::string& fragmentSrc) {
    GLuint program = glCreateProgram();
    GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexSrc);
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentSrc);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    // Error handling
    GLint isLinked;
    glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE) {
        GLint length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        std::string message(length, ' ');
        glGetProgramInfoLog(program, length, &length, &message[0]);

        std::cerr << "Failed to link program:\n" << message << std::endl;

        glDeleteProgram(program);
        glDeleteShader(vs);
        glDeleteShader(fs);
        return 0;
    }

    glDetachShader(program, vs);
    glDetachShader(program, fs);
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}