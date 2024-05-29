#include "Renderer.h"
#include "VertexBuffer.h"



VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
    GLCall(glGenBuffers(1, &m_BufferID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_BufferID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::VertexBuffer()
{
}

VertexBuffer::~VertexBuffer() {
    GLCall(glDeleteBuffers(1, &m_BufferID));
}

void VertexBuffer::Bind() const {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_BufferID));

}

void VertexBuffer::Unbind() const {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        
}

void VertexBuffer::LateInit(const void* data, unsigned int size) {
    
    GLCall(glGenBuffers(1, &m_BufferID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_BufferID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

void VertexBuffer::MeshDataInit(std::vector<Mesh> data) {

    //size_t vertexSize = data.vertices.size() * sizeof(glm::vec3);
    //size_t normalSize = data.normals.size() * sizeof(glm::vec3);
    //size_t texCordSize = data.uvs.size() * sizeof(glm::vec2);

    //GLCall(glGenBuffers(1, &m_BufferID));

    //GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_BufferID));

    //GLCall(glBufferData(GL_ARRAY_BUFFER, vertexSize + normalSize + texCordSize, nullptr, GL_STATIC_DRAW));
    //GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, vertexSize, data.vertices.data()));
    //GLCall(glBufferSubData(GL_ARRAY_BUFFER, vertexSize, normalSize, data.normals.data()));
    //GLCall(glBufferSubData(GL_ARRAY_BUFFER, vertexSize + normalSize, texCordSize, data.uvs.data()));



}