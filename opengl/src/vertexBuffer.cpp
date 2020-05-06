#include "vertexBuffer.h"
#include "render.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size) {

    glGenBuffers(1, &m_renderId);        
    glBindBuffer(GL_ARRAY_BUFFER, m_renderId);            
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

}
VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &m_renderId);
}
void VertexBuffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_renderId);

}

void VertexBuffer::unbind() const{
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}