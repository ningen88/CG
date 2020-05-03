#pragma once

#include "render.h"

class Texture {

private:
	unsigned int m_renderId;
	std::string m_filePath;
	unsigned char* m_localBuffer;
	int m_width, m_height, m_BPP;   //BPP bit per pixel

public:
	Texture(const std::string& path);
	~Texture();

	void bind(unsigned int slot = 0) const;
	void unbind() const;

	inline int getWidth() const { return m_width; }
	inline int getHeight() const { return m_height; }
};