#pragma once
class Texture
{
public:
	Texture();
	~Texture();

	void init(unsigned int numTextures, unsigned int width, unsigned int height, int* attachments, int* textureTypes, void* pixels = 0, int* filters = nullptr);
	void initFramebuffer(int* attachments);
	void cleanup();

	inline unsigned int getWidth() { return m_width; }
	inline unsigned int getHeight() { return m_height; }

	void bind(int index = 0, int slot = 0);
	void unbind(int slot = 0);
	void bindRenderTarget();
private:
	unsigned int* m_textures;
	unsigned int m_numTextures;
	unsigned int m_framebuffer;
	unsigned int m_renderbuffer;
	unsigned int m_width;
	unsigned int m_height;
};

