#pragma once

#include <glm/glm.hpp>
#include <GL/gl3w.h>

namespace Engine {
namespace Assets {
    
struct Texture {
    GLuint id;
    glm::vec2 size;

    static Texture* Load(const char* path);
};


}
}
