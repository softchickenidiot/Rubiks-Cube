#include "Texture.h"

Texture::Texture(GLenum TextureTarget, const char* FileName)
{
	ilInit();
	iluInit();
	ilutEnable(ILUT_OPENGL_CONV);
	m_textureTarget = TextureTarget;
	m_fileName = FileName;
}

bool Texture::Load()
{
	ilGenImages(1, &m_pImage);
	ilBindImage(m_pImage);
	ilLoad(IL_PNG, m_fileName);
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

	glGenTextures(1, &m_textureObj);
	glBindTexture(m_textureTarget, m_textureObj);
	glTexImage2D(m_textureTarget, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());

	glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;
}

void Texture::Bind(GLenum TextureUnit)
{
	glActiveTexture(TextureUnit);
	glBindTexture(m_textureTarget, m_textureObj);
}
