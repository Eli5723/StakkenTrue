#pragma once

#include <defines.h>

#include <glm/glm.hpp>
#include <GL/gl3w.h>

namespace Engine::Draw {

    class Framebuffer {
    public:
        Framebuffer(const glm::vec2& size);
        ~Framebuffer();

        void Bind();
        void Unbind();

        void Resize(const glm::vec2& size);

        const glm::vec2& GetSize() const { return m_Size; }
        const GLuint GetColorAttachment() const { return m_ColorAttachment; }
        const GLuint GetDepthAttachment() const { return m_DepthAttachment; }

    private:
        GLuint m_Framebuffer;
        GLuint m_ColorAttachment;
        GLuint m_DepthAttachment;
        glm::vec2 m_Size;
    };

}