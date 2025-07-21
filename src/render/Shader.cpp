#include "Shader.h"
#include <iostream>

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

void Shader::SetMat4(const std::string &name, float data[16]) {
    glUniformMatrix4fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, GL_FALSE, data);
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