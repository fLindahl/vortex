#include "config.h"
#include "textureresource.h"

#include "stb_image.h"
//#include "render/softwarerender/rasterizer.h"

namespace Render
{

TextureResource::TextureResource()
{

}

TextureResource::~TextureResource()
{
	glDeleteTextures(1, &texture);
}

void TextureResource::loadFromFile(const char * filename)
{

	int w, h, n; //Width, Height, components per pixel (ex. RGB = 3, RGBA = 4)
	unsigned char *image = stbi_load(filename, &w, &h, &n, 0);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// If there's no alpha channel, use RGB colors. else: use RGBA.
	if (n == 3){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	}
	else if (n == 4){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	}

	glGenerateMipmap(GL_TEXTURE_2D);
	

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(image);

}

void TextureResource::BindTexture(GLuint slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, this->texture);
}

}