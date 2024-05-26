#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "Renderer.h"

Shader::Shader(const std::string& fragmentFile, const std::string& vertexFile)
    : m_FragmentFile(fragmentFile), m_VertexFile(vertexFile), m_BufferID(0)
{
    std::string fragSource, vertSource;
    fragSource = ParseShader(m_FragmentFile);
    vertSource = ParseShader(m_VertexFile);

    m_BufferID = CreateShader(vertSource, fragSource);
}

Shader::~Shader() {
    GLCall(glDeleteProgram(m_BufferID));
}

std::string Shader::ParseShader(const std::string& filepath) {
    // Abre o ficheiro 'filename' em binário, e coloca-se na última posição do ficheiro.
    std::ifstream ficheiro(filepath, std::ifstream::ate | std::ifstream::binary);
    // Se o ficheiro foi aberto.
    if (ficheiro.is_open()) {
        // Leitura da próxima posição de leitura.
        std::streampos tamanhoDoFicheiroEmBytes = ficheiro.tellg();
        // Reposiciona a leitura do ficheiro no seu início.
        ficheiro.seekg(0, std::ios::beg);

        // Alocação de espaço de memória para dados do ficheiro.
        GLchar* source = new GLchar[int(tamanhoDoFicheiroEmBytes) + 1];
        // Leitura do ficheiro para o array 'source'.
        ficheiro.read(source, tamanhoDoFicheiroEmBytes);
        // Fecha a string.
        source[tamanhoDoFicheiroEmBytes] = 0;

        // Fecha o ficheiro.
        ficheiro.close();

        // Retorna o endereço da string alocada.
        return const_cast<const GLchar*>(source);
    }
    else {
        std::cerr << "Erro ao abrir o ficheiro '" << filepath << "'" << std::endl;
    }

    return nullptr;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
    GLCall(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

    if (result == GL_FALSE) {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = new char[length + 1];
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to Compile " << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << " Shader" << std::endl;
        std::cout << message << std::endl;
        delete[] message;
        GLCall(glDeleteShader(id));
        return 0;
    }

    return id;
}


unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    GLCall(unsigned int program = glCreateProgram());
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;

}

void Shader::Bind() const {

    GLCall(glUseProgram(m_BufferID));
}

void Shader::Unbind() const {
    GLCall(glUseProgram(0));
}

#pragma region set uniforms
void Shader::SetUniform1i(const std::string& name, int value) {
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value) {
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2) {
    GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string& name, glm::mat4 matrix) {
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, false, &matrix[0][0]));
}

void Shader::SetUniformMat3f(const std::string& name, glm::mat3 matrix) {
    GLCall(glUniformMatrix3fv(GetUniformLocation(name), 1, false, &matrix[0][0]));
}

void Shader::SetUniformVec3(const std::string& name, glm::vec3 v)
{
    SetUniform3f(name, v.x, v.y, v.z);
}

#pragma endregion


int Shader::GetUniformLocation(const std::string& name) {
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) return m_UniformLocationCache[name];

    GLCall(int location = glGetUniformLocation(m_BufferID, name.c_str()));
    if (location == -1) {
        std::cout << "Warning uniform '" << name << "' doesnt exist!" << std::endl;
    }
    m_UniformLocationCache[name] = location;
    return location;
}