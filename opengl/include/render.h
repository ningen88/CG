#pragma once

#include <GL/glew.h>
#include "vertexArray.h"
#include "indexBuffer.h"
#include "shader.h"

class Render {

private:

public:
	void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void clear();
};