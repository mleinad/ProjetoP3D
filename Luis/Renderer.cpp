#include "Renderer.h"
#include <iostream>

void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL ERROR] (" << error << "):" << function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}


//sem ebo 
void Buffer::Draw(const VertexArray& VA, const int count, const Shader& shader) const {
    

    shader.Bind();
    VA.Bind();
    GLCall(glDrawArrays(GL_TRIANGLES,0, count));
}

void Buffer::Draw(const VertexArray& VA, const IndexBuffer& IB, const Shader& shader) const {
    shader.Bind();
    VA.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, IB.GetCount(), GL_UNSIGNED_INT, nullptr));
}


void Buffer::Clear() {
    GLCall(glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT));
}