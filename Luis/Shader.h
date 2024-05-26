#pragma once

#include<string>
#include<unordered_map>
#include <glm/glm.hpp>


class Shader {
private:
    std::string m_FragmentFile, m_VertexFile;
    unsigned int m_BufferID;
    //caching for uniforms
    std::unordered_map<std::string, int> m_UniformLocationCache;
public:
    Shader(const std::string& fragmentFile, const std::string& vertexFile);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    //Set Uniforms
    void SetUniform1i(const std::string& name, int value);
    void SetUniform1f(const std::string& name, float value);
    void SetUniform3f(const std::string& name, float v0, float v1, float v2);
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void SetUniformMat4f(const std::string& name, const glm::mat4 matrix);
    void SetUniformMat3f(const std::string& name, glm::mat3 matrix);
    void SetUniformVec3(const std::string& name, glm::vec3 v);
private:
    int GetUniformLocation(const std::string& name);
    unsigned int CompileShader(unsigned int type, const std::string& source);
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    std::string ParseShader(const std::string& filepath);
};