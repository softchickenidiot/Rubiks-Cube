#pragma once

#include "GLEW/glew.h"
#include "IL/il.h"
#include "IL/ilu.h"
#include "IL/ilut.h"
#include <iostream>

using namespace std;

class Texture {

	GLenum m_textureTarget;
	GLenum m_textureObj;
	const char* m_fileName;
	ILuint m_pImage;

public:

	Texture(GLenum TextureTarget, const char* FileName);

	bool Load();

	void Bind(GLenum TextureUnit);

};