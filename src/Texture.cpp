#include "Texture.h"

#include <iostream>
#include <glew.h>

Texture::Texture() : m_textures(nullptr), m_numTextures(0), m_width(0), m_height(0), m_framebuffer(0), m_renderbuffer(0)
{
}

Texture::~Texture()
{
	cleanup();
}

void Texture::init(unsigned int numTextures, unsigned int width, unsigned int height, int* attachments, int* textureTypes, void* pixels, int* filters)
{
	m_width = width;
	m_height = height;
	m_numTextures = numTextures;

	if (m_textures == nullptr) {
		m_textures = new unsigned int[numTextures];
	}
	glGenTextures(numTextures, m_textures);

	for (unsigned int i = 0; i < m_numTextures; i++) {
		bind(i, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filters == nullptr ? GL_NEAREST : filters[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filters == nullptr ? GL_NEAREST : filters[i]);
		
		int internalFormat = GL_RGBA;
		int dataType = GL_UNSIGNED_INT;

		switch (textureTypes[i]) {
		case (0) : internalFormat = GL_RGBA; dataType = GL_UNSIGNED_BYTE; break;
		case (1) : internalFormat = GL_RGBA32F; dataType = GL_FLOAT; break;
		case (2) : internalFormat = GL_DEPTH_COMPONENT; dataType = GL_FLOAT; break;
		}
		
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_width, m_height, 0, GL_RGBA, dataType, pixels);
	}

	if (attachments != nullptr) initFramebuffer(attachments);
}

void Texture::initFramebuffer(int* attachments)
{
	unsigned int drawBuffers[32];
	bool depth = false;

	for (unsigned int i = 0; i < m_numTextures; i++) {
		if (attachments[i] == GL_DEPTH_ATTACHMENT) depth = true;
		if (attachments[i] == GL_DEPTH_ATTACHMENT || attachments[i] == GL_STENCIL_ATTACHMENT) {
			drawBuffers[i] = GL_NONE;
		} else {
			drawBuffers[i] = attachments[i];
		}

		if (attachments[i] == GL_NONE) continue;

		if (!m_framebuffer) {
			glGenFramebuffers(1, &m_framebuffer);
			glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
		}
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachments[i], GL_TEXTURE_2D, m_textures[i], 0);
	}

	if (!m_framebuffer) {
		std::cout << "Framebuffer hasn't been created" << std::endl;
		return;
	}

	if (!depth) {
		glGenRenderbuffers(1, &m_renderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_width, m_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_renderbuffer);
	}

	glDrawBuffers(m_numTextures, drawBuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Failed to create Framebuffer" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Texture::cleanup()
{
	glDeleteFramebuffers(1, &m_framebuffer);
	glDeleteRenderbuffers(1, &m_renderbuffer);
	delete[] m_textures;
}

void Texture::bind(int index, int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_textures[index]);
}

void Texture::unbind(int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bindRenderTarget()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
	glViewport(0, 0, m_width, m_height);
}