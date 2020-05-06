#include "texture.h"
#include "stb_image/stb_image.h"

Texture::Texture(const std::string& path): m_renderId(0), m_filePath(path), m_localBuffer(nullptr), m_width(0), m_height(0), m_BPP(0){

	stbi_set_flip_vertically_on_load(1);                                                //flip the image and make (0,0) on the top left
	m_localBuffer = stbi_load(path.c_str(), &m_width, &m_height, &m_BPP, 4);

	glGenTextures(1, &m_renderId);
	glBindTexture(GL_TEXTURE_2D, m_renderId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);                   //adapt the dimension of the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer);

	glBindTexture(GL_TEXTURE_2D, 0);

	if (m_localBuffer) {
		stbi_image_free(m_localBuffer);
	}

}
Texture::~Texture(){
	glDeleteTextures(1, &m_renderId);

}

void Texture::bind(unsigned int slot) const{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_renderId);

}

void Texture::unbind() const{
	glBindTexture(GL_TEXTURE_2D, 0);
}