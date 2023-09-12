#include <Assets/Texture.hpp>

#include <stb_image.h>
#include <spdlog/spdlog.h>

namespace Engine {
namespace Assets {

Texture* Texture::Load(const char* path){
    int width, height, nrChannels;
	unsigned char* imageData = stbi_load(path, &width, &height, &nrChannels, 4);

    if (!imageData){
        spdlog::warn("Failed to load texture.");
        return nullptr;
    }

    GLuint id;
	glGenTextures(1, &id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, id);
    
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(imageData);
    
    Texture* newTex = new Texture;

    newTex->id = id;
    newTex->size = {width,height};

    return newTex;
}

}
}