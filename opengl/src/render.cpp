#include "render.h"

void Render::draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {

    shader.bind();
    va.bind();
    ib.bind();

    glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr);

}

void Render::clear() {
    glClear(GL_COLOR_BUFFER_BIT);
}